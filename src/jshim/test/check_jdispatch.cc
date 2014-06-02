/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "terminal.hh"
#include "jterminals.hh"
#include "jdispatch.hh"
#include "gtest/gtest.h"
#include <string>
#include "warningmacros.h"
#include <iostream>
#include "jvmmanager.hh"
#include "debug.h"
#include "equals.hh"
#include "test/fake_jvm.hh"

using namespace std;
using namespace convert;

// test dispatch to real AoA

TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGExpr)
{
  OGNumeric::Ptr scal = OGRealScalar::create(10);
  OGExpr::Ptr expr = NORM2::create(scal);
  ASSERT_ANY_THROW(Real16AoA{expr});
}

TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGRealScalar)
{
  real16 value = 1234;
  OGNumeric::Ptr scal = OGRealScalar::create(value);
  Real16AoA r = Real16AoA{scal};
  ASSERT_TRUE(r.getRows()==1);
  ASSERT_TRUE(r.getCols()==1);
  ASSERT_TRUE(SingleValueFuzzyEquals(r.getData()[0][0],value));
}

TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGComplexScalar)
{
  OGNumeric::Ptr scal = OGComplexScalar::create({1,2});
  ASSERT_ANY_THROW(Real16AoA{scal});
}

TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGIntegerScalar)
{
  OGNumeric::Ptr scal = OGIntegerScalar::create(12);
  ASSERT_ANY_THROW(Real16AoA{scal});
}


TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGRealMatrix)
{
  const int rows = 3;
  const int cols = 2;
  real16 * data = new real16[rows*cols]{1,3,5,2,4,6};
  real16 ** dataAoA = new real16 * [rows];
  dataAoA[0] = new real16[cols]{1,2};
  dataAoA[1] = new real16[cols]{3,4};
  dataAoA[2] = new real16[cols]{5,6};
  OGNumeric::Ptr mat = OGRealMatrix::create(data,rows,cols, OWNER);
  Real16AoA r = Real16AoA{mat};
  ASSERT_TRUE(r.getRows()==rows);
  ASSERT_TRUE(r.getCols()==cols);
  for(int k = 0; k < rows; k++)
  {
    ASSERT_TRUE(ArrayFuzzyEquals(r.getData()[k],dataAoA[k],cols));
    delete[] dataAoA[k];
  }
  delete [] dataAoA;
}

TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGComplexMatrix)
{
  OGNumeric::Ptr mat = OGComplexMatrix::create(new complex16[1]{12},1,1, OWNER);
  ASSERT_ANY_THROW(Real16AoA{mat});
}

TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGRealDiagonalMatrix)
{
  const int rows = 3;
  const int cols = 2;
  const int dlen = rows > cols ? cols:rows;
  real16 * data = new real16[dlen]{10,20};
  real16 ** dataAoA = new real16 * [rows];
  dataAoA[0] = new real16[cols]{10,0};
  dataAoA[1] = new real16[cols]{0,20};
  dataAoA[2] = new real16[cols]{0,0};
  OGNumeric::Ptr mat = OGRealDiagonalMatrix::create(data,rows,cols);
  Real16AoA r = Real16AoA{mat};
  ASSERT_TRUE(r.getRows()==rows);
  ASSERT_TRUE(r.getCols()==cols);
  for(int k = 0; k < rows; k++)
  {
    ASSERT_TRUE(ArrayFuzzyEquals(r.getData()[k],dataAoA[k],cols));
    delete[] dataAoA[k];
  }
  delete [] dataAoA;
  delete [] data;
}

TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGComplexDiagonalMatrix)
{
  complex16 data[1] = { {12, 0} };
  OGNumeric::Ptr mat = OGComplexDiagonalMatrix::create(data,1,1);
  ASSERT_ANY_THROW(Real16AoA{mat});
}


TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGRealSparseMatrix)
{
  const int rows = 4;
  const int cols = 3;
  real16 ** dataAoA = new real16 * [rows];
  dataAoA[0] = new real16[cols]{ 1, 2, 0 };
  dataAoA[1] = new real16[cols]{ 0, 3, 0 };
  dataAoA[2] = new real16[cols]{ 4, 0, 0 };
  dataAoA[3] = new real16[cols]{ 0, 0, 5 };

  real16 * data= new real16[5]{1.0, 4.0, 2.0, 3.0, 5.0};
  int * rowInd=new int[5]{0, 2, 0, 1, 3};
  int * colPtr=new int[4]{0, 2, 4, 5};

  OGNumeric::Ptr mat = OGRealSparseMatrix::create(colPtr, rowInd, data, rows, cols);
  Real16AoA r = Real16AoA{mat};
  ASSERT_TRUE(r.getRows()==rows);
  ASSERT_TRUE(r.getCols()==cols);
  for(int k = 0; k < rows; k++)
  {
    ASSERT_TRUE(ArrayFuzzyEquals(r.getData()[k],dataAoA[k],cols));
    delete[] dataAoA[k];
  }
  delete [] dataAoA;
  delete[] data;
  delete[] rowInd;
  delete[] colPtr;
}

TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGComplexSparseMatrix)
{
  const int rows = 4;
  const int cols = 3;
  complex16 * data= new complex16[5]{{1.0,10}, {4.0,40}, {2.0,20}, {3.0,30}, {5.0,50}};
  int * rowInd=new int[5]{0, 2, 0, 1, 3};
  int * colPtr=new int[4]{0, 2, 4, 5};

  OGNumeric::Ptr mat = OGComplexSparseMatrix::create(colPtr, rowInd, data, rows, cols);
  ASSERT_ANY_THROW(Real16AoA{mat});
  delete[] data;
  delete[] rowInd;
  delete[] colPtr;
}


// test dispatch to complex AoA

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGExpr)
{
  OGNumeric::Ptr scal = OGRealScalar::create(10);
  OGExpr::Ptr expr = NORM2::create(scal);
  ASSERT_ANY_THROW(Complex16AoA{expr});
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGComplexScalar)
{
  complex16 value = {1234,5678};
  OGNumeric::Ptr scal = OGComplexScalar::create(value);
  Complex16AoA c = Complex16AoA{scal};
  ASSERT_TRUE(c.getRows()==1);
  ASSERT_TRUE(c.getCols()==1);
  ASSERT_TRUE(SingleValueFuzzyEquals(c.getData()[0][0],value));
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGRealScalar)
{
  complex16 value = {1234,0};
  OGNumeric::Ptr scal = OGRealScalar::create(std::real(value));
  Complex16AoA c = Complex16AoA{scal};
  ASSERT_TRUE(c.getRows()==1);
  ASSERT_TRUE(c.getCols()==1);
  ASSERT_TRUE(SingleValueFuzzyEquals(c.getData()[0][0],value));
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGIntegerScalar)
{
  OGNumeric::Ptr scal = OGIntegerScalar::create(10);
  ASSERT_ANY_THROW(Complex16AoA{scal});
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGComplexMatrix)
{
  const int rows = 3;
  const int cols = 2;
  complex16 * data = new complex16[rows*cols]{{1,10},{3,30},{5,50},{2,20},{4,40},{6,60}};
  complex16 ** dataAoA = new complex16 * [rows];
  dataAoA[0] = new complex16[cols]{{1,10},{2,20}};
  dataAoA[1] = new complex16[cols]{{3,30},{4,40}};
  dataAoA[2] = new complex16[cols]{{5,50},{6,60}};
  OGNumeric::Ptr mat = OGComplexMatrix::create(data,rows,cols,OWNER);
  Complex16AoA c = Complex16AoA{mat};
  ASSERT_TRUE(c.getRows()==rows);
  ASSERT_TRUE(c.getCols()==cols);
  for(int k = 0; k < rows; k++)
  {
    ASSERT_TRUE(ArrayFuzzyEquals(c.getData()[k],dataAoA[k],cols));
    delete[] dataAoA[k];
  }
  delete [] dataAoA;
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGRealMatrix)
{
  const int rows = 3;
  const int cols = 2;
  real16 * data = new real16[rows*cols]{1,3,5,2,4,6};
  complex16 ** dataAoA = new complex16 * [rows];
  dataAoA[0] = new complex16[cols]{{1,0},{2,0}};
  dataAoA[1] = new complex16[cols]{{3,0},{4,0}};
  dataAoA[2] = new complex16[cols]{{5,0},{6,0}};
  OGNumeric::Ptr mat = OGRealMatrix::create(data,rows,cols,OWNER);
  Complex16AoA c = Complex16AoA{mat};
  ASSERT_TRUE(c.getRows()==rows);
  ASSERT_TRUE(c.getCols()==cols);
  for(int k = 0; k < rows; k++)
  {
    ASSERT_TRUE(ArrayFuzzyEquals(c.getData()[k],dataAoA[k],cols));
    delete[] dataAoA[k];
  }
  delete [] dataAoA;
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGComplexDiagonalMatrix)
{
  const int rows = 3;
  const int cols = 2;
  const int dlen = rows > cols ? cols:rows;
  complex16 * data = new complex16[dlen]{10,20};
  complex16 ** dataAoA = new complex16 * [rows];
  dataAoA[0] = new complex16[cols]{10,0};
  dataAoA[1] = new complex16[cols]{0,20};
  dataAoA[2] = new complex16[cols]{0,0};
  OGNumeric::Ptr mat = OGComplexDiagonalMatrix::create(data,rows,cols);
  Complex16AoA c = Complex16AoA{mat};
  ASSERT_TRUE(c.getRows()==rows);
  ASSERT_TRUE(c.getCols()==cols);
  for(int k = 0; k < rows; k++)
  {
    ASSERT_TRUE(ArrayFuzzyEquals(c.getData()[k],dataAoA[k],cols));
    delete[] dataAoA[k];
  }
  delete [] dataAoA;
  delete [] data;
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGRealDiagonalMatrix)
{
  real16 * data = new real16[1]{12};
  OGNumeric::Ptr mat = OGRealDiagonalMatrix::create(data,1,1);
  ASSERT_ANY_THROW(Complex16AoA{mat});
  delete [] data;
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGComplexSparseMatrix)
{
  const int rows = 4;
  const int cols = 3;
  complex16 ** dataAoA = new complex16 * [rows];
  dataAoA[0] = new complex16[cols]{ {1,10}, {2,20}, {0,0} };
  dataAoA[1] = new complex16[cols]{ {0,0} , {3,30}, {0,0} };
  dataAoA[2] = new complex16[cols]{ {4,40}, {0,0} , {0,0} };
  dataAoA[3] = new complex16[cols]{ {0,0} , {0,0} , {5,50} };

  complex16 * data= new complex16[5]{{1.0,10.0}, {4.0,40}, {2.0,20}, {3.0,30}, {5.0,50}};
  int * rowInd=new int[5]{0, 2, 0, 1, 3};
  int * colPtr=new int[4]{0, 2, 4, 5};

  OGNumeric::Ptr mat = OGComplexSparseMatrix::create(colPtr, rowInd, data, rows, cols);
  Complex16AoA c = Complex16AoA{mat};
  ASSERT_TRUE(c.getRows()==rows);
  ASSERT_TRUE(c.getCols()==cols);
  for(int k = 0; k < rows; k++)
  {
    ASSERT_TRUE(ArrayFuzzyEquals(c.getData()[k],dataAoA[k],cols));
    delete[] dataAoA[k];
  }
  delete [] dataAoA;
  delete[] data;
  delete[] rowInd;
  delete[] colPtr;
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGRealSparseMatrix)
{

  const int rows = 4;
  const int cols = 3;
  real16 * data= new real16[5]{1,4,2,3,5};
  int * rowInd=new int[5]{0, 2, 0, 1, 3};
  int * colPtr=new int[4]{0, 2, 4, 5};

  OGNumeric::Ptr mat = OGRealSparseMatrix::create(colPtr, rowInd, data, rows, cols);
  ASSERT_ANY_THROW(Complex16AoA{mat});
  delete[] data;
  delete[] rowInd;
  delete[] colPtr;
}
