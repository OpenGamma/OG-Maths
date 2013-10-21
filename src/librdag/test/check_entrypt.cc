/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "gtest/gtest.h"
#include "entrypt.hh"
#include "expression.hh"
#include "terminal.hh"
#include "test/terminals.hh"

using namespace std;
using namespace librdag;

class EntryptOneNodeTest: public ::testing::TestWithParam<const OGTerminal*> {};

TEST_P(EntryptOneNodeTest, TerminalTypes)
{
  // Test "base case" - one node
  const OGNumeric* node = GetParam();
  // Check that result is not null
  const OGNumeric* result = entrypt(node);
  ASSERT_NE(nullptr, result);
  delete result;
  delete node;
}

INSTANTIATE_TEST_CASE_P(ValueParam, EntryptOneNodeTest, ::testing::ValuesIn(terminals));

/**
 * Very simple test that entrypt is "working", as in it returns something not null that is sane.
 */
TEST(EntryptTest, ExprResultNull)
{
  ArgContainer *args = new ArgContainer();
  args->push_back(new OGRealScalar(2));
  args->push_back(new OGRealScalar(3));
  OGExpr *plus = new PLUS(args);
  const OGTerminal* result = entrypt(plus);
  EXPECT_EQ(result->asOGRealScalar()->getValue(), 5);
  delete result;
  delete plus;
}

class EntryptNegateTest: public ::testing::TestWithParam<std::pair<const OGNumeric*, const OGNumeric*> > {};

TEST_P(EntryptNegateTest, Running)
{
  const NEGATE* node = GetParam().first->asNEGATE();
  const OGRealScalar* expectedResult = GetParam().second->asOGRealScalar();
  const OGNumeric *result = entrypt(node);
  const OGRealScalar* resultScalar = result->asOGRealScalar();
  ASSERT_NE(resultScalar, nullptr);
  EXPECT_EQ(resultScalar->getValue(), expectedResult->getValue());
  delete node;
  delete expectedResult;
  delete result;
}

pair<const OGNumeric*, const OGNumeric*> negatepair(double v)
{
  ArgContainer* args = new ArgContainer();
  args->push_back(new OGRealScalar(v));
  const OGNumeric* negate = new NEGATE(args);
  const OGNumeric* expected = new OGRealScalar(-v);
  return pair<const OGNumeric*, const OGNumeric*>(negate, expected);
}

pair<const OGNumeric*, const OGNumeric*> negates[] = { negatepair(1.0), negatepair(-1.0), negatepair(0.0) };

INSTANTIATE_TEST_CASE_P(ValueParam, EntryptNegateTest, ::testing::ValuesIn(negates));
