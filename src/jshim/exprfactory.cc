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
  jlong ID = env->GetLongField(typeobj, JVMManager::getOGExprTypeEnumClazz__hashdefined());
  VAL64BIT_PRINT("Class type", ID);

  librdag::OGNumeric * _expr = nullptr;

  switch(ID)
  {
  case REAL_SCALAR_ENUM:
  {
    DEBUG_PRINT("Binding a JOGRealScalar\n");
    _expr = new JOGRealScalar(obj);
  }
  break;
  case COMPLEX_SCALAR_ENUM:
  {
    DEBUG_PRINT("Binding a JOGComplexScalar\n");
    _expr = new JOGComplexScalar(obj);
  }
  break;
  case REAL_MATRIX_ENUM:
  {
    DEBUG_PRINT("Binding a JOGRealMatrix\n");
    _expr = new JOGRealMatrix(obj);
  }
  break;
  case COMPLEX_MATRIX_ENUM:
  {
    DEBUG_PRINT("Binding a JOGComplexMatrix\n");
    _expr = new JOGComplexMatrix(obj);
  }
  break;
  case REAL_SPARSE_MATRIX_ENUM:
  {
    DEBUG_PRINT("Binding a JOGRealSparseMatrix\n");
    _expr = new JOGRealSparseMatrix(obj);
  }
  break;
  case COMPLEX_SPARSE_MATRIX_ENUM:
  {
    DEBUG_PRINT("Binding a JOGComplexSparseMatrix\n");
    _expr = new JOGComplexSparseMatrix(obj);
  }
  break;
  case REAL_DIAGONAL_MATRIX_ENUM:
  {
    DEBUG_PRINT("Binding a JOGRealDiagonalMatrix\n");
    _expr = new JOGRealDiagonalMatrix(obj);
  }
  break;
  case COMPLEX_DIAGONAL_MATRIX_ENUM:
  {
    DEBUG_PRINT("Binding a JOGComplexDiagonalMatrix\n");
    _expr = new JOGComplexDiagonalMatrix(obj);
  }
  break;
  case COPY_ENUM:
  {
    DEBUG_PRINT("COPY function\n");
    _expr = new COPY(generateArgs(obj));
  }
  break;
  case NEGATE_ENUM:
  {
    DEBUG_PRINT("NEGATE function\n");
    _expr = new NEGATE(generateArgs(obj));
  }
  break;
  case PLUS_ENUM:
  {
    DEBUG_PRINT("PLUS function\n");
    _expr = new PLUS(generateArgs(obj));
  }
  break;
  case SVD_ENUM:
  {
    DEBUG_PRINT("SVD function\n");
    _expr = new SVD(generateArgs(obj));
  }
  break;
  case SELECTRESULT_ENUM:
  {
    DEBUG_PRINT("Select Result function\n");
    _expr = new SELECTRESULT(generateArgs(obj));
  }
  break;
  default:
  {
    throw convert_error("Unknown node type");
  }
  break;
  }

  if(_expr == nullptr)
  {
    throw convert_error("_expr is NULL, and probably hasn't been set by the factory");
  }

  DEBUG_PRINT("Returning from exprfactory\n");
  return _expr;
}

} // namespace convert
