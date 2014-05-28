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
#include "debug.h"


/**
 *  Unit contains code for LU node runners
 */
namespace librdag {

template<typename T> void lu_dense_runner(RegContainer& reg, shared_ptr<const OGMatrix<T>> arg)
{
  int m = arg->getRows();
  int n = arg->getCols();
  int lda = m;
  int minmn = m > n ? n : m;
  int mn = m * n;
  int info = 0;

// Sizes of output
//   L = [m x minmn ]
//   U = [minmn x n]

  T * L = new T[m*minmn]();
  T * U = new T[minmn*n]();

  // copy A else it's destroyed
  T * A = new T[mn];
  std::memcpy(A, arg->getData(), sizeof(T)*mn);

  // create pivot vector
  int * ipiv = new int[minmn]();

  // call lapack
  try
  {
    lapack::xgetrf(&m, &n, A, &lda, ipiv, &info);
  }
  catch (exception& e)
  {
    if(info < 0) // illegal arg
    {
      delete[] ipiv;
      delete[] A;
      delete[] U;
      delete[] L;
      throw e;
    }
    else // failed as system is singular TODO: this will end up in logs (MAT-369) and userland (MAT-370).
    {
      cerr << e.what() << std::endl;
    }
  }

  // The following is adapted from DOGMAv1

  // extract U, get triangle, then square
  // U strides in 'minmn', A strides in 'm'
  int lim = minmn > n ? n : minmn;
  for (int i = 0; i < lim - 1; i++)
  {
    int mi = m * i;
    int ni = minmn * i;
    for (int j = 0; j <= i; j++)
    {
      U[ni + j] = A[mi + j];
    }
  }
  for (int i = lim - 1; i < n; i++)
  {
    int mi = m * i;
    int ni = minmn * i;
    for (int j = 0; j < minmn; j++)
    {
      U[ni + j] = A[mi + j];
    }
  }

  // Transpose the pivot... create as permutation
  int * perm = new int[m];
  // 1) turn into 0 based indexing
  for (int i = 0; i < minmn; i++)
  {
    ipiv[i] -= 1;
  }
  // 2) 0:m-1 range vector, will be permuted in a tick
  for (int i = 0; i < m; i++)
  {
    perm[i] = i;
  }
  // 3) apply permutation to range indexed vector, just walk through in order and apply the swaps
  int swp;
  for (int i = 0; i < minmn; i++)
  {
    int piv = ipiv[i]; // get pivot at index "i"
    // apply the pivot by swapping the corresponding "row" indices in the perm index vector
    if (piv != i)
    {
      swp = perm[piv];
      perm[piv] = perm[i];
      perm[i] = swp;
    }
  }

  // kill triu of A, write 1 onto diag of A too
  A[0] = 1.e0;
  for (int i = 1; i < minmn; i++)
  {
    A[m*i+i] = 1.e0;
    for (int j = 0; j < i; j++)
    {
      A[i*m+j]=0.e0;
    }
  }

  // apply pivot during assign to L
  for (int i = 0; i < m; i++)
  {
    int permi = perm[i];
    int row = perm[permi];
    if(row==i)
    {
      for (int j = 0; j < minmn; j++)
      {
        int jm = j*m;
        L[jm+i] = A[jm+permi];
      }
    }
    else
    {
      for (int j = 0; j < minmn; j++)
      {
        int jm = j*m;
        L[jm+i] = A[jm+row];
      }
    }
  }

  delete[] ipiv;
  delete[] A;
  delete[] perm;

  OGNumeric::Ptr cL = makeConcreteDenseMatrix(L, m, minmn, OWNER);
  OGNumeric::Ptr cU = makeConcreteDenseMatrix(U, minmn, n, OWNER);

  reg.push_back(cL);
  reg.push_back(cU);
}

void *
LURunner::run(RegContainer& reg, OGRealScalar::Ptr arg) const
{
  // single real space lu is just l=1, u=value
  reg.push_back(OGNumeric::Ptr{new OGRealScalar(1.e0)});
  if(arg->getValue()==0.e0)
  {
    cerr << "Warning: singular system detected in LU decomposition" << std::endl;
  }
  reg.push_back(OGNumeric::Ptr{new OGRealScalar(arg->getValue())});
  return nullptr;
}
void *
LURunner::run(RegContainer& reg, OGRealMatrix::Ptr arg) const
{
  lu_dense_runner<real16>(reg, arg);
  return nullptr;
}

void *
LURunner::run(RegContainer& reg, OGComplexMatrix::Ptr arg) const
{
  lu_dense_runner<complex16>(reg, arg);
  return nullptr;
}

} // end namespace
