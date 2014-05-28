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
  new CheckUnary<NORM2>( OGNumeric::Ptr{new OGRealScalar(1.0)}, OGNumeric::Ptr{new OGRealScalar(1.0)}, MATHSEQUAL),
  new CheckUnary<NORM2>( OGNumeric::Ptr{new OGRealScalar(-1.0)}, OGNumeric::Ptr{new OGRealScalar(1.0)}, MATHSEQUAL),
  new CheckUnary<NORM2>( OGNumeric::Ptr{new OGRealScalar(0.0)}, OGNumeric::Ptr{new OGRealScalar(0.0)}, MATHSEQUAL),
  new CheckUnary<NORM2>( OGNumeric::Ptr{new OGRealMatrix(new real16[1]{-1},1,1, OWNER)}, OGNumeric::Ptr{new OGRealScalar(1.0)}, MATHSEQUAL),
  new CheckUnary<NORM2>( OGNumeric::Ptr{new OGRealMatrix(new real16[3]{1,2,3},1,3, OWNER)}, OGNumeric::Ptr{new OGRealScalar(3.74165738677394)}, MATHSEQUAL),
  new CheckUnary<NORM2>( OGNumeric::Ptr{new OGRealMatrix(new real16[12]{1,4,7,10,2,5,8,11,3,6,9,12},4,3, OWNER)}, OGNumeric::Ptr{new OGRealScalar(25.4624074360364)}, MATHSEQUAL),
  // test complex
  new CheckUnary<NORM2>( OGNumeric::Ptr{new OGComplexScalar({1.0,0})}, OGNumeric::Ptr{new OGRealScalar(1.0)}, MATHSEQUAL),
  new CheckUnary<NORM2>( OGNumeric::Ptr{new OGComplexScalar({0,1.0})}, OGNumeric::Ptr{new OGRealScalar(1.0)}, MATHSEQUAL),
  new CheckUnary<NORM2>( OGNumeric::Ptr{new OGComplexScalar({1.0,1.0})}, OGNumeric::Ptr{new OGRealScalar(std::sqrt(2))}, MATHSEQUAL),
new CheckUnary<NORM2>( OGNumeric::Ptr{new OGComplexScalar({-1.0,1.0})}, OGNumeric::Ptr{new OGRealScalar(std::sqrt(2))}, MATHSEQUAL),
  new CheckUnary<NORM2>( OGNumeric::Ptr{new OGComplexScalar({0,0})}, OGNumeric::Ptr{new OGRealScalar(0.0)}, MATHSEQUAL),
  new CheckUnary<NORM2>( OGNumeric::Ptr{new OGComplexMatrix(new complex16[3]{{1,10},{2,20},{3,30}},1,3, OWNER)}, OGNumeric::Ptr{new OGRealScalar(37.6031913539263)}, MATHSEQUAL),
  new CheckUnary<NORM2>( OGNumeric::Ptr{new OGComplexMatrix(new complex16[12]{{1,10},{4,40},{7,70},{10,100},{2,20},{5,50},{8,80},{11,110},{3,30},{6,60},{9,90},{12,120}},4,3, OWNER)}, OGNumeric::Ptr{new OGRealScalar(255.894027746469)}, MATHSEQUAL)
  )
);
