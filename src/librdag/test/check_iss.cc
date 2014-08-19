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
    OGRealDenseMatrix::Ptr _ogrealmatrix = nullptr;
    OGComplexDenseMatrix::Ptr _ogcomplexmatrix = nullptr;
    OGLogicalMatrix::Ptr _oglogicalmatrix = nullptr;
    OGRealDiagonalMatrix::Ptr _ogrealdiagonalmatrix = nullptr;
    OGComplexDiagonalMatrix::Ptr _ogcomplexdiagonalmatrix = nullptr;
    OGRealSparseMatrix::Ptr _ogrealsparsematrix = nullptr;
    OGComplexSparseMatrix::Ptr _ogcomplexsparsematrix = nullptr;
    OGRealDenseMatrix::Ptr _ogrealvector1 = nullptr;
    OGRealDenseMatrix::Ptr _ogrealvector2 = nullptr;
    OGComplexDenseMatrix::Ptr _ogcomplexvector1 = nullptr;
    OGComplexDenseMatrix::Ptr _ogcomplexvector2 = nullptr;
    real8 * r_data;
    complex16 * c_data;
    int4 * colPtr;
    int4 * rowIdx;
    virtual void SetUp()
  {
    r_data = new real8[1]{12};
    c_data = new complex16[1]{{12,34}};
    rowIdx = new int4[1]{0};
    colPtr = new int4[2]{0,1};
    _ogrealscalar = OGRealScalar::create(12);
    _ogcomplexscalar = OGComplexScalar::create({12,34});
    _ogintegerscalar = OGIntegerScalar::create(4);
    _ogrealmatrix = OGRealDenseMatrix::create(r_data,1,1);
    _ogcomplexmatrix = OGComplexDenseMatrix::create(c_data,1,1);
    _oglogicalmatrix = OGLogicalMatrix::create(r_data,1,1);
    _ogrealdiagonalmatrix = OGRealDiagonalMatrix::create(r_data,1,1);
    _ogcomplexdiagonalmatrix = OGComplexDiagonalMatrix::create(c_data,1,1);
    _ogrealsparsematrix = OGRealSparseMatrix::create(colPtr,rowIdx,r_data,1,1);
    _ogcomplexsparsematrix = OGComplexSparseMatrix::create(colPtr,rowIdx,c_data,1,1);
    _ogrealvector1 = OGRealDenseMatrix::create(new real8[3]{1,2,3},1,3, OWNER);
    _ogrealvector2 = OGRealDenseMatrix::create(new real8[3]{1,2,3},3,1, OWNER);
    _ogcomplexvector1 = OGComplexDenseMatrix::create(new complex16[3]{{1,10},{2,20},{3,30}},1,3, OWNER);
    _ogcomplexvector2 = OGComplexDenseMatrix::create(new complex16[3]{{1,10},{2,20},{3,30}},3,1, OWNER);
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


TEST(issTest_isfinite, real8)
{
  std::unique_ptr<real8> rd (new real8[4]);
  real8 * r = rd.get();
  r[0]=1;
  r[1]=2;
  r[2]=std::numeric_limits<real8>::signaling_NaN();
  r[3]=4;

  EXPECT_TRUE(isfinite(r, 2));
  EXPECT_FALSE(isfinite(r, 4));
}

TEST(issTest_isfinite, complex16)
{
  std::unique_ptr<complex16> cd (new complex16[4]);
  complex16 * c = cd.get();
  c[0]={1,2};
  c[1]={2,3};
  c[2]={std::numeric_limits<real8>::signaling_NaN(),1};
  c[3]={4,3};

  EXPECT_TRUE(isfinite(c, 2));
  EXPECT_FALSE(isfinite(c, 4));

  // check NaN in imag part
  c[2]={5,std::numeric_limits<real8>::signaling_NaN()};
  EXPECT_FALSE(isfinite(c, 4));

}