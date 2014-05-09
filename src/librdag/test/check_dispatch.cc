/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "terminal.hh"
#include "expression.hh"
#include "visitor.hh"
#include "exceptions.hh"
#include "gtest/gtest.h"
#include "warningmacros.h"
#include "exprtypeenum.h"
#include "execution.hh"
#include "dispatch.hh"
#include <stdio.h>

using namespace std;
using namespace librdag;

void dispatchfn(pOGNumeric thing)
{
    Dispatcher * v = new Dispatcher();
    v->dispatch(thing);
    delete v;
}

TEST(DispatchTest, SimpleTest) {
    // One binary node holding two terminals
  pOGNumeric real1 = pOGNumeric{new OGRealScalar(1.0)};
  pOGNumeric real2 = pOGNumeric{new OGRealScalar(2.0)};
  pOGNumeric plus = pOGNumeric{new PLUS(real1, real2)};
  ExecutionList el1 = ExecutionList{plus};
  int counter = 0;
  for (auto it: el1)
  {
    cout << "counter is" << ++counter << std::endl;
    dispatchfn(it);
  }
  const RegContainer& reg = plus->asOGExpr()->getRegs();
  pOGNumeric answer = reg[0];
  answer->debug_print();
}
