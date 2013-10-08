/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "gtest/gtest.h"
#include "entrypt.hh"
#include "expression.hh"
#include "terminal.hh"

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

const OGTerminal* ogreal = new OGRealScalar(1.0);
const OGTerminal* ogcomplex = new OGComplexScalar(complex16(1.0, 2.0));
const OGTerminal* ogint = new OGIntegerScalar(1);

double realData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
const OGTerminal* ogrealmatrix = new OGRealMatrix(realData, 2, 3);


complex16 complexData[6] = { {1.0, 2.0}, {3.0, 4.0},  {5.0, 6.0},
                             {7.0, 8.0}, {9.0, 10.0}, {11.0, 12.0} };
const OGComplexMatrix* ogcomplexmatrix = new OGComplexMatrix(complexData, 2, 3);

double rdiagData[3] = { 1.0, 2.0, 3.0 };
const OGTerminal* ogrealdiagonalmatrix = new OGRealDiagonalMatrix(rdiagData, 3, 4);

complex16 cdiagData[3] = { {1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0} };
const OGComplexDiagonalMatrix* ogcomplexdiagonalmatrix = new OGComplexDiagonalMatrix(cdiagData, 3, 4);

int colPtr[3] = { 0, 2, 2 };
int rowIdx[2] = { 0, 1 };
double rsparseData[2] = { 1.0, 2.0 };
const OGRealSparseMatrix *ogrealsparsematrix = new OGRealSparseMatrix(colPtr, rowIdx, rsparseData, 2, 2);

complex16 csparseData[2] = { {1.0, 2.0}, {3.0, 4.0} };
const OGComplexSparseMatrix *ogcomplexsparsematrix = new OGComplexSparseMatrix(colPtr, rowIdx, csparseData, 2, 2);

const OGTerminal* terminals[] = { ogreal, ogcomplex, ogint, ogrealmatrix, ogcomplexmatrix, ogrealdiagonalmatrix,
                                  ogcomplexdiagonalmatrix, ogrealsparsematrix, ogcomplexsparsematrix };
INSTANTIATE_TEST_CASE_P(ValueParam, EntryptOneNodeTest, ::testing::ValuesIn(terminals));

/**
 * Because entrypt doesn't call the dispatcher yet, feeding entrypt an expression will
 * result in null being returned. This test is a placeholder for a test of actual
 * functionality, and will need changing when dispatch is implemented.
 */
TEST(EntryptTest, ExprResultNull) {
  ArgContainer *args = new ArgContainer();
  args->push_back(new OGRealScalar(2));
  args->push_back(new OGRealScalar(3));
  OGExpr *plus = new PLUS(args);
  const OGTerminal* result = entrypt(plus);
  EXPECT_EQ(nullptr, result);
  delete plus;
}
