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

using namespace std;

/*
 *  Unit contains code for INV node runners
 */
namespace librdag {

void *
INVRunner::run(RegContainer& reg, OGRealScalar::Ptr arg) const
{
  OGNumeric::Ptr ret;
  real8 x = arg->getValue();
  if(x == 0.e0)
  {
    cerr << "Warning: singular system detected in matrix inversion" << std::endl;
    ret = OGRealScalar::create(std::numeric_limits<real8>::infinity());
  }
  else
  {
    ret = OGRealScalar::create(1.e0/x);
  }
  reg.push_back(ret);
  return nullptr;
}

template<typename T>
void
inv_dense_runner(RegContainer& reg, shared_ptr<const OGMatrix<T>> arg)
{
  OGNumeric::Ptr ret; // the returned item

  // Matrix in scalar context, i.e. a 1x1 matrix, inv is simply value**-1
  if(arg->getRows()==1 && arg->getCols()==1)
  {
    // NOTE: currently no cutoff grounding is applied to the argument, x;
    // if x == 0.e0 then inv(x) = inf;
    // else inv(x) = 1/x;
    T x = arg->getData()[0];
    if(x == 0.e0)
    {
      cerr << "Warning: singular system detected in matrix inversion." << std::endl;
      ret = makeConcreteScalar(std::numeric_limits<real8>::infinity());
    }
    else
    {
      ret = makeConcreteScalar(1.e0/x);
    }
  }
  else
  {
    int4 m = arg->getRows();
    int4 n = arg->getCols();

    // require matrix is square.
    if(m!=n)
    {
      stringstream message;
      message << "Cannot invert a matrix that is not square. Matrix presented has shape: [" << m <<"x"<< n <<"].";
      throw rdag_error(message.str());
    }

    // dimensions
    int4 size = m;
    int4 sizesize = size*size;
    int4 lda = size;

    // status
    int4 info = 0;

    // copy A else it's destroyed
    unique_ptr<T[]> Aptr (new T[sizesize]);
    T * A = Aptr.get();
    std::memcpy(A, arg->getData(), sizeof(T)*sizesize);

    // create pivot vector
    unique_ptr<int4[]> ipivptr (new int4[size]());
    int4 * ipiv = ipivptr.get();

    // call lapack to get LU decomp
    try
    {
      // LU decomp
      lapack::xgetrf<T, lapack::OnInputCheck::isfinite>(&size, &size, A, &lda, ipiv, &info);
      // Inversion backsolve
      lapack::xgetri(&size, A, &lda, ipiv, &info);
    }
    catch (rdag_recoverable_error& e)
    {
      cerr << "Warning: singular system detected in matrix inversion." << std::endl;
      cerr << "---> LAPACK details: " << e.what() << std::endl;
    }
    catch (rdag_error& e)
    {
      throw;
    }

    ret = makeConcreteDenseMatrix(Aptr.release(), size, size, OWNER);
  }

  // shove ret into register
  reg.push_back(ret);
}

void *
INVRunner::run(RegContainer& reg, OGRealDenseMatrix::Ptr arg) const
{
  inv_dense_runner<real8>(reg, arg);
  return nullptr;
}

void *
INVRunner::run(RegContainer& reg, OGComplexDenseMatrix::Ptr arg) const
{
  inv_dense_runner<complex16>(reg, arg);
  return nullptr;
}

} // end namespace
