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

using namespace std;
using namespace librdag;
using namespace testnodes;
using ::testing::TestWithParam;
using ::testing::Values;

/*
 * Check TRANSPOSE node behaves correctly
 */

UNARY_NODE_TEST_SETUP(TRANSPOSE)

INSTANTIATE_NODE_TEST_CASE_P(TRANSPOSETests,TRANSPOSE,
  Values
  (

  // Test scalar context real space

  // transpose(1) = 1
  new CheckUnary<TRANSPOSE>( pOGNumeric{new OGRealScalar(1.0)}, pOGNumeric{new OGRealScalar(1.0)}, MATHSEQUAL),
  // transpose(-1) = -1
  new CheckUnary<TRANSPOSE>( pOGNumeric{new OGRealScalar(-1.0)}, pOGNumeric{new OGRealScalar(-1.0)}, MATHSEQUAL),
  // transpose(0) = 0
  new CheckUnary<TRANSPOSE>( pOGNumeric{new OGRealScalar(0.0)}, pOGNumeric{new OGRealScalar(0.0)}, MATHSEQUAL),
  // transpose(10) = 10
  new CheckUnary<TRANSPOSE>( pOGNumeric{new OGRealScalar(10.0)}, pOGNumeric{new OGRealScalar(10)}, MATHSEQUAL),

  // Test matrix context real space

  // transpose(1) = 1
  new CheckUnary<TRANSPOSE>( pOGNumeric{new OGRealMatrix(new real16[1]{1},1,1, OWNER)}, pOGNumeric{new OGRealScalar(1.0)}, MATHSEQUAL),
  // transpose(-1) = -1
  new CheckUnary<TRANSPOSE>( pOGNumeric{new OGRealMatrix(new real16[1]{-1},1,1, OWNER)}, pOGNumeric{new OGRealScalar(-1.0)},
  MATHSEQUAL),
  // transpose(0) = 0
  new CheckUnary<TRANSPOSE>( pOGNumeric{new OGRealMatrix(new real16[1]{0.e0},1,1, OWNER)}, pOGNumeric{new OGRealScalar(0.0)}, MATHSEQUAL),
   // transpose vector
  new CheckUnary<TRANSPOSE>(
    pOGNumeric{new OGRealMatrix(new real16[3]{1,2,3},1,3, OWNER)},
    pOGNumeric{new OGRealMatrix(new real16[3]{1,2,3},3,1, OWNER)},
    MATHSEQUAL),
  // transpose(4x3 system)
  new CheckUnary<TRANSPOSE>(
      pOGNumeric{new OGRealMatrix(new real16[12] {1.,-4.,7.,-12.,2.,2.,9.,4.,3.,1.,11.,7.},4,3, OWNER)},
      pOGNumeric{new OGRealMatrix(new real16[12] {1,2,3,-4,2,1,7,9,11,-12,4,7},3,4, OWNER)},
      MATHSEQUAL),

  // Test matrix context complex space

  // transpose(1+0i) = 1
  new CheckUnary<TRANSPOSE>( pOGNumeric{new OGComplexScalar({1.0,0})}, pOGNumeric{new OGRealScalar(1.0)}, MATHSEQUAL),
  // transpose(0+1i) = i
  new CheckUnary<TRANSPOSE>( pOGNumeric{new OGComplexScalar({0,1.0})}, pOGNumeric{new OGComplexScalar({0,1.0})}, MATHSEQUAL),
  // transpose(0+0i) = 0
  new CheckUnary<TRANSPOSE>( pOGNumeric{new OGComplexScalar({0,0})}, pOGNumeric{new OGComplexScalar({0,0})}, MATHSEQUAL),
  // transpose(1+1i) = 1+1i
  new CheckUnary<TRANSPOSE>( pOGNumeric{new OGComplexScalar({1.0,1.0})}, pOGNumeric{new OGComplexScalar({1.0,1.0})}, MATHSEQUAL),
  // transpose(10-10i) = 10-10i
  new CheckUnary<TRANSPOSE>( pOGNumeric{new OGComplexScalar({10.0,-10.0})}, pOGNumeric{new OGComplexScalar({10.0,-10.0})}, MATHSEQUAL),
   // transpose vector
  new CheckUnary<TRANSPOSE>(
    pOGNumeric{new OGComplexMatrix(new complex16[3]{{1,10},{2,20},{3,30}},1,3, OWNER)},
    pOGNumeric{new OGComplexMatrix(new complex16[3]{{1,10},{2,20},{3,30}},3,1, OWNER)},
    MATHSEQUAL),
  // transpose(4x3 system)
  new CheckUnary<TRANSPOSE>(
      pOGNumeric{new OGComplexMatrix(new complex16[12]{{1,-10}, {-4,40}, {7,-70}, {-12,120}, {2,-20}, {2,-20}, {9,-90}, {4,-40}, {3,-30}, {1,-10}, {11,-110}, {7,-70}},4,3, OWNER)},
      pOGNumeric{new OGComplexMatrix(new complex16[12]{{1,-10}, {2,-20}, {3,-30}, {-4,40}, {2,-20}, {1,-10}, {7,-70}, {9,-90}, {11,-110}, {-12,120}, {4,-40}, {7,-70}},3,4, OWNER)},
      MATHSEQUAL)
  )
);

// Reconstruction Tests, does transpose(transpose(A)) = A?

namespace testinternal {

using namespace librdag;
  real16 reals[12] = {1.,-4.,7.,-12.,2.,2.,9.,4.,3.,1.,11.,7.};
  pOGTerminal real = pOGTerminal{new OGRealMatrix(reals,4,3)};
  complex16 complexs[12] = {{1,-10}, {-4,40}, {7,-70}, {-12,120}, {2,-20}, {2,-20}, {9,-90}, {4,-40}, {3,-30}, {1,-10}, {11,-110}, {7,-70}};
  pOGTerminal complex = pOGTerminal{new OGComplexMatrix(complexs,4,3)};
}

// Reconstruction Testing
pOGTerminal terminals[] = { testinternal::real,
                            testinternal::complex};

class ReconstructTransposeNodeTest: public ::testing::TestWithParam<pOGTerminal> {};

TEST_P(ReconstructTransposeNodeTest, TerminalTypes)
{
  pOGTerminal A = GetParam();
  pOGExpr t = pOGExpr{new TRANSPOSE(A)};
  pOGExpr ttA = pOGExpr{new TRANSPOSE(t)}; 
  runtree(ttA);
  EXPECT_TRUE(ttA->getRegs()[0]->asOGTerminal()->mathsequals(A));
}

INSTANTIATE_TEST_CASE_P(TRANSPOSETests, ReconstructTransposeNodeTest, ::testing::ValuesIn(terminals));
