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

class norm2Test: public UnaryOpTest<NORM2>{};

TEST_P(norm2Test, SimpleAssertResultTrue) {
  CheckUnary<NORM2> * impl = GetParam();
  ASSERT_TRUE(impl->resultCorrect());
}

INSTANTIATE_TEST_CASE_P(NORM2Tests,norm2Test,
  Values
  (
  new CheckUnary<NORM2>( new OGRealScalar(1.0), new OGRealScalar(1.0), MATHSEQUAL),
  new CheckUnary<NORM2>( new OGRealScalar(-1.0), new OGRealScalar(1.0), MATHSEQUAL),
  new CheckUnary<NORM2>( new OGRealScalar(0.0), new OGRealScalar(0.0), MATHSEQUAL),
  new CheckUnary<NORM2>( new OGOwningRealMatrix(new real16[1]{-1},1,1), new OGRealScalar(1.0), MATHSEQUAL),
  new CheckUnary<NORM2>( new OGOwningRealMatrix(new real16[3]{1,2,3},1,3), new OGRealScalar(3.74165738677394), MATHSEQUAL),
  new CheckUnary<NORM2>( new OGOwningRealMatrix(new real16[12]{1,4,7,10,2,5,8,11,3,6,9,12},4,3), new OGRealScalar(25.4624074360364), MATHSEQUAL)
  )
);
