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
 *  Unit contains code for INV node runners
 */
namespace librdag {

void *
INVRunner::run(RegContainer& reg, OGRealScalar::Ptr arg) const
{
  OGNumeric::Ptr ret;
  real16 x = arg->getValue();
  if(x == 0.e0)
  {
    cerr << "Warning: singular system detected in matrix inversion" << std::endl;
    ret = OGNumeric::Ptr{new OGRealScalar(std::numeric_limits<real16>::infinity())};
  }
  else
  {
    ret = OGNumeric::Ptr{new OGRealScalar(1.e0/x)};
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
      ret = makeConcreteScalar(std::numeric_limits<real16>::infinity());
    }
    else
    {
      ret = makeConcreteScalar(1.e0/x);
    }
  }
  else
  {
    int m = arg->getRows();
    int n = arg->getCols();

    // require matrix is square.
    if(m!=n)
    {
      stringstream message;
      message << "Cannot invert a matrix that is not square. Matrix presented has shape: [" << m <<"x"<< n <<"].";
      throw rdag_error(message.str());
    }

    // dimensions
    int size = m;
    int sizesize = size*size;
    int lda = size;

    // status
    int info = 0;

    // copy A else it's destroyed
    T * A = new T[sizesize];
    std::memcpy(A, arg->getData(), sizeof(T)*sizesize);

    // create pivot vector
    int * ipiv = new int[size]();

    // call lapack to get LU decomp
    try
    {
      // LU decomp
      lapack::xgetrf(&size, &size, A, &lda, ipiv, &info);
      // Inversion backsolve
      lapack::xgetri(&size, A, &lda, ipiv, &info);
    }
    catch (exception& e)
    {
      if(info < 0) // illegal arg
      {
        delete[] ipiv;
        delete[] A;
        throw ;
      }
      else
      {
        cerr << "Warning: singular system detected in matrix inversion." << std::endl;
        cerr << "---> LAPACK details: " << e.what() << std::endl;
      }
    }

    // normal return, delete pivot info
    delete [] ipiv;

    ret = makeConcreteDenseMatrix(A, size, size, OWNER);
  }

  // shove ret into register
  reg.push_back(ret);
}

void *
INVRunner::run(RegContainer& reg, OGRealMatrix::Ptr arg) const
{
  inv_dense_runner<real16>(reg, arg);
  return nullptr;
}

void *
INVRunner::run(RegContainer& reg, OGComplexMatrix::Ptr arg) const
{
  inv_dense_runner<complex16>(reg, arg);
  return nullptr;
}

} // end namespace
