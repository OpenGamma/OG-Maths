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

void
dummy_svd_run(RegContainer* reg)
{
  reg->push_back(new OGRealScalar(1.0));
  reg->push_back(new OGRealScalar(2.0));
  reg->push_back(new OGRealScalar(3.0));
}

void *
SVDRunner::run(RegContainer *reg, OGRealScalar const SUPPRESS_UNUSED *arg) const
{
  dummy_svd_run(reg);
  return nullptr;
}
void *
SVDRunner::run(RegContainer *reg, OGRealMatrix const SUPPRESS_UNUSED *arg) const
{
  dummy_svd_run(reg);
  return nullptr;
}

void *
SVDRunner::run(RegContainer *reg, OGComplexMatrix const SUPPRESS_UNUSED *arg) const
{
  dummy_svd_run(reg);
  return nullptr;
}

} // end namespace
