/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <stdio.h>
#include "entrypt.hh"
#include "dispatch.hh"
#include "numeric.hh"
#include "expression.hh"
#include "execution.hh"
#include "terminal.hh"
#include "exprtypeenum.h"
#include <typeinfo>
#include <iostream>
#include "lapack_raw.h"

using namespace std;

namespace librdag
{

const OGTerminal::Ptr
entrypt(const OGNumeric::Ptr& expr)
{
  // Sort out LAPACK so xerbla calls don't kill the processes.
  int4 zero = 0;
  set_xerbla_death_switch(&zero);

  OGTerminal::Ptr terminal = expr->asOGTerminal();
  if (terminal != OGTerminal::Ptr{})
  {
    // If we were passed a terminal, simply return it.
    return terminal;
  }
  else
  {
    ExecutionList el{expr};
    Dispatcher disp;
    
    DEBUG_PRINT("Dispatching from entrypt\n");

    for (auto it = el.begin(); it != el.end(); ++it)
    {
      OGExpr::Ptr expr = (*it)->asOGExpr();
      if (expr != OGExpr::Ptr{})
      {
        disp.dispatch(expr);
      }
    }

    const RegContainer& regs = expr->asOGExpr()->getRegs();
    if(regs[0]->asOGTerminal() == nullptr)
    {
      throw rdag_error("Evaluated terminal is not casting asOGTerminal correctly.");
    }

    return static_pointer_cast<const OGTerminal, const OGNumeric>(regs[0]);
  }
}

} // namespace librdag
