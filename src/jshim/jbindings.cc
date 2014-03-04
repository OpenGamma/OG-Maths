/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "debug.h"
#include "jbindings.hh"
#include "jvmmanager.hh"
#include "exceptions.hh"

namespace convert {

/**
 * An adaptor that allows calling Get<type>ArrayElements in a template-parameterised way.
 *
 * @param nativeT the native type of the data that will be obtained
 * @param javaT the Java type of the data
 * @return the pointer to native data from Get<type>ArrayElements
 */
template<typename nativeT, typename javaT>
nativeT* getArrayFromJava(JNIEnv* env, javaT arr);

template<typename nativeT = real16, typename javaT = jdoubleArray>
real16* getArrayFromJava(JNIEnv *env, jdoubleArray arr)
{
  real16* p = env->GetDoubleArrayElements(arr, NULL);
  if (p == nullptr)
  {
    throw convert_error("Null pointer returned by GetDoubleArrayElements");
  }
  return p;
}

template<typename nativeT = jint, typename javaT = jintArray>
jint* getArrayFromJava(JNIEnv *env, jintArray arr)
{
  jint* p = env->GetIntArrayElements(arr, NULL);
  if (p == nullptr)
  {
    throw convert_error("Null pointer returned by GetIntArrayElements");
  }
  return p;
}

/**
 * An adaptor that allows calling Release<type>ArrayElements in a template-parameterised way.
 * The mode is always 0, which copies back the native data and frees the native array.
 *
 * @param nativeT the native type of the data that will be obtained
 * @param javaT the Java type of the data
 * @param nativeArr the pointer to native data from Get<type>ArrayElements
 * @param arr the Java array from which nativeArr was obtained
 */
template<typename nativeT, typename javaT>
void releaseArrayFromJava(JNIEnv* env, nativeT* nativeArr, javaT arr);

template<>
void releaseArrayFromJava(JNIEnv* env, real16* nativeArr, jdoubleArray arr)
{
  env->ReleaseDoubleArrayElements(arr, nativeArr, 0);
}

template<>
void releaseArrayFromJava(JNIEnv* env, complex16* nativeArr, jdoubleArray arr)
{
  env->ReleaseDoubleArrayElements(arr, (real16*) nativeArr, 0);
}

template<>
void releaseArrayFromJava(JNIEnv* env, jint* nativeArr, jintArray arr)
{
  env->ReleaseIntArrayElements(arr, nativeArr, 0);
}

/**
 * bindPrimitiveArrayData
 */

template <typename nativeT, typename javaT> nativeT * bindPrimitiveArrayData(jobject obj, jmethodID method)
{
  if(!obj)
  {
    throw convert_error("bindPrimitiveArrayData: null obj");
  }
  if(!method)
  {
    throw convert_error("bindPrimitiveArrayData: null method");
  }
  VAL64BIT_PRINT("Binding for jobject", obj);
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMManager::getJVM()->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
    throw convert_error("Thread attach failed");
  }
  jobject dataobj = NULL;
  dataobj = env->CallObjectMethod(obj, method);
  if(!dataobj)
  {
    throw convert_error("CallObjectMethod failed");
  }
  javaT * array = reinterpret_cast<javaT *>(&dataobj);
  nativeT * _dataptr = (nativeT*) getArrayFromJava<nativeT, javaT>(env, *array);
  return _dataptr;
}

template real16*
bindPrimitiveArrayData<real16, jdoubleArray>(jobject obj, jmethodID method);

template complex16*
bindPrimitiveArrayData<complex16, jdoubleArray>(jobject obj, jmethodID method);

template jint*
bindPrimitiveArrayData<jint, jintArray>(jobject obj, jmethodID method);

/**
 * unbindPrimitiveArrayData
 */

template <typename nativeT, typename javaT> void unbindPrimitiveArrayData(nativeT * nativeData, jobject obj, jmethodID method)
{
  if(!nativeData)
  {
    throw convert_error("unbindPrimitiveArrayData: null nativeData");
  }
  if(!obj)
  {
    throw convert_error("unbindPrimitiveArrayData: null obj");
  }
  if(!method)
  {
    throw convert_error("unbindPrimitiveArrayData: null method");
  }
  VAL64BIT_PRINT("Unbinding for jobject", obj);
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMManager::getJVM()->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
    throw convert_error("Thread attach failed");
  }
  jobject dataobj = env->CallObjectMethod(obj, method);
  javaT * array = reinterpret_cast<javaT *>(&dataobj);
  releaseArrayFromJava<nativeT, javaT>(env, nativeData, *array);
}

template void
unbindPrimitiveArrayData<real16, jdoubleArray>(real16* nativeData, jobject obj, jmethodID method);

template void
unbindPrimitiveArrayData<complex16, jdoubleArray>(complex16* nativeData, jobject obj, jmethodID method);

template void
unbindPrimitiveArrayData<jint, jintArray>(jint* nativeData, jobject obj, jmethodID method);

/**
 * unbindOGArrayData
 */

template <typename nativeT, typename javaT> void unbindOGArrayData(nativeT * nativeData, jobject obj)
{
  JNIEnv *env = NULL;
  jint jStatus = 0;
  VAL64BIT_PRINT("Unbinding for OGArrayData", obj);
  jStatus=JVMManager::getJVM()->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
    throw convert_error("Thread attach failed");
  }
  jobject dataobj = env->CallObjectMethod(obj, JVMManager::getOGTerminalClazz_getData());
  javaT * array = reinterpret_cast<javaT *>(&dataobj);
  releaseArrayFromJava<nativeT, javaT>(env, nativeData, *array);
}

template void
unbindOGArrayData<real16, jdoubleArray>(real16* nativeData, jobject obj);

template void
unbindOGArrayData<complex16, jdoubleArray>(complex16* nativeData, jobject obj);

template void
unbindOGArrayData<jint, jintArray>(jint* nativeData, jobject obj);

} //namespace convert
