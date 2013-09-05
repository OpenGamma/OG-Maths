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
  delete complx;
  delete complxCopy1;
  delete complxCopy2;
}

TEST(OGExprTest, COPY){

}

TEST(OGExprTest, PLUS){

}

TEST(OGExprTest, MINUS){

}

TEST(OGExprTest, SVD){

}

TEST(OGExprTest, SELECTRESULT){

}

