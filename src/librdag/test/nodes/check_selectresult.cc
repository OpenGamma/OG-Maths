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
