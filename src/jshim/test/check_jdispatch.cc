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

using namespace std;
using namespace convert;

// mock JVM
class Fake_JavaVM: public JavaVM
{
  public:
    virtual ~Fake_JavaVM(){};
    virtual int GetEnv(void ** env, int SUPPRESS_UNUSED version) override
    {
     *env = _env;
     return JNI_OK;
    }
    void setEnv(JNIEnv * env)
    {
        this->_env = env;
    }
    JNIEnv * getEnv()
    {
        return this->_env;
    }
    virtual jint AttachCurrentThread(void SUPPRESS_UNUSED **penv, void SUPPRESS_UNUSED *args)
    {
      *penv = this->_env;
      return JNI_OK;
    }
};

// mock JNIEnv
class Fake_JNIEnv: public JNIEnv
{
  public:
    Fake_JNIEnv()
    {
      _someNonClass = new _jclass();
      _someNonMethodID = new _jmethodID();
      _someNonFieldID = new _jfieldID();
    }
    virtual ~Fake_JNIEnv()
    {
      delete _someNonMethodID;
      delete _someNonFieldID;
      delete _someNonClass;
    }
    virtual jclass FindClass(const char SUPPRESS_UNUSED * name)
    {
      return _someNonClass;
    }
    virtual jmethodID GetMethodID(jclass SUPPRESS_UNUSED clazz, const char SUPPRESS_UNUSED *name, const char SUPPRESS_UNUSED *sig) override
    {
      return _someNonMethodID;
    }
    virtual jfieldID GetFieldID(jclass SUPPRESS_UNUSED clazz, const char SUPPRESS_UNUSED *name, const char SUPPRESS_UNUSED *sig) override
    {
      return _someNonFieldID;
    }
private:
    jclass _someNonClass;
    jmethodID _someNonMethodID;
    jfieldID _someNonFieldID;
};

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