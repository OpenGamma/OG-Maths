/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "execution.hh"
#include "expression.hh"
#include "gtest/gtest.h"

using namespace std;
using namespace librdag;

TEST(LinearisationTest, OneNodeLinearisation)
{
  // Test "base case" - one node
  OGNumeric* real = new OGRealScalar(1.0);
  ExecutionList* el1 = new ExecutionList(real);
  // Check size
  EXPECT_EQ(1, el1->size());
  // Check iteration
  auto it = el1->begin();
  EXPECT_EQ(real, *it);
  ++it;
  EXPECT_EQ(it, el1->end());
  // Check subscripting
  EXPECT_EQ(real, (*el1)[0]);
  // Should free all memory
  delete el1;
  delete real;
}

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
