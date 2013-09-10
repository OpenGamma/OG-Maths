#include "entrypt.h"
#include "expression.hh"
#include "gtest/gtest.h"

using namespace std;
using namespace librdag;

TEST(OGExprTest, ScalarValues) {
	// Constructor
  OGRealScalar *real = new OGRealScalar(1.0);
  ASSERT_EQ(1.0, real->getValue());

  // Copy constructor from pointer
  OGRealScalar *realCopy1 = new OGRealScalar(real);
  EXPECT_EQ(1.0, realCopy1->getValue());

  // Copy constructor from object
  OGRealScalar *realCopy2 = new OGRealScalar(*real);
  EXPECT_EQ(1.0, realCopy2->getValue());

  // Cleanup
  delete real;
  delete realCopy1;
  delete realCopy2;
}

TEST(OGExprTest, ComplexValues) {
	// Constructor
  OGComplexScalar *complx = new OGComplexScalar(complex16(1.0,2.0));
  ASSERT_EQ(complex16(1.0,2.0), complx->getValue());

  // Copy constructor from pointer
  OGComplexScalar *complxCopy1 = new OGComplexScalar(complx);
  EXPECT_EQ(complex16(1.0,2.0), complxCopy1->getValue());

  // Copy constructor from object
  OGComplexScalar *complxCopy2 = new OGComplexScalar(*complx);
  EXPECT_EQ(complex16(1.0,2.0), complxCopy2->getValue());

  // Cleanup
  delete complx;
  delete complxCopy1;
  delete complxCopy2;
}

TEST(OGExprTest, COPY){
  // Constructor
  OGRealScalar *real = new OGRealScalar(1.0);
  OGExpr *copy = new COPY(real);
  ArgContainer* args = copy->getArgs();
  ASSERT_EQ(1, copy->getNArgs());
  EXPECT_EQ(real, ((*args)[0])->asOGRealScalar());

  // Constructor with args
  ArgContainer* newArgs = new ArgContainer();
  OGRealScalar *newReal = new OGRealScalar(3.14);
  newArgs->push_back(newReal);
  OGExpr *copyWithArgs = new COPY(newArgs);
  ASSERT_EQ(1, copyWithArgs->getNArgs());
  EXPECT_EQ(newReal, ((*newArgs)[0])->asOGRealScalar());

  // Constructor with args of wrong length
  // FIXME: Needs implementing once this throws an exception.

  // Cleanup
  delete copy;
  delete copyWithArgs;
}

TEST(OGExprTest, PLUS){
  // Constructor
  OGRealScalar *real = new OGRealScalar(1.0);
  OGComplexScalar *complx = new OGComplexScalar(complex16(2.0,3.0));
  OGExpr *plus = new PLUS(real, complx);
  ArgContainer* args = plus->getArgs();
  ASSERT_EQ(2, plus->getNArgs());
  EXPECT_EQ(real, ((*args)[0])->asOGRealScalar());
  EXPECT_EQ(complx, ((*args)[1])->asOGComplexScalar());

  // Constructor with args
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
  delete plus;
  delete plusWithArgs;
}

TEST(OGExprTest, MINUS){
  // Constructor
  OGRealScalar *real = new OGRealScalar(1.0);
  OGComplexScalar *complx = new OGComplexScalar(complex16(2.0,3.0));
  OGExpr *minus = new MINUS(real, complx);
  ArgContainer* args = minus->getArgs();
  ASSERT_EQ(2, minus->getNArgs());
  EXPECT_EQ(real, ((*args)[0])->asOGRealScalar());
  EXPECT_EQ(complx, ((*args)[1])->asOGComplexScalar());

  // Constructor with args
  ArgContainer* newArgs = new ArgContainer();
  OGRealScalar *newReal = new OGRealScalar(3.14);
  OGComplexScalar *newComplx = new OGComplexScalar(complex16(2.7182, 2.7182));
  newArgs->push_back(newReal);
  newArgs->push_back(newComplx);
  OGExpr *minusWithArgs = new MINUS(newArgs);
  ASSERT_EQ(2, minusWithArgs->getNArgs());
  EXPECT_EQ(newReal, ((*newArgs)[0])->asOGRealScalar());
  EXPECT_EQ(newComplx, ((*newArgs)[1])->asOGComplexScalar());

  // Constructor with args of wrong length
  // FIXME: Needs implementing once this throws an exception.

  // Cleanup
  delete minus;
  delete minusWithArgs;
}

TEST(OGExprTest, SVD){
  // Constructor
  OGRealScalar *real = new OGRealScalar(1.0);
  OGExpr *svd = new SVD(real);
  ArgContainer* args = svd->getArgs();
  ASSERT_EQ(1, svd->getNArgs());
  EXPECT_EQ(real, ((*args)[0])->asOGRealScalar());

  // Constructor with args
  ArgContainer* newArgs = new ArgContainer();
  OGRealScalar *newReal = new OGRealScalar(3.14);
  newArgs->push_back(newReal);
  OGExpr *svdWithArgs = new SVD(newArgs);
  ASSERT_EQ(1, svdWithArgs->getNArgs());
  EXPECT_EQ(newReal, ((*newArgs)[0])->asOGRealScalar());

  // Constructor with args of wrong length
  // FIXME: Needs implementing once this throws an exception.

  // Cleanup
  delete svd;
  delete svdWithArgs;
}

TEST(OGExprTest, SELECTRESULT){
  // Constructor
  OGRealScalar *real = new OGRealScalar(1.0);
  OGIntegerScalar *index = new OGIntegerScalar(1);
  OGExpr *select = new SELECTRESULT(real, index);
  ArgContainer* args = select->getArgs();
  ASSERT_EQ(2, select->getNArgs());
  EXPECT_EQ(real, ((*args)[0])->asOGRealScalar());
  EXPECT_EQ(index, ((*args)[1])->asOGIntegerScalar());

  // Constructor with args
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
  delete select;
  delete selectWithArgs;
}

