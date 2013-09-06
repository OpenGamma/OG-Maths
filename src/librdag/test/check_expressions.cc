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
  std::vector<OGNumeric *> * args = copy->getArgs();
  ASSERT_EQ(1, copy->getNArgs());
  EXPECT_EQ(real, dynamic_cast<OGRealScalar*>((*args)[0]));

  // Constructor with args
  std::vector<OGNumeric *> * newArgs = new std::vector<OGNumeric *>();
  OGRealScalar *newReal = new OGRealScalar(3.14);
  newArgs->push_back(newReal);
  OGExpr *copyWithArgs = new COPY(newArgs);
  ASSERT_EQ(1, copy->getNArgs());
  EXPECT_EQ(newReal, dynamic_cast<OGRealScalar*>((*newArgs)[0]));

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
  std::vector<OGNumeric *> * args = plus->getArgs();
  ASSERT_EQ(2, plus->getNArgs());
  EXPECT_EQ(real, dynamic_cast<OGRealScalar*>((*args)[0]));
  EXPECT_EQ(complx, dynamic_cast<OGComplexScalar*>((*args)[1]));
}

TEST(OGExprTest, MINUS){

}

TEST(OGExprTest, SVD){

}

TEST(OGExprTest, SELECTRESULT){

}

