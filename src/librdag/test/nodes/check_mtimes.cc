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
 * Check MTIMES node behaves
 */

BINARY_NODE_TEST_SETUP(MTIMES)

INSTANTIATE_NODE_TEST_CASE_P(MTIMESTests,MTIMES,
  Values
  (
  new CheckBinary<MTIMES>( new OGRealScalar(2.0), new OGRealScalar(3.0), new OGRealScalar(6.0),MATHSEQUAL),
  new CheckBinary<MTIMES>( new OGOwningRealMatrix(new real16[6]{1,3,5,2,4,6},3,2), new OGOwningRealMatrix(new real16[2]{10,20},2,1),new OGOwningRealMatrix(new real16[3]{50,110,170},3,1), MATHSEQUAL)
  )
);
