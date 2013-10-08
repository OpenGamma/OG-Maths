/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "execution.hh"
#include "expression.hh"
#include "terminal.hh"
#include "gtest/gtest.h"

using namespace std;
using namespace librdag;

class ExecutionOneNodeTest: public ::testing::TestWithParam<const OGTerminal*> {};

TEST_P(ExecutionOneNodeTest, TerminalTypes)
{
  // Test "base case" - one node
  const OGTerminal* node = GetParam();
  ExecutionList* el1 = new ExecutionList(node);
  // Check size
  EXPECT_EQ(1, el1->size());
  // Check iteration
  auto it = el1->begin();
  EXPECT_EQ(node, *it);
  ++it;
  EXPECT_EQ(it, el1->end());
  // Check subscripting
  EXPECT_EQ(node, (*el1)[0]);
  // Should free all memory
  delete el1;
  delete node;
}

const OGTerminal* ogreal = new OGRealScalar(1.0);
const OGTerminal* ogcomplex = new OGComplexScalar(complex16(1.0, 2.0));

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

const OGTerminal* terminals[] = { ogreal, ogcomplex };
INSTANTIATE_TEST_CASE_P(ValueParam, ExecutionOneNodeTest, ::testing::ValuesIn(terminals));

TEST(LinearisationTest, UnaryTreeLinearisation)
{
  // One unary node holding one terminal
  OGNumeric* real = new OGRealScalar(1.0);
  ArgContainer *copyArgs = new ArgContainer();
  copyArgs->push_back(real);
  OGNumeric* copy = new COPY(copyArgs);
  ExecutionList* el1 = new ExecutionList(copy);
  EXPECT_EQ(2, el1->size());
  // Check ordering
  auto it = el1->begin();
  EXPECT_EQ(*it, real);
  ++it;
  EXPECT_EQ(*it, copy);
  ++it;
  EXPECT_EQ(it, el1->end());
  // Check subscripting
  EXPECT_EQ(real, (*el1)[0]);
  EXPECT_EQ(copy, (*el1)[1]);
  // Should free all memory
  delete el1;
  delete copy;
}

TEST(LinearisationTest, BinaryTreeLinearisation)
{
  // One binary node holding two terminals
  OGNumeric *real1 = new OGRealScalar(1.0);
  OGNumeric *real2 = new OGRealScalar(2.0);
  ArgContainer* plusArgs = new ArgContainer();
  plusArgs->push_back(real1);
  plusArgs->push_back(real2);
  OGNumeric *plus = new PLUS(plusArgs);
  ExecutionList* el1 = new ExecutionList(plus);
  EXPECT_EQ(3, el1->size());
  // Check ordering. We iterate over the list and get
  // its contents first.
  const OGNumeric* nodes[3];
  int i = 0;
  for (auto it = el1->begin(); it != el1->end(); ++it)
  {
    nodes[i] = *it;
    ++i;
  }
  ASSERT_EQ(3, i);
  // The plus node should be last
  EXPECT_EQ(plus, nodes[2]);
  // Then the other two should have come first, but the
  // order of those doesn't matter.
  EXPECT_TRUE(   (nodes[0] == real1 && nodes[1] == real2)
              || (nodes[1] == real1 && nodes[0] == real2) );
  // Check subscripting is the same as the iteration order
  EXPECT_EQ(nodes[0], (*el1)[0]);
  EXPECT_EQ(nodes[1], (*el1)[1]);
  EXPECT_EQ(nodes[2], (*el1)[2]);
  // Should free all memory
  delete el1;
  delete plus;
}

TEST(LinearisationTest, BinaryUnaryLinearisation)
{
  /*
   * A Binary node holding a Terminal and a Unary which holds a terminal
   *
   *       +
   *      / \
   *    1.0 COPY
   *          \
   *           2.0
   */
  OGNumeric *real1 = new OGRealScalar(1.0);
  OGNumeric *real2 = new OGRealScalar(2.0);
  ArgContainer *copyArgs = new ArgContainer();
  copyArgs->push_back(real2);
  OGNumeric* copy = new COPY(copyArgs);
  ArgContainer* plusArgs = new ArgContainer();
  plusArgs->push_back(real1);
  plusArgs->push_back(copy);
  OGNumeric *plus = new PLUS(plusArgs);
  ExecutionList *el1 = new ExecutionList(plus);
  // Check ordering. We iterate over the list and get
  // its contents first.
  const OGNumeric* nodes[4];
  int i = 0;
  for (auto it = el1->begin(); it != el1->end(); ++it)
  {
    nodes[i] = *it;
    ++i;
  }
  ASSERT_EQ(4, i);
  // The plus node should be last
  EXPECT_EQ(nodes[3], plus);
  // The copy node should be after 2.0
  EXPECT_TRUE(   (nodes[2] == copy && ((nodes[0] == real2) || (nodes[1] == real2)))
              || (nodes[1] == copy && nodes[0] == real2) );
  // The 1.0 node should appear once in the first three nodes
  EXPECT_TRUE(   (nodes[0] == real1 && nodes[1] != real1 && nodes[2] != real1)
              || (nodes[0] != real1 && nodes[1] == real1 && nodes[2] != real1)
              || (nodes[0] != real1 && nodes[1] != real1 && nodes[2] == real1) );
  // Check subscripting order is the same as iteration order
  for (int j = 0; j<4; ++j)
  {
    EXPECT_EQ(nodes[j], (*el1)[j]);
  }
  // Should free all memory
  delete el1;
  delete plus;
}
