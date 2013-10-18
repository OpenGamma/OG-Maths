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
 * Entrypt with NEGATE nodes
 */
class EntryptNegateTest: public ::testing::TestWithParam<std::pair<const OGNumeric*, const OGNumeric*> > {};

TEST_P(EntryptNegateTest, Running)
{
  const OGNumeric* node = GetParam().first;
  const OGTerminal* expectedResult = GetParam().second->asOGTerminal();
  const OGTerminal *result = entrypt(node)->asOGTerminal();
  ASSERT_NE(result, nullptr);
  EXPECT_TRUE((*result)==(*expectedResult));
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

double realData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
double realNegData[6] = { -1.0, -2.0, -3.0, -4.0, -5.0, -6.0 };

pair<const OGNumeric*, const OGNumeric*> negaterealmatrix()
{
  const OGTerminal* ogrealmatrix = new OGRealMatrix(realData, 2, 3);
  ArgContainer* arg = new ArgContainer();
  arg->push_back(ogrealmatrix);
  const OGNumeric* negate = new NEGATE(arg);
  const OGTerminal* ogrealnegmatrix = new OGRealMatrix(realNegData, 2, 3);
  return pair<const OGNumeric*, const OGNumeric*>(negate, ogrealnegmatrix);
}

pair<const OGNumeric*, const OGNumeric*> negates[] = { negatepair(1.0), negatepair(-1.0), negatepair(0.0) };

INSTANTIATE_TEST_CASE_P(ValueParam, EntryptNegateTest, ::testing::ValuesIn(negates));

/**
 * Entrypt with PLUS nodes
 */
class EntryptPlusTest: public ::testing::TestWithParam<std::pair<const OGNumeric*, const OGNumeric*> > {} ;

TEST_P(EntryptPlusTest, Running)
{
  const OGNumeric* node = GetParam().first;
  const OGTerminal* expectedResult = GetParam().second->asOGTerminal();
  const OGTerminal *result = entrypt(node)->asOGTerminal();
  ASSERT_NE(result, nullptr);
  EXPECT_TRUE((*result)==(*expectedResult));
  delete node;
  delete expectedResult;
  delete result;
}

pair<const OGNumeric*, const OGNumeric*> plustestsimple()
{
  ArgContainer* args = new ArgContainer();
  args->push_back(new OGRealScalar(2.0));
  args->push_back(new OGRealScalar(3.0));
  const OGNumeric* plus = new PLUS(args);
  const OGNumeric* expected = new OGRealScalar(2.0+3.0);
  return pair<const OGNumeric*, const OGNumeric*>(plus, expected);
}

pair<const OGNumeric*, const OGNumeric*> plustesttwoplus()
{
  ArgContainer* args = new ArgContainer();
  args->push_back(new OGRealScalar(2.0));
  args->push_back(new OGRealScalar(3.0));
  const OGNumeric* plus = new PLUS(args);
  args = new ArgContainer();
  args->push_back(new OGRealScalar(1.0));
  args->push_back(plus);
  plus = new PLUS(args);
  const OGNumeric* expected = new OGRealScalar(1.0+2.0+3.0);
  return pair<const OGNumeric*, const OGNumeric*>(plus, expected);
}

pair<const OGNumeric*, const OGNumeric*> plustestbigtree()
{
  const OGNumeric* bigtree = new OGRealScalar(0.0);
  double sum = 0.0;
  for (int i = 0; i < 5000; ++i)
  {
    sum += i;
    ArgContainer* args = new ArgContainer();

    if (i%2 == 0)
    {
      args->push_back(new OGRealScalar(i));
      args->push_back(bigtree);
    }
    else
    {
      args->push_back(bigtree);
      args->push_back(new OGRealScalar(i));
    }
    bigtree = new PLUS(args);
  }

  return pair<const OGNumeric*, const OGNumeric*>(bigtree, new OGRealScalar(sum));
}

pair<const OGNumeric*, const OGNumeric*> pluses[] = { plustestsimple(), plustesttwoplus(), plustestbigtree() };

INSTANTIATE_TEST_CASE_P(ValueParam, EntryptPlusTest, ::testing::ValuesIn(pluses));

