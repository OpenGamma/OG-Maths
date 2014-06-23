/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */


#include "gtest/gtest.h"
#include "terminal.hh"
#include "execution.hh"
#include "dispatch.hh"
#include "testnodes.hh"
#include "runtree.hh"
#include "test/test_utils.hh"

using namespace std;
using namespace librdag;
using namespace testnodes;
using ::testing::TestWithParam;
using ::testing::Values;

/*
 * Check MLDIVIDE node behaves
 */

BINARY_NODE_TEST_SETUP(MLDIVIDE)

INSTANTIATE_NODE_TEST_CASE_P(MLDIVIDETests,MLDIVIDE,
  Values
  (

  // CHECKING REAL SPACE MATRICES
  new CheckBinary<MLDIVIDE>
  (
    // input
    OGRealScalar::create(2.0),
    OGRealScalar::create(3.0),
    // expected
    OGRealScalar::create(5.0),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    OGRealDenseMatrix::create({{2.0}}),
    OGRealDenseMatrix::create({{3.0}}),
    // expected
    OGRealDenseMatrix::create({{5.0}}),
    MATHSEQUAL
  )

  )
);


TEST(MLDIVIDETests, CheckBadCommuteThrows) {
  OGNumeric::Ptr m1 = OGRealDenseMatrix::create(new real8[6]{1,3,5,2,4,6},3,2,OWNER);
  OGNumeric::Ptr m2 = OGRealDenseMatrix::create(new real8[7]{10,30,20,40,50,60,70},1,7,OWNER);
  OGExpr::Ptr node = MLDIVIDE::create(m1, m2);
  ASSERT_THROW(
  runtree(node),
  rdag_error);
}
