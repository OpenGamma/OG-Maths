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
 * Unit contains code for MTIMES node runners
 */

namespace librdag {

template<typename T>
void*
mtimes_dense_runner(RegContainer& reg0, shared_ptr<const OGMatrix<T>> arg0, shared_ptr<const OGMatrix<T>> arg1)
{
  int4 colsArray1 = arg0->getCols();
  int4 colsArray2 = arg1->getCols();
  int4 rowsArray1 = arg0->getRows();
  int4 rowsArray2 = arg1->getRows();
  T * data1 = arg0->getData();
  T * data2 = arg1->getData();

  T * tmp = nullptr;

  // Fortran vars
  T fp_one = 1.e0;

  OGNumeric::Ptr ret;

  if (colsArray1 == 1 && rowsArray1 == 1) { // We have scalar * matrix
    T deref = data1[0];
    int4 n = rowsArray2 * colsArray2;
    tmp = new T[n];
    memcpy(tmp,data2,n*sizeof(T));
    lapack::xscal(&n,&deref,tmp,lapack::ione);
    ret = makeConcreteDenseMatrix(tmp, rowsArray2, colsArray2, OWNER);
  } else if (colsArray2 == 1 && rowsArray2 == 1) { // We have matrix * scalar
    T deref = data2[0];
    int4 n = rowsArray1 * colsArray1;
    tmp = new T[n];
    memcpy(tmp,data1,n*sizeof(T));
    lapack::xscal(&n,&deref,tmp,lapack::ione);
    ret = makeConcreteDenseMatrix(tmp, rowsArray1, colsArray1, OWNER);
  } else {
    if(colsArray1!=rowsArray2)
    {
      stringstream message;
      message << "Matrices do not commute. First is: " << rowsArray1 <<"x"<< colsArray1 <<". Second is: " << rowsArray2 <<"x"<< colsArray2;
      throw rdag_error(message.str());
    }
    if (colsArray2 == 1) { // A*x
      tmp = new T[rowsArray1]();
      lapack::xgemv(lapack::N, &rowsArray1, &colsArray1, &fp_one, data1, &rowsArray1, data2, lapack::ione, &fp_one, tmp, lapack::ione);
      ret = makeConcreteDenseMatrix(tmp, rowsArray1, 1, OWNER);
    } else {
      int4 fm = rowsArray1;
      int4 fn = colsArray2;
      int4 fk = colsArray1;
      int4 lda = fm;
      int4 ldb = fk;
      T beta = 0.e0;
      tmp = new T[fm * fn];
      int4 ldc = fm;
      lapack::xgemm(lapack::N, lapack::N, &fm, &fn, &fk, &fp_one, data1, &lda, data2, &ldb, &beta, tmp, &ldc);
      ret = makeConcreteDenseMatrix(tmp, fm, fn, OWNER);
    }
  }
  // shove ret into register
  reg0.push_back(ret);
  return nullptr;
}




// MTIMES runner:
void * MTIMESRunner::run(RegContainer& reg0, OGComplexMatrix::Ptr arg0, OGComplexMatrix::Ptr arg1) const
{
  mtimes_dense_runner<complex16>(reg0, arg0, arg1);
  return nullptr;
}


void * MTIMESRunner::run(RegContainer& reg0, OGRealMatrix::Ptr arg0, OGRealMatrix::Ptr arg1) const
{
  mtimes_dense_runner<real8>(reg0, arg0, arg1);
  return nullptr;
}

void *
MTIMESRunner::run(RegContainer& reg0, OGRealScalar::Ptr arg0, OGRealScalar::Ptr arg1) const
{
    OGRealScalar::Ptr ret = OGRealScalar::create(arg0->getValue()*arg1->getValue());
    reg0.push_back(ret);
    return nullptr;
}

}
