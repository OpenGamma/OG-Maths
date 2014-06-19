/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "terminal.hh"
#include "expression.hh"
#include "gtest/gtest.h"
#include <string>

using namespace std;
using namespace librdag;

// Generate a check that ensures that the cast returns a pointer to the type, or nullptr as required.
#define CHECK_CAST(CLAZZ) \
  if(name.compare(#CLAZZ)==0) { EXPECT_EQ(node, node->as##CLAZZ()); } else { EXPECT_EQ(CLAZZ::Ptr{}, node->as##CLAZZ()); }

// Check casts for all types. If a new type is added it should have a CHECK_CAST entry added.
void check_rtti(OGNumeric::Ptr node, string name)
{
  CHECK_CAST(COPY);
  CHECK_CAST(PLUS);
  CHECK_CAST(MTIMES);
  CHECK_CAST(NEGATE);
  CHECK_CAST(SVD);
  CHECK_CAST(LU);
  CHECK_CAST(PINV);
  CHECK_CAST(TRANSPOSE);
  CHECK_CAST(CTRANSPOSE);
  CHECK_CAST(SELECTRESULT);
  CHECK_CAST(OGRealScalar);
  CHECK_CAST(OGComplexScalar);
  CHECK_CAST(OGIntegerScalar);
  CHECK_CAST(OGRealDenseMatrix);
  CHECK_CAST(OGComplexDenseMatrix);
  CHECK_CAST(OGRealDiagonalMatrix);
  CHECK_CAST(OGComplexDiagonalMatrix);
  CHECK_CAST(OGRealSparseMatrix);
  CHECK_CAST(OGComplexSparseMatrix);
}

// Creates an OGRealScalar for use in construction of a unary node.
class RTTIUnaryTest: public testing::Test
{
  protected:
    virtual void SetUp()
    {
      arg = OGRealScalar::create(1.0);
    }

    OGNumeric::Ptr node;
    OGNumeric::Ptr arg;
};

// Creates two OGRealScalars for use in construction of a binary node.
class RTTIBinaryTest: public testing::Test
{
  protected:
    virtual void SetUp()
    {
      arg0 = OGRealScalar::create(1.0);
      arg1 = OGRealScalar::create(2.0);
    }

    OGNumeric::Ptr node;
    OGNumeric::Ptr arg0;
    OGNumeric::Ptr arg1;
};

TEST_F(RTTIUnaryTest, TestCOPY) {
  node = COPY::create(arg);
  check_rtti(node, "COPY");
}

TEST_F(RTTIUnaryTest, TestSVD) {
  node = SVD::create(arg);
  check_rtti(node, "SVD");
}

TEST_F(RTTIBinaryTest, TestPLUS) {
  node = PLUS::create(arg0, arg1);
  check_rtti(node, "PLUS");
}

TEST_F(RTTIBinaryTest, TestMTIMES) {
  node = MTIMES::create(arg0, arg1);
  check_rtti(node, "MTIMES");
}

TEST_F(RTTIUnaryTest, TestNEGATE) {
  node = NEGATE::create(arg);
  check_rtti(node, "NEGATE");
}

TEST_F(RTTIUnaryTest, TestLU) {
  node = LU::create(arg);
  check_rtti(node, "LU");
}

TEST_F(RTTIUnaryTest, TestPINV) {
  node = PINV::create(arg);
  check_rtti(node, "PINV");
}

TEST_F(RTTIUnaryTest, TestTRANSPOSE) {
  node = TRANSPOSE::create(arg);
  check_rtti(node, "TRANSPOSE");
}

TEST_F(RTTIUnaryTest, TestCTRANSPOSE) {
  node = CTRANSPOSE::create(arg);
  check_rtti(node, "CTRANSPOSE");
}

// Not using the fixture because SELECTRESULT requires specific arg types
TEST(RTTITest, TestSELECTRESULT) {
  OGRealScalar::Ptr real = OGRealScalar::create(3.14);
  OGIntegerScalar::Ptr index = OGIntegerScalar::create(0);
  SELECTRESULT::Ptr node = SELECTRESULT::create(real, index);
  check_rtti(node, "SELECTRESULT");
}

TEST(RTTITerminalTest, TestOGRealScalar) {
  OGNumeric::Ptr node = OGRealScalar::create(1.0);
  check_rtti(node, "OGRealScalar");
}

TEST(RTTITerminalTest, TestOGComplexScalar) {
  OGNumeric::Ptr node = OGComplexScalar::create(complex16{1.0,2.0});
  check_rtti(node, "OGComplexScalar");
}

TEST(RTTITerminalTest, TestOGIntegerScalar) {
  OGNumeric::Ptr node = OGIntegerScalar::create(1);
  check_rtti(node, "OGIntegerScalar");
}

TEST(RTTITerminalTest, TestOGRealDenseMatrix) {
  real8 realData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
  OGNumeric::Ptr node = OGRealDenseMatrix::create(realData, 2, 3);
  check_rtti(node, "OGRealDenseMatrix");
}

TEST(RTTITerminalTest, TestOGComplexDenseMatrix) {
  complex16 complexData[6] = { {1.0, 2.0}, {3.0, 4.0},  {5.0, 6.0},
                               {7.0, 8.0}, {9.0, 10.0}, {11.0, 12.0} };
  OGNumeric::Ptr node = OGComplexDenseMatrix::create(complexData, 2, 3);
  check_rtti(node, "OGComplexDenseMatrix");
}

TEST(RTTITerminalTest, TestOGRealDiagonalMatrix) {
  real8 rdiagData[3] = { 1.0, 2.0, 3.0 };
  OGNumeric::Ptr node = OGRealDiagonalMatrix::create(rdiagData, 3, 4);
  check_rtti(node, "OGRealDiagonalMatrix");
}

TEST(RTTITerminalTest, TestOGComplexDiagonalMatrix) {
  complex16 cdiagData[3] = { {1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0} };
  OGNumeric::Ptr node = OGComplexDiagonalMatrix::create(cdiagData, 3, 4);
  check_rtti(node, "OGComplexDiagonalMatrix");
}

TEST(RTTITerminalTest, TestOGRealSparseMatrix) {
  int4 colPtr[3] = { 0, 2, 2 };
  int4 rowIdx[2] = { 0, 1 };
  real8 rsparseData[2] = { 1.0, 2.0 };
  OGNumeric::Ptr node = OGRealSparseMatrix::create(colPtr, rowIdx, rsparseData, 2, 2);
  check_rtti(node, "OGRealSparseMatrix");
}

TEST(RTTITerminalTest, TestOGComplexSparseMatrix) {
  int4 colPtr[3] = { 0, 2, 2 };
  int4 rowIdx[2] = { 0, 1 };
  complex16 csparseData[2] = { {1.0, 2.0}, {3.0, 4.0} };
  OGNumeric::Ptr node = OGComplexSparseMatrix::create(colPtr, rowIdx, csparseData, 2, 2);
  check_rtti(node, "OGComplexSparseMatrix");
}
