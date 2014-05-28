/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <sstream>
#include "com_opengamma_maths_materialisers_Materialisers.h"
#include "entrypt.hh"
#include "jvmmanager.hh"
#include "expression.hh"
#include "exprfactory.hh"
#include "warningmacros.h"
#include "jdispatch.hh"
#include "visitor.hh"
#include "debug.h"
#include <stdio.h>

using namespace convert;
using namespace dogma_exceptions;

/* Used to signal if something has gone wrong in our exception reporting code.
 * If this has happened, we are in real trouble (probably out of memory).
 */
typedef enum {
  EXCEPTION_OK = 0,
  EXCEPTION_ERROR = 1
} exc_handle_status;

/**
 * Check for local capacity and describe the Java exception if the EnsureLocalCapacity
 * call fails.
 *
 * @param env the JNI environment pointer
 * @param capacity the number of local refs to ensure
 * @return EXCEPTION_OK if capacity is ensured, EXCEPTION_ERROR otherwise
 */
exc_handle_status checkedLocalCapacity(JNIEnv* env, jint capacity)
{
  jint capStatus;
  capStatus = env->EnsureLocalCapacity(capacity); // for new exception/new string
  if (capStatus < 0)
  {
    // If capStatus < 0 then an OutOfMemoryError has been thrown.
    cerr << "Exception in native code, but cannot create Java exception. Exception in EnsureLocalCapacity." << endl;
    return EXCEPTION_ERROR;
  }
  return EXCEPTION_OK;
}

/**
 * Macro for checked calls. If the call fails, we need to exit with nullptr returned
 * as soon as possible.
 */
#define JNI_RETONFAIL(arg) if ((arg) == nullptr) return nullptr

/**
 * Build an array of strings suitable for passing to the MathsExceptionNative constructor.
 * Since we're already dealing with an exception here, if anything goes wrong in this
 * function, we just return null, since throwing another exception isn't going to do us
 * much good.
 *
 * @param env The JNI environment pointer
 * @param backtrace the native stack
 * @param traceSize the number of frames in the native stack
 * @return a Java array of strings representing the backtrace, or nullptr if something went wrong.
 */
jobjectArray javaBacktrace(JNIEnv* env, const BacktraceElement* backtrace, size_t traceSize)
{
  // 4 strings per stack frame, plus the array of strings, the string class,
  // and an empty string to populate the array with
  if (checkedLocalCapacity(env, (traceSize * 4) + 3) == EXCEPTION_ERROR)
  {
    return nullptr;
  }

  jclass strClass;
  jstring empty;
  jobjectArray btArray;

  // Create new array of strings for backtrace
  size_t nStr = traceSize * 4; // File name, class, method, line number
  JNI_RETONFAIL(strClass = env->FindClass("java/lang/String"));
  JNI_RETONFAIL(empty = env->NewStringUTF(""));
  JNI_RETONFAIL(btArray = env->NewObjectArray(nStr, strClass, empty));

  for (size_t i = 0; i < traceSize; i++)
  {
    jstring className, methodName, fileName, lineNo;
    const BacktraceElement* frame = &backtrace[i];

    stringstream ss;
    ss << (long long unsigned) frame->getAddress();

    JNI_RETONFAIL(className = env->NewStringUTF(frame->getObjectFile().c_str()));
    JNI_RETONFAIL(methodName = env->NewStringUTF(frame->getFunction().c_str()));
    JNI_RETONFAIL(fileName = env->NewStringUTF(ss.str().c_str()));
    JNI_RETONFAIL(lineNo = env->NewStringUTF("1"));
    env->SetObjectArrayElement(btArray, ((i * 4) + 0), className);
    if(env->ExceptionCheck() == JNI_TRUE) return nullptr;
    env->SetObjectArrayElement(btArray, ((i * 4) + 1), methodName);
    if(env->ExceptionCheck() == JNI_TRUE) return nullptr;
    env->SetObjectArrayElement(btArray, ((i * 4) + 2), fileName);
    if(env->ExceptionCheck() == JNI_TRUE) return nullptr;
    env->SetObjectArrayElement(btArray, ((i * 4) + 3), lineNo);
    if(env->ExceptionCheck() == JNI_TRUE) return nullptr;
  }

  return btArray;
}

/**
 * Macro for checked calls in exceptionJava. If the call fails, we need to exit with
 * EXCEPTION_ERROR returned as soon as possible.
 */
#define JNI_ERRONFAIL(arg) if ((arg) == nullptr) return EXCEPTION_ERROR

/**
 * Throw a new java exception with the backtrace from native code. On Windows, a backtrace
 * is not provided.
 *
 * @param env The JNI environment pointer
 * @param e the thrown native exception
 * @param exClass the class of the Java exception to construct
 * @return EXCEPTION_OK if the exception was successfully thrown in Java. EXCEPTION_ERROR otherwise.
 */
exc_handle_status exceptionJava(JNIEnv* env, dogma_error& e, jclass exClass)
{
  // Make sure we have capacity for a new exception (Windows) or new string, method ID,
  // and new exception (others)
  if (checkedLocalCapacity(env, 3) == EXCEPTION_ERROR)
  {
    return EXCEPTION_ERROR;
  }

  jint throwStatus;
#ifdef __MINGW32__
  throwStatus = env->ThrowNew(exClass, e.what());
#else
  jstring msg;
  jmethodID ctor;
  jobjectArray btArray;
  jthrowable jex;

  JNI_ERRONFAIL(msg = env->NewStringUTF(e.what()));
  JNI_ERRONFAIL(ctor = env->GetMethodID(JVMManager::getMathsExceptionNativeConversionClazz(), "<init>", "(Ljava/lang/String;[Ljava/lang/String;)V"));
  JNI_ERRONFAIL(btArray = javaBacktrace(env, e.getBacktrace(), e.getTraceSize()));
  JNI_ERRONFAIL(jex = (jthrowable) env->NewObject(exClass, ctor, msg, btArray));
  throwStatus = env->Throw(jex);
#endif // __MINGW32__

  if (throwStatus)
  {
    return EXCEPTION_ERROR;
  }
  else
  {
    return EXCEPTION_OK;
  }
}

/**
 * Throw a new Java MathsExceptionNativeConversion from a native convert_error. If the
 * Java throw fails, stderr receives a warning and the what() of the exception because
 * there is little else that can be done.
 *
 * @param env the JNI environment pointer
 * @param e the native exception.
 */
void convertExceptionJava(JNIEnv* env, convert_error& e)
{
  exc_handle_status stat;
  stat = exceptionJava(env, e, JVMManager::getMathsExceptionNativeConversionClazz());
  if (stat == EXCEPTION_ERROR)
  {
    cerr << "Error creating Java exception from convert_error." << endl;
    cerr << e.what();
  }
}

/**
 * Throw a new Java MathsExceptionNativeComputation from a native rdag_error. If the
 * Java throw fails, stderr receives a warning and the what() of the exception because
 * there is little else that can be done.
 *
 * @param env the JNI environment pointer
 * @param e the native exception.
 */
void rdagExceptionJava(JNIEnv* env, rdag_error& e)
{
  exc_handle_status stat;
  stat = exceptionJava(env, e, JVMManager::getMathsExceptionNativeComputationClazz());
  if (stat == EXCEPTION_ERROR)
  {
    cerr << "Error creating Java exception from rdag_error." << endl;
    cerr << e.what();
  }
}

/**
 * Throw a new Java MathsExceptionNativeUnspecified from a native exception. If the
 * Java throw fails, stderr receives a warning and the what() of the exception because
 * there is little else that can be done.
 *
 * @param env the JNI environment pointer
 * @param e the native exception.
 */
void unspecifiedExceptionJava(JNIEnv* env, exception& e)
{
  if (checkedLocalCapacity(env, 1) == EXCEPTION_ERROR)
  {
    cerr << "Error ensuring local capacity for Java exception." << endl;
    cerr << e.what();
  }

  jint throwStatus;
  throwStatus = env->ThrowNew(JVMManager::getMathsExceptionNativeUnspecifiedClazz(), e.what());

  if (throwStatus)
  {
    cerr << "Error ensuring throwing Java exception." << endl;
    cerr << e.what();
  }
}


#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_opengamma_maths_materialisers_Materialisers
 * Method:    materialiseToJDoubleArrayOfArrays
 * Signature: (Lcom/opengamma/maths/datacontainers/OGNumeric;)[[D
 */
JNIEXPORT jobjectArray JNICALL Java_com_opengamma_maths_materialisers_Materialisers_materialiseToJDoubleArrayOfArrays
(JNIEnv *env, jclass SUPPRESS_UNUSED clazz, jobject obj)
{
  DEBUG_PRINT("Entering materialise function\n");

  jobjectArray returnVal;

  try
  {
    // convert obj to OGNumeric objs
    DEBUG_PRINT("Calling convert::createExpression\n");
    librdag::OGNumeric::Ptr chain = convert::createExpression(obj);
    DEBUG_PRINT("Check for exception before entrypt\n");
    checkEx(env);
    DEBUG_PRINT("Calling entrypt function\n");
    librdag::OGTerminal::Ptr answer = entrypt(chain);
    DEBUG_PRINT("Returning from entrypt function\n");

    returnVal = Real16AoA{answer}.toJDoubleAoA(env);
  }
  catch (convert_error e)
  {
    convertExceptionJava(env, e);
    return nullptr;
  }
  catch (rdag_error e)
  {
    rdagExceptionJava(env, e);
    return nullptr;
  }
  catch (exception e)
  {
    unspecifiedExceptionJava(env, e);
    return nullptr;
  }

  DEBUG_PRINT("Returning\n");
  return returnVal;
}

/*
 * Class:     com_opengamma_maths_materialisers_Materialisers
 * Method:    materialiseToJComplexArrayContainer
 * Signature: (Lcom/opengamma/maths/datacontainers/OGNumeric;)Lcom/opengamma/maths/datacontainers/other/ComplexArrayContainer;
 */
JNIEXPORT jobject JNICALL Java_com_opengamma_maths_materialisers_Materialisers_materialiseToJComplexArrayContainer
(JNIEnv *env, jclass SUPPRESS_UNUSED clazz, jobject obj)
{
  DEBUG_PRINT("materialiseToJComplexArrayContainer\n");
  DEBUG_PRINT("Entering materialise function\n");

  jobject returnVal;

  try
  {
    DEBUG_PRINT("Calling convert::createExpression\n");
    // convert obj to OGNumeric objs
    librdag::OGNumeric::Ptr chain = convert::createExpression(obj);

    DEBUG_PRINT("Check for exception before entrypt\n");
    checkEx(env);
    DEBUG_PRINT("Calling entrypt function\n");
    librdag::OGTerminal::Ptr answer = entrypt(chain);

    Complex16AoA c = Complex16AoA{answer};
    jobjectArray realPart = c.realPartToJDoubleAoA(env);
    jobjectArray imagPart = c.imagPartToJDoubleAoA(env);
    returnVal = env->NewObject(JVMManager::getComplexArrayContainerClazz(),
                               JVMManager::getComplexArrayContainerClazz_ctor_DAoA_DAoA(),
                               realPart, imagPart);
  }
  catch (convert_error e)
  {
    convertExceptionJava(env, e);
    return nullptr;
  }
  catch (rdag_error e)
  {
    rdagExceptionJava(env, e);
    return nullptr;
  }
  catch (exception e)
  {
    unspecifiedExceptionJava(env, e);
    return nullptr;
  }

  DEBUG_PRINT("Returning\n");
  return returnVal;
}

/*
 * Class:     com_opengamma_maths_materialisers_Materialisers
 * Method:    materialiseToJBoolean
 * Signature: (Lcom/opengamma/maths/datacontainers/OGNumeric;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_opengamma_maths_materialisers_Materialisers_materialiseToJBoolean
(JNIEnv SUPPRESS_UNUSED *env, jclass SUPPRESS_UNUSED clazz, jobject SUPPRESS_UNUSED obj)
{
  jboolean returnVal = 0;
  return returnVal;
}

JNIEXPORT jobject JNICALL
Java_com_opengamma_maths_materialisers_Materialisers_materialiseToOGTerminal(JNIEnv SUPPRESS_UNUSED *env, jclass SUPPRESS_UNUSED clazz, jobject obj)
{
  DEBUG_PRINT("materialiseToOGTerminal\n");
  DEBUG_PRINT("Calling convert::createExpression\n");
  jobject result;

  try
  {
  // convert obj to OGNumeric objs
    librdag::OGNumeric::Ptr chain = convert::createExpression(obj);

    DEBUG_PRINT("Check for exception before entrypt\n");
    checkEx(env);
    DEBUG_PRINT("Calling entrypt function\n");
    librdag::OGTerminal::Ptr answer = entrypt(chain);

    DispatchToOGTerminal visitor{env};
    answer->accept(visitor);
    result = visitor.getObject();
  }
  catch (convert_error e)
  {
    convertExceptionJava(env, e);
    return nullptr;
  }
  catch (rdag_error e)
  {
    rdagExceptionJava(env, e);
    return nullptr;
  }
  catch (exception e)
  {
    unspecifiedExceptionJava(env, e);
    return nullptr;
  }

  DEBUG_PRINT("Returning\n");
  return result;
}

#ifdef __cplusplus
}
#endif
