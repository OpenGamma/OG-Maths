/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "containers.hh"
#include "gtest/gtest.h"

using namespace std;
using namespace librdag;

/*
 * Test rationale: check the functionality that we wrapped in the derived class
 * only. Otherwise we're just testing the C++ compiler/STL implementation.
 */
TEST(ContainerTest, OwningPtrVectorTest) {
  // Default constructor
  OwningPtrVector<int> *pv1 = new OwningPtrVector<int>();
  EXPECT_EQ(0, pv1->size());

  // Add elements
  size_t n = 5;
  int** i_ptr_list = (int**)malloc(sizeof(int*)*n);
  for (size_t i = 0; i < n; ++i)
  {
    int* intp = new int(i);
    i_ptr_list[i] = intp;
    pv1->push_back(intp);
  }

  // Check size matches number of inserted elements
  ASSERT_EQ(5, pv1->size());

  // Test operator[]
  for (size_t i = 0; i < n; ++i)
  {
    const int* ptr = (*pv1)[i];
    EXPECT_EQ(i_ptr_list[i], ptr);
    EXPECT_EQ(i, *ptr);
  }

  // Test iteration over list
  int i = 0;
  for (auto it = pv1->begin(); it != pv1->end(); ++it)
  {
    EXPECT_EQ(*it, i_ptr_list[i]);
    EXPECT_EQ(i, **it);
    ++i;
  }
  EXPECT_EQ(5, i);

  // Copy the OwningPtrVector
  OwningPtrVector<int> *pv2 = pv1->copy();

  // Test the copy is the same as the original
  OwningPtrVector<int>::citerator it1, it2;
  for (it1 = pv1->begin(), it2 = pv2->begin(); it1 != pv1->end(), it2 != pv2->end(); ++it1, ++it2)
  {
    EXPECT_EQ(**it1, **it2);
  }
  EXPECT_TRUE( (it1 == pv1->end()) && (it2 == pv2->end()) );

  // Delete - no leaks should occur. pv1 owns the pointers to individual ints.
  delete pv1;
  delete pv2;
  // However, the list of pointers is not owned by it.
  free(i_ptr_list);
}
