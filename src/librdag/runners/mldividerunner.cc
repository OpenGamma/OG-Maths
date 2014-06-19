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

#include <stdio.h>
#include <complex>
#include <sstream>

using namespace std;

/*
 * Unit contains code for MLDIVIDE node runners
 */

namespace librdag {

template<typename T>
void*
mldivide_dense_runner(RegContainer& reg0, shared_ptr<const OGMatrix<T>> arg0, shared_ptr<const OGMatrix<T>> arg1)
{
  T * data1 = arg0->getData();
  T * data2 = arg1->getData();
  T * ans = new T[1]();
  ans[0]=data1[0]+data2[0];
  OGNumeric::Ptr ret = makeConcreteDenseMatrix(ans,1,1,VIEWER);
  reg0.push_back(ret);

  return nullptr;
}

// MLDIVIDE runner:
void * MLDIVIDERunner::run(RegContainer& reg0, OGComplexDenseMatrix::Ptr arg0, OGComplexDenseMatrix::Ptr arg1) const
{
  mldivide_dense_runner<complex16>(reg0, arg0, arg1);
  return nullptr;
}


void * MLDIVIDERunner::run(RegContainer& reg0, OGRealDenseMatrix::Ptr arg0, OGRealDenseMatrix::Ptr arg1) const
{
  mldivide_dense_runner<real8>(reg0, arg0, arg1);
  return nullptr;
}

void *
MLDIVIDERunner::run(RegContainer& reg0, OGRealScalar::Ptr arg0, OGRealScalar::Ptr arg1) const
{
  real8 data1 = arg0->getValue();
  real8 data2 = arg1->getValue();
  OGNumeric::Ptr ret = OGRealScalar::create(data1+data2);
  reg0.push_back(ret);
  return nullptr;
}

}
