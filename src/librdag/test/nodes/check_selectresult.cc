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
#include "debug.h"

using namespace std;
using namespace librdag;
using namespace testnodes;
using ::testing::TestWithParam;
using ::testing::Values;

/*
 * Check SELECTRESULT node behaves correctly
 */


TEST(SELECTRESULTTests,CheckBehaviour)
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
