/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "numeric.hh"
#include "exprfactory.hh"
#include "jvmmanager.hh"
#include "debug.h"

namespace convert {

using namespace librdag;

/**
 * Generate the libRDAG expression objects for a given Java object
 * @param obj a Java OGNumeric type
 * @return a pointer to an ArgContainer representing the parameters
 */
ArgContainer* generateArgs(jobject obj)
{
  // get object array
  jmethodID method = JVMManager::getOGExprClazz_getExprs();
  JNIEnv *env = nullptr;
  JVMManager::getEnv((void **)&env);
  jobject dataobj = JVMManager::callObjectMethod(env, obj, method);
  jobjectArray * args = reinterpret_cast<jobjectArray *>(&dataobj);
  jsize len = env->GetArrayLength((jarray)*args);
  DEBUG_PRINT("JOGExpr arg size is %d\n",(int)len);
  ArgContainer* local_args = new ArgContainer();
  for(int i=0; i<len; i++)
  {
    jobject tmp = (jobject) env->GetObjectArrayElement(*args, i);
    local_args->push_back(createExpression(tmp));
  }
  return local_args;
}

/**
 * Generates an RDAG expression tree from a java object
 * @param obj a Java OGNumeric type
 * @return the equivalent RDAG expression
 */
OGNumeric* createExpression(jobject obj)
{
  DEBUG_PRINT("In createExpression\n");
  JNIEnv *env=nullptr;
  JVMManager::getEnv((void **)&env);
  jobject typeobj = env->CallObjectMethod(obj, JVMManager::getOGNumericClazz_getType());
  jlong ID = env->GetLongField(typeobj, JVMManager::getOGExprEnumClazz__hashdefined());
  VAL64BIT_PRINT("Class type", ID);

  return createExprWithID(ID, obj);
}

} // namespace convert
