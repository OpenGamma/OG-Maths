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
 * Check CTRANSPOSE node behaves correctly
 */

UNARY_NODE_TEST_SETUP(CTRANSPOSE)

INSTANTIATE_NODE_TEST_CASE_P(CTRANSPOSETests,CTRANSPOSE,
  Values
  (

  // Test scalar context real space

  // ctranspose(1) = 1
  new CheckUnary<CTRANSPOSE>( OGRealScalar::create(1.0), OGRealScalar::create(1.0), MATHSEQUAL),
  // ctranspose(-1) = -1
  new CheckUnary<CTRANSPOSE>( OGRealScalar::create(-1.0), OGRealScalar::create(-1.0), MATHSEQUAL),
  // ctranspose(0) = 0
  new CheckUnary<CTRANSPOSE>( OGRealScalar::create(0.0), OGRealScalar::create(0.0), MATHSEQUAL),
  // ctranspose(10) = 10
  new CheckUnary<CTRANSPOSE>( OGRealScalar::create(10.0), OGRealScalar::create(10), MATHSEQUAL),

  // Test matrix context real space

  // ctranspose(1) = 1
  new CheckUnary<CTRANSPOSE>( OGRealMatrix::create(new real16[1]{1},1,1, OWNER), OGRealScalar::create(1.0), MATHSEQUAL),
  // ctranspose(-1) = -1
  new CheckUnary<CTRANSPOSE>( OGRealMatrix::create(new real16[1]{-1},1,1, OWNER), OGRealScalar::create(-1.0),
  MATHSEQUAL),
  // ctranspose(0) = 0
  new CheckUnary<CTRANSPOSE>( OGRealMatrix::create(new real16[1]{0.e0},1,1, OWNER), OGRealScalar::create(0.0), MATHSEQUAL),
  // ctranspose(4x3 system)
  new CheckUnary<CTRANSPOSE>(
      OGRealMatrix::create(new real16[12] {1.,-4.,7.,-12.,2.,2.,9.,4.,3.,1.,11.,7.},4,3, OWNER),
      OGRealMatrix::create(new real16[12] {1,2,3,-4,2,1,7,9,11,-12,4,7},3,4, OWNER),
      MATHSEQUAL),

  // Test matrix context complex space

  // ctranspose(1+0i) = 1
  new CheckUnary<CTRANSPOSE>( OGComplexScalar::create({1.0,0}), OGRealScalar::create(1.0), MATHSEQUAL),
  // ctranspose(0+1i) = -i
  new CheckUnary<CTRANSPOSE>( OGComplexScalar::create({0,1.0}), OGComplexScalar::create({0,-1.0}), MATHSEQUAL),
  // ctranspose(0+0i) = 0
  new CheckUnary<CTRANSPOSE>( OGComplexScalar::create({0,0}), OGComplexScalar::create({0,0}), MATHSEQUAL),
  // ctranspose(1+1i) = 1-1i
  new CheckUnary<CTRANSPOSE>( OGComplexScalar::create({1.0,1.0}), OGComplexScalar::create({1.0,-1.0}), MATHSEQUAL),
  // ctranspose(10-10i) = 10+10i
  new CheckUnary<CTRANSPOSE>( OGComplexScalar::create({10.0,-10.0}), OGComplexScalar::create({10.0,10.0}), MATHSEQUAL),
  // ctranspose(4x3 system)
  new CheckUnary<CTRANSPOSE>(
      OGComplexMatrix::create(new complex16[12]{{1,-10}, {-4,40}, {7,-70}, {-12,120}, {2,-20}, {2,-20}, {9,-90}, {4,-40}, {3,-30}, {1,-10}, {11,-110}, {7,-70}},4,3, OWNER),
      OGComplexMatrix::create(new complex16[12]{{1,10}, {2,20}, {3,30}, {-4,-40}, {2,20}, {1,10}, {7,70}, {9,90}, {11,110}, {-12,-120}, {4,40}, {7,70}},3,4, OWNER),
      MATHSEQUAL)
  )
);

// Reconstruction Tests, does ctranspose(ctranspose(A)) = A?

namespace testinternal {

using namespace librdag;
  real16 reals[12] = {1.,-4.,7.,-12.,2.,2.,9.,4.,3.,1.,11.,7.};
  OGTerminal::Ptr real = OGRealMatrix::create(reals,4,3);
  complex16 complexs[12] = {{1,-10}, {-4,40}, {7,-70}, {-12,120}, {2,-20}, {2,-20}, {9,-90}, {4,-40}, {3,-30}, {1,-10}, {11,-110}, {7,-70}};
  OGTerminal::Ptr complex = OGComplexMatrix::create(complexs,4,3);
}

// Reconstruction Testing
OGTerminal::Ptr terminals[] = { testinternal::real,
                            testinternal::complex};

class ReconstructCtransposeNodeTest: public ::testing::TestWithParam<OGTerminal::Ptr> {};

TEST_P(ReconstructCtransposeNodeTest, TerminalTypes)
{
  OGTerminal::Ptr A = GetParam();
  OGExpr::Ptr ct = OGExpr::Ptr{new CTRANSPOSE(A)};
  OGExpr::Ptr ctctA = OGExpr::Ptr{new CTRANSPOSE(ct)}; // use copy else there's two refs to one terminal floating about
  runtree(ctctA);
  EXPECT_TRUE(ctctA->getRegs()[0]->asOGTerminal()->mathsequals(A));
}

INSTANTIATE_TEST_CASE_P(CTRANSPOSETests, ReconstructCtransposeNodeTest, ::testing::ValuesIn(terminals));
