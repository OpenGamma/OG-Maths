/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "gtest/gtest.h"
#include "register.hh"

namespace librdag {

template<typename T>
class ScalarRegisterTest: public ::testing::Test {};
TYPED_TEST_CASE_P(ScalarRegisterTest);

TYPED_TEST_P(ScalarRegisterTest, AllocDealloc)
{
  TypeParam* r = new TypeParam();
  r->incRef();
  r->decRef();
  EXPECT_ANY_THROW(r->decRef());
  delete r;
}

REGISTER_TYPED_TEST_CASE_P(ScalarRegisterTest, AllocDealloc);
typedef ::testing::Types<OGRealScalarRegister, OGComplexScalarRegister> ScalarRegisterTypes;
INSTANTIATE_TYPED_TEST_CASE_P(ScalarTypes, ScalarRegisterTest, ScalarRegisterTypes);

template<typename T>
class MatrixRegisterTest: public ::testing::Test {};
TYPED_TEST_CASE_P(MatrixRegisterTest);

TYPED_TEST_P(MatrixRegisterTest, AllocDealloc)
{
  int ii = 10, jj = 10;
  // Check new register has no data
  TypeParam* r = new TypeParam(ii, jj);
  EXPECT_EQ(nullptr, r->getData());
  // Increment the ref count a couple of times and check data is not null
  r->incRef();
  EXPECT_NE(nullptr, r->getData());
  r->incRef();
  EXPECT_NE(nullptr, r->getData());
  // Attempt to write to every element of the matrix
  auto data = r->getData();
  for (int i = 0; i < ii; ++i)
  {
    for (int j = 0; j < jj; ++j)
    {
      data[j*ii+i] = 1.0;
    }
  }
  // Decrement the ref to zero and check mem is deallocated at zero
  r->decRef();
  EXPECT_NE(nullptr, r->getData());
  r->decRef();
  EXPECT_EQ(nullptr, r->getData());
  EXPECT_ANY_THROW(r->decRef());
  delete r;
}

REGISTER_TYPED_TEST_CASE_P(MatrixRegisterTest, AllocDealloc);
typedef ::testing::Types<OGRealMatrixRegister, OGComplexMatrixRegister> MatrixRegisterTypes;
INSTANTIATE_TYPED_TEST_CASE_P(MatTypes, MatrixRegisterTest, MatrixRegisterTypes);

} // namespace librdag
