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
  pOGTerminal one = pOGTerminal{new OGRealScalar(1.0)};
  pOGTerminal r0 = pOGTerminal{new OGRealScalar(10.0)};
  pOGExpr svd = pOGExpr{new SVD(r0)};

  Dispatcher d;

  // Check selecting 0 (U)
  pOGExpr s0 = pOGExpr{new SELECTRESULT(svd, pOGNumeric{new OGIntegerScalar(0)})};
  ExecutionList el0 = ExecutionList{s0};
  for (auto it = el0.begin(); it != el0.end(); ++it)
  {
    d.dispatch(*it);
  }
  pOGNumeric answer = s0->getRegs()[0];
  EXPECT_TRUE((*one) ==~ (*(answer->asOGTerminal())));

  // Check selecting 1 (S)
  pOGExpr s1 = pOGExpr{new SELECTRESULT(svd, pOGNumeric{new OGIntegerScalar(1)})};
  ExecutionList el1 = ExecutionList{s1};
  for (auto it = el1.begin(); it != el1.end(); ++it)
  {
    d.dispatch(*it);
  }
  answer = s1->getRegs()[0];
  EXPECT_TRUE((*r0) ==~ (*(answer->asOGTerminal())));

  // Check selecting 2 (V)
  pOGExpr s2 = pOGExpr{new SELECTRESULT(svd, pOGNumeric{new OGIntegerScalar(2)})};
  ExecutionList el2 = ExecutionList{s2};
  for (auto it = el2.begin(); it != el2.end(); ++it)
  {
    d.dispatch(*it);
  }
  answer = s2->getRegs()[0];
  EXPECT_TRUE((*one) ==~ (*(answer->asOGTerminal())));
}


TEST(SVDTests,CheckRealMatrix)
{

  // answers
  pOGTerminal U = pOGTerminal{new OGRealMatrix(new real16[9] {-0.2298476964000714,-0.5247448187602936,-0.8196419411205156,0.8834610176985253,0.2407824921325463,-0.4018960334334317,0.4082482904638627,-0.8164965809277263,0.4082482904638631},3,3,OWNER)};
  pOGTerminal S = pOGTerminal{new OGRealDiagonalMatrix(new real16[2] {9.525518091565107,  0.514300580658644},3,2,OWNER)};
  pOGTerminal VT = pOGTerminal{new OGRealMatrix(new real16[4]{-0.6196294838293402,-0.7848944532670524,-0.7848944532670524,0.6196294838293402}, 2,2, OWNER)};

  // input
  pOGTerminal M = pOGTerminal{new OGRealMatrix(new real16[6]{1,3,5,2,4,6},3,2,OWNER)};
  pOGExpr svd = pOGExpr{new SVD(M)};

  // computed answer pointers
  pOGNumeric answerU, answerS, answerVT, reconstruct;

  Dispatcher d;

  // Check selecting 0 (U)
  pOGExpr s0 = pOGExpr{new SELECTRESULT(svd, pOGNumeric{new OGIntegerScalar(0)})};
  ExecutionList el0 = ExecutionList{s0};
  for (auto it = el0.begin(); it != el0.end(); ++it)
  {
    d.dispatch(*it);
  }
  answerU = s0->getRegs()[0];
  EXPECT_TRUE((*U) % (*(answerU->asOGTerminal())));

  // Check selecting 1 (S)
  pOGExpr s1 = pOGExpr{new SELECTRESULT(svd, pOGNumeric{new OGIntegerScalar(1)})};
  ExecutionList el1 = ExecutionList{s1};
  for (auto it = el1.begin(); it != el1.end(); ++it)
  {
    d.dispatch(*it);
  }
  answerS = s1->getRegs()[0];
  EXPECT_TRUE((*S) ==~ (*(answerS->asOGTerminal())));

  // Check selecting 2 (V)
  pOGExpr s2 = pOGExpr{new SELECTRESULT(svd, pOGNumeric{new OGIntegerScalar(2)})};
  ExecutionList el2 = ExecutionList{s2};
  for (auto it = el2.begin(); it != el2.end(); ++it)
  {
    d.dispatch(*it);
  }
  answerVT = s2->getRegs()[0];
  EXPECT_TRUE((*VT) % (*(answerVT->asOGTerminal())));

  // reconstruction test i.e. recover A from U,S,V**T as A=U*S*V**T
  pOGExpr m1 = pOGExpr{new MTIMES(answerU, answerS)};
  pOGExpr m2 = pOGExpr{new MTIMES(m1, answerVT)};
  ExecutionList el3 = ExecutionList{m2};
  for (auto it = el3.begin(); it != el3.end(); ++it)
  {
    d.dispatch(*it);
  }
  reconstruct = m2->getRegs()[0];
  EXPECT_TRUE((*M) ==~ (*(reconstruct->asOGTerminal())));
}

// MAT-404, test demonstrates fixing of bug.
TEST(SVDTests,CheckRealHVector)
{

  // answers
  OGRealMatrix* U = new OGRealMatrix(new real16[1] {1},1,1,OWNER);
  OGTerminal* S = new OGRealDiagonalMatrix(new real16[3] {3.74165738677394,0,0 },1,3,OWNER);
  OGTerminal * VT = new OGRealMatrix(new real16[9]{0.2672612419124243,-0.5345224838248488,-0.8017837257372732,0.5345224838248488,0.7745419205884383,-0.3381871191173426,0.8017837257372732,-0.3381871191173427,0.4927193213239860}, 3,3, OWNER);

  // input
  OGTerminal* M = new OGRealMatrix(new real16[3]{1,2,3},1,3,OWNER);
  SVD* svd = new SVD(M);

  // computed answer pointers
  const OGNumeric * answerU, * answerS, * answerVT, * reconstruct;

  Dispatcher * d = new Dispatcher();

  // Check selecting 0 (U)
  SELECTRESULT* s0 = new SELECTRESULT(svd, new OGIntegerScalar(0));
  ExecutionList* el0 = new ExecutionList(s0);
  for (auto it = el0->begin(); it != el0->end(); ++it)
  {
    d->dispatch(*it);
  }
  answerU = s0->getRegs()[0];
  EXPECT_TRUE((*U) % (*(answerU->asOGTerminal())));

  // Check selecting 1 (S)
  SELECTRESULT* s1 = new SELECTRESULT(svd->copy(), new OGIntegerScalar(1));
  ExecutionList* el1 = new ExecutionList(s1);
  for (auto it = el1->begin(); it != el1->end(); ++it)
  {
    d->dispatch(*it);
  }
  answerS = s1->getRegs()[0];
  EXPECT_TRUE((*S) ==~ (*(answerS->asOGTerminal())));

  // Check selecting 2 (V)
  SELECTRESULT* s2 = new SELECTRESULT(svd->copy(), new OGIntegerScalar(2));
  ExecutionList* el2 = new ExecutionList(s2);
  for (auto it = el2->begin(); it != el2->end(); ++it)
  {
    d->dispatch(*it);
  }
  answerVT = s2->getRegs()[0];
  EXPECT_TRUE((*VT) % (*(answerVT->asOGTerminal())));

  // reconstruction test i.e. recover A from U,S,V**T as A=U*S*V**T
  MTIMES * m1 = new MTIMES(answerU->asOGTerminal()->createOwningCopy(),
                           answerS->asOGTerminal()->createOwningCopy());
  MTIMES * m2 = new MTIMES(m1, answerVT->asOGTerminal()->createOwningCopy());
  ExecutionList* el3 = new ExecutionList(m2);
  for (auto it = el3->begin(); it != el3->end(); ++it)
  {
    d->dispatch(*it);
  }
  reconstruct = m2->getRegs()[0];
  EXPECT_TRUE((*M) ==~ (*(reconstruct->asOGTerminal())));

  // Clean up
  delete s0;
  delete d;
  delete el0;
  delete s1;
  delete el1;
  delete s2;
  delete el2;
  delete m2;
  delete el3;
  delete S;
  delete U;
  delete VT;
}


TEST(SVDTests,CheckComplexMatrix)
{

  // answers
  pOGTerminal U = pOGTerminal{new OGComplexMatrix(new complex16[9] {{     -0.0228707006002169,      -0.2287070060021659}, {     -0.0522140610036492,      -0.5221406100364927}, {     -0.0815574214070819,      -0.8155742140708198}, {     -0.0879076568710847,      -0.8790765687107978}, {     -0.0239587534423321,      -0.2395875344233279}, {      0.0399901499864153,       0.3999014998641418}, {      0.3147401422050641,       0.2600102104752862}, {     -0.6294802844101258,      -0.5200204209505755}, {      0.3147401422050625,       0.2600102104752883}},3,3,OWNER)};
  pOGTerminal S = pOGTerminal{new OGRealDiagonalMatrix(new real16[2] {95.7302720469661779,5.1686568674896343},3,2,OWNER)};
  pOGTerminal VT = pOGTerminal{new OGComplexMatrix(new complex16[4]{{-0.6196294838293402,0},{0.7848944532670524,-0.e0},{-0.7848944532670524,0},{-0.6196294838293402,0}}, 2,2, OWNER)};

  // input
  pOGTerminal M = pOGTerminal{new OGComplexMatrix(new complex16[6]{{1,10}, {3,30}, {5,50}, {2,20}, {4,40}, {6,60}},3,2,OWNER)};
  pOGExpr svd = pOGExpr{new SVD(M)};

  // computed answer pointers
  pOGNumeric answerU, answerS, answerVT, reconstruct;

  Dispatcher d;

  // Check selecting 0 (U)
  pOGExpr s0 = pOGExpr{new SELECTRESULT(svd, pOGTerminal{new OGIntegerScalar(0)})};
  ExecutionList el0 = ExecutionList{s0};
  for (auto it = el0.begin(); it != el0.end(); ++it)
  {
    d.dispatch(*it);
  }
  answerU = s0->getRegs()[0];
  EXPECT_TRUE((*U) % (*(answerU->asOGTerminal())));

  // Check selecting 1 (S)
  pOGExpr s1 = pOGExpr{new SELECTRESULT(svd, pOGTerminal{new OGIntegerScalar(1)})};
  ExecutionList el1 = ExecutionList{s1};
  for (auto it = el1.begin(); it != el1.end(); ++it)
  {
    d.dispatch(*it);
  }
  answerS = s1->getRegs()[0];
  EXPECT_TRUE((*S) ==~ (*(answerS->asOGTerminal())));

  // Check selecting 2 (V)
  pOGExpr s2 = pOGExpr{new SELECTRESULT(svd, pOGTerminal{new OGIntegerScalar(2)})};
  ExecutionList el2 = ExecutionList{s2};
  for (auto it = el2.begin(); it != el2.end(); ++it)
  {
    d.dispatch(*it);
  }
  answerVT = s2->getRegs()[0];
  EXPECT_TRUE((*VT) % (*(answerVT->asOGTerminal())));

  // reconstruction test i.e. recover A from U,S,V**T as A=U*S*V**T
  pOGExpr m1 = pOGExpr{new MTIMES(answerU, answerS)};
  pOGExpr m2 = pOGExpr{new MTIMES(m1, answerVT)};
  ExecutionList el3 = ExecutionList{m2};
  for (auto it = el3.begin(); it != el3.end(); ++it)
  {
    d.dispatch(*it);
  }
  reconstruct = m2->getRegs()[0];
  // FP fuzz causes grief on reconstruction
  EXPECT_TRUE(ArrayFuzzyEquals(M->asOGComplexMatrix()->getData(),reconstruct->asOGComplexMatrix()->getData(),1e-15,1e-15));
}
