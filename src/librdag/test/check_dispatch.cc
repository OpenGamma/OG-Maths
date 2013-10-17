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

void dispatchfn(const OGNumeric * thing)
{
    Dispatcher * v = new Dispatcher();
    v->dispatch(thing);
    delete v;
}

TEST(DispatchTest, SimpleTest) {
    // One binary node holding two terminals
  OGNumeric *real1 = new OGRealScalar(1.0);
  OGNumeric *real2 = new OGRealScalar(2.0);
  ArgContainer* plusArgs = new ArgContainer();
  plusArgs->push_back(real1);
  plusArgs->push_back(real2);
  OGExpr *plus = new PLUS(plusArgs);
  ExecutionList* el1 = new ExecutionList(plus);
  const OGNumeric * val;
  int counter = 0;
  for (auto it = el1->begin(); it != el1->end(); ++it)
  {
    cout << "counter is" << ++counter << std::endl;
    val = *it;
    dispatchfn(val);
  }
  const RegContainer * reg = plus->getRegs();
  const OGNumeric * answer = (*reg)[0];
  answer->debug_print();
  delete plus;
  delete el1;
}