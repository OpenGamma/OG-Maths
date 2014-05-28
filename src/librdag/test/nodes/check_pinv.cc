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

using namespace std;
using namespace librdag;
using namespace testnodes;
using ::testing::TestWithParam;
using ::testing::Values;

/*
 * Check PINV node behaves correctly
 */

UNARY_NODE_TEST_SETUP(PINV)

INSTANTIATE_NODE_TEST_CASE_P(PINVTests,PINV,
  Values
  (

  // Test scalar context real space

  // pinv(1) = 1
  new CheckUnary<PINV>( OGNumeric::Ptr{new OGRealScalar(1.0)}, OGNumeric::Ptr{new OGRealScalar(1.0)}, MATHSEQUAL),
  // pinv(-1) = -1
  new CheckUnary<PINV>( OGNumeric::Ptr{new OGRealScalar(-1.0)}, OGNumeric::Ptr{new OGRealScalar(-1.0)}, MATHSEQUAL),
  // pinv(0) = 0
  new CheckUnary<PINV>( OGNumeric::Ptr{new OGRealScalar(0.0)}, OGNumeric::Ptr{new OGRealScalar(0.0)}, MATHSEQUAL),
  // pinv(10) = 0.1
  new CheckUnary<PINV>( OGNumeric::Ptr{new OGRealScalar(10.0)}, OGNumeric::Ptr{new OGRealScalar(0.1)}, MATHSEQUAL),

  // Test matrix context real space

  // pinv(1) = 1
  new CheckUnary<PINV>( OGNumeric::Ptr{new OGRealMatrix(new real16[1]{1},1,1, OWNER)}, OGNumeric::Ptr{new OGRealScalar(1.0)}, MATHSEQUAL),
  // pinv(-1) = -1
  new CheckUnary<PINV>( OGNumeric::Ptr{new OGRealMatrix(new real16[1]{-1},1,1, OWNER)}, OGNumeric::Ptr{new OGRealScalar(-1.0)},
  MATHSEQUAL),
  // pinv(0) = 0
  new CheckUnary<PINV>( OGNumeric::Ptr{new OGRealMatrix(new real16[1]{0.e0},1,1, OWNER)}, OGNumeric::Ptr{new OGRealScalar(0.0)}, MATHSEQUAL),
  // pinv(full rank system) [condition number ~= 13]
  new CheckUnary<PINV>(
      OGNumeric::Ptr{new OGRealMatrix(new real16[12] {1.,-4.,7.,-12.,2.,2.,9.,4.,3.,1.,11.,7.},4,3, OWNER)},
      OGNumeric::Ptr{new OGRealMatrix(new real16[12] {0.0142560142560143,-0.1236907236907242,0.1100683100683104,-0.0598455598455599,0.5727155727155739,-0.4266409266409274,0.0299970299970300,0.0800118800118803,0.0024354024354022,-0.0446985446985447,-0.1545391545391548,0.1528066528066529},3,4, OWNER)},
      MATHSEQUAL),
  // pinv(rank 2 system of size 4) [condition number ~=1.4e16]
  new CheckUnary<PINV>(
      OGNumeric::Ptr{new OGRealMatrix(new real16[12]{1,4,7,10,2,5,8,11,3,6,9,12},4,3, OWNER)},
      OGNumeric::Ptr{new OGRealMatrix(new real16[12]{-0.4833333333333338,-0.0333333333333326,0.4166666666666664,-0.2444444444444440,-0.0111111111111107,0.2222222222222215,-0.0055555555555554,0.0111111111111111,0.0277777777777776,0.2333333333333329,0.0333333333333330,-0.1666666666666660},3,4,OWNER)},
      MATHSEQUAL),


  // Test matrix context complex space

  // pinv(1+0i) = 1
  new CheckUnary<PINV>( OGNumeric::Ptr{new OGComplexScalar({1.0,0})}, OGNumeric::Ptr{new OGRealScalar(1.0)}, MATHSEQUAL),
  // pinv(0+1i) = -i
  new CheckUnary<PINV>( OGNumeric::Ptr{new OGComplexScalar({0,1.0})}, OGNumeric::Ptr{new OGComplexScalar({0,-1.0})}, MATHSEQUAL),
  // pinv(0+0i) = 0
  new CheckUnary<PINV>( OGNumeric::Ptr{new OGComplexScalar({0,0})}, OGNumeric::Ptr{new OGComplexScalar({0,0})}, MATHSEQUAL),
  // pinv(1+1i) = 0.5-0.5i
  new CheckUnary<PINV>( OGNumeric::Ptr{new OGComplexScalar({1.0,1.0})}, OGNumeric::Ptr{new OGComplexScalar({0.5,-0.5})}, MATHSEQUAL),
  // pinv(full rank system) [condition number ~= 13]
  new CheckUnary<PINV>(
      OGNumeric::Ptr{new OGComplexMatrix(new complex16[12]{{1,10}, {-4,-40}, {7,70}, {-12,-120}, {2,20}, {2,20}, {9,90}, {4,40}, {3,30}, {1,10}, {11,110}, {7,70}},4,3, OWNER)},
      OGNumeric::Ptr{new OGComplexMatrix(new complex16[12] {{0.0001411486560001,-0.0014114865600014}, {-0.0012246606306012, 0.0122466063060123}, {0.0010897852482011,-0.0108978524820109}, {-0.0005925302955006, 0.0059253029550059}, {0.0056704512150057,-0.0567045121500567}, {-0.0042241675905042, 0.0422416759050422}, {0.0002970002970003,-0.0029700029700030}, {0.0007921968318008,-0.0079219683180079}, {0.0000241128954000,-0.0002411289540002}, {-0.0004425598485004, 0.0044255984850044}, {-0.0015300906390015, 0.0153009063900153}, {0.0015129371565015,-0.0151293715650151}},3,4, OWNER)},
      MATHSEQUAL),
  // pinv(rank 2 system of size 4) [condition number ~=1e16]
  new CheckUnary<PINV>(
      OGNumeric::Ptr{new OGComplexMatrix(new complex16[12]{{1,10},{4,40},{7,70},{10,100},{2,20},{5,50},{8,80},{11,110},{3,30},{6,60},{9,90},{12,120}},4,3, OWNER)},
      OGNumeric::Ptr{new OGComplexMatrix(new complex16[12]{{-0.0047854785478551, 0.0478547854785478}, {-0.0003300330033002, 0.0033003300330034}, {0.0041254125412542,-0.0412541254125413}, {-0.0024202420242024, 0.0242024202420241}, {-0.0001100110011000, 0.0011001100110011}, {0.0022002200220021,-0.0220022002200220}, {-0.0000550055005500, 0.0005500550055006}, {0.0001100110011001,-0.0011001100110011}, {0.0002750275027502,-0.0027502750275028}, {0.0023102310231024,-0.0231023102310231}, {0.0003300330033003,-0.0033003300330033}, {-0.0016501650165016, 0.0165016501650165}},3,4,OWNER)},
      MATHSEQUAL)
  )
);


// Reconstruction Tests, does A*pinv(A) = identity matrix?

namespace testinternal {

real16 reals[9] = {1,-4,7,2,2,9,3,1,11};
OGTerminal::Ptr real = OGTerminal::Ptr{new OGRealMatrix(reals,3,3)};
complex16 complexs[9] = {{1,10},{-4,-40},{7,70},{2,20},{2,20},{9,90},{3,30},{1,10},{11,110}};
OGTerminal::Ptr complex = OGTerminal::Ptr{new OGComplexMatrix(complexs,3,3)};

} // end namespace testinternal

// Reconstruction Testing
OGTerminal::Ptr terminals[] = { testinternal::real,
                            testinternal::complex};

class ReconstructPinvNodeTest: public ::testing::TestWithParam<OGTerminal::Ptr> {};

TEST_P(ReconstructPinvNodeTest, TerminalTypes)
{
  OGTerminal::Ptr A = GetParam();
  OGExpr::Ptr pinv = OGExpr::Ptr{new PINV(A)};
  OGExpr::Ptr AtimesPinvA = OGExpr::Ptr{new MTIMES(A, pinv)};
  OGTerminal::Ptr expected = OGTerminal::Ptr{new OGRealMatrix(new real16[9] {1,0,0,0,1,0,0,0,1},3,3, OWNER)};
  runtree(AtimesPinvA);
  EXPECT_TRUE(AtimesPinvA->getRegs()[0]->asOGTerminal()->mathsequals(expected, 1e-14, 1e-14));
}

INSTANTIATE_TEST_CASE_P(PINVTests, ReconstructPinvNodeTest, ::testing::ValuesIn(terminals));

