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

TreeResultPair negatepair(real8 v)
{
  OGNumeric::Ptr negate = NEGATE::create(OGRealScalar::create(v));
  OGTerminal::Ptr expected = OGRealScalar::create(-v);
  return TreeResultPair(negate, expected);
}

TreeResultPair doublenegate(real8 v)
{
  OGNumeric::Ptr negate = NEGATE::create(OGRealScalar::create(v));
  negate = NEGATE::create(negate);
  OGTerminal::Ptr expected = OGRealScalar::create(v);
  return TreeResultPair(negate, expected);
}

real8 realData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
real8 realNegData[6] = { -1.0, -2.0, -3.0, -4.0, -5.0, -6.0 };

TreeResultPair negaterealmatrix()
{
  OGNumeric::Ptr ogrealmatrix = OGRealDenseMatrix::create(realData, 2, 3);
  OGNumeric::Ptr negate = NEGATE::create(ogrealmatrix);
  OGTerminal::Ptr ogrealnegmatrix = OGRealDenseMatrix::create(realNegData, 2, 3);
  return TreeResultPair(negate, ogrealnegmatrix);
}

complex16 complexData[6] = { {1.0, 2.0}, {3.0, 4.0},  {5.0,  6.0},
                             {7.0, 8.0}, {9.0, 10.0}, {11.0, 12.0} };
complex16 complexNegData[6] = { {-1.0, -2.0}, {-3.0, -4.0},  {-5.0,  -6.0},
                                {-7.0, -8.0}, {-9.0, -10.0}, {-11.0, -12.0} };

TreeResultPair negatecomplexmatrix()
{
  OGNumeric::Ptr ogcomplexmatrix = OGComplexDenseMatrix::create(complexData, 2, 3);
  OGNumeric::Ptr negate = NEGATE::create(ogcomplexmatrix);
  OGTerminal::Ptr ogcomplexnegmatrix = OGComplexDenseMatrix::create(complexNegData, 2, 3);
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
  OGNumeric::Ptr plus = PLUS::create(OGRealScalar::create(2.0), OGRealScalar::create(3.0));
  OGTerminal::Ptr expected = OGRealScalar::create(2.0+3.0);
  return TreeResultPair(plus, expected);
}

TreeResultPair plustesttwoplus()
{
  OGNumeric::Ptr plus = PLUS::create(OGRealScalar::create(2.0), OGRealScalar::create(3.0));
  plus = PLUS::create(OGRealScalar::create(1.0), plus);
  OGTerminal::Ptr expected = OGRealScalar::create(1.0+2.0+3.0);
  return TreeResultPair(plus, expected);
}

TreeResultPair plustestbigtree()
{
  OGNumeric::Ptr bigtree = OGRealScalar::create(0.0);
  real8 sum = 0.0;
  for (size_t i = 0; i < 5000; ++i)
  {
    sum += i;

    if (i%2 == 0)
    {
      bigtree = PLUS::create(OGRealScalar::create(i), bigtree);
    }
    else
    {
      bigtree = PLUS::create(bigtree, OGRealScalar::create(i));
    }
  }

  return TreeResultPair(bigtree, OGRealScalar::create(sum));
}

TreeResultPair pluses[] = { plustestsimple(), plustesttwoplus(), plustestbigtree() };

INSTANTIATE_TEST_CASE_P(ValueParam, EntryptPlusTest, ::testing::ValuesIn(pluses));

