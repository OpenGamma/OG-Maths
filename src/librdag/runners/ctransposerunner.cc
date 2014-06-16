/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for licence.
 *
 */

#include <complex>
#include <sstream>

#include "dispatch.hh"
#include "runners.hh"
#include "expression.hh"
#include "iss.hh"
#include "terminal.hh"
#include "uncopyable.hh"
#include "lapack.hh"

using namespace std;

/*
 *  Unit contains code for CTRANSPOSE node runners
 */
namespace librdag {

void *
CTRANSPOSERunner::run(RegContainer& reg, OGRealScalar::Ptr arg) const
{
  OGNumeric::Ptr ret = OGRealScalar::create(arg->getValue());
  reg.push_back(ret);
  return nullptr;
}

template<typename T>
void
ctranspose_dense_runner(RegContainer& reg, shared_ptr<const OGMatrix<T>>  arg)
{
  OGNumeric::Ptr ret; // the returned item

  // Matrix in scalar context, i.e. a 1x1 matrix, transpose is simply value
  if(arg->getRows()==1 && arg->getCols()==1)
  {
    ret = makeConcreteScalar(std::conj(arg->getData()[0]));
  }
  else // Matrix is a full matrix
  {
    size_t m = arg->getRows();
    size_t n = arg->getCols();
    size_t retRows = n, retCols = m;
    T * data = arg->getData();
    T * tmp = new T[m * n];
    size_t ir;
    for (size_t i = 0; i < n; i++) {
      ir = i * m;
      for (size_t j = 0; j < m; j++) {
        tmp[j * n + i] = std::conj(data[ir + j]);
      }
    }
    ret = makeConcreteDenseMatrix(tmp, retRows, retCols, OWNER);
  }

  // shove ret into register
  reg.push_back(ret);
}

void *
CTRANSPOSERunner::run(RegContainer& reg, OGRealMatrix::Ptr arg) const
{
  ctranspose_dense_runner<real8>(reg, arg);
  return nullptr;
}

void *
CTRANSPOSERunner::run(RegContainer& reg, OGComplexMatrix::Ptr arg) const
{
  ctranspose_dense_runner<complex16>(reg, arg);
  return nullptr;
}

} // end namespace
