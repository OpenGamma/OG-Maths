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

using namespace std;
using namespace librdag;
using namespace testnodes;
using ::testing::TestWithParam;
using ::testing::Values;

/*
 * Check NORM2 node behaves
 */

UNARY_NODE_TEST_SETUP(NORM2)

INSTANTIATE_NODE_TEST_CASE_P(NORM2Tests,NORM2,
  Values
  (
  new CheckUnary<NORM2>( OGRealScalar::create(1.0), OGRealScalar::create(1.0), MATHSEQUAL),
  new CheckUnary<NORM2>( OGRealScalar::create(-1.0), OGRealScalar::create(1.0), MATHSEQUAL),
  new CheckUnary<NORM2>( OGRealScalar::create(0.0), OGRealScalar::create(0.0), MATHSEQUAL),
  new CheckUnary<NORM2>( OGRealDenseMatrix::create(new real8[1]{-1},1,1, OWNER), OGRealScalar::create(1.0), MATHSEQUAL),
  new CheckUnary<NORM2>( OGRealDenseMatrix::create(new real8[3]{1,2,3},1,3, OWNER), OGRealScalar::create(3.74165738677394), MATHSEQUAL),
  new CheckUnary<NORM2>( OGRealDenseMatrix::create(new real8[12]{1,4,7,10,2,5,8,11,3,6,9,12},4,3, OWNER), OGRealScalar::create(25.4624074360364), MATHSEQUAL),
  // test complex
  new CheckUnary<NORM2>( OGComplexScalar::create({1.0,0}), OGRealScalar::create(1.0), MATHSEQUAL),
  new CheckUnary<NORM2>( OGComplexScalar::create({0,1.0}), OGRealScalar::create(1.0), MATHSEQUAL),
  new CheckUnary<NORM2>( OGComplexScalar::create({1.0,1.0}), OGRealScalar::create(std::sqrt(2)), MATHSEQUAL),
new CheckUnary<NORM2>( OGComplexScalar::create({-1.0,1.0}), OGRealScalar::create(std::sqrt(2)), MATHSEQUAL),
  new CheckUnary<NORM2>( OGComplexScalar::create({0,0}), OGRealScalar::create(0.0), MATHSEQUAL),
  new CheckUnary<NORM2>( OGComplexMatrix::create(new complex16[3]{{1,10},{2,20},{3,30}},1,3, OWNER), OGRealScalar::create(37.6031913539263), MATHSEQUAL),
  new CheckUnary<NORM2>( OGComplexMatrix::create(new complex16[12]{{1,10},{4,40},{7,70},{10,100},{2,20},{5,50},{8,80},{11,110},{3,30},{6,60},{9,90},{12,120}},4,3, OWNER), OGRealScalar::create(255.894027746469), MATHSEQUAL)
  )
);
