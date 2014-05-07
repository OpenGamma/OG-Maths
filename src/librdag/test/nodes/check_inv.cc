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
#include "equals.hh"
#include <limits>

using namespace std;
using namespace librdag;
using namespace testnodes;
using ::testing::TestWithParam;
using ::testing::Values;

/*
 * Check INV node behaves correctly
 */

UNARY_NODE_TEST_SETUP(INV)

INSTANTIATE_NODE_TEST_CASE_P(INVTests,INV,
  Values
  (

  // Test scalar context real space

  // inv(1) = 1
  new CheckUnary<INV>( new OGRealScalar(1.0), new OGRealScalar(1.0), MATHSEQUAL),
  // inv(-1) = -1
  new CheckUnary<INV>( new OGRealScalar(-1.0), new OGRealScalar(-1.0), MATHSEQUAL),
  // inv(0) = +inf
  new CheckUnary<INV>( new OGRealScalar(0.0), new OGRealScalar(std::numeric_limits<real16>::infinity()), MATHSEQUAL),
  // inv(10) = 0.1
  new CheckUnary<INV>( new OGRealScalar(10.0), new OGRealScalar(0.1), MATHSEQUAL),

  // Test matrix context real space

  // inv(1) = 1
  new CheckUnary<INV>( new OGRealMatrix(new real16[1]{1},1,1, OWNER), new OGRealScalar(1.0), MATHSEQUAL),
  // inv(-1) = -1
  new CheckUnary<INV>( new OGRealMatrix(new real16[1]{-1},1,1, OWNER), new OGRealScalar(-1.0),
  MATHSEQUAL),
  // inv(0) = +inf
  new CheckUnary<INV>( new OGRealMatrix(new real16[1]{0.e0},1,1, OWNER), new OGRealScalar(std::numeric_limits<real16>::infinity()), MATHSEQUAL),
  // inv(full rank 3x3 system) [condition number ~= 3.7]
  new CheckUnary<INV>(
      new OGRealMatrix(new real16[9]{1,-4,7,2,2,9,3,1,-4},3,3, OWNER),
      new OGRealMatrix(new real16[9] {0.0918918918918919,0.0486486486486487,0.2702702702702703,-0.1891891891891892,0.1351351351351351,-0.0270270270270270,0.0216216216216216,0.0702702702702703,-0.0540540540540541},3,3, OWNER),
      MATHSEQUAL),

  // Test matrix context complex space

  // inv(1+0i) = 1
  new CheckUnary<INV>( new OGComplexScalar({1.0,0}), new OGRealScalar(1.0), MATHSEQUAL),
  // inv(0+1i) = -i
  new CheckUnary<INV>( new OGComplexScalar({0,1.0}), new OGComplexScalar({0,-1.0}), MATHSEQUAL),
  // inv(0+0i) = inf
  new CheckUnary<INV>( new OGComplexScalar({0,0}), new OGComplexScalar({std::numeric_limits<real16>::infinity(),0}), MATHSEQUAL),
  // inv(1+1i) = 0.5-0.5i
  new CheckUnary<INV>( new OGComplexScalar({1.0,1.0}), new OGComplexScalar({0.5,-0.5}), MATHSEQUAL),
  // inv(full rank 3x3 system) [condition number ~= 4.9]
  new CheckUnary<INV>(  new OGComplexMatrix(new complex16[9]{{1.,10.}, {-4.,-40.}, {7.,70.}, {2.,20.}, {2.,20.}, {9.,90.}, {3.,30.}, {1.,10.}, {11.,11.}},3,3, OWNER),
      new OGComplexMatrix(new complex16[9] {{0.0015906219620334,-0.0048867825577376}, {0.0026942738518561, 0.0088704319348774}, {0.0009739401444214,-0.0372879941007054}, {-0.0019412404140251, 0.0183104604339916}, {0.0011167112286758,-0.0147484293321016}, {-0.0000973940144421, 0.0037287994100705}, {0.0000779152115537,-0.0029830395280564}, {0.0002532244375496,-0.0096948784661834}, {     -0.0001947880288843, 0.0074575988201411}},3,3, OWNER),
      MATHSEQUAL)
  )
);

// Check invalid/bad input

TEST(INVTests, NonSquareInput)
{
  real16 * rdat = new real16[6]{1,2,3,4,5,6};
  complex16 * cdat = new complex16[6]{{1,10},{2,20},{3,30},{4,40},{5,50},{6,60}};
  OGTerminal * mat;
  OGExpr * inv;

  // check throws as matrix must be square

  // real space: try with more rows than cols
  mat = new OGRealMatrix(rdat,3,2,VIEWER);
  inv = new INV(mat);
  ASSERT_THROW(runtree(inv), rdag_error);
  delete inv;

  // real space: try with more cols than rows
  mat = new OGRealMatrix(rdat,2,3,VIEWER);
  inv = new INV(mat);
  ASSERT_THROW(runtree(inv), rdag_error);
  delete inv;

  // complex space: try with more rows than cols
  mat = new OGComplexMatrix(cdat,3,2,VIEWER);
  inv = new INV(mat);
  ASSERT_THROW(runtree(inv), rdag_error);
  delete inv;

  // complex space try with more cols than rows
  mat = new OGComplexMatrix(cdat,2,3,VIEWER);
  inv = new INV(mat);
  ASSERT_THROW(runtree(inv), rdag_error);
  delete inv;

  // clean up
  delete [] rdat;
  delete [] cdat;
}

// Explicitly test singular systems
// NOTE: This test is a place holder for asserting that a warning is raised

TEST(INVTests, WarnOnSingularInput)
{
  // singular 3x3
  real16 rsingular3x3[9] = {1,10,1,2,20,2,3,30,3};
  complex16 csingular3x3[9] = {{1,10},{10,100},{1,10},{2,20},{20,200},{2,20},{3,30},{30,300},{3,30}};

  OGTerminal * mat;
  OGExpr * inv;

  // real space
  mat = new OGRealMatrix(rsingular3x3,3,3,VIEWER);
  inv = new INV(mat);
  // TODO: assert warn check goes here
  delete inv;

  // complex space
  mat = new OGComplexMatrix(csingular3x3,3,3,VIEWER);
  inv = new INV(mat);
  // TODO: assert warn check goes here
  delete inv;

}

// Reconstruction Tests, does A*inv(A) = identity matrix?

namespace testinternal {

using namespace librdag;
  real16 reals[9] = {1,-4,7,2,2,9,3,1,11};
  OGRealMatrix * real = new OGRealMatrix(reals,3,3);
  complex16 complexs[9] = {{1,10},{-4,-40},{7,70},{2,20},{2,20},{9,90},{3,30},{1,10},{11,110}};
  OGComplexMatrix * complex = new OGComplexMatrix(complexs,3,3);
}

// Reconstruction Testing
const librdag::OGTerminal * terminals[] = { testinternal::real,
                                           testinternal::complex};

class ReconstructInvNodeTest: public ::testing::TestWithParam<const OGTerminal*> {};

TEST_P(ReconstructInvNodeTest, TerminalTypes)
{
  const OGTerminal * A = GetParam();
  OGExpr * inv = new INV(A);
  OGExpr * AtimesInvA = new MTIMES(A->createOwningCopy(),inv); // use copy else there's two refs to one terminal floating about
  OGRealMatrix * expected = new OGRealMatrix(new real16[9] {1,0,0,0,1,0,0,0,1},3,3, OWNER);
  runtree(AtimesInvA);
  EXPECT_TRUE(AtimesInvA->getRegs()[0]->asOGTerminal()->mathsequals(expected, 1e-14, 1e-14));
  delete AtimesInvA;
  delete expected;
}

INSTANTIATE_TEST_CASE_P(INVTests, ReconstructInvNodeTest, ::testing::ValuesIn(terminals));

