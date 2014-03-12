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
  DispatchToReal16ArrayOfArrays * d = new DispatchToReal16ArrayOfArrays();
  OGRealScalar * scal = new OGRealScalar(10);
  ArgContainer * args = new ArgContainer();
  args->push_back(scal);
  OGExpr * expr = new NORM2(args);
  ASSERT_ANY_THROW(d->visit(expr));
  delete d;
  delete expr;
}

TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGRealScalar)
{
  DispatchToReal16ArrayOfArrays * d = new DispatchToReal16ArrayOfArrays();
  real16 value = 1234;
  OGRealScalar * scal = new OGRealScalar(value);
  d->visit(scal);
  ASSERT_TRUE(d->getRows()==1);
  ASSERT_TRUE(d->getCols()==1);
  ASSERT_TRUE(SingleValueFuzzyEquals(d->getData()[0][0],value));
  delete d;
  delete scal;
}

TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGComplexScalar)
{
  DispatchToReal16ArrayOfArrays * d = new DispatchToReal16ArrayOfArrays();
  OGComplexScalar * scal = new OGComplexScalar({1,2});
  ASSERT_ANY_THROW(d->visit(scal));
  delete d;
  delete scal;
}

TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGIntegerScalar)
{
  DispatchToReal16ArrayOfArrays * d = new DispatchToReal16ArrayOfArrays();
  OGIntegerScalar * scal = new OGIntegerScalar(12);
  ASSERT_ANY_THROW(d->visit(scal));
  delete d;
  delete scal;
}


TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGRealMatrix)
{
  DispatchToReal16ArrayOfArrays * d = new DispatchToReal16ArrayOfArrays();
  const int rows = 3;
  const int cols = 2;
  real16 * data = new real16[rows*cols]{1,3,5,2,4,6};
  real16 ** dataAoA = new real16 * [rows];
  dataAoA[0] = new real16[cols]{1,2};
  dataAoA[1] = new real16[cols]{3,4};
  dataAoA[2] = new real16[cols]{5,6};
  OGRealMatrix * mat = new OGOwningRealMatrix(data,rows,cols);
  d->visit(mat);
  ASSERT_TRUE(d->getRows()==rows);
  ASSERT_TRUE(d->getCols()==cols);
  for(int k = 0; k < rows; k++)
  {
    ASSERT_TRUE(ArrayFuzzyEquals(d->getData()[k],dataAoA[k],cols));
    delete[] dataAoA[k];
  }
  delete [] dataAoA;
  delete d;
  delete mat;
}

TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGComplexMatrix)
{
  DispatchToReal16ArrayOfArrays * d = new DispatchToReal16ArrayOfArrays();
  OGOwningComplexMatrix * mat = new OGOwningComplexMatrix(new complex16[1]{12},1,1);
  ASSERT_ANY_THROW(d->visit(mat));
  delete d;
  delete mat;
}

TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGRealDiagonalMatrix)
{
  DispatchToReal16ArrayOfArrays * d = new DispatchToReal16ArrayOfArrays();
  const int rows = 3;
  const int cols = 2;
  const int dlen = rows > cols ? cols:rows;
  real16 * data = new real16[dlen]{10,20};
  real16 ** dataAoA = new real16 * [rows];
  dataAoA[0] = new real16[cols]{10,0};
  dataAoA[1] = new real16[cols]{0,20};
  dataAoA[2] = new real16[cols]{0,0};
  OGRealDiagonalMatrix * mat = new OGRealDiagonalMatrix(data,rows,cols);
  d->visit(mat);
  ASSERT_TRUE(d->getRows()==rows);
  ASSERT_TRUE(d->getCols()==cols);
  for(int k = 0; k < rows; k++)
  {
    ASSERT_TRUE(ArrayFuzzyEquals(d->getData()[k],dataAoA[k],cols));
    delete[] dataAoA[k];
  }
  delete [] dataAoA;
  delete [] data;
  delete d;
  delete mat;
}

TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGComplexDiagonalMatrix)
{
  DispatchToReal16ArrayOfArrays * d = new DispatchToReal16ArrayOfArrays();
  complex16 * data = new complex16[1]{12};
  OGComplexDiagonalMatrix * mat = new OGComplexDiagonalMatrix(data,1,1);
  ASSERT_ANY_THROW(d->visit(mat));
  delete d;
  delete mat;
  delete [] data;
}


TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGRealSparseMatrix)
{
  DispatchToReal16ArrayOfArrays * d = new DispatchToReal16ArrayOfArrays();
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

  OGRealSparseMatrix * mat = new OGRealSparseMatrix(colPtr, rowInd, data, rows, cols);
  d->visit(mat);
  ASSERT_TRUE(d->getRows()==rows);
  ASSERT_TRUE(d->getCols()==cols);
  for(int k = 0; k < rows; k++)
  {
    ASSERT_TRUE(ArrayFuzzyEquals(d->getData()[k],dataAoA[k],cols));
    delete[] dataAoA[k];
  }
  delete [] dataAoA;
  delete d;
  delete mat;
  delete[] data;
  delete[] rowInd;
  delete[] colPtr;
}

TEST(JDispatch, Test_DispatchToReal16ArrayOfArrays_OGComplexSparseMatrix)
{
  DispatchToReal16ArrayOfArrays * d = new DispatchToReal16ArrayOfArrays();
  const int rows = 4;
  const int cols = 3;
  complex16 * data= new complex16[5]{{1.0,10}, {4.0,40}, {2.0,20}, {3.0,30}, {5.0,50}};
  int * rowInd=new int[5]{0, 2, 0, 1, 3};
  int * colPtr=new int[4]{0, 2, 4, 5};

  OGComplexSparseMatrix * mat = new OGComplexSparseMatrix(colPtr, rowInd, data, rows, cols);
  ASSERT_ANY_THROW(d->visit(mat));
  delete d;
  delete mat;
  delete[] data;
  delete[] rowInd;
  delete[] colPtr;
}


// test dispatch to complex AoA

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGExpr)
{
  DispatchToComplex16ArrayOfArrays * d = new DispatchToComplex16ArrayOfArrays();
  OGRealScalar * scal = new OGRealScalar(10);
  ArgContainer * args = new ArgContainer();
  args->push_back(scal);
  OGExpr * expr = new NORM2(args);
  ASSERT_ANY_THROW(d->visit(expr));
  delete d;
  delete expr;
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGComplexScalar)
{
  DispatchToComplex16ArrayOfArrays * d = new DispatchToComplex16ArrayOfArrays();
  complex16 value = {1234,5678};
  OGComplexScalar * scal = new OGComplexScalar(value);
  d->visit(scal);
  ASSERT_TRUE(d->getRows()==1);
  ASSERT_TRUE(d->getCols()==1);
  ASSERT_TRUE(SingleValueFuzzyEquals(d->getData()[0][0],value));
  delete d;
  delete scal;
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGRealScalar)
{
  DispatchToComplex16ArrayOfArrays * d = new DispatchToComplex16ArrayOfArrays();
  OGRealScalar * scal = new OGRealScalar(1234);
  ASSERT_ANY_THROW(d->visit(scal));
  delete d;
  delete scal;
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGIntegerScalar)
{
  DispatchToComplex16ArrayOfArrays * d = new DispatchToComplex16ArrayOfArrays();
  OGIntegerScalar * scal = new OGIntegerScalar(10);
  ASSERT_ANY_THROW(d->visit(scal));
  delete d;
  delete scal;
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGComplexMatrix)
{
  DispatchToComplex16ArrayOfArrays * d = new DispatchToComplex16ArrayOfArrays();
  const int rows = 3;
  const int cols = 2;
  complex16 * data = new complex16[rows*cols]{{1,10},{3,30},{5,50},{2,20},{4,40},{6,60}};
  complex16 ** dataAoA = new complex16 * [rows];
  dataAoA[0] = new complex16[cols]{{1,10},{2,20}};
  dataAoA[1] = new complex16[cols]{{3,30},{4,40}};
  dataAoA[2] = new complex16[cols]{{5,50},{6,60}};
  OGComplexMatrix * mat = new OGOwningComplexMatrix(data,rows,cols);
  d->visit(mat);
  ASSERT_TRUE(d->getRows()==rows);
  ASSERT_TRUE(d->getCols()==cols);
  for(int k = 0; k < rows; k++)
  {
    ASSERT_TRUE(ArrayFuzzyEquals(d->getData()[k],dataAoA[k],cols));
    delete[] dataAoA[k];
  }
  delete [] dataAoA;
  delete d;
  delete mat;
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGRealMatrix)
{
  DispatchToComplex16ArrayOfArrays * d = new DispatchToComplex16ArrayOfArrays();
  OGRealMatrix * mat = new OGOwningRealMatrix(new real16[4]{1,2,3,4},2,2);
  ASSERT_ANY_THROW(d->visit(mat));
  delete d;
  delete mat;
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGComplexDiagonalMatrix)
{
  DispatchToComplex16ArrayOfArrays * d = new DispatchToComplex16ArrayOfArrays();
  const int rows = 3;
  const int cols = 2;
  const int dlen = rows > cols ? cols:rows;
  complex16 * data = new complex16[dlen]{10,20};
  complex16 ** dataAoA = new complex16 * [rows];
  dataAoA[0] = new complex16[cols]{10,0};
  dataAoA[1] = new complex16[cols]{0,20};
  dataAoA[2] = new complex16[cols]{0,0};
  OGComplexDiagonalMatrix * mat = new OGComplexDiagonalMatrix(data,rows,cols);
  d->visit(mat);
  ASSERT_TRUE(d->getRows()==rows);
  ASSERT_TRUE(d->getCols()==cols);
  for(int k = 0; k < rows; k++)
  {
    ASSERT_TRUE(ArrayFuzzyEquals(d->getData()[k],dataAoA[k],cols));
    delete[] dataAoA[k];
  }
  delete [] dataAoA;
  delete [] data;
  delete d;
  delete mat;
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGRealDiagonalMatrix)
{
  DispatchToComplex16ArrayOfArrays * d = new DispatchToComplex16ArrayOfArrays();
  real16 * data = new real16[1]{12};
  OGRealDiagonalMatrix * mat = new OGRealDiagonalMatrix(data,1,1);
  ASSERT_ANY_THROW(d->visit(mat));
  delete d;
  delete mat;
  delete [] data;
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGComplexSparseMatrix)
{
  DispatchToComplex16ArrayOfArrays * d = new DispatchToComplex16ArrayOfArrays();
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

  OGComplexSparseMatrix * mat = new OGComplexSparseMatrix(colPtr, rowInd, data, rows, cols);
  d->visit(mat);
  ASSERT_TRUE(d->getRows()==rows);
  ASSERT_TRUE(d->getCols()==cols);
  for(int k = 0; k < rows; k++)
  {
    ASSERT_TRUE(ArrayFuzzyEquals(d->getData()[k],dataAoA[k],cols));
    delete[] dataAoA[k];
  }
  delete [] dataAoA;
  delete d;
  delete mat;
  delete[] data;
  delete[] rowInd;
  delete[] colPtr;
}

TEST(JDispatch, Test_DispatchToComplex16ArrayOfArrays_OGRealSparseMatrix)
{

  DispatchToComplex16ArrayOfArrays * d = new DispatchToComplex16ArrayOfArrays();
  const int rows = 4;
  const int cols = 3;
  real16 * data= new real16[5]{1,4,2,3,5};
  int * rowInd=new int[5]{0, 2, 0, 1, 3};
  int * colPtr=new int[4]{0, 2, 4, 5};

  OGRealSparseMatrix * mat = new OGRealSparseMatrix(colPtr, rowInd, data, rows, cols);
  ASSERT_ANY_THROW(d->visit(mat));
  delete d;
  delete mat;
  delete[] data;
  delete[] rowInd;
  delete[] colPtr;
}
