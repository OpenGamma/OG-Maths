/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "terminal.hh"
#include "expression.hh"
#include "visitor.hh"
#include "exceptions.hh"
#include "gtest/gtest.h"
#include "warningmacros.h"
#include "exprtypeenum.h"

using namespace std;
using namespace librdag;
/**
 * These tests are here to make sure that the templated types can always be used
 * in their templated form.
 */

// Adds the first data element of one ogmatrix to another
template <typename T>
T
ogmatrix_t_add_1x1(typename OGMatrix<T>::Ptr arr0, typename OGMatrix<T>::Ptr arr1)
{
  T ret = arr0->getData()[0] + arr1->getData()[0];
  return ret;
}

// Adds the first data element of one ogdiagonalmatrix to another
template <typename T>
T
ogdiagonalmatrix_t_add_1x1(typename OGDiagonalMatrix<T>::Ptr arr0, typename OGDiagonalMatrix<T>::Ptr arr1)
{
  T ret = arr0->getData()[0] + arr1->getData()[0];
  return ret;
}

// Adds the first data element of one ogdiagonalmatrix to another
template <typename T>
T
ogsparsematrix_t_add_1x1(typename OGSparseMatrix<T>::Ptr arr0, typename OGSparseMatrix<T>::Ptr arr1)
{
  T ret = arr0->getData()[0] + arr1->getData()[0];
  return ret;
}

// Adds the first data element of one ogscalar to another
template <typename T>
T
ogscalar_t_add(typename OGScalar<T>::Ptr arr0, typename OGScalar<T>::Ptr arr1)
{
  T ret = arr0->getValue() + arr1->getValue();
  return ret;
}

/**
 * Enforcing check that OGMatrix<T> can be used in a templated function.
 */
TEST(AbstractTerminalsRegressionTest, OGMatrix_T) {
  OGMatrix<real8>::Ptr r8_0 = OGMatrix<real8>::create(new real8[1]{10},1,1,OWNER);
  OGMatrix<real8>::Ptr r8_1 = OGMatrix<real8>::create(new real8[1]{20},1,1,OWNER);
  real8 ranswer = ogmatrix_t_add_1x1<real8>(r8_0, r8_1);
  real8 rexpected = 30;
  ASSERT_TRUE(ranswer==rexpected);

  OGMatrix<complex16>::Ptr c16_0 = OGMatrix<complex16>::create(new complex16[1]{{10,7}},1,1,OWNER);
  OGMatrix<complex16>::Ptr c16_1 = OGMatrix<complex16>::create(new complex16[1]{{20,14}},1,1,OWNER);
  complex16 canswer = ogmatrix_t_add_1x1<complex16>(c16_0, c16_1);
  complex16 cexpected = {30,21};
  ASSERT_TRUE(canswer==cexpected);
}


/**
 * Enforcing check that OGDiagonalMatrix<T> can be used in a templated function.
 */
TEST(AbstractTerminalsRegressionTest, OGDiagonalMatrix_T) {
  OGDiagonalMatrix<real8>::Ptr r8_0 = OGDiagonalMatrix<real8>::create(new real8[1]{10},1,1,OWNER);
  OGDiagonalMatrix<real8>::Ptr r8_1 = OGDiagonalMatrix<real8>::create(new real8[1]{20},1,1,OWNER);
  real8 ranswer = ogdiagonalmatrix_t_add_1x1<real8>(r8_0, r8_1);
  real8 rexpected = 30;
  ASSERT_TRUE(ranswer==rexpected);

  OGDiagonalMatrix<complex16>::Ptr c16_0 = OGDiagonalMatrix<complex16>::create(new complex16[1]{{10,7}},1,1,OWNER);
  OGDiagonalMatrix<complex16>::Ptr c16_1 = OGDiagonalMatrix<complex16>::create(new complex16[1]{{20,14}},1,1,OWNER);
  complex16 canswer = ogdiagonalmatrix_t_add_1x1<complex16>(c16_0, c16_1);
  complex16 cexpected = {30,21};
  ASSERT_TRUE(canswer==cexpected);
}


/**
 * Enforcing check that OGSparseMatrix<T> can be used in a templated function.
 */
TEST(AbstractTerminalsRegressionTest, OGSparseMatrix_T) {
  OGSparseMatrix<real8>::Ptr r8_0 = OGSparseMatrix<real8>::create(new int[2]{0,1}, new int[1]{0},new real8[1]{10},1,1,OWNER);
  OGSparseMatrix<real8>::Ptr r8_1 = OGSparseMatrix<real8>::create(new int[2]{0,1}, new int[1]{0},new real8[1]{20},1,1,OWNER);
  real8 ranswer = ogsparsematrix_t_add_1x1<real8>(r8_0, r8_1);
  real8 rexpected = 30;
  ASSERT_TRUE(ranswer==rexpected);

  OGSparseMatrix<complex16>::Ptr c16_0 = OGSparseMatrix<complex16>::create(new int[2]{0,1}, new int[1]{0},new complex16[1]{{10,7}},1,1,OWNER);
  OGSparseMatrix<complex16>::Ptr c16_1 = OGSparseMatrix<complex16>::create(new int[2]{0,1}, new int[1]{0},new complex16[1]{{20,14}},1,1,OWNER);
  complex16 canswer = ogsparsematrix_t_add_1x1<complex16>(c16_0, c16_1);
  complex16 cexpected = {30,21};
  ASSERT_TRUE(canswer==cexpected);
}


/**
 * Enforcing check that OGScalar<T> can be used in a templated function.
 */
TEST(AbstractTerminalsRegressionTest, OGScalar_T) {
  OGScalar<real8>::Ptr r8_0 = OGScalar<real8>::create(10);
  OGScalar<real8>::Ptr r8_1 = OGScalar<real8>::create(20);
  real8 ranswer = ogscalar_t_add<real8>(r8_0, r8_1);
  real8 rexpected = 30;
  ASSERT_TRUE(ranswer==rexpected);

  OGScalar<complex16>::Ptr c16_0 = OGScalar<complex16>::create({10,7});
  OGScalar<complex16>::Ptr c16_1 = OGScalar<complex16>::create({20,14});
  complex16 canswer = ogscalar_t_add<complex16>(c16_0, c16_1);
  complex16 cexpected = {30,21};
  ASSERT_TRUE(canswer==cexpected);

  OGScalar<int>::Ptr i4_0 = OGScalar<int>::create(10);
  OGScalar<int>::Ptr i4_1 = OGScalar<int>::create(20);
  int ianswer = ogscalar_t_add<int>(i4_0, i4_1);
  int iexpected = 30;
  ASSERT_TRUE(ianswer==iexpected);
}
