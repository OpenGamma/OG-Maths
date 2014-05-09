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

class EntryptOneNodeTest: public ::testing::TestWithParam<pOGTerminal> {};

TEST_P(EntryptOneNodeTest, TerminalTypes)
{
  // Test "base case" - one node
  pOGTerminal node = GetParam();
  // Check that result is not null
  pOGTerminal result = entrypt(node);
  ASSERT_NE(result, pOGTerminal{});
}

INSTANTIATE_TEST_CASE_P(ValueParam, EntryptOneNodeTest, ::testing::ValuesIn(terminals));

/**
 * A TreeResultPair consists of a tree and the terminal it is expected to evaluate to.
 */
typedef pair<pOGNumeric, pOGTerminal> TreeResultPair;

/**
 * run_entrypt calls entrypt, checks the result and cleans up. This is common to
 * all entrypt tests. Factoring it into a function that is called from TEST functions
 * allows the tests to keep separate names for clarity.
 */
void run_entrypt(TreeResultPair param)
{
  pOGNumeric node = param.first;
  pOGTerminal expectedResult = param.second;
  pOGTerminal result = entrypt(node);
  ASSERT_NE(result, pOGNumeric{});
  // PR: There is something very unexpected about this syntax.
  // PR This is super-confusing! Needs API fixing
  EXPECT_TRUE((*result)==(expectedResult));
}

/**
 * Entrypt with NEGATE nodes
 */
class EntryptNegateTest: public ::testing::TestWithParam<TreeResultPair> {};

TEST_P(EntryptNegateTest, Running)
{
  run_entrypt(GetParam());
}

TreeResultPair negatepair(double v)
{
  pOGNumeric negate = pOGNumeric{new NEGATE(pOGNumeric{new OGRealScalar(v)})};
  pOGTerminal expected = pOGTerminal{new OGRealScalar(-v)};
  return TreeResultPair(negate, expected);
}

TreeResultPair doublenegate(double v)
{
  pOGNumeric negate = pOGNumeric{new NEGATE(pOGNumeric{new OGRealScalar(v)})};
  negate = pOGNumeric{new NEGATE(negate)};
  pOGTerminal expected = pOGTerminal{new OGRealScalar(v)};
  return TreeResultPair(negate, expected);
}

double realData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
double realNegData[6] = { -1.0, -2.0, -3.0, -4.0, -5.0, -6.0 };

TreeResultPair negaterealmatrix()
{
  pOGNumeric ogrealmatrix = pOGNumeric{new OGRealMatrix(realData, 2, 3)};
  pOGNumeric negate = pOGNumeric{new NEGATE(ogrealmatrix)};
  pOGTerminal ogrealnegmatrix = pOGTerminal{new OGRealMatrix(realNegData, 2, 3)};
  return TreeResultPair(negate, ogrealnegmatrix);
}

complex16 complexData[6] = { {1.0, 2.0}, {3.0, 4.0},  {5.0,  6.0},
                             {7.0, 8.0}, {9.0, 10.0}, {11.0, 12.0} };
complex16 complexNegData[6] = { {-1.0, -2.0}, {-3.0, -4.0},  {-5.0,  -6.0},
                                {-7.0, -8.0}, {-9.0, -10.0}, {-11.0, -12.0} };

TreeResultPair negatecomplexmatrix()
{
  pOGNumeric ogcomplexmatrix = pOGNumeric{new OGComplexMatrix(complexData, 2, 3)};
  pOGNumeric negate = pOGNumeric{new NEGATE(ogcomplexmatrix)};
  pOGTerminal ogcomplexnegmatrix = pOGTerminal{new OGComplexMatrix(complexNegData, 2, 3)};
  return TreeResultPair(negate, ogcomplexnegmatrix);
}

TreeResultPair negates[] =
   { negatepair(1.0), negatepair(-1.0), negatepair(0.0),
     doublenegate(1.0), doublenegate(-1.0), doublenegate(0.0),
     negaterealmatrix(), negatecomplexmatrix() };

INSTANTIATE_TEST_CASE_P(ValueParam, EntryptNegateTest, ::testing::ValuesIn(negates));


/**
 * Entrypt with PLUS nodes
 */
class EntryptPlusTest: public ::testing::TestWithParam<TreeResultPair> {} ;

TEST_P(EntryptPlusTest, Running)
{
  run_entrypt(GetParam());
}

TreeResultPair plustestsimple()
{
  pOGNumeric plus = pOGNumeric{new PLUS(pOGNumeric{new OGRealScalar(2.0)}, pOGNumeric{new OGRealScalar(3.0)})};
  pOGTerminal expected = pOGTerminal{new OGRealScalar(2.0+3.0)};
  return TreeResultPair(plus, expected);
}

TreeResultPair plustesttwoplus()
{
  pOGNumeric plus = pOGNumeric{new PLUS(pOGNumeric{new OGRealScalar(2.0)}, pOGNumeric{new OGRealScalar(3.0)})};
  plus = pOGNumeric{new PLUS(pOGNumeric{new OGRealScalar(1.0)}, plus)};
  pOGTerminal expected = pOGTerminal{new OGRealScalar(1.0+2.0+3.0)};
  return TreeResultPair(plus, expected);
}

TreeResultPair plustestbigtree()
{
  pOGNumeric bigtree = pOGNumeric{new OGRealScalar(0.0)};
  double sum = 0.0;
  for (int i = 0; i < 5000; ++i)
  {
    sum += i;

    if (i%2 == 0)
    {
      bigtree = pOGNumeric{new PLUS(pOGNumeric{new OGRealScalar(i)}, bigtree)};
    }
    else
    {
      bigtree = pOGNumeric{new PLUS(bigtree, pOGNumeric{new OGRealScalar(i)})};
    }
  }

  return TreeResultPair(bigtree, pOGTerminal{new OGRealScalar(sum)});
}

TreeResultPair pluses[] = { plustestsimple(), plustesttwoplus(), plustestbigtree() };

INSTANTIATE_TEST_CASE_P(ValueParam, EntryptPlusTest, ::testing::ValuesIn(pluses));

