/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "expression.hh"
#include "exprtypeenum.h"
#include "exprfactory.hh"
#include "jvmmanager.hh"
#include "jterminals.hh"
#include "debug.h"

namespace convert {

using namespace librdag;

/**
 * Generate the libRDAG expression objects for a given Java object
 * @param obj a Java OGNumeric type
 * @return a pointer to an ArgContainer representing the parameters
 */
ArgContainer* generateArgs(jobject *obj)
{
  // get object array
  jmethodID method = JVMManager::getOGExprClazz_getExprs();
  if(!method)
  {
    printf("JOGExpr: null method\n");
    exit(1);
  }
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMManager::getJVM()->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
    printf("Thread attach failed\n");
    exit(1);
  }
  jobject dataobj = NULL;
  dataobj = env->CallObjectMethod(*obj, method);
  if(!dataobj)
  {
    printf("CallObjectMethod failed\n");
    exit(1);
  }
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
  DEBUG_PRINT("vm ptr at 0x%llx\n",(long long unsigned int)JVMManager::getJVM());
  JNIEnv *env=NULL;
  jint jStatus = 0;
  jStatus=JVMManager::getJVM()->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
    fprintf(stderr, "Thread attach failed\n");
    exit(1);
  }
  jobject typeobj = env->CallObjectMethod(obj, JVMManager::getOGNumericClazz_getType());
  jlong ID = env->GetLongField(typeobj, JVMManager::getOGExprTypeEnumClazz__hashdefined());
  // FIXME: What is the point of printing out the pointer to the class type?
#ifdef __MINGW32__
  unsigned int high, low;
  low =   (long long unsigned int)ID & 0x00000000FFFFFFFFULL;
  high = ((long long unsigned int)ID & 0xFFFFFFFF00000000ULL) >> 32;
  printf("Clazz Type is hashdefined as %x%x\n", high, low);
#else
  printf("Clazz Type is hashdefined as %lld\n",(long long int)ID);
#endif

  librdag::OGNumeric * _expr = nullptr;

  switch(ID)
  {
  case OGREALSCALAR_ENUM:
  {
    DEBUG_PRINT("Binding a JOGRealScalar\n");
    _expr = new JOGRealScalar(&obj);
  }
  break;
  case OGCOMPLEXSCALAR_ENUM:
  {
    DEBUG_PRINT("Binding a JOGComplexScalar\n");
    _expr = new JOGComplexScalar(&obj);
  }
  break;
  case OGREALMATRIX_ENUM:
  {
    DEBUG_PRINT("Binding a JOGRealMatrix\n");
    _expr = new JOGRealMatrix(&obj);
  }
  break;
  case OGCOMPLEXMATRIX_ENUM:
  {
    DEBUG_PRINT("Binding a JOGComplexMatrix\n");
    _expr = new JOGComplexMatrix(&obj);
  }
  break;
  case OGREALSPARSEMATRIX_ENUM:
  {
    DEBUG_PRINT("Binding a JOGRealSparseMatrix\n");
    _expr = new JOGRealSparseMatrix(&obj);
  }
  break;
  case OGCOMPLEXSPARSEMATRIX_ENUM:
  {
    DEBUG_PRINT("Binding a JOGComplexSparseMatrix\n");
    _expr = new JOGComplexSparseMatrix(&obj);
  }
  break;
  case OGREALDIAGONALMATRIX_ENUM:
  {
    DEBUG_PRINT("Binding a JOGRealDiagonalMatrix\n");
    _expr = new JOGRealDiagonalMatrix(&obj);
  }
  break;
  case OGCOMPLEXDIAGONALMATRIX_ENUM:
  {
    DEBUG_PRINT("Binding a JOGComplexDiagonalMatrix\n");
    _expr = new JOGComplexDiagonalMatrix(&obj);
  }
  break;
  case COPY_ENUM:
  {
    DEBUG_PRINT("COPY function\n");
    _expr = new COPY(generateArgs(&obj));
  }
  break;
  case MINUS_ENUM:
  {
    DEBUG_PRINT("MINUS function\n");
    _expr = new MINUS(generateArgs(&obj));
  }
  break;
  case PLUS_ENUM:
  {
    DEBUG_PRINT("PLUS function\n");
    _expr = new PLUS(generateArgs(&obj));
  }
  break;
  case SVD_ENUM:
  {
    DEBUG_PRINT("SVD function\n");
    _expr = new SVD(generateArgs(&obj));
  }
  break;
  case SELECTRESULT_ENUM:
  {
    DEBUG_PRINT("Select Result function\n");
    _expr = new SELECTRESULT(generateArgs(&obj));
  }
  break;
  default:
  {
    throw convertException("Unknown node type");
  }
  break;
  }

  if(_expr == nullptr)
  {
    throw convertException("_expr is NULL, and probably hasn't been set by the factory");
  }

  DEBUG_PRINT("Returning from exprfactory\n");
  return _expr;
}

} // namespace convert
