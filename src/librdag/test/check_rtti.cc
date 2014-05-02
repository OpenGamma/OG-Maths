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
  if(name.compare(#CLAZZ)==0) { EXPECT_EQ(node, node->as##CLAZZ()); } else { EXPECT_EQ(nullptr, node->as##CLAZZ()); }

// Check casts for all types. If a new type is added it should have a CHECK_CAST entry added.
void check_rtti(OGNumeric* node, string name)
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
  CHECK_CAST(OGRealMatrix);
  CHECK_CAST(OGComplexMatrix);
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
      arg = new OGRealScalar(1.0);
    }

    virtual void TearDown()
    {
      delete node;
    }
    OGNumeric* node;
    OGNumeric* arg;
};

// Creates two OGRealScalars for use in construction of a binary node.
class RTTIBinaryTest: public testing::Test
{
  protected:
    virtual void SetUp()
    {
      arg0 = new OGRealScalar(1.0);
      arg1 = new OGRealScalar(2.0);
    }

    virtual void TearDown()
    {
      delete node;
    }
    OGNumeric* node;
    OGNumeric* arg0;
    OGNumeric* arg1;
};

// Clears up the node after use to keep format of tests uniform between terminal and expr tests.
class RTTITerminalTest: public testing::Test
{
  protected:
    virtual void TearDown()
    {
      delete node;
    }
    OGNumeric *node;
};

TEST_F(RTTIUnaryTest, TestCOPY) {
  node = new COPY(arg);
  check_rtti(node, "COPY");
}

TEST_F(RTTIUnaryTest, TestSVD) {
  node = new SVD(arg);
  check_rtti(node, "SVD");
}

TEST_F(RTTIBinaryTest, TestPLUS) {
  node = new PLUS(arg0, arg1);
  check_rtti(node, "PLUS");
}

TEST_F(RTTIBinaryTest, TestMTIMES) {
  node = new MTIMES(arg0, arg1);
  check_rtti(node, "MTIMES");
}

TEST_F(RTTIUnaryTest, TestNEGATE) {
  node = new NEGATE(arg);
  check_rtti(node, "NEGATE");
}

TEST_F(RTTIUnaryTest, TestLU) {
  node = new LU(arg);
  check_rtti(node, "LU");
}

TEST_F(RTTIUnaryTest, TestPINV) {
  node = new PINV(arg);
  check_rtti(node, "PINV");
}

TEST_F(RTTIUnaryTest, TestTRANSPOSE) {
  node = new TRANSPOSE(arg);
  check_rtti(node, "TRANSPOSE");
}

TEST_F(RTTIUnaryTest, TestCTRANSPOSE) {
  node = new CTRANSPOSE(arg);
  check_rtti(node, "CTRANSPOSE");
}

// Not using the fixture because SELECTRESULT requires specific arg types
TEST(RTTITest, TestSELECTRESULT) {
  OGRealScalar *real = new OGRealScalar(3.14);
  OGIntegerScalar *index = new OGIntegerScalar(0);
  OGNumeric* node = new SELECTRESULT(real, index);
  check_rtti(node, "SELECTRESULT");
  delete node;
}

TEST_F(RTTITerminalTest, TestOGRealScalar) {
  node = new OGRealScalar(1.0);
  check_rtti(node, "OGRealScalar");
}

TEST_F(RTTITerminalTest, TestOGComplexScalar) {
  node = new OGComplexScalar(complex16(1.0,2.0));
  check_rtti(node, "OGComplexScalar");
}

TEST_F(RTTITerminalTest, TestOGIntegerScalar) {
  node = new OGIntegerScalar(1);
  check_rtti(node, "OGIntegerScalar");
}

TEST_F(RTTITerminalTest, TestOGRealMatrix) {
  double realData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
  node = new OGRealMatrix(realData, 2, 3);
  check_rtti(node, "OGRealMatrix");
}

TEST_F(RTTITerminalTest, TestOGComplexMatrix) {
  complex16 complexData[6] = { {1.0, 2.0}, {3.0, 4.0},  {5.0, 6.0},
                               {7.0, 8.0}, {9.0, 10.0}, {11.0, 12.0} };
  node = new OGComplexMatrix(complexData, 2, 3);
  check_rtti(node, "OGComplexMatrix");
}

TEST_F(RTTITerminalTest, TestOGRealDiagonalMatrix) {
  double rdiagData[3] = { 1.0, 2.0, 3.0 };
  node = new OGRealDiagonalMatrix(rdiagData, 3, 4);
  check_rtti(node, "OGRealDiagonalMatrix");
}

TEST_F(RTTITerminalTest, TestOGComplexDiagonalMatrix) {
  complex16 cdiagData[3] = { {1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0} };
  node = new OGComplexDiagonalMatrix(cdiagData, 3, 4);
  check_rtti(node, "OGComplexDiagonalMatrix");
}

TEST_F(RTTITerminalTest, TestOGRealSparseMatrix) {
  int colPtr[3] = { 0, 2, 2 };
  int rowIdx[2] = { 0, 1 };
  double rsparseData[2] = { 1.0, 2.0 };
  node = new OGRealSparseMatrix(colPtr, rowIdx, rsparseData, 2, 2);
  check_rtti(node, "OGRealSparseMatrix");
}

TEST_F(RTTITerminalTest, TestOGComplexSparseMatrix) {
  int colPtr[3] = { 0, 2, 2 };
  int rowIdx[2] = { 0, 1 };
  complex16 csparseData[2] = { {1.0, 2.0}, {3.0, 4.0} };
  node = new OGComplexSparseMatrix(colPtr, rowIdx, csparseData, 2, 2);
  check_rtti(node, "OGComplexSparseMatrix");
}
