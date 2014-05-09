/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "runtree.hh"
#include "dispatch.hh"
#include "execution.hh"

namespace librdag {

void runtree(pOGNumeric root)
{
  // create dispatcher
  Dispatcher * d = new Dispatcher();

  // create execution list, doesn't throw.
  ExecutionList * el = new ExecutionList(root);

  // try and run the list, clean up and rethrow on error
  try
  {
    for (auto it = el->begin(); it != el->end(); ++it)
    {
      d->dispatch(*it);
    }
  }
  catch(rdag_error e)
  {
    delete d;
    delete el;
    throw e;
  }
  catch(exception e)
  {
    delete d;
    delete el;
    throw e;
  }
  // clean up
  delete d;
  delete el;
}


}
