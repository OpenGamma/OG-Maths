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

class EntryptOneNodeTest: public ::testing::TestWithParam<OGTerminal::Ptr> {};

TEST_P(EntryptOneNodeTest, TerminalTypes)
{
  // Test "base case" - one node
  OGTerminal::Ptr node = GetParam();
  // Check that result is not null
  OGTerminal::Ptr result = entrypt(node);
  ASSERT_NE(result, OGTerminal::Ptr{});
}

INSTANTIATE_TEST_CASE_P(ValueParam, EntryptOneNodeTest, ::testing::ValuesIn(terminals));

/**
 * A TreeResultPair consists of a tree and the terminal it is expected to evaluate to.
 */
typedef pair<OGNumeric::Ptr, OGTerminal::Ptr> TreeResultPair;

/**
 * run_entrypt calls entrypt, checks the result and cleans up. This is common to
 * all entrypt tests. Factoring it into a function that is called from TEST functions
 * allows the tests to keep separate names for clarity.
 */
void run_entrypt(TreeResultPair param)
{
  OGNumeric::Ptr node = param.first;
  OGTerminal::Ptr expectedResult = param.second;
  OGTerminal::Ptr result = entrypt(node);
  ASSERT_NE(result, OGNumeric::Ptr{});
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
  OGNumeric::Ptr negate = OGNumeric::Ptr{new NEGATE(OGNumeric::Ptr{new OGRealScalar(v)})};
  OGTerminal::Ptr expected = OGTerminal::Ptr{new OGRealScalar(-v)};
  return TreeResultPair(negate, expected);
}

TreeResultPair doublenegate(double v)
{
  OGNumeric::Ptr negate = OGNumeric::Ptr{new NEGATE(OGNumeric::Ptr{new OGRealScalar(v)})};
  negate = OGNumeric::Ptr{new NEGATE(negate)};
  OGTerminal::Ptr expected = OGTerminal::Ptr{new OGRealScalar(v)};
  return TreeResultPair(negate, expected);
}

double realData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
double realNegData[6] = { -1.0, -2.0, -3.0, -4.0, -5.0, -6.0 };

TreeResultPair negaterealmatrix()
{
  OGNumeric::Ptr ogrealmatrix = OGNumeric::Ptr{new OGRealMatrix(realData, 2, 3)};
  OGNumeric::Ptr negate = OGNumeric::Ptr{new NEGATE(ogrealmatrix)};
  OGTerminal::Ptr ogrealnegmatrix = OGTerminal::Ptr{new OGRealMatrix(realNegData, 2, 3)};
  return TreeResultPair(negate, ogrealnegmatrix);
}

complex16 complexData[6] = { {1.0, 2.0}, {3.0, 4.0},  {5.0,  6.0},
                             {7.0, 8.0}, {9.0, 10.0}, {11.0, 12.0} };
complex16 complexNegData[6] = { {-1.0, -2.0}, {-3.0, -4.0},  {-5.0,  -6.0},
                                {-7.0, -8.0}, {-9.0, -10.0}, {-11.0, -12.0} };

TreeResultPair negatecomplexmatrix()
{
  OGNumeric::Ptr ogcomplexmatrix = OGNumeric::Ptr{new OGComplexMatrix(complexData, 2, 3)};
  OGNumeric::Ptr negate = OGNumeric::Ptr{new NEGATE(ogcomplexmatrix)};
  OGTerminal::Ptr ogcomplexnegmatrix = OGTerminal::Ptr{new OGComplexMatrix(complexNegData, 2, 3)};
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
  OGNumeric::Ptr plus = OGNumeric::Ptr{new PLUS(OGNumeric::Ptr{new OGRealScalar(2.0)}, OGNumeric::Ptr{new OGRealScalar(3.0)})};
  OGTerminal::Ptr expected = OGTerminal::Ptr{new OGRealScalar(2.0+3.0)};
  return TreeResultPair(plus, expected);
}

TreeResultPair plustesttwoplus()
{
  OGNumeric::Ptr plus = OGNumeric::Ptr{new PLUS(OGNumeric::Ptr{new OGRealScalar(2.0)}, OGNumeric::Ptr{new OGRealScalar(3.0)})};
  plus = OGNumeric::Ptr{new PLUS(OGNumeric::Ptr{new OGRealScalar(1.0)}, plus)};
  OGTerminal::Ptr expected = OGTerminal::Ptr{new OGRealScalar(1.0+2.0+3.0)};
  return TreeResultPair(plus, expected);
}

TreeResultPair plustestbigtree()
{
  OGNumeric::Ptr bigtree = OGNumeric::Ptr{new OGRealScalar(0.0)};
  double sum = 0.0;
  for (int i = 0; i < 5000; ++i)
  {
    sum += i;

    if (i%2 == 0)
    {
      bigtree = OGNumeric::Ptr{new PLUS(OGNumeric::Ptr{new OGRealScalar(i)}, bigtree)};
    }
    else
    {
      bigtree = OGNumeric::Ptr{new PLUS(bigtree, OGNumeric::Ptr{new OGRealScalar(i)})};
    }
  }

  return TreeResultPair(bigtree, OGTerminal::Ptr{new OGRealScalar(sum)});
}

TreeResultPair pluses[] = { plustestsimple(), plustesttwoplus(), plustestbigtree() };

INSTANTIATE_TEST_CASE_P(ValueParam, EntryptPlusTest, ::testing::ValuesIn(pluses));

