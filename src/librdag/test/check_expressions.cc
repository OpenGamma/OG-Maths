/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "expression.hh"
#include "terminal.hh"
#include "gtest/gtest.h"

using namespace std;
using namespace librdag;

/**
 * BinaryExpr tests
 */

template<typename T>
class BinaryExprTest: public ::testing::Test {};
TYPED_TEST_CASE_P(BinaryExprTest);

TYPED_TEST_P(BinaryExprTest, Functionality){
  // Constructor
  pOGNumeric real = pOGNumeric{new OGRealScalar(3.14)};
  pOGNumeric complx = pOGNumeric{new OGComplexScalar(complex16(2.7182, 2.7182))};
  TypeParam *expr = new TypeParam(real, complx);
  ASSERT_EQ(2, expr->getNArgs());
  const ArgContainer& gotArgs = expr->getArgs();
  EXPECT_EQ(real->asOGRealScalar(), (gotArgs[0])->asOGRealScalar());
  EXPECT_EQ(complx->asOGComplexScalar(), (gotArgs[1])->asOGComplexScalar());

  // Debug string
  expr->debug_print();

  // Constructor with null args
  EXPECT_THROW(new TypeParam(nullptr, real), rdag_error);
  EXPECT_THROW(new TypeParam(real, nullptr), rdag_error);

  // Cleanup
  delete expr;
}

REGISTER_TYPED_TEST_CASE_P(BinaryExprTest, Functionality);
typedef ::testing::Types<PLUS> BinaryExprTypes;
INSTANTIATE_TYPED_TEST_CASE_P(Binary, BinaryExprTest, BinaryExprTypes);

/**
 * UnaryExpr tests
 */

template<typename T>
class UnaryExprTest: public ::testing::Test {};
TYPED_TEST_CASE_P(UnaryExprTest);

TYPED_TEST_P(UnaryExprTest, Functionality){
  // Constructor
  pOGNumeric real = pOGNumeric{new OGRealScalar(3.14)};
  TypeParam *expr = new TypeParam(real);
  ASSERT_EQ(1, expr->getNArgs());
  const ArgContainer& gotArgs = expr->getArgs();
  EXPECT_EQ(real->asOGRealScalar(), (gotArgs[0])->asOGRealScalar());

  // Debug string
  expr->debug_print();

  // Constructor with null args
  EXPECT_THROW(new TypeParam(nullptr), rdag_error);

  // Cleanup
  delete expr;
}

REGISTER_TYPED_TEST_CASE_P(UnaryExprTest, Functionality);
typedef ::testing::Types<NEGATE, SVD, COPY> UnaryExprTypes;
INSTANTIATE_TYPED_TEST_CASE_P(Unary, UnaryExprTest, UnaryExprTypes);

/**
 * Tests for nodes with more specialised requirements
 */

TEST(OGExprTest, SELECTRESULT){
  // Constructor
  pOGNumeric real = pOGNumeric{new OGRealScalar(3.14)};
  pOGNumeric index = pOGNumeric{new OGIntegerScalar(2)};
  pOGNumeric selectresult = pOGNumeric{new SELECTRESULT(real, index)};
  ASSERT_EQ(2, selectresult->asOGExpr()->getNArgs());
  const ArgContainer& gotArgs = selectresult->asOGExpr()->getArgs();
  EXPECT_EQ(real->asOGRealScalar(), (gotArgs[0])->asOGRealScalar());
  EXPECT_EQ(index->asOGIntegerScalar(), (gotArgs[1])->asOGIntegerScalar());

  // Debug string
  selectresult->debug_print();

  // Constructor with null args
  EXPECT_THROW(new SELECTRESULT(nullptr, index), rdag_error);
  EXPECT_THROW(new SELECTRESULT(real, nullptr), rdag_error);

  // Constructor where second argument is not an integer type
  pOGNumeric realcopy = real->copy();
  EXPECT_THROW(new SELECTRESULT(real, realcopy), rdag_error);
}

TEST(VirtualCopyTest, RealScalar){
  pOGNumeric r1 = pOGNumeric{new OGRealScalar(3.142)};
  pOGNumeric r2 = r1->copy();
  EXPECT_EQ(r1->asOGRealScalar()->getValue(), r2->asOGRealScalar()->getValue());
  EXPECT_NE(r1, r2);
}

TEST(VirtualCopyTest, ComplexScalar){
  pOGNumeric c1 = pOGNumeric{new OGComplexScalar(complex16(1.0, 2.0))};
  pOGNumeric c2 = c1->copy();
  EXPECT_EQ(c1->asOGComplexScalar()->getValue(), c2->asOGComplexScalar()->getValue());
  EXPECT_NE(c1, c2);
}

TEST(VirtualCopyTest, IntegerScalar){
  pOGNumeric i1 = pOGNumeric{new OGIntegerScalar(7)};
  pOGNumeric i2 = pOGNumeric{i1->copy()};
  EXPECT_EQ(i1->asOGIntegerScalar()->getValue(), i2->asOGIntegerScalar()->getValue());
  EXPECT_NE(i1, i2);
}

TEST(VirtualCopyTest, RealMatrix){
  double realData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
  pOGRealMatrix rm1 = pOGRealMatrix{new OGRealMatrix(realData, 2, 3)};
  pOGRealMatrix rm2 = rm1->copy()->asOGRealMatrix();
  EXPECT_EQ(rm1->getData(), rm2->getData());
  EXPECT_EQ(rm1->getRows(), rm2->getRows());
  EXPECT_EQ(rm1->getCols(), rm2->getCols());
  EXPECT_EQ(rm1->getDatalen(), rm2->getDatalen());
  EXPECT_NE(rm1, rm2);
}

TEST(VirtualCopyTest, ComplexMatrix){
  complex16 complexData[6] = { {1.0, 2.0}, {3.0, 4.0},  {5.0, 6.0},
                               {7.0, 8.0}, {9.0, 10.0}, {11.0, 12.0} };
  pOGComplexMatrix cm1 = pOGComplexMatrix{new OGComplexMatrix(complexData, 2, 3)};
  pOGComplexMatrix cm2 = cm1->copy()->asOGComplexMatrix();
  EXPECT_EQ(cm1->getData(), cm2->getData());
  EXPECT_EQ(cm1->getRows(), cm2->getRows());
  EXPECT_EQ(cm1->getCols(), cm2->getCols());
  EXPECT_EQ(cm1->getDatalen(), cm2->getDatalen());
  EXPECT_NE(cm1, cm2);
}

TEST(VirtualCopyTest, RealDiagonalMatrix){
  double rdiagData[3] = { 1.0, 2.0, 3.0 };
  pOGRealDiagonalMatrix rdm1 = pOGRealDiagonalMatrix{new OGRealDiagonalMatrix(rdiagData, 3, 4)};
  pOGRealDiagonalMatrix rdm2 = rdm1->copy()->asOGRealDiagonalMatrix();
  EXPECT_EQ(rdm1->getData(), rdm2->getData());
  EXPECT_EQ(rdm1->getRows(), rdm2->getRows());
  EXPECT_EQ(rdm1->getCols(), rdm2->getCols());
  EXPECT_EQ(rdm1->getDatalen(), rdm2->getDatalen());
  EXPECT_NE(rdm1, rdm2);
}

TEST(VirtualCopyTest, ComplexDiagonalMatrix){
  complex16 cdiagData[3] = { {1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0} };
  pOGComplexDiagonalMatrix cdm1 = pOGComplexDiagonalMatrix{new OGComplexDiagonalMatrix(cdiagData, 3, 4)};
  pOGComplexDiagonalMatrix cdm2 = cdm1->copy()->asOGComplexDiagonalMatrix();
  EXPECT_EQ(cdm1->getData(), cdm2->getData());
  EXPECT_EQ(cdm1->getRows(), cdm2->getRows());
  EXPECT_EQ(cdm1->getCols(), cdm2->getCols());
  EXPECT_EQ(cdm1->getDatalen(), cdm2->getDatalen());
  EXPECT_NE(cdm1, cdm2);
}

TEST(VirtualCopyTest, RealSparseMatrix){
  int colPtr[3] = { 0, 2, 2 };
  int rowIdx[2] = { 0, 1 };
  double rsparseData[2] = { 1.0, 2.0 };
  pOGRealSparseMatrix rsm1 = pOGRealSparseMatrix{new OGRealSparseMatrix(colPtr, rowIdx, rsparseData, 2, 2)};
  pOGRealSparseMatrix rsm2 = rsm1->copy()->asOGRealSparseMatrix();
  EXPECT_EQ(rsm1->getData(), rsm2->getData());
  EXPECT_EQ(rsm1->getColPtr(), rsm2->getColPtr());
  EXPECT_EQ(rsm1->getRowIdx(), rsm2->getRowIdx());
  EXPECT_EQ(rsm1->getRows(), rsm2->getRows());
  EXPECT_EQ(rsm1->getCols(), rsm2->getCols());
  EXPECT_NE(rsm1, rsm2);
}

TEST(VirtualCopyTest, ComplexSparseMatrix){
  int colPtr[3] = { 0, 2, 2 };
  int rowIdx[2] = { 0, 1 };
  complex16 csparseData[2] = { {1.0, 2.0}, {3.0, 4.0} };
  pOGComplexSparseMatrix csm1 = pOGComplexSparseMatrix{new OGComplexSparseMatrix(colPtr, rowIdx, csparseData, 2, 2)};
  pOGComplexSparseMatrix csm2 = csm1->copy()->asOGComplexSparseMatrix();
  EXPECT_EQ(csm1->getData(), csm2->getData());
  EXPECT_EQ(csm1->getColPtr(), csm2->getColPtr());
  EXPECT_EQ(csm1->getRowIdx(), csm2->getRowIdx());
  EXPECT_EQ(csm1->getRows(), csm2->getRows());
  EXPECT_EQ(csm1->getCols(), csm2->getCols());
  EXPECT_NE(csm1, csm2);
}

TEST(VirtualCopyTest, COPY) {
  pOGNumeric real = pOGNumeric{new OGRealScalar(1.0)};
  pCOPY copy1 = pCOPY{new COPY(real)};
  
  // Check copy is of the correct type
  pCOPY copy2 = copy1->copy()->asCOPY();
  ASSERT_NE(pCOPY{}, copy2);

  const ArgContainer& a1 = copy1->getArgs();
  const ArgContainer& a2 = copy2->getArgs();
  EXPECT_EQ(a1.size(), a2.size());
  pOGNumeric c1arg1 = a1[0];
  pOGNumeric c2arg1 = a2[0];
  EXPECT_NE(c1arg1, c2arg1);
  pOGRealScalar c1s = c1arg1->asOGRealScalar();
  pOGRealScalar c2s = c2arg1->asOGRealScalar();
  ASSERT_NE(pOGRealScalar{}, c1s);
  ASSERT_NE(pOGRealScalar{}, c2s);
  EXPECT_EQ(c1s->getValue(), c2s->getValue());
}

TEST(VirtualCopyTest, PLUS) {
  pOGNumeric real1 = pOGNumeric{new OGRealScalar(1.0)};
  pOGNumeric real2 = pOGNumeric{new OGRealScalar(2.0)};
  pPLUS plus1 = pPLUS{new PLUS(real1, real2)};
  
  // Check copy is of the correct type
  pPLUS plus2 = plus1->copy()->asPLUS();
  ASSERT_NE(pPLUS{}, plus2);
  
  const ArgContainer& a1 = plus1->getArgs();
  const ArgContainer& a2 = plus2->getArgs();
  EXPECT_EQ(a1.size(), a2.size());
  pOGNumeric p1arg1 = a1[0];
  pOGNumeric p2arg1 = a2[0];
  EXPECT_NE(p1arg1, p2arg1);
  pOGNumeric p1arg2 = a1[1];
  pOGNumeric p2arg2 = a2[1];
  EXPECT_NE(p1arg2, p2arg2);
  pOGRealScalar p1s1 = p1arg1->asOGRealScalar();
  pOGRealScalar p2s1 = p2arg1->asOGRealScalar();
  ASSERT_NE(pOGRealScalar{}, p1s1);
  ASSERT_NE(pOGRealScalar{}, p2s1);
  EXPECT_EQ(p1s1->getValue(), p2s1->getValue());
  pOGRealScalar p1s2 = p1arg2->asOGRealScalar();
  pOGRealScalar p2s2 = p2arg2->asOGRealScalar();
  ASSERT_NE(pOGRealScalar{}, p1s2);
  ASSERT_NE(pOGRealScalar{}, p2s2);
  EXPECT_EQ(p1s2->getValue(), p1s2->getValue());
}

TEST(VirtualCopyTest, NEGATE) {
  pOGNumeric real1 = pOGNumeric{new OGRealScalar(1.0)};
  pNEGATE negate1 = pNEGATE{new NEGATE(real1)};
  
  // Check that the copy is of the correct type
  pNEGATE negate2 = negate1->copy()->asNEGATE();
  ASSERT_NE(pNEGATE{}, negate2);
  
  const ArgContainer& a1 = negate1->getArgs();
  const ArgContainer& a2 = negate2->getArgs();
  EXPECT_EQ(a1.size(), a2.size());
  pOGNumeric n1arg1 = a1[0];
  pOGNumeric n2arg1 = a2[0];
  EXPECT_NE(n1arg1, n2arg1);
  pOGRealScalar n1s1 = n1arg1->asOGRealScalar();
  pOGRealScalar n2s1 = n2arg1->asOGRealScalar();
  ASSERT_NE(pOGRealScalar{}, n1s1);
  ASSERT_NE(pOGRealScalar{}, n2s1);
  EXPECT_EQ(n1s1->getValue(), n2s1->getValue());
}

TEST(VirtualCopyTest, NORM2) {
  double matData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
  pOGNumeric realMat = pOGNumeric{new OGRealMatrix(matData, 3, 2)};
  pNORM2 norm1 = pNORM2{new NORM2(realMat)};
  
  // Check that the copy is of the correct type
  pNORM2 norm2 = norm1->copy()->asNORM2();
  ASSERT_NE(pNORM2{}, norm2);
  
  const ArgContainer& a1 = norm1->getArgs();
  const ArgContainer& a2 = norm2->getArgs();
  EXPECT_EQ(a1.size(), a2.size());
  pOGNumeric s1arg1 = a1[0];
  pOGNumeric s2arg1 = a2[0];
  EXPECT_NE(s1arg1, s2arg1);
  pOGRealMatrix s1m = s1arg1->asOGRealMatrix();
  pOGRealMatrix s2m = s2arg1->asOGRealMatrix();
  ASSERT_NE(pOGRealMatrix{}, s1m);
  ASSERT_NE(pOGRealMatrix{}, s2m);
  EXPECT_EQ(s1m->getData(), s2m->getData());
  EXPECT_EQ(s1m->getRows(), s2m->getRows());
  EXPECT_EQ(s1m->getCols(), s2m->getCols());
  EXPECT_EQ(s1m->getDatalen(), s2m->getDatalen());
}


TEST(VirtualCopyTest, SVD) {
  double matData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
  pOGNumeric realMat = pOGNumeric{new OGRealMatrix(matData, 3, 2)};
  pSVD svd1 = pSVD{new SVD(realMat)};

  // Check that the copy is of the correct type
  pSVD svd2 = svd1->copy()->asSVD();
  ASSERT_NE(pOGExpr{}, svd2);
  
  const ArgContainer& a1 = svd1->getArgs();
  const ArgContainer& a2 = svd2->getArgs();
  EXPECT_EQ(a1.size(), a2.size());
  pOGNumeric s1arg1 = a1[0];
  pOGNumeric s2arg1 = a2[0];
  EXPECT_NE(s1arg1, s2arg1);
  pOGRealMatrix s1m = s1arg1->asOGRealMatrix();
  pOGRealMatrix s2m = s2arg1->asOGRealMatrix();
  ASSERT_NE(pOGRealMatrix{}, s1m);
  ASSERT_NE(pOGRealMatrix{}, s2m);
  EXPECT_EQ(s1m->getData(), s2m->getData());
  EXPECT_EQ(s1m->getRows(), s2m->getRows());
  EXPECT_EQ(s1m->getCols(), s2m->getCols());
  EXPECT_EQ(s1m->getDatalen(), s2m->getDatalen());
}

TEST(VirtualCopyTest, LU) {
  double matData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
  pOGNumeric realMat = pOGNumeric{new OGRealMatrix(matData, 3, 2)};
  pLU lu1 = pLU{new LU(realMat)};
  
  // Check that the copy is of the correct type
  pLU lu2 = lu1->copy()->asLU();
  ASSERT_NE(pOGExpr{}, lu2);
  
  const ArgContainer& a1 = lu1->getArgs();
  const ArgContainer& a2 = lu2->getArgs();
  EXPECT_EQ(a1.size(), a2.size());
  pOGNumeric s1arg1 = a1[0];
  pOGNumeric s2arg1 = a2[0];
  EXPECT_NE(s1arg1, s2arg1);
  pOGRealMatrix s1m = s1arg1->asOGRealMatrix();
  pOGRealMatrix s2m = s2arg1->asOGRealMatrix();
  ASSERT_NE(pOGRealMatrix{}, s1m);
  ASSERT_NE(pOGRealMatrix{}, s2m);
  EXPECT_EQ(s1m->getData(), s2m->getData());
  EXPECT_EQ(s1m->getRows(), s2m->getRows());
  EXPECT_EQ(s1m->getCols(), s2m->getCols());
  EXPECT_EQ(s1m->getDatalen(), s2m->getDatalen());
}

TEST(VirtualCopyTest, SELECTRESULT) {
  double matData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
  pOGNumeric realMat = pOGNumeric{new OGRealMatrix(matData, 3, 2)};
  pOGExpr svd = pOGExpr{new SVD(realMat)};
  pOGNumeric i = pOGNumeric{new OGIntegerScalar(0)};
  pSELECTRESULT sr1 = pSELECTRESULT{new SELECTRESULT(svd, i)};
  
  // Check that the copy is of the correct type
  pSELECTRESULT sr2 = sr1->copy()->asSELECTRESULT();
  ASSERT_NE(pSELECTRESULT{}, sr2);
  
  const ArgContainer& a1 = sr1->getArgs();
  const ArgContainer& a2 = sr2->getArgs();
  EXPECT_EQ(a1.size(), a2.size());
  pOGNumeric sr1arg1 = a1[0];
  pOGNumeric sr2arg1 = a2[0];
  EXPECT_NE(sr1arg1, sr2arg1);
  pOGNumeric sr1arg2 = a1[1];
  pOGNumeric sr2arg2 = a2[1];
  EXPECT_NE(sr1arg2, sr2arg2);
  pSVD sr1svd = sr1arg1->asSVD();
  pSVD sr2svd = sr2arg1->asSVD();
  ASSERT_NE(pSVD{}, sr1svd);
  ASSERT_NE(pSVD{}, sr2svd);
  //EXPECT_EQ(m1s1->getValue(), m2s1->getValue()); // Need to recurse
  // in to SVD.
  pOGIntegerScalar sr1index = sr1arg2->asOGIntegerScalar();
  pOGIntegerScalar sr2index = sr2arg2->asOGIntegerScalar();
  ASSERT_NE(pOGIntegerScalar{}, sr1index);
  ASSERT_NE(pOGIntegerScalar{}, sr2index);
  EXPECT_EQ(sr1index->getValue(), sr2index->getValue());
}
