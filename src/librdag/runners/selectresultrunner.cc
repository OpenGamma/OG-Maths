/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for licence.
 *
 */

#include "dispatch.hh"
#include "runners.hh"
#include "expression.hh"
#include "terminal.hh"

#include <stdio.h>
#include <sstream>

namespace librdag {

/**
 * Implementations
 */

void*
SELECTRESULTRunner::eval(RegContainer& reg, const RegContainer& arg0, pOGIntegerScalar arg1) const
{
  return run(reg, arg0, arg1);
}

void*
SELECTRESULTRunner::run(RegContainer& reg0, const RegContainer& arg0, pOGIntegerScalar arg1) const
{
  reg0.push_back(arg0[arg1->getValue()]);
  return nullptr;
}

} // end namespace
