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
  // scalar * scalar
  new CheckBinary<MTIMES>( new OGRealScalar(2.0), new OGRealScalar(3.0), new OGRealScalar(6.0),MATHSEQUAL),
  // scalar * matrix
  new CheckBinary<MTIMES>( new OGRealMatrix(new real16[1]{2},1,1,OWNER), new OGRealMatrix(new real16[2]{10,20},2,1,OWNER),new OGRealMatrix(new real16[2]{20,40},2,1,OWNER), MATHSEQUAL),
   // matrix * scalar
  new CheckBinary<MTIMES>( new OGRealMatrix(new real16[2]{10,20},2,1,OWNER), new OGRealMatrix(new real16[1]{2},1,1,OWNER), new OGRealMatrix(new real16[2]{20,40},2,1,OWNER), MATHSEQUAL),
  // matrix * vector
  new CheckBinary<MTIMES>( new OGRealMatrix(new real16[6]{1,3,5,2,4,6},3,2,OWNER), new OGRealMatrix(new real16[2]{10,20},2,1,OWNER),new OGRealMatrix(new real16[3]{50,110,170},3,1,OWNER), MATHSEQUAL),
  // matrix * matrix
  new CheckBinary<MTIMES>( new OGRealMatrix(new real16[6]{1,3,5,2,4,6},3,2,OWNER), new OGRealMatrix(new real16[4]{10,30,20,40},2,2,OWNER),new OGRealMatrix(new real16[6]{70,150,230,100,220,340},3,2,OWNER), MATHSEQUAL),
   // cscalar * cscalar
  new CheckBinary<MTIMES>( new OGComplexScalar({2.0,4.0}), new OGComplexScalar({3.0,5.e0}), new OGComplexScalar({-14,22}),MATHSEQUAL)

  )
);
