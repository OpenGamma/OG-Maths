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
#include "equals.hh"
#include "runtree.hh"

using namespace std;
using namespace librdag;
using namespace testnodes;
using ::testing::TestWithParam;
using ::testing::Values;

/*
 * Check SVD node behaves correctly
 */


TEST(SVDTests,CheckScalar)
{
  OGTerminal::Ptr one = OGRealScalar::create(1.0);
  OGTerminal::Ptr r0 = OGRealScalar::create(10.0);
  OGExpr::Ptr svd = SVD::create(r0);

  Dispatcher d;

  // Check selecting 0 (U)
  OGExpr::Ptr s0 = SELECTRESULT::create(svd, OGIntegerScalar::create(0));
  ExecutionList el0 = ExecutionList{s0};
  for (auto it = el0.begin(); it != el0.end(); ++it)
  {
    d.dispatch(*it);
  }
  OGNumeric::Ptr answer = s0->getRegs()[0];
  EXPECT_TRUE((*one) ==~ (*(answer->asOGTerminal())));

  // Check selecting 1 (S)
  OGExpr::Ptr s1 = SELECTRESULT::create(svd, OGIntegerScalar::create(1));
  ExecutionList el1 = ExecutionList{s1};
  for (auto it = el1.begin(); it != el1.end(); ++it)
  {
    d.dispatch(*it);
  }
  answer = s1->getRegs()[0];
  EXPECT_TRUE((*r0) ==~ (*(answer->asOGTerminal())));

  // Check selecting 2 (V)
  OGExpr::Ptr s2 = SELECTRESULT::create(svd, OGIntegerScalar::create(2));
  ExecutionList el2 = ExecutionList{s2};
  for (auto it = el2.begin(); it != el2.end(); ++it)
  {
    d.dispatch(*it);
  }
  answer = s2->getRegs()[0];
  EXPECT_TRUE((*one) ==~ (*(answer->asOGTerminal())));
}


TEST(SVDTests,CheckRealDenseMatrix)
{

  // answers
  OGTerminal::Ptr U = OGRealDenseMatrix::create(new real8[9] {-0.2298476964000714,-0.5247448187602936,-0.8196419411205156,0.8834610176985253,0.2407824921325463,-0.4018960334334317,0.4082482904638627,-0.8164965809277263,0.4082482904638631},3,3,OWNER);
  OGTerminal::Ptr S = OGRealDiagonalMatrix::create(new real8[2] {9.525518091565107,  0.514300580658644},3,2,OWNER);
  OGTerminal::Ptr VT = OGRealDenseMatrix::create(new real8[4]{-0.6196294838293402,-0.7848944532670524,-0.7848944532670524,0.6196294838293402}, 2,2, OWNER);

  // input
  OGTerminal::Ptr M = OGRealDenseMatrix::create(new real8[6]{1,3,5,2,4,6},3,2,OWNER);
  OGExpr::Ptr svd = SVD::create(M);

  // computed answer pointers
  OGNumeric::Ptr answerU, answerS, answerVT, reconstruct;

  Dispatcher d;

  // Check selecting 0 (U)
  OGExpr::Ptr s0 = SELECTRESULT::create(svd, OGIntegerScalar::create(0));
  ExecutionList el0 = ExecutionList{s0};
  for (auto it = el0.begin(); it != el0.end(); ++it)
  {
    d.dispatch(*it);
  }
  answerU = s0->getRegs()[0];
  EXPECT_TRUE((*U) % (answerU->asOGTerminal()));

  // Check selecting 1 (S)
  OGExpr::Ptr s1 = SELECTRESULT::create(svd, OGIntegerScalar::create(1));
  ExecutionList el1 = ExecutionList{s1};
  for (auto it = el1.begin(); it != el1.end(); ++it)
  {
    d.dispatch(*it);
  }
  answerS = s1->getRegs()[0];
  EXPECT_TRUE((*S) ==~ (*(answerS->asOGTerminal())));

  // Check selecting 2 (V)
  OGExpr::Ptr s2 = SELECTRESULT::create(svd, OGIntegerScalar::create(2));
  ExecutionList el2 = ExecutionList{s2};
  for (auto it = el2.begin(); it != el2.end(); ++it)
  {
    d.dispatch(*it);
  }
  answerVT = s2->getRegs()[0];
  EXPECT_TRUE((*VT) % (answerVT->asOGTerminal()));

  // reconstruction test i.e. recover A from U,S,V**T as A=U*S*V**T
  OGExpr::Ptr m1 = MTIMES::create(answerU, answerS);
  OGExpr::Ptr m2 = MTIMES::create(m1, answerVT);
  ExecutionList el3 = ExecutionList{m2};
  for (auto it = el3.begin(); it != el3.end(); ++it)
  {
    d.dispatch(*it);
  }
  reconstruct = m2->getRegs()[0];
  EXPECT_TRUE((*M) ==~ (*(reconstruct->asOGTerminal())));

  // Check sending in a non finite number throws unrecoverable error
  OGTerminal::Ptr Mbad = OGRealDenseMatrix::create(new real8[6]{std::numeric_limits<real8>::signaling_NaN(),3,5,2,4,6},3,2,OWNER);
  svd = SVD::create(Mbad);
  EXPECT_THROW(runtree(svd), rdag_unrecoverable_error);
}

// MAT-404, test demonstrates fixing of bug.
TEST(SVDTests,CheckRealHVector)
{

  // answers
  OGRealDenseMatrix::Ptr U = OGRealDenseMatrix::create(new real8[1] {1},1,1,OWNER);
  OGTerminal::Ptr S = OGRealDiagonalMatrix::create(new real8[3] {3.74165738677394,0,0 },1,3,OWNER);
  OGTerminal::Ptr VT = OGRealDenseMatrix::create(new real8[9]{0.2672612419124243,-0.5345224838248488,-0.8017837257372732,0.5345224838248488,0.7745419205884383,-0.3381871191173426,0.8017837257372732,-0.3381871191173427,0.4927193213239860}, 3,3, OWNER);

  // input
  OGTerminal::Ptr M = OGRealDenseMatrix::create(new real8[3]{1,2,3},1,3,OWNER);
  SVD::Ptr svd = SVD::create(M);

  // computed answer pointers
  OGNumeric::Ptr answerU, answerS, answerVT, reconstruct;

  Dispatcher d;

  // Check selecting 0 (U)
  SELECTRESULT::Ptr s0 = SELECTRESULT::create(svd, OGIntegerScalar::create(0));
  ExecutionList el0{s0};
  for (auto it = el0.begin(); it != el0.end(); ++it)
  {
    d.dispatch(*it);
  }
  answerU = s0->getRegs()[0];
  EXPECT_TRUE((*U) % (answerU->asOGTerminal()));

  // Check selecting 1 (S)
  SELECTRESULT::Ptr s1 = SELECTRESULT::create(svd, OGIntegerScalar::create(1));
  ExecutionList el1{s1};
  for (auto it = el1.begin(); it != el1.end(); ++it)
  {
    d.dispatch(*it);
  }
  answerS = s1->getRegs()[0];
  EXPECT_TRUE((*S) ==~ *(answerS->asOGTerminal()));

  // Check selecting 2 (V)
  SELECTRESULT::Ptr s2 = SELECTRESULT::create(svd, OGIntegerScalar::create(2));
  ExecutionList el2{s2};
  for (auto it = el2.begin(); it != el2.end(); ++it)
  {
    d.dispatch(*it);
  }
  answerVT = s2->getRegs()[0];
  EXPECT_TRUE((*VT) % (answerVT->asOGTerminal()));

  // reconstruction test i.e. recover A from U,S,V**T as A=U*S*V**T
  MTIMES::Ptr m1 = MTIMES::create(answerU, answerS);
  MTIMES::Ptr m2 = MTIMES::create(m1, answerVT);
  ExecutionList el3{m2};
  for (auto it = el3.begin(); it != el3.end(); ++it)
  {
    d.dispatch(*it);
  }
  reconstruct = m2->getRegs()[0];
  EXPECT_TRUE((*M) ==~ *(reconstruct->asOGTerminal()));
}


TEST(SVDTests,CheckComplexDenseMatrix)
{

  // answers
  OGTerminal::Ptr U = OGComplexDenseMatrix::create(new complex16[9] {{     -0.0228707006002169,      -0.2287070060021659}, {     -0.0522140610036492,      -0.5221406100364927}, {     -0.0815574214070819,      -0.8155742140708198}, {     -0.0879076568710847,      -0.8790765687107978}, {     -0.0239587534423321,      -0.2395875344233279}, {      0.0399901499864153,       0.3999014998641418}, {      0.3147401422050641,       0.2600102104752862}, {     -0.6294802844101258,      -0.5200204209505755}, {      0.3147401422050625,       0.2600102104752883}},3,3,OWNER);
  OGTerminal::Ptr S = OGRealDiagonalMatrix::create(new real8[2] {95.7302720469661779,5.1686568674896343},3,2,OWNER);
  OGTerminal::Ptr VT = OGComplexDenseMatrix::create(new complex16[4]{{-0.6196294838293402,0},{0.7848944532670524,-0.e0},{-0.7848944532670524,0},{-0.6196294838293402,0}}, 2,2, OWNER);

  // input
  OGTerminal::Ptr M = OGComplexDenseMatrix::create(new complex16[6]{{1,10}, {3,30}, {5,50}, {2,20}, {4,40}, {6,60}},3,2,OWNER);
  OGExpr::Ptr svd = SVD::create(M);

  // computed answer pointers
  OGNumeric::Ptr answerU, answerS, answerVT, reconstruct;

  Dispatcher d;

  // Check selecting 0 (U)
  OGExpr::Ptr s0 = SELECTRESULT::create(svd, OGIntegerScalar::create(0));
  ExecutionList el0 = ExecutionList{s0};
  for (auto it = el0.begin(); it != el0.end(); ++it)
  {
    d.dispatch(*it);
  }
  answerU = s0->getRegs()[0];
  EXPECT_TRUE((*U) % (answerU->asOGTerminal()));

  // Check selecting 1 (S)
  OGExpr::Ptr s1 = SELECTRESULT::create(svd, OGIntegerScalar::create(1));
  ExecutionList el1 = ExecutionList{s1};
  for (auto it = el1.begin(); it != el1.end(); ++it)
  {
    d.dispatch(*it);
  }
  answerS = s1->getRegs()[0];
  EXPECT_TRUE((*S) ==~ (*(answerS->asOGTerminal())));

  // Check selecting 2 (V)
  OGExpr::Ptr s2 = SELECTRESULT::create(svd, OGIntegerScalar::create(2));
  ExecutionList el2 = ExecutionList{s2};
  for (auto it = el2.begin(); it != el2.end(); ++it)
  {
    d.dispatch(*it);
  }
  answerVT = s2->getRegs()[0];
  EXPECT_TRUE((*VT) % (answerVT->asOGTerminal()));

  // reconstruction test i.e. recover A from U,S,V**T as A=U*S*V**T
  OGExpr::Ptr m1 = MTIMES::create(answerU, answerS);
  OGExpr::Ptr m2 = MTIMES::create(m1, answerVT);
  ExecutionList el3 = ExecutionList{m2};
  for (auto it = el3.begin(); it != el3.end(); ++it)
  {
    d.dispatch(*it);
  }
  reconstruct = m2->getRegs()[0];
  // FP fuzz causes grief on reconstruction
  EXPECT_TRUE(ArrayFuzzyEquals(M->asOGComplexDenseMatrix()->getData(),reconstruct->asOGComplexDenseMatrix()->getData(),1e-15,1e-15));

  // Check sending in a non finite number throws unrecoverable error
  OGTerminal::Ptr Mbad = OGComplexDenseMatrix::create(new complex16[6]{{std::numeric_limits<real8>::signaling_NaN(),10}, {3,30}, {5,50}, {2,20}, {4,40}, {6,60}},3,2,OWNER);
  svd = SVD::create(Mbad);
  EXPECT_THROW(runtree(svd), rdag_unrecoverable_error);
}
