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
#include "execution.hh"
#include "dispatch.hh"
#include "uncopyable.hh"
#include "lapack.hh"

#include <stdio.h>
#include <complex>
#include <sstream>
#include <limits>

/*
 *  Unit contains code for PINV node runners
 */
namespace librdag {

void *
PINVRunner::run(RegContainer& reg, OGRealScalar const * arg) const
{
  const OGRealScalar* ret;
  real16 x = arg->getValue();
  if(x == 0.e0)
  {
    ret = new OGRealScalar(0.e0);
  }
  else
  {
    ret = new OGRealScalar(1.e0/x);
  }
  reg.push_back(ret);
  return nullptr;
}

/**
 * Computes the threshold for numerical "zero" in the pinv sense
 * @param msv the max singular value
 * @param rows the number of rows in the matrix
 * @param cols the number of cols in the matrix
 * @return the threshold value for "zero"
 */
real16 pinv_threshold(real16 msv, int rows, int cols)
{
  return (rows > cols ? rows : cols) * msv * std::numeric_limits<real16>::epsilon();
}

template<typename T>
void
pinv_dense_runner(RegContainer& reg, OGMatrix<T> const * arg)
{
  const OGTerminal* ret = nullptr; // the returned item

  // Matrix in scalar context, i.e. a 1x1 matrix, pinv is simply value**-1
  if(arg->getRows()==1 && arg->getCols()==1)
  {
    // NOTE: currently no cutoff grounding is applied to the argument, x;
    // if x == 0.e0 then pinv(x) = 0;
    // else pinv(x) = 1/x;
    T x = arg->getData()[0];
    if(x == 0.e0)
    {
      ret = makeConcreteScalar(0.e0);
    }
    else
    {
      ret = makeConcreteScalar(1.e0/x);
    }
  }
  else
  {
    const int m = arg->getRows();
    const int n = arg->getCols();
    const int minmn = m > n ? n : m;

    // create dispatcher for use in computation
    Dispatcher * d = new Dispatcher();
    // pointer to execution list
    ExecutionList * el0, *  el1;

    // svd the arg
    SVD * svd = new SVD(arg->createOwningCopy());

    el0 = new ExecutionList(svd);
    for (auto it = el0->begin(); it != el0->end(); ++it)
    {
      d->dispatch(*it);
    }

    // svd regs now hold [U,S,V**T]
    const OGNumeric * numericU = svd->getRegs()[0]->asOGTerminal()->createOwningCopy();
    const OGNumeric * numericS = svd->getRegs()[1]->asOGTerminal()->createOwningCopy();
    const OGNumeric * numericVT = svd->getRegs()[2]->asOGTerminal()->createOwningCopy();

    // don't need the svd any more, we have the data.
    delete svd;

    // walk S matrix, see if we have anything that is numerically zero.
    // go backwards as singular values are ordered descending.
    real16 * S = numericS->asOGRealDiagonalMatrix()->getData();
    real16 thres = pinv_threshold(S[0], m, n);
    int lim;
    for(lim = minmn - 1 ; lim >= 0; lim--)
    {
      if(std::abs(S[lim])>thres)
      {
        break;
      }
    }

    // scale the diags in the reachable part, zero the rest
    for(int i = 0 ; i <= lim; i++)
    {
      S[i] = 1.e0/S[i];
    }
    for(int i = lim + 1; i < minmn; i++)
    {
      S[i] = 0.e0;
    }

    // create a new transposed inverted diag matrix
    OGRealDiagonalMatrix * diag_transpose = new OGRealDiagonalMatrix(S,n,m);

    // need to transpose U
    CTRANSPOSE * ctransposeU = new CTRANSPOSE(numericU);

    // need to transpose VT
    CTRANSPOSE * ctransposeVT = new CTRANSPOSE(numericVT);

    // multiply back together as [(V**T)**T * inv(S) * U**T]
    MTIMES * VTS = new MTIMES(ctransposeVT, diag_transpose);
    MTIMES * VTSUT = new MTIMES(VTS, ctransposeU);

    el1 = new ExecutionList(VTSUT);
    for (auto it = el1->begin(); it != el1->end(); ++it)
    {
      d->dispatch(*it);
    }

    ret = VTSUT->getRegs()[0]->asOGTerminal()->createOwningCopy();

    delete el0;
    delete el1;
    delete d;
    delete VTSUT;
    // This is floating about still as it's owning and diag_transpose was just a view
    delete numericS;
  }

  // shove ret into register
  reg.push_back(ret);
}

void *
PINVRunner::run(RegContainer& reg, OGRealMatrix const * arg) const
{
  pinv_dense_runner(reg, arg);
  return nullptr;
}

void *
PINVRunner::run(RegContainer& reg, OGComplexMatrix const * arg) const
{
  pinv_dense_runner(reg, arg);
  return nullptr;
}

} // end namespace
