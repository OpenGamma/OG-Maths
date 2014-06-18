/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "terminal.hh"
#include "expression.hh"
#include "exceptions.hh"
#include "gtest/gtest.h"
#include "warningmacros.h"
#include "exprtypeenum.h"
#include "execution.hh"
#include "dispatch.hh"
#include <stdio.h>

using namespace std;
using namespace librdag;

void dispatchfn(OGNumeric::Ptr thing)
{
    Dispatcher * v = new Dispatcher();
    v->dispatch(thing);
    delete v;
}

TEST(DispatchTest, SimpleTest) {
    // One binary node holding two terminals
  OGNumeric::Ptr real1 = OGRealScalar::create(1.0);
  OGNumeric::Ptr real2 = OGRealScalar::create(2.0);
  OGNumeric::Ptr plus = PLUS::create(real1, real2);
  ExecutionList el1 = ExecutionList{plus};
  size_t counter = 0;
  for (auto it: el1)
  {
    cout << "counter is" << ++counter << std::endl;
    dispatchfn(it);
  }
  const RegContainer& reg = plus->asOGExpr()->getRegs();
  OGNumeric::Ptr answer = reg[0];
  answer->debug_print();
}
