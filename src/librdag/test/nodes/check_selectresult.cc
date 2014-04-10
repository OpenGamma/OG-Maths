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
  SVD* svd = new SVD(r0);

  Dispatcher * d = new Dispatcher();

  // Check selecting 0 (U)
  SELECTRESULT* s0 = new SELECTRESULT(svd, new OGIntegerScalar(0));
  ExecutionList* el0 = new ExecutionList(s0);
  for (auto it = el0->begin(); it != el0->end(); ++it)
  {
    d->dispatch(*it);
  }
  const OGNumeric * answer = s0->getRegs()[0];
  EXPECT_TRUE((*one) ==~ (*(answer->asOGTerminal())));

  // Check selecting 1 (S)
  SELECTRESULT* s1 = new SELECTRESULT(svd->copy(), new OGIntegerScalar(1));
  ExecutionList* el1 = new ExecutionList(s1);
  for (auto it = el1->begin(); it != el1->end(); ++it)
  {
    d->dispatch(*it);
  }
  answer = s1->getRegs()[0];
  EXPECT_TRUE((*r0) ==~ (*(answer->asOGTerminal())));

  // Check selecting 2 (V)
  SELECTRESULT* s2 = new SELECTRESULT(svd->copy(), new OGIntegerScalar(2));
  ExecutionList* el2 = new ExecutionList(s2);
  for (auto it = el2->begin(); it != el2->end(); ++it)
  {
    d->dispatch(*it);
  }
  answer = s2->getRegs()[0];
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
