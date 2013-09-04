#include "entrypt.h"
#include "expression.hh"

using namespace std;
using namespace librdag;

TEST(OGExprTest, ScalarValues) {
  OGNumeric *real = OGRealScalar(1.0);
  EXPECT_EQ(1.0, real->getValue());
}

TEST(OGExprTest, ComplexValues) {
  OGNumeric *complx = OGComplexScalar(1.0, 2.0);
  EXPECT_EQ(1.0+2.0i, complx->getValue();
}
