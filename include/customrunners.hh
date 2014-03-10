/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 *
 */

#ifndef _CUSTOMRUNNERS_HH
#define _CUSTOMRUNNERS_HH

#include "dispatch.hh"
#include "uncopyable.hh"

namespace librdag {

class NORM2Runner: public DispatchVoidUnaryOp, private Uncopyable
{
  public:
    virtual void * run(RegContainer* reg, const OGRealScalar*    arg) const override;
    virtual void * run(RegContainer* reg, const OGRealMatrix*    arg) const override;
    virtual void * run(RegContainer* reg, const OGComplexMatrix* arg) const override;
};

class MTIMESRunner: public DispatchVoidBinaryOp, private Uncopyable
{
  public:
    using DispatchBinaryOp<void *>::run;
    virtual void * run(RegContainer* reg0, const OGComplexMatrix* arg0, const OGComplexMatrix* arg1) const override;
    virtual void * run(RegContainer* reg0, const OGRealMatrix*    arg0, const OGRealMatrix*    arg1) const override;
    virtual void * run(RegContainer* reg0, const OGRealScalar*    arg0, const OGRealScalar*    arg1) const override;
};


}

#endif // _CUSTOMRUNNERS_HH