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
  OGNumeric::Ptr real = OGRealScalar::create(3.14);
  OGNumeric::Ptr complx = OGComplexScalar::create(complex16{2.7182, 2.7182});
  typename TypeParam::Ptr expr = TypeParam::create(real, complx);
  ASSERT_EQ(2, expr->getNArgs());
  const ArgContainer& gotArgs = expr->getArgs();
  EXPECT_EQ(real->asOGRealScalar(), (gotArgs[0])->asOGRealScalar());
  EXPECT_EQ(complx->asOGComplexScalar(), (gotArgs[1])->asOGComplexScalar());

  // Debug string
  expr->debug_print();

  // Constructor with null args
  EXPECT_THROW(TypeParam::create(OGNumeric::Ptr{}, real), rdag_error);
  EXPECT_THROW(TypeParam::create(real, OGNumeric::Ptr{}), rdag_error);
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
  OGNumeric::Ptr real = OGRealScalar::create(3.14);
  typename TypeParam::Ptr expr = TypeParam::create(real);
  ASSERT_EQ(1, expr->getNArgs());
  const ArgContainer& gotArgs = expr->getArgs();
  EXPECT_EQ(real->asOGRealScalar(), (gotArgs[0])->asOGRealScalar());

  // Debug string
  expr->debug_print();

  // Constructor with null args
  EXPECT_THROW(TypeParam::create(OGNumeric::Ptr{}), rdag_error);
}

REGISTER_TYPED_TEST_CASE_P(UnaryExprTest, Functionality);
typedef ::testing::Types<NEGATE, SVD, COPY> UnaryExprTypes;
INSTANTIATE_TYPED_TEST_CASE_P(Unary, UnaryExprTest, UnaryExprTypes);

/**
 * Tests for nodes with more specialised requirements
 */

TEST(OGExprTest, SELECTRESULT){
  // Constructor
  OGNumeric::Ptr real = OGRealScalar::create(3.14);
  OGNumeric::Ptr index = OGIntegerScalar::create(2);
  OGNumeric::Ptr selectresult = SELECTRESULT::create(real, index);
  ASSERT_EQ(2, selectresult->asOGExpr()->getNArgs());
  const ArgContainer& gotArgs = selectresult->asOGExpr()->getArgs();
  EXPECT_EQ(real->asOGRealScalar(), (gotArgs[0])->asOGRealScalar());
  EXPECT_EQ(index->asOGIntegerScalar(), (gotArgs[1])->asOGIntegerScalar());

  // Debug string
  selectresult->debug_print();

  // Constructor with null args
  EXPECT_THROW(SELECTRESULT::create(nullptr, index), rdag_error);
  EXPECT_THROW(SELECTRESULT::create(real, nullptr), rdag_error);

  // Constructor where second argument is not an integer type
  OGNumeric::Ptr realcopy = real->copy();
  EXPECT_THROW(SELECTRESULT::create(real, realcopy), rdag_error);
}

TEST(VirtualCopyTest, OGRealScalar){
  OGNumeric::Ptr r1 = OGRealScalar::create(3.142);
  OGNumeric::Ptr r2 = r1->copy();
  EXPECT_EQ(r1->asOGRealScalar()->getValue(), r2->asOGRealScalar()->getValue());
  EXPECT_NE(r1, r2);
}

TEST(VirtualCopyTest, OGComplexScalar){
  OGNumeric::Ptr c1 = OGComplexScalar::create(complex16{1.0, 2.0});
  OGNumeric::Ptr c2 = c1->copy();
  EXPECT_EQ(c1->asOGComplexScalar()->getValue(), c2->asOGComplexScalar()->getValue());
  EXPECT_NE(c1, c2);
}

TEST(VirtualCopyTest, OGIntegerScalar){
  OGNumeric::Ptr i1 = OGIntegerScalar::create(7);
  OGNumeric::Ptr i2 = OGNumeric::Ptr{i1->copy()};
  EXPECT_EQ(i1->asOGIntegerScalar()->getValue(), i2->asOGIntegerScalar()->getValue());
  EXPECT_NE(i1, i2);
}

TEST(VirtualCopyTest, OGRealDenseMatrix){
  real8 realData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
  OGRealDenseMatrix::Ptr rm1 = OGRealDenseMatrix::create(realData, 2, 3);
  OGRealDenseMatrix::Ptr rm2 = rm1->copy()->asOGRealDenseMatrix();
  EXPECT_EQ(rm1->getData(), rm2->getData());
  EXPECT_EQ(rm1->getRows(), rm2->getRows());
  EXPECT_EQ(rm1->getCols(), rm2->getCols());
  EXPECT_EQ(rm1->getDatalen(), rm2->getDatalen());
  EXPECT_NE(rm1, rm2);
}

TEST(VirtualCopyTest, OGComplexDenseMatrix){
  complex16 complexData[6] = { {1.0, 2.0}, {3.0, 4.0},  {5.0, 6.0},
                               {7.0, 8.0}, {9.0, 10.0}, {11.0, 12.0} };
  OGComplexDenseMatrix::Ptr cm1 = OGComplexDenseMatrix::create(complexData, 2, 3);
  OGComplexDenseMatrix::Ptr cm2 = cm1->copy()->asOGComplexDenseMatrix();
  EXPECT_EQ(cm1->getData(), cm2->getData());
  EXPECT_EQ(cm1->getRows(), cm2->getRows());
  EXPECT_EQ(cm1->getCols(), cm2->getCols());
  EXPECT_EQ(cm1->getDatalen(), cm2->getDatalen());
  EXPECT_NE(cm1, cm2);
}

TEST(VirtualCopyTest, OGRealDiagonalMatrix){
  real8 rdiagData[3] = { 1.0, 2.0, 3.0 };
  OGRealDiagonalMatrix::Ptr rdm1 = OGRealDiagonalMatrix::create(rdiagData, 3, 4);
  OGRealDiagonalMatrix::Ptr rdm2 = rdm1->copy()->asOGRealDiagonalMatrix();
  EXPECT_EQ(rdm1->getData(), rdm2->getData());
  EXPECT_EQ(rdm1->getRows(), rdm2->getRows());
  EXPECT_EQ(rdm1->getCols(), rdm2->getCols());
  EXPECT_EQ(rdm1->getDatalen(), rdm2->getDatalen());
  EXPECT_NE(rdm1, rdm2);
}

TEST(VirtualCopyTest, OGComplexDiagonalMatrix){
  complex16 cdiagData[3] = { {1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0} };
  OGComplexDiagonalMatrix::Ptr cdm1 = OGComplexDiagonalMatrix::create(cdiagData, 3, 4);
  OGComplexDiagonalMatrix::Ptr cdm2 = cdm1->copy()->asOGComplexDiagonalMatrix();
  EXPECT_EQ(cdm1->getData(), cdm2->getData());
  EXPECT_EQ(cdm1->getRows(), cdm2->getRows());
  EXPECT_EQ(cdm1->getCols(), cdm2->getCols());
  EXPECT_EQ(cdm1->getDatalen(), cdm2->getDatalen());
  EXPECT_NE(cdm1, cdm2);
}

TEST(VirtualCopyTest, OGRealSparseMatrix){
  int4 colPtr[3] = { 0, 2, 2 };
  int4 rowIdx[2] = { 0, 1 };
  real8 rsparseData[2] = { 1.0, 2.0 };
  OGRealSparseMatrix::Ptr rsm1 = OGRealSparseMatrix::create(colPtr, rowIdx, rsparseData, 2, 2);
  OGRealSparseMatrix::Ptr rsm2 = rsm1->copy()->asOGRealSparseMatrix();
  EXPECT_EQ(rsm1->getData(), rsm2->getData());
  EXPECT_EQ(rsm1->getColPtr(), rsm2->getColPtr());
  EXPECT_EQ(rsm1->getRowIdx(), rsm2->getRowIdx());
  EXPECT_EQ(rsm1->getRows(), rsm2->getRows());
  EXPECT_EQ(rsm1->getCols(), rsm2->getCols());
  EXPECT_NE(rsm1, rsm2);
}

TEST(VirtualCopyTest, OGComplexSparseMatrix){
  int4 colPtr[3] = { 0, 2, 2 };
  int4 rowIdx[2] = { 0, 1 };
  complex16 csparseData[2] = { {1.0, 2.0}, {3.0, 4.0} };
  OGComplexSparseMatrix::Ptr csm1 = OGComplexSparseMatrix::create(colPtr, rowIdx, csparseData, 2, 2);
  OGComplexSparseMatrix::Ptr csm2 = csm1->copy()->asOGComplexSparseMatrix();
  EXPECT_EQ(csm1->getData(), csm2->getData());
  EXPECT_EQ(csm1->getColPtr(), csm2->getColPtr());
  EXPECT_EQ(csm1->getRowIdx(), csm2->getRowIdx());
  EXPECT_EQ(csm1->getRows(), csm2->getRows());
  EXPECT_EQ(csm1->getCols(), csm2->getCols());
  EXPECT_NE(csm1, csm2);
}

TEST(VirtualCopyTest, COPY) {
  OGNumeric::Ptr real = OGRealScalar::create(1.0);
  COPY::Ptr copy1 = COPY::create(real);
  
  // Check copy is of the correct type
  COPY::Ptr copy2 = copy1->copy()->asCOPY();
  ASSERT_NE(COPY::Ptr{}, copy2);

  const ArgContainer& a1 = copy1->getArgs();
  const ArgContainer& a2 = copy2->getArgs();
  EXPECT_EQ(a1.size(), a2.size());
  OGNumeric::Ptr c1arg1 = a1[0];
  OGNumeric::Ptr c2arg1 = a2[0];
  EXPECT_NE(c1arg1, c2arg1);
  OGRealScalar::Ptr c1s = c1arg1->asOGRealScalar();
  OGRealScalar::Ptr c2s = c2arg1->asOGRealScalar();
  ASSERT_NE(OGRealScalar::Ptr{}, c1s);
  ASSERT_NE(OGRealScalar::Ptr{}, c2s);
  EXPECT_EQ(c1s->getValue(), c2s->getValue());
}

TEST(VirtualCopyTest, PLUS) {
  OGNumeric::Ptr real1 = OGRealScalar::create(1.0);
  OGNumeric::Ptr real2 = OGRealScalar::create(2.0);
  PLUS::Ptr plus1 = PLUS::create(real1, real2);
  
  // Check copy is of the correct type
  PLUS::Ptr plus2 = plus1->copy()->asPLUS();
  ASSERT_NE(PLUS::Ptr{}, plus2);
  
  const ArgContainer& a1 = plus1->getArgs();
  const ArgContainer& a2 = plus2->getArgs();
  EXPECT_EQ(a1.size(), a2.size());
  OGNumeric::Ptr p1arg1 = a1[0];
  OGNumeric::Ptr p2arg1 = a2[0];
  EXPECT_NE(p1arg1, p2arg1);
  OGNumeric::Ptr p1arg2 = a1[1];
  OGNumeric::Ptr p2arg2 = a2[1];
  EXPECT_NE(p1arg2, p2arg2);
  OGRealScalar::Ptr p1s1 = p1arg1->asOGRealScalar();
  OGRealScalar::Ptr p2s1 = p2arg1->asOGRealScalar();
  ASSERT_NE(OGRealScalar::Ptr{}, p1s1);
  ASSERT_NE(OGRealScalar::Ptr{}, p2s1);
  EXPECT_EQ(p1s1->getValue(), p2s1->getValue());
  OGRealScalar::Ptr p1s2 = p1arg2->asOGRealScalar();
  OGRealScalar::Ptr p2s2 = p2arg2->asOGRealScalar();
  ASSERT_NE(OGRealScalar::Ptr{}, p1s2);
  ASSERT_NE(OGRealScalar::Ptr{}, p2s2);
  EXPECT_EQ(p1s2->getValue(), p1s2->getValue());
}

TEST(VirtualCopyTest, NEGATE) {
  OGNumeric::Ptr real1 = OGRealScalar::create(1.0);
  NEGATE::Ptr negate1 = NEGATE::create(real1);
  
  // Check that the copy is of the correct type
  NEGATE::Ptr negate2 = negate1->copy()->asNEGATE();
  ASSERT_NE(NEGATE::Ptr{}, negate2);
  
  const ArgContainer& a1 = negate1->getArgs();
  const ArgContainer& a2 = negate2->getArgs();
  EXPECT_EQ(a1.size(), a2.size());
  OGNumeric::Ptr n1arg1 = a1[0];
  OGNumeric::Ptr n2arg1 = a2[0];
  EXPECT_NE(n1arg1, n2arg1);
  OGRealScalar::Ptr n1s1 = n1arg1->asOGRealScalar();
  OGRealScalar::Ptr n2s1 = n2arg1->asOGRealScalar();
  ASSERT_NE(OGRealScalar::Ptr{}, n1s1);
  ASSERT_NE(OGRealScalar::Ptr{}, n2s1);
  EXPECT_EQ(n1s1->getValue(), n2s1->getValue());
}

TEST(VirtualCopyTest, NORM2) {
  real8 matData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
  OGNumeric::Ptr realMat = OGRealDenseMatrix::create(matData, 3, 2);
  NORM2::Ptr norm1 = NORM2::create(realMat);
  
  // Check that the copy is of the correct type
  NORM2::Ptr norm2 = norm1->copy()->asNORM2();
  ASSERT_NE(NORM2::Ptr{}, norm2);
  
  const ArgContainer& a1 = norm1->getArgs();
  const ArgContainer& a2 = norm2->getArgs();
  EXPECT_EQ(a1.size(), a2.size());
  OGNumeric::Ptr s1arg1 = a1[0];
  OGNumeric::Ptr s2arg1 = a2[0];
  EXPECT_NE(s1arg1, s2arg1);
  OGRealDenseMatrix::Ptr s1m = s1arg1->asOGRealDenseMatrix();
  OGRealDenseMatrix::Ptr s2m = s2arg1->asOGRealDenseMatrix();
  ASSERT_NE(OGRealDenseMatrix::Ptr{}, s1m);
  ASSERT_NE(OGRealDenseMatrix::Ptr{}, s2m);
  EXPECT_EQ(s1m->getData(), s2m->getData());
  EXPECT_EQ(s1m->getRows(), s2m->getRows());
  EXPECT_EQ(s1m->getCols(), s2m->getCols());
  EXPECT_EQ(s1m->getDatalen(), s2m->getDatalen());
}


TEST(VirtualCopyTest, SVD) {
  real8 matData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
  OGNumeric::Ptr realMat = OGRealDenseMatrix::create(matData, 3, 2);
  SVD::Ptr svd1 = SVD::create(realMat);

  // Check that the copy is of the correct type
  SVD::Ptr svd2 = svd1->copy()->asSVD();
  ASSERT_NE(SVD::Ptr{}, svd2);
  
  const ArgContainer& a1 = svd1->getArgs();
  const ArgContainer& a2 = svd2->getArgs();
  EXPECT_EQ(a1.size(), a2.size());
  OGNumeric::Ptr s1arg1 = a1[0];
  OGNumeric::Ptr s2arg1 = a2[0];
  EXPECT_NE(s1arg1, s2arg1);
  OGRealDenseMatrix::Ptr s1m = s1arg1->asOGRealDenseMatrix();
  OGRealDenseMatrix::Ptr s2m = s2arg1->asOGRealDenseMatrix();
  ASSERT_NE(OGRealDenseMatrix::Ptr{}, s1m);
  ASSERT_NE(OGRealDenseMatrix::Ptr{}, s2m);
  EXPECT_EQ(s1m->getData(), s2m->getData());
  EXPECT_EQ(s1m->getRows(), s2m->getRows());
  EXPECT_EQ(s1m->getCols(), s2m->getCols());
  EXPECT_EQ(s1m->getDatalen(), s2m->getDatalen());
}

TEST(VirtualCopyTest, LU) {
  real8 matData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
  OGNumeric::Ptr realMat = OGRealDenseMatrix::create(matData, 3, 2);
  LU::Ptr lu1 = LU::create(realMat);
  
  // Check that the copy is of the correct type
  LU::Ptr lu2 = lu1->copy()->asLU();
  ASSERT_NE(LU::Ptr{}, lu2);
  
  const ArgContainer& a1 = lu1->getArgs();
  const ArgContainer& a2 = lu2->getArgs();
  EXPECT_EQ(a1.size(), a2.size());
  OGNumeric::Ptr s1arg1 = a1[0];
  OGNumeric::Ptr s2arg1 = a2[0];
  EXPECT_NE(s1arg1, s2arg1);
  OGRealDenseMatrix::Ptr s1m = s1arg1->asOGRealDenseMatrix();
  OGRealDenseMatrix::Ptr s2m = s2arg1->asOGRealDenseMatrix();
  ASSERT_NE(OGRealDenseMatrix::Ptr{}, s1m);
  ASSERT_NE(OGRealDenseMatrix::Ptr{}, s2m);
  EXPECT_EQ(s1m->getData(), s2m->getData());
  EXPECT_EQ(s1m->getRows(), s2m->getRows());
  EXPECT_EQ(s1m->getCols(), s2m->getCols());
  EXPECT_EQ(s1m->getDatalen(), s2m->getDatalen());
}

TEST(VirtualCopyTest, SELECTRESULT) {
  real8 matData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
  OGNumeric::Ptr realMat = OGRealDenseMatrix::create(matData, 3, 2);
  OGExpr::Ptr svd = SVD::create(realMat);
  OGNumeric::Ptr i = OGIntegerScalar::create(0);
  SELECTRESULT::Ptr sr1 = SELECTRESULT::create(svd, i);
  
  // Check that the copy is of the correct type
  SELECTRESULT::Ptr sr2 = sr1->copy()->asSELECTRESULT();
  ASSERT_NE(SELECTRESULT::Ptr{}, sr2);
  
  const ArgContainer& a1 = sr1->getArgs();
  const ArgContainer& a2 = sr2->getArgs();
  EXPECT_EQ(a1.size(), a2.size());
  OGNumeric::Ptr sr1arg1 = a1[0];
  OGNumeric::Ptr sr2arg1 = a2[0];
  EXPECT_NE(sr1arg1, sr2arg1);
  OGNumeric::Ptr sr1arg2 = a1[1];
  OGNumeric::Ptr sr2arg2 = a2[1];
  EXPECT_NE(sr1arg2, sr2arg2);
  SVD::Ptr sr1svd = sr1arg1->asSVD();
  SVD::Ptr sr2svd = sr2arg1->asSVD();
  ASSERT_NE(SVD::Ptr{}, sr1svd);
  ASSERT_NE(SVD::Ptr{}, sr2svd);
  //EXPECT_EQ(m1s1->getValue(), m2s1->getValue()); // Need to recurse
  // in to SVD.
  OGIntegerScalar::Ptr sr1index = sr1arg2->asOGIntegerScalar();
  OGIntegerScalar::Ptr sr2index = sr2arg2->asOGIntegerScalar();
  ASSERT_NE(OGIntegerScalar::Ptr{}, sr1index);
  ASSERT_NE(OGIntegerScalar::Ptr{}, sr2index);
  EXPECT_EQ(sr1index->getValue(), sr2index->getValue());
}
