/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for licence.
 *
 */

#include "dispatch.hh"
#include "runners.hh"
#include "expression.hh"
#include "iss.hh"
#include "terminal.hh"
#include "uncopyable.hh"
#include "lapack.hh"
#include "debug.h"


/**
 * SVD custom runners. For now they are placeholders so the compile
 * doesn't fail.
 */
namespace librdag {

void *
SVDRunner::run(RegContainer SUPPRESS_UNUSED *reg, OGRealScalar const SUPPRESS_UNUSED *arg) const
{
  return nullptr;
}
void *
SVDRunner::run(RegContainer SUPPRESS_UNUSED *reg, OGRealMatrix const SUPPRESS_UNUSED *arg) const
{
  return nullptr;
}

void *
SVDRunner::run(RegContainer SUPPRESS_UNUSED *reg, OGComplexMatrix const SUPPRESS_UNUSED *arg) const
{
  return nullptr;
}

} // end namespace
