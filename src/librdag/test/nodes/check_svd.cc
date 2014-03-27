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
  OGTerminal* one = new OGRealScalar(1.0);
  OGTerminal* r0 = new OGRealScalar(10.0);
  ArgContainer *args = new ArgContainer();
  args->push_back(r0);
  SVD* svd = new SVD(args);

  Dispatcher * d = new Dispatcher();

  // Check selecting 0 (U)
  ArgContainer* args0 = new ArgContainer();
  args0->push_back(svd);
  args0->push_back(new OGIntegerScalar(0));
  SELECTRESULT* s0 = new SELECTRESULT(args0);
  ExecutionList* el0 = new ExecutionList(s0);
  for (auto it = el0->begin(); it != el0->end(); ++it)
  {
    d->dispatch(*it);
  }
  const RegContainer * regs = s0->getRegs();
  const OGNumeric * answer = (*regs)[0];
  EXPECT_TRUE((*one) ==~ (*(answer->asOGTerminal())));

  // Check selecting 1 (S)
  ArgContainer* args1 = new ArgContainer();
  args1->push_back(svd->copy());
  args1->push_back(new OGIntegerScalar(1));
  SELECTRESULT* s1 = new SELECTRESULT(args1);
  ExecutionList* el1 = new ExecutionList(s1);
  for (auto it = el1->begin(); it != el1->end(); ++it)
  {
    d->dispatch(*it);
  }
  regs = s1->getRegs();
  answer = (*regs)[0];
  EXPECT_TRUE((*r0) ==~ (*(answer->asOGTerminal())));

  // Check selecting 2 (V)
  ArgContainer* args2 = new ArgContainer();
  args2->push_back(svd->copy());
  args2->push_back(new OGIntegerScalar(2));
  SELECTRESULT* s2 = new SELECTRESULT(args2);
  ExecutionList* el2 = new ExecutionList(s2);
  for (auto it = el2->begin(); it != el2->end(); ++it)
  {
    d->dispatch(*it);
  }
  regs = s2->getRegs();
  answer = (*regs)[0];
  EXPECT_TRUE((*one) ==~ (*(answer->asOGTerminal())));

  // Clean up
  delete one;
  delete s0;
  delete d;
  delete el0;
  delete s1;
  delete el1;
  delete s2;
  delete el2;
}


TEST(SVDTests,CheckRealMatrix)
{

  // answers
  OGRealMatrix* U = new OGRealMatrix(new real16[9] {-0.2298476964000714,-0.5247448187602936,-0.8196419411205156,0.8834610176985253,0.2407824921325463,-0.4018960334334317,0.4082482904638627,-0.8164965809277263,0.4082482904638631},3,3,OWNER);
  OGTerminal* S = new OGRealDiagonalMatrix(new real16[2] {9.525518091565107,  0.514300580658644},3,2,OWNER);
  OGTerminal * VT = new OGRealMatrix(new real16[4]{-0.6196294838293402,-0.7848944532670524,-0.7848944532670524,0.6196294838293402}, 2,2, OWNER);

  // input
  OGTerminal* M = new OGRealMatrix(new real16[6]{1,3,5,2,4,6},3,2,OWNER);
  ArgContainer *args = new ArgContainer();
  args->push_back(M);
  SVD* svd = new SVD(args);

  // computed answer pointers
  const OGNumeric * answerU, * answerS, * answerVT, * reconstruct;
  // computed answer reg containers
  const RegContainer * regs0, * regs1, * regs2, * regs3;

  Dispatcher * d = new Dispatcher();

  // Check selecting 0 (U)
  ArgContainer* args0 = new ArgContainer();
  args0->push_back(svd);
  args0->push_back(new OGIntegerScalar(0));
  SELECTRESULT* s0 = new SELECTRESULT(args0);
  ExecutionList* el0 = new ExecutionList(s0);
  for (auto it = el0->begin(); it != el0->end(); ++it)
  {
    d->dispatch(*it);
  }
  regs0 = s0->getRegs();
  answerU = (*regs0)[0];
  EXPECT_TRUE((*U) % (*(answerU->asOGTerminal())));

  // Check selecting 1 (S)
  ArgContainer* args1 = new ArgContainer();
  args1->push_back(svd->copy());
  args1->push_back(new OGIntegerScalar(1));
  SELECTRESULT* s1 = new SELECTRESULT(args1);
  ExecutionList* el1 = new ExecutionList(s1);
  for (auto it = el1->begin(); it != el1->end(); ++it)
  {
    d->dispatch(*it);
  }
  regs1 = s1->getRegs();
  answerS = (*regs1)[0];
  EXPECT_TRUE((*S) ==~ (*(answerS->asOGTerminal())));

  // Check selecting 2 (V)
  ArgContainer* args2 = new ArgContainer();
  args2->push_back(svd->copy());
  args2->push_back(new OGIntegerScalar(2));
  SELECTRESULT* s2 = new SELECTRESULT(args2);
  ExecutionList* el2 = new ExecutionList(s2);
  for (auto it = el2->begin(); it != el2->end(); ++it)
  {
    d->dispatch(*it);
  }
  regs2 = s2->getRegs();
  answerVT = (*regs2)[0];
  EXPECT_TRUE((*VT) % (*(answerVT->asOGTerminal())));

  // reconstruction test i.e. recover A from U,S,V**T as A=U*S*V**T
  ArgContainer* args3 = new ArgContainer();
  args3->push_back(answerU->asOGTerminal()->createOwningCopy());
  args3->push_back(answerS->asOGTerminal()->createOwningCopy());

  MTIMES * m1 = new MTIMES(args3);
  ArgContainer* args4 = new ArgContainer();
  args4->push_back(m1);
  args4->push_back(answerVT->asOGTerminal()->createOwningCopy());
  MTIMES * m2 = new MTIMES(args4);
  ExecutionList* el3 = new ExecutionList(m2);
  for (auto it = el3->begin(); it != el3->end(); ++it)
  {
    d->dispatch(*it);
  }
  regs3 = m2->getRegs();
  reconstruct = (*regs3)[0];
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
  OGComplexMatrix* U = new OGComplexMatrix(new complex16[9] {{     -0.0228707006002169,      -0.2287070060021659}, {     -0.0522140610036492,      -0.5221406100364927}, {     -0.0815574214070819,      -0.8155742140708198}, {     -0.0879076568710847,      -0.8790765687107978}, {     -0.0239587534423321,      -0.2395875344233279}, {      0.0399901499864153,       0.3999014998641418}, {      0.3147401422050641,       0.2600102104752862}, {     -0.6294802844101258,      -0.5200204209505755}, {      0.3147401422050625,       0.2600102104752883}},3,3,OWNER);
  OGTerminal* S = new OGRealDiagonalMatrix(new real16[2] {95.7302720469661779,5.1686568674896343},3,2,OWNER);
  OGTerminal * VT = new OGComplexMatrix(new complex16[4]{{-0.6196294838293402,0},{0.7848944532670524,-0.e0},{-0.7848944532670524,0},{-0.6196294838293402,0}}, 2,2, OWNER);

  // input
  OGTerminal* M = new OGComplexMatrix(new complex16[6]{{1,10}, {3,30}, {5,50}, {2,20}, {4,40}, {6,60}},3,2,OWNER);
  ArgContainer *args = new ArgContainer();
  args->push_back(M);
  SVD* svd = new SVD(args);

  // computed answer pointers
  const OGNumeric * answerU, * answerS, * answerVT, * reconstruct;
  // computed answer reg containers
  const RegContainer * regs0, * regs1, * regs2, * regs3;

  Dispatcher * d = new Dispatcher();

  // Check selecting 0 (U)
  ArgContainer* args0 = new ArgContainer();
  args0->push_back(svd);
  args0->push_back(new OGIntegerScalar(0));
  SELECTRESULT* s0 = new SELECTRESULT(args0);
  ExecutionList* el0 = new ExecutionList(s0);
  for (auto it = el0->begin(); it != el0->end(); ++it)
  {
    d->dispatch(*it);
  }
  regs0 = s0->getRegs();
  answerU = (*regs0)[0];
  EXPECT_TRUE((*U) % (*(answerU->asOGTerminal())));

  // Check selecting 1 (S)
  ArgContainer* args1 = new ArgContainer();
  args1->push_back(svd->copy());
  args1->push_back(new OGIntegerScalar(1));
  SELECTRESULT* s1 = new SELECTRESULT(args1);
  ExecutionList* el1 = new ExecutionList(s1);
  for (auto it = el1->begin(); it != el1->end(); ++it)
  {
    d->dispatch(*it);
  }
  regs1 = s1->getRegs();
  answerS = (*regs1)[0];
  EXPECT_TRUE((*S) ==~ (*(answerS->asOGTerminal())));

  // Check selecting 2 (V)
  ArgContainer* args2 = new ArgContainer();
  args2->push_back(svd->copy());
  args2->push_back(new OGIntegerScalar(2));
  SELECTRESULT* s2 = new SELECTRESULT(args2);
  ExecutionList* el2 = new ExecutionList(s2);
  for (auto it = el2->begin(); it != el2->end(); ++it)
  {
    d->dispatch(*it);
  }
  regs2 = s2->getRegs();
  answerVT = (*regs2)[0];
  EXPECT_TRUE((*VT) % (*(answerVT->asOGTerminal())));

  // reconstruction test i.e. recover A from U,S,V**T as A=U*S*V**T
  ArgContainer* args3 = new ArgContainer();
  args3->push_back(answerU->asOGTerminal()->createOwningCopy());
  args3->push_back(answerS->asOGTerminal()->createOwningCopy());

  MTIMES * m1 = new MTIMES(args3);
  ArgContainer* args4 = new ArgContainer();
  args4->push_back(m1);
  args4->push_back(answerVT->asOGTerminal()->createOwningCopy());
  MTIMES * m2 = new MTIMES(args4);
  ExecutionList* el3 = new ExecutionList(m2);
  for (auto it = el3->begin(); it != el3->end(); ++it)
  {
    d->dispatch(*it);
  }
  regs3 = m2->getRegs();
  reconstruct = (*regs3)[0];
  // FP fuzz causes grief on reconstruction
  EXPECT_TRUE(ArrayFuzzyEquals(M->asOGComplexMatrix()->getData(),reconstruct->asOGComplexMatrix()->getData(),1e-15,1e-15));

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