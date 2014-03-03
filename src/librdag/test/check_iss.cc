/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "gtest/gtest.h"
#include "warningmacros.h"
#include "iss.hh"
#include "terminal.hh"
#include <cfloat>
#include <iostream>
#include <cstdint>

using namespace std;
using namespace librdag;

class issTest : public ::testing::Test
{
  protected:
    OGRealScalar * _ogrealscalar = nullptr;
    OGComplexScalar * _ogcomplexscalar = nullptr;
    OGIntegerScalar * _ogintegerscalar = nullptr;
    OGRealMatrix * _ogrealmatrix = nullptr;
    OGComplexMatrix * _ogcomplexmatrix = nullptr;
    OGLogicalMatrix * _oglogicalmatrix = nullptr;
    OGRealDiagonalMatrix * _ogrealdiagonalmatrix = nullptr;
    OGComplexDiagonalMatrix * _ogcomplexdiagonalmatrix = nullptr;
    OGRealSparseMatrix * _ogrealsparsematrix = nullptr;
    OGComplexSparseMatrix * _ogcomplexsparsematrix = nullptr;
    OGOwningRealMatrix * _ogowningrealmatrix = nullptr;
    OGOwningComplexMatrix * _ogowningcomplexmatrix = nullptr;
    OGOwningRealMatrix * _ogrealvector1 = nullptr;
    OGOwningRealMatrix * _ogrealvector2 = nullptr;
    OGOwningComplexMatrix * _ogcomplexvector1 = nullptr;
    OGOwningComplexMatrix * _ogcomplexvector2 = nullptr;
    real16 * r_data;
    complex16 * c_data;
    int * colPtr;
    int * rowIdx;
    virtual void SetUp()
  {
    r_data = new real16[1]{12};
    c_data = new complex16[1]{{12,34}};
    rowIdx = new int[1]{0};
    colPtr = new int[2]{0,1};
    _ogrealscalar = new OGRealScalar(12);
    _ogcomplexscalar = new OGComplexScalar({12,34});
    _ogintegerscalar = new OGIntegerScalar(4);
    _ogrealmatrix = new OGRealMatrix(r_data,1,1);
    _ogcomplexmatrix = new OGComplexMatrix(c_data,1,1);
    _oglogicalmatrix = new OGLogicalMatrix(r_data,1,1);
    _ogrealdiagonalmatrix = new OGRealDiagonalMatrix(r_data,1,1);
    _ogcomplexdiagonalmatrix = new OGComplexDiagonalMatrix(c_data,1,1);
    _ogrealsparsematrix = new OGRealSparseMatrix(colPtr,rowIdx,r_data,1,1);
    _ogcomplexsparsematrix = new OGComplexSparseMatrix(colPtr,rowIdx,c_data,1,1);
    _ogowningrealmatrix = new OGOwningRealMatrix(new real16[1]{1},1,1);
    _ogowningcomplexmatrix = new OGOwningComplexMatrix(new complex16[1]{{1,2}},1,1);
    _ogrealvector1 = new OGOwningRealMatrix(new real16[3]{1,2,3},1,3);
    _ogrealvector2 = new OGOwningRealMatrix(new real16[3]{1,2,3},3,1);
    _ogcomplexvector1 = new OGOwningComplexMatrix(new complex16[3]{{1,10},{2,20},{3,30}},1,3);
    _ogcomplexvector2 = new OGOwningComplexMatrix(new complex16[3]{{1,10},{2,20},{3,30}},3,1);
  }

  virtual void TearDown()
  {
    delete [] r_data;
    delete [] c_data;
    delete [] rowIdx;
    delete [] colPtr;
    delete _ogrealscalar;
    delete _ogcomplexscalar;
    delete _ogintegerscalar;
    delete _ogrealmatrix;
    delete _ogcomplexmatrix;
    delete _oglogicalmatrix;
    delete _ogrealdiagonalmatrix;
    delete _ogcomplexdiagonalmatrix;
    delete _ogrealsparsematrix;
    delete _ogcomplexsparsematrix;
    delete _ogowningrealmatrix;
    delete _ogowningcomplexmatrix;
    delete _ogrealvector1;
    delete _ogrealvector2;
    delete _ogcomplexvector1;
    delete _ogcomplexvector2;
  }
};

TEST_F(issTest, isReal)
{
  ASSERT_TRUE (isReal(_ogrealscalar));
  ASSERT_FALSE(isReal(_ogcomplexscalar));
  ASSERT_TRUE (isReal(_ogintegerscalar));
  ASSERT_TRUE (isReal(_ogrealmatrix));
  ASSERT_FALSE(isReal(_ogcomplexmatrix));
  ASSERT_TRUE (isReal(_oglogicalmatrix));
  ASSERT_TRUE (isReal(_ogrealdiagonalmatrix));
  ASSERT_FALSE(isReal(_ogcomplexdiagonalmatrix));
  ASSERT_TRUE (isReal(_ogrealsparsematrix));
  ASSERT_FALSE(isReal(_ogcomplexsparsematrix));
  ASSERT_TRUE (isReal(_ogowningrealmatrix));
  ASSERT_FALSE(isReal(_ogowningcomplexmatrix));
}

TEST_F(issTest, isComplex)
{
  ASSERT_FALSE (isComplex(_ogrealscalar));
  ASSERT_TRUE(isComplex(_ogcomplexscalar));
  ASSERT_FALSE (isComplex(_ogintegerscalar));
  ASSERT_FALSE (isComplex(_ogrealmatrix));
  ASSERT_TRUE(isComplex(_ogcomplexmatrix));
  ASSERT_FALSE (isComplex(_oglogicalmatrix));
  ASSERT_FALSE (isComplex(_ogrealdiagonalmatrix));
  ASSERT_TRUE(isComplex(_ogcomplexdiagonalmatrix));
  ASSERT_FALSE (isComplex(_ogrealsparsematrix));
  ASSERT_TRUE(isComplex(_ogcomplexsparsematrix));
  ASSERT_FALSE (isComplex(_ogowningrealmatrix));
  ASSERT_TRUE(isComplex(_ogowningcomplexmatrix));
}


TEST_F(issTest, isScalar)
{
  ASSERT_TRUE (isScalar(_ogrealscalar));
  ASSERT_TRUE (isScalar(_ogcomplexscalar));
  ASSERT_TRUE (isScalar(_ogintegerscalar));
  ASSERT_FALSE(isScalar(_ogrealmatrix));
  ASSERT_FALSE(isScalar(_ogcomplexmatrix));
  ASSERT_FALSE(isScalar(_oglogicalmatrix));
  ASSERT_FALSE(isScalar(_ogrealdiagonalmatrix));
  ASSERT_FALSE(isScalar(_ogcomplexdiagonalmatrix));
  ASSERT_FALSE(isScalar(_ogrealsparsematrix));
  ASSERT_FALSE(isScalar(_ogcomplexsparsematrix));
  ASSERT_FALSE(isScalar(_ogowningrealmatrix));
  ASSERT_FALSE(isScalar(_ogowningcomplexmatrix));
}

TEST_F(issTest, isVector)
{
  ASSERT_FALSE(isVector(_ogrealscalar));
  ASSERT_FALSE(isVector(_ogcomplexscalar));
  ASSERT_FALSE(isVector(_ogintegerscalar));
  ASSERT_FALSE(isVector(_ogrealmatrix));
  ASSERT_FALSE(isVector(_ogcomplexmatrix));
  ASSERT_FALSE(isVector(_oglogicalmatrix));
  ASSERT_FALSE(isVector(_ogrealdiagonalmatrix));
  ASSERT_FALSE(isVector(_ogcomplexdiagonalmatrix));
  ASSERT_FALSE(isVector(_ogrealsparsematrix));
  ASSERT_FALSE(isVector(_ogcomplexsparsematrix));
  ASSERT_FALSE(isVector(_ogowningrealmatrix));
  ASSERT_FALSE(isVector(_ogowningcomplexmatrix));
  ASSERT_TRUE(isVector(_ogrealvector1));
  ASSERT_TRUE(isVector(_ogrealvector2));
  ASSERT_TRUE(isVector(_ogcomplexvector1));
  ASSERT_TRUE(isVector(_ogcomplexvector2));
}

TEST_F(issTest, isMatrix)
{
  ASSERT_FALSE(isMatrix(_ogrealscalar));
  ASSERT_FALSE(isMatrix(_ogcomplexscalar));
  ASSERT_FALSE (isMatrix(_ogintegerscalar));
  ASSERT_TRUE (isMatrix(_ogrealmatrix));
  ASSERT_TRUE (isMatrix(_ogcomplexmatrix));
  ASSERT_TRUE (isMatrix(_oglogicalmatrix));
  ASSERT_TRUE (isMatrix(_ogrealdiagonalmatrix));
  ASSERT_TRUE (isMatrix(_ogcomplexdiagonalmatrix));
  ASSERT_TRUE (isMatrix(_ogrealsparsematrix));
  ASSERT_TRUE (isMatrix(_ogcomplexsparsematrix));
  ASSERT_TRUE (isMatrix(_ogowningrealmatrix));
  ASSERT_TRUE (isMatrix(_ogowningcomplexmatrix));
}