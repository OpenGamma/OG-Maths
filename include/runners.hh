/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef RUNNERS_HH
#define RUNNERS_HH

#include "dispatch.hh"
#include "uncopyable.hh"

namespace librdag {

class PlusRunner: public DispatchVoidBinaryOp, private Uncopyable
{
public:
  using DispatchBinaryOp<void>::run;
  virtual void run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const override;
  virtual void run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const override;
  virtual void run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const override;
};


class NegateRunner: public DispatchVoidUnaryOp, private Uncopyable
{
  public:
    virtual void run(RegContainer* reg, const OGRealScalar* arg) const override;
    virtual void run(RegContainer* reg, const OGRealMatrix* arg) const override;
    virtual void run(RegContainer* reg, const OGComplexMatrix* arg) const override;
};

} // end namespace librdag

#endif
