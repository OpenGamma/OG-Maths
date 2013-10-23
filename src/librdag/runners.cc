/*
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * See distribution for licence.
 *
 */
#include "dispatch.hh"
#include "runners.hh"
#include "expression.hh"
#include "terminal.hh"
#include "uncopyable.hh"

namespace librdag {

void PlusRunner::run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const
{

}

void PlusRunner::run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const
{

}

void PlusRunner::run(RegContainer* reg0, OGRealScalar const * arg0, OGRealScalar const * arg1) const
{
  // impl convert and run for types OGRealScalar and OGRealScalar
  DEBUG_PRINT("In virtual overridden PlusRunner T run() REAL REAL \n");
  const OGRealScalar * ret = new OGRealScalar(arg0->getValue()+arg1->getValue());
  reg0->push_back(ret);
}

void
NegateRunner::run(RegContainer* reg, const OGRealScalar* arg) const
{
  DEBUG_PRINT("IN Negate runner.\n");
  const OGRealScalar* ret = new OGRealScalar(-(arg->getValue()));
  reg->push_back(ret);
}

void
NegateRunner::run(RegContainer *reg, const OGRealMatrix *arg) const
{
  real16* data = arg->getData();
  int datalen = arg->getDatalen();
  real16* newData = new real16[datalen];
  for (int i = 0; i < datalen; ++i)
  {
    newData[i] = -data[i];
  }
  reg->push_back(new OGRealMatrix(newData, arg->getRows(), arg->getCols()));
}

void
NegateRunner::run(RegContainer *reg, const OGComplexMatrix *arg) const
{
  complex16* data = arg->getData();
  int datalen = arg->getDatalen();
  complex16* newData = new complex16[datalen];
  for (int i = 0; i < datalen; ++i)
  {
    newData[i] = -data[i];
  }
  reg->push_back(new OGComplexMatrix(newData, arg->getRows(), arg->getCols()));
}

} // end namespace librdag
