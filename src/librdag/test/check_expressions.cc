/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "expression.hh"
#include "gtest/gtest.h"

using namespace std;
using namespace librdag;

TEST(OGExprTest, ScalarValues) {
	// Constructor
  OGRealScalar *real = new OGRealScalar(1.0);
  ASSERT_EQ(1.0, real->getValue());

  // Cleanup
  delete real;
}

TEST(OGExprTest, ComplexValues) {
	// Constructor
  OGComplexScalar *complx = new OGComplexScalar(complex16(1.0,2.0));
  ASSERT_EQ(complex16(1.0,2.0), complx->getValue());

  // Cleanup
  delete complx;
}

TEST(OGExprTest, COPY){
  // Constructor
  ArgContainer* newArgs = new ArgContainer();
  OGRealScalar *newReal = new OGRealScalar(3.14);
  newArgs->push_back(newReal);
  OGExpr *copyWithArgs = new COPY(newArgs);
  ASSERT_EQ(1, copyWithArgs->getNArgs());
  EXPECT_EQ(newReal, ((*newArgs)[0])->asOGRealScalar());

  // Constructor with args of wrong length
  // FIXME: Needs implementing once this throws an exception.

  // Cleanup
  delete copyWithArgs;
}

TEST(OGExprTest, PLUS){
  // FIXME: It looks like there is a bug in this test (and probably
  // similar ones) - checking arguments looks like the args that
  // belong to the node are not actually being checked.
  // Constructor
  ArgContainer* newArgs = new ArgContainer();
  OGRealScalar *newReal = new OGRealScalar(3.14);
  OGComplexScalar *newComplx = new OGComplexScalar(complex16(2.7182, 2.7182));
  newArgs->push_back(newReal);
  newArgs->push_back(newComplx);
  OGExpr *plusWithArgs = new PLUS(newArgs);
  ASSERT_EQ(2, plusWithArgs->getNArgs());
  EXPECT_EQ(newReal, ((*newArgs)[0])->asOGRealScalar());
  EXPECT_EQ(newComplx, ((*newArgs)[1])->asOGComplexScalar());

  // Constructor with args of wrong length
  // FIXME: Needs implementing once this throws an exception.

  // Cleanup
  delete plusWithArgs;
}

TEST(OGExprTest, NEGATE){
  // Constructor
  ArgContainer* newArgs = new ArgContainer();
  OGRealScalar *newReal = new OGRealScalar(3.14);
  newArgs->push_back(newReal);
  OGExpr *negateWithArgs = new NEGATE(newArgs);
  ASSERT_EQ(1, negateWithArgs->getNArgs());
  EXPECT_EQ(newReal, ((*newArgs)[0])->asOGRealScalar());

  // Constructor with args of wrong length
  // FIXME: Needs implementing once this throws an exception.

  // Cleanup
  delete negateWithArgs;
}

TEST(OGExprTest, SVD){
  // Constructor
  ArgContainer* newArgs = new ArgContainer();
  OGRealScalar *newReal = new OGRealScalar(3.14);
  newArgs->push_back(newReal);
  OGExpr *svdWithArgs = new SVD(newArgs);
  ASSERT_EQ(1, svdWithArgs->getNArgs());
  EXPECT_EQ(newReal, ((*newArgs)[0])->asOGRealScalar());

  // Constructor with args of wrong length
  // FIXME: Needs implementing once this throws an exception.

  // Cleanup
  delete svdWithArgs;
}

TEST(OGExprTest, SELECTRESULT){
  // Constructor
  ArgContainer* newArgs = new ArgContainer();
  OGRealScalar *newReal = new OGRealScalar(3.14);
  OGIntegerScalar *newIndex = new OGIntegerScalar(2);
  newArgs->push_back(newReal);
  newArgs->push_back(newIndex);
  OGExpr *selectWithArgs = new SELECTRESULT(newArgs);
  ASSERT_EQ(2, selectWithArgs->getNArgs());
  EXPECT_EQ(newReal, ((*newArgs)[0])->asOGRealScalar());
  EXPECT_EQ(newIndex, ((*newArgs)[1])->asOGIntegerScalar());

  // Constructor with args of wrong length
  // FIXME: Needs implementing once this throws an exception.

  // Constructor with index argument of wrong type
  // FIXME: Needs implementing once this throws an exception.

  // Cleanup
  delete selectWithArgs;
}

TEST(VirtualCopyTest, RealScalar){
  OGRealScalar* r1 = new OGRealScalar(3.142);
  OGRealScalar const *  r2 = r1->copy()->asOGRealScalar();
  EXPECT_EQ(r1->getValue(), r2->getValue());
  EXPECT_NE(r1, r2);
  delete r1;
  delete r2;
}

TEST(VirtualCopyTest, ComplexScalar){
  OGComplexScalar* c1 = new OGComplexScalar(complex16(1.0, 2.0));
  OGComplexScalar const * c2 = c1->copy()->asOGComplexScalar();
  EXPECT_EQ(c1->getValue(), c2->getValue());
  EXPECT_NE(c1, c2);
  delete c1;
  delete c2;
}

TEST(VirtualCopyTest, IntegerScalar){
  OGIntegerScalar* i1 = new OGIntegerScalar(7);
  OGIntegerScalar const * i2 = i1->copy()->asOGIntegerScalar();
  EXPECT_EQ(i1->getValue(), i2->getValue());
  EXPECT_NE(i1, i2);
  delete i1;
  delete i2;
}

TEST(VirtualCopyTest, RealMatrix){
  double realData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
  OGRealMatrix *rm1 = new OGRealMatrix(realData, 2, 3);
  OGRealMatrix const * rm2 = rm1->copy()->asOGRealMatrix();
  EXPECT_EQ(rm1->getData(), rm2->getData());
  EXPECT_EQ(rm1->getRows(), rm2->getRows());
  EXPECT_EQ(rm1->getCols(), rm2->getCols());
  EXPECT_EQ(rm1->getDatalen(), rm2->getDatalen());
  EXPECT_NE(rm1, rm2);
  delete rm1;
  delete rm2;
}

TEST(VirtualCopyTest, ComplexMatrix){
  complex16 complexData[6] = { {1.0, 2.0}, {3.0, 4.0},  {5.0, 6.0},
                               {7.0, 8.0}, {9.0, 10.0}, {11.0, 12.0} };
  OGComplexMatrix *cm1 = new OGComplexMatrix(complexData, 2, 3);
  OGComplexMatrix const * cm2 = cm1->copy()->asOGComplexMatrix();
  EXPECT_EQ(cm1->getData(), cm2->getData());
  EXPECT_EQ(cm1->getRows(), cm2->getRows());
  EXPECT_EQ(cm1->getCols(), cm2->getCols());
  EXPECT_EQ(cm1->getDatalen(), cm2->getDatalen());
  EXPECT_NE(cm1, cm2);
  delete cm1;
  delete cm2;
}

TEST(VirtualCopyTest, RealDiagonalMatrix){
  double rdiagData[3] = { 1.0, 2.0, 3.0 };
  OGRealDiagonalMatrix *rdm1 = new OGRealDiagonalMatrix(rdiagData, 3, 4);
  OGRealDiagonalMatrix const *rdm2 = rdm1->copy()->asOGRealDiagonalMatrix();
  EXPECT_EQ(rdm1->getData(), rdm2->getData());
  EXPECT_EQ(rdm1->getRows(), rdm2->getRows());
  EXPECT_EQ(rdm1->getCols(), rdm2->getCols());
  EXPECT_EQ(rdm1->getDatalen(), rdm2->getDatalen());
  EXPECT_NE(rdm1, rdm2);
  delete rdm1;
  delete rdm2;
}

TEST(VirtualCopyTest, ComplexDiagonalMatrix){
  complex16 cdiagData[3] = { {1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0} };
  OGComplexDiagonalMatrix *cdm1 = new OGComplexDiagonalMatrix(cdiagData, 3, 4);
  OGComplexDiagonalMatrix const *cdm2 = cdm1->copy()->asOGComplexDiagonalMatrix();
  EXPECT_EQ(cdm1->getData(), cdm2->getData());
  EXPECT_EQ(cdm1->getRows(), cdm2->getRows());
  EXPECT_EQ(cdm1->getCols(), cdm2->getCols());
  EXPECT_EQ(cdm1->getDatalen(), cdm2->getDatalen());
  EXPECT_NE(cdm1, cdm2);
  delete cdm1;
  delete cdm2;
}

TEST(VirtualCopyTest, RealSparseMatrix){
  int colPtr[2] = { 0, 1 };
  int rowIdx[2] = { 0, 1 };
  double rsparseData[2] = { 1.0, 2.0 };
  OGRealSparseMatrix *rsm1 = new OGRealSparseMatrix(colPtr, rowIdx, rsparseData, 2, 2);
  OGRealSparseMatrix const *rsm2 = rsm1->copy()->asOGRealSparseMatrix();
  EXPECT_EQ(rsm1->getData(), rsm2->getData());
  EXPECT_EQ(rsm1->getColPtr(), rsm2->getColPtr());
  EXPECT_EQ(rsm1->getRowIdx(), rsm2->getRowIdx());
  EXPECT_EQ(rsm1->getRows(), rsm2->getRows());
  EXPECT_EQ(rsm1->getCols(), rsm2->getCols());
  EXPECT_NE(rsm1, rsm2);
  delete rsm1;
  delete rsm2;
}

TEST(VirtualCopyTest, ComplexSparseMatrix){
  int colPtr[2] = { 0, 1 };
  int rowIdx[2] = { 0, 1 };
  complex16 csparseData[2] = { {1.0, 2.0}, {3.0, 4.0} };
  OGComplexSparseMatrix *csm1 = new OGComplexSparseMatrix(colPtr, rowIdx, csparseData, 2, 2);
  OGComplexSparseMatrix const *csm2 = csm1->copy()->asOGComplexSparseMatrix();
  EXPECT_EQ(csm1->getData(), csm2->getData());
  EXPECT_EQ(csm1->getColPtr(), csm2->getColPtr());
  EXPECT_EQ(csm1->getRowIdx(), csm2->getRowIdx());
  EXPECT_EQ(csm1->getRows(), csm2->getRows());
  EXPECT_EQ(csm1->getCols(), csm2->getCols());
  EXPECT_NE(csm1, csm2);
  delete csm1;
  delete csm2;
}

TEST(VirtualCopyTest, COPY) {
  OGRealScalar *real = new OGRealScalar(1.0);
  ArgContainer *a = new ArgContainer();
  a->push_back(real);
  COPY *copy1 = new COPY(a);
  COPY const *copy2 = copy1->copy()->asCOPY();
  ASSERT_NE(nullptr, copy2);
  const ArgContainer* a1 = copy1->getArgs();
  const ArgContainer* a2 = copy2->getArgs();
  EXPECT_NE(a1, a2);
  EXPECT_EQ(a1->size(), a2->size());
  const OGNumeric* c1arg1 = (*a1)[0];
  const OGNumeric* c2arg1 = (*a2)[0];
  EXPECT_NE(c1arg1, c2arg1);
  const OGRealScalar* c1s = c1arg1->asOGRealScalar();
  const OGRealScalar* c2s = c2arg1->asOGRealScalar();
  ASSERT_NE(nullptr, c1s);
  ASSERT_NE(nullptr, c2s);
  EXPECT_EQ(c1s->getValue(), c2s->getValue());
  delete copy1;
  delete copy2;
}

TEST(VirtualCopyTest, PLUS) {
  OGRealScalar *real1 = new OGRealScalar(1.0);
  OGRealScalar *real2 = new OGRealScalar(2.0);
  ArgContainer *a = new ArgContainer();
  a->push_back(real1);
  a->push_back(real2);
  PLUS *plus1 = new PLUS(a);
  const PLUS *plus2 = plus1->copy()->asPLUS();
  ASSERT_NE(nullptr, plus2);
  const ArgContainer* a1 = plus1->getArgs();
  const ArgContainer* a2 = plus2->getArgs();
  EXPECT_NE(a1, a2);
  EXPECT_EQ(a1->size(), a2->size());
  const OGNumeric* p1arg1 = (*a1)[0];
  const OGNumeric* p2arg1 = (*a2)[0];
  EXPECT_NE(p1arg1, p2arg1);
  const OGNumeric* p1arg2 = (*a1)[1];
  const OGNumeric* p2arg2 = (*a2)[1];
  EXPECT_NE(p1arg2, p2arg2);
  const OGRealScalar* p1s1 = p1arg1->asOGRealScalar();
  const OGRealScalar* p2s1 = p2arg1->asOGRealScalar();
  ASSERT_NE(nullptr, p1s1);
  ASSERT_NE(nullptr, p2s1);
  EXPECT_EQ(p1s1->getValue(), p2s1->getValue());
  const OGRealScalar* p1s2 = p1arg2->asOGRealScalar();
  const OGRealScalar* p2s2 = p2arg2->asOGRealScalar();
  ASSERT_NE(nullptr, p1s2);
  ASSERT_NE(nullptr, p2s2);
  EXPECT_EQ(p1s2->getValue(), p1s2->getValue());
  delete plus1;
  delete plus2;
}

TEST(VirtualCopyTest, NEGATE) {
  OGRealScalar *real1 = new OGRealScalar(1.0);
  ArgContainer *a = new ArgContainer();
  a->push_back(real1);
  NEGATE *negate1 = new NEGATE(a);
  const NEGATE *negate2 = negate1->copy()->asNEGATE();
  ASSERT_NE(nullptr, negate2);
  const ArgContainer* a1 = negate1->getArgs();
  const ArgContainer* a2 = negate2->getArgs();
  EXPECT_NE(a1, a2);
  EXPECT_EQ(a1->size(), a2->size());
  const OGNumeric* n1arg1 = (*a1)[0];
  const OGNumeric* n2arg1 = (*a2)[0];
  EXPECT_NE(n1arg1, n2arg1);
  const OGRealScalar* n1s1 = n1arg1->asOGRealScalar();
  const OGRealScalar* n2s1 = n2arg1->asOGRealScalar();
  ASSERT_NE(nullptr, n1s1);
  ASSERT_NE(nullptr, n2s1);
  EXPECT_EQ(n1s1->getValue(), n2s1->getValue());
  delete negate1;
  delete negate2;
}

TEST(VirtualCopyTest, SVD) {
  double matData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
  OGRealMatrix *realMat = new OGRealMatrix(matData, 3, 2);
  ArgContainer *a = new ArgContainer();
  a->push_back(realMat);
  SVD *svd1 = new SVD(a);
  SVD const *svd2 = svd1->copy()->asSVD();
  ASSERT_NE(nullptr, svd2);
  const ArgContainer* a1 = svd1->getArgs();
  const ArgContainer* a2 = svd2->getArgs();
  EXPECT_NE(a1, a2);
  EXPECT_EQ(a1->size(), a2->size());
  const OGNumeric* s1arg1 = (*a1)[0];
  const OGNumeric* s2arg1 = (*a2)[0];
  EXPECT_NE(s1arg1, s2arg1);
  const OGRealMatrix* s1m = s1arg1->asOGRealMatrix();
  const OGRealMatrix* s2m = s2arg1->asOGRealMatrix();
  ASSERT_NE(nullptr, s1m);
  ASSERT_NE(nullptr, s2m);
  EXPECT_EQ(s1m->getData(), s2m->getData());
  EXPECT_EQ(s1m->getRows(), s2m->getRows());
  EXPECT_EQ(s1m->getCols(), s2m->getCols());
  EXPECT_EQ(s1m->getDatalen(), s2m->getDatalen());
  delete svd1;
  delete svd2;
}

TEST(VirtualCopyTest, SELECTRESULT) {
  double matData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
  OGRealMatrix *realMat = new OGRealMatrix(matData, 3, 2);
  ArgContainer *svdArgs = new ArgContainer();
  svdArgs->push_back(realMat);
  SVD *svd = new SVD(svdArgs);
  OGIntegerScalar* i = new OGIntegerScalar(0);
  ArgContainer *srArgs = new ArgContainer();
  srArgs->push_back(svd);
  srArgs->push_back(i);
  SELECTRESULT *sr1 = new SELECTRESULT(srArgs);
  const SELECTRESULT *sr2 = sr1->copy()->asSELECTRESULT();
  ASSERT_NE(nullptr, sr2);
  const ArgContainer* a1 = sr1->getArgs();
  const ArgContainer* a2 = sr2->getArgs();
  EXPECT_NE(a1, a2);
  EXPECT_EQ(a1->size(), a2->size());
  const OGNumeric* sr1arg1 = (*a1)[0];
  const OGNumeric* sr2arg1 = (*a2)[0];
  EXPECT_NE(sr1arg1, sr2arg1);
  const OGNumeric* sr1arg2 = (*a1)[1];
  const OGNumeric* sr2arg2 = (*a2)[1];
  EXPECT_NE(sr1arg2, sr2arg2);
  const SVD* sr1svd = sr1arg1->asSVD();
  const SVD* sr2svd = sr2arg1->asSVD();
  ASSERT_NE(nullptr, sr1svd);
  ASSERT_NE(nullptr, sr2svd);
  //EXPECT_EQ(m1s1->getValue(), m2s1->getValue()); // Need to recurse
  // in to SVD.
  const OGIntegerScalar* sr1index = sr1arg2->asOGIntegerScalar();
  const OGIntegerScalar* sr2index = sr2arg2->asOGIntegerScalar();
  ASSERT_NE(nullptr, sr1index);
  ASSERT_NE(nullptr, sr2index);
  EXPECT_EQ(sr1index->getValue(), sr2index->getValue());
  delete sr1;
  delete sr2;
}
