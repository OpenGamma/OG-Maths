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
template <typename T> T ogmatrix_t_add_1x1(OGMatrix<T> * arr0, OGMatrix<T> * arr1)
{
  T ret = arr0->getData()[0] + arr1->getData()[0];
  return ret;
}

// Adds the first data element of one ogdiagonalmatrix to another
template <typename T> T ogdiagonalmatrix_t_add_1x1(OGDiagonalMatrix<T> * arr0, OGDiagonalMatrix<T> * arr1)
{
  T ret = arr0->getData()[0] + arr1->getData()[0];
  return ret;
}

// Adds the first data element of one ogdiagonalmatrix to another
template <typename T> T ogsparsematrix_t_add_1x1(OGSparseMatrix<T> * arr0, OGSparseMatrix<T> * arr1)
{
  T ret = arr0->getData()[0] + arr1->getData()[0];
  return ret;
}

// Adds the first data element of one ogscalar to another
template <typename T> T ogscalar_t_add(OGScalar<T> * arr0, OGScalar<T> * arr1)
{
  T ret = arr0->getValue() + arr1->getValue();
  return ret;
}

/**
 * Enforcing check that OGMatrix<T> can be used in a templated function.
 */
TEST(AbstractTerminalsRegressionTest, OGMatrix_T) {
  OGMatrix<real16>  * r16_0 = new OGMatrix<real16>(new real16[1]{10},1,1,OWNER);
  OGMatrix<real16>  * r16_1 = new OGMatrix<real16>(new real16[1]{20},1,1,OWNER);
  real16 ranswer = ogmatrix_t_add_1x1(r16_0, r16_1);
  real16 rexpected = 30;
  ASSERT_TRUE(ranswer==rexpected);

  delete r16_0;
  delete r16_1;

  OGMatrix<complex16>  * c16_0 = new OGMatrix<complex16>(new complex16[1]{{10,7}},1,1,OWNER);
  OGMatrix<complex16>  * c16_1 = new OGMatrix<complex16>(new complex16[1]{{20,14}},1,1,OWNER);
  complex16 canswer = ogmatrix_t_add_1x1(c16_0, c16_1);
  complex16 cexpected = {30,21};
  ASSERT_TRUE(canswer==cexpected);

  delete c16_0;
  delete c16_1;
}


/**
 * Enforcing check that OGDiagonalMatrix<T> can be used in a templated function.
 */
TEST(AbstractTerminalsRegressionTest, OGDiagonalMatrix_T) {
  OGDiagonalMatrix<real16>  * r16_0 = new OGDiagonalMatrix<real16>(new real16[1]{10},1,1,OWNER);
  OGDiagonalMatrix<real16>  * r16_1 = new OGDiagonalMatrix<real16>(new real16[1]{20},1,1,OWNER);
  real16 ranswer = ogdiagonalmatrix_t_add_1x1(r16_0, r16_1);
  real16 rexpected = 30;
  ASSERT_TRUE(ranswer==rexpected);

  delete r16_0;
  delete r16_1;

  OGDiagonalMatrix<complex16>  * c16_0 = new OGDiagonalMatrix<complex16>(new complex16[1]{{10,7}},1,1,OWNER);
  OGDiagonalMatrix<complex16>  * c16_1 = new OGDiagonalMatrix<complex16>(new complex16[1]{{20,14}},1,1,OWNER);
  complex16 canswer = ogdiagonalmatrix_t_add_1x1(c16_0, c16_1);
  complex16 cexpected = {30,21};
  ASSERT_TRUE(canswer==cexpected);

  delete c16_0;
  delete c16_1;
}


/**
 * Enforcing check that OGSparseMatrix<T> can be used in a templated function.
 */
TEST(AbstractTerminalsRegressionTest, OGSparseMatrix_T) {
  OGSparseMatrix<real16>  * r16_0 = new OGSparseMatrix<real16>(new int[2]{0,1}, new int[1]{0},new real16[1]{10},1,1,OWNER);
  OGSparseMatrix<real16>  * r16_1 = new OGSparseMatrix<real16>(new int[2]{0,1}, new int[1]{0},new real16[1]{20},1,1,OWNER);
  real16 ranswer = ogsparsematrix_t_add_1x1(r16_0, r16_1);
  real16 rexpected = 30;
  ASSERT_TRUE(ranswer==rexpected);

  delete r16_0;
  delete r16_1;

  OGSparseMatrix<complex16>  * c16_0 = new OGSparseMatrix<complex16>(new int[2]{0,1}, new int[1]{0},new complex16[1]{{10,7}},1,1,OWNER);
  OGSparseMatrix<complex16>  * c16_1 = new OGSparseMatrix<complex16>(new int[2]{0,1}, new int[1]{0},new complex16[1]{{20,14}},1,1,OWNER);
  complex16 canswer = ogsparsematrix_t_add_1x1(c16_0, c16_1);
  complex16 cexpected = {30,21};
  ASSERT_TRUE(canswer==cexpected);

  delete c16_0;
  delete c16_1;
}


/**
 * Enforcing check that OGScalar<T> can be used in a templated function.
 */
TEST(AbstractTerminalsRegressionTest, OGScalar_T) {
  OGScalar<real16>  * r16_0 = new OGScalar<real16>(10);
  OGScalar<real16>  * r16_1 = new OGScalar<real16>(20);
  real16 ranswer = ogscalar_t_add(r16_0, r16_1);
  real16 rexpected = 30;
  ASSERT_TRUE(ranswer==rexpected);

  delete r16_0;
  delete r16_1;

  OGScalar<complex16>  * c16_0 = new OGScalar<complex16>({10,7});
  OGScalar<complex16>  * c16_1 = new OGScalar<complex16>({20,14});
  complex16 canswer = ogscalar_t_add(c16_0, c16_1);
  complex16 cexpected = {30,21};
  ASSERT_TRUE(canswer==cexpected);

  delete c16_0;
  delete c16_1;

  OGScalar<int>  * i16_0 = new OGScalar<int>(10);
  OGScalar<int>  * i16_1 = new OGScalar<int>(20);
  int ianswer = ogscalar_t_add(i16_0, i16_1);
  int iexpected = 30;
  ASSERT_TRUE(ianswer==iexpected);

  delete i16_0;
  delete i16_1;
}