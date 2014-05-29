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
    OGRealScalar::Ptr _ogrealscalar = nullptr;
    OGComplexScalar::Ptr _ogcomplexscalar = nullptr;
    OGIntegerScalar::Ptr _ogintegerscalar = nullptr;
    OGRealMatrix::Ptr _ogrealmatrix = nullptr;
    OGComplexMatrix::Ptr _ogcomplexmatrix = nullptr;
    OGLogicalMatrix::Ptr _oglogicalmatrix = nullptr;
    OGRealDiagonalMatrix::Ptr _ogrealdiagonalmatrix = nullptr;
    OGComplexDiagonalMatrix::Ptr _ogcomplexdiagonalmatrix = nullptr;
    OGRealSparseMatrix::Ptr _ogrealsparsematrix = nullptr;
    OGComplexSparseMatrix::Ptr _ogcomplexsparsematrix = nullptr;
    OGRealMatrix::Ptr _ogrealvector1 = nullptr;
    OGRealMatrix::Ptr _ogrealvector2 = nullptr;
    OGComplexMatrix::Ptr _ogcomplexvector1 = nullptr;
    OGComplexMatrix::Ptr _ogcomplexvector2 = nullptr;
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
    _ogrealscalar = OGRealScalar::create(12);
    _ogcomplexscalar = OGComplexScalar::create({12,34});
    _ogintegerscalar = OGIntegerScalar::create(4);
    _ogrealmatrix = OGRealMatrix::create(r_data,1,1);
    _ogcomplexmatrix = OGComplexMatrix::create(c_data,1,1);
    _oglogicalmatrix = OGLogicalMatrix::create(r_data,1,1);
    _ogrealdiagonalmatrix = OGRealDiagonalMatrix::create(r_data,1,1);
    _ogcomplexdiagonalmatrix = OGComplexDiagonalMatrix::create(c_data,1,1);
    _ogrealsparsematrix = OGRealSparseMatrix::create(colPtr,rowIdx,r_data,1,1);
    _ogcomplexsparsematrix = OGComplexSparseMatrix::create(colPtr,rowIdx,c_data,1,1);
    _ogrealvector1 = OGRealMatrix::create(new real16[3]{1,2,3},1,3, OWNER);
    _ogrealvector2 = OGRealMatrix::create(new real16[3]{1,2,3},3,1, OWNER);
    _ogcomplexvector1 = OGComplexMatrix::create(new complex16[3]{{1,10},{2,20},{3,30}},1,3, OWNER);
    _ogcomplexvector2 = OGComplexMatrix::create(new complex16[3]{{1,10},{2,20},{3,30}},3,1, OWNER);
  }

  virtual void TearDown()
  {
    delete [] r_data;
    delete [] c_data;
    delete [] rowIdx;
    delete [] colPtr;
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
}
