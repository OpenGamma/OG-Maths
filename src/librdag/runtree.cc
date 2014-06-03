/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "runtree.hh"
#include "dispatch.hh"
#include "execution.hh"

namespace librdag {

void runtree(const OGNumeric::Ptr& root)
{
  Dispatcher d;
  ExecutionList el{root};
  for (auto it = el.begin(); it != el.end(); ++it)
  {
    d.dispatch(*it);
  }
}

} // end namespace librdag
