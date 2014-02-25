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

}

#endif // _CUSTOMRUNNERS_HH