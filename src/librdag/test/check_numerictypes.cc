/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "gtest/gtest.h"
#include "warningmacros.h"
#include "equals.hh"
#include "terminal.hh"

using namespace std;
using namespace librdag;

TEST(NumericTypesTest, getNaN) {
  ASSERT_TRUE(std::isnan(getNaN()));
}

TEST(NumericTypesTest, getComplexNaN) {
  ASSERT_TRUE(std::isnan(std::real(getComplexNaN())));
  ASSERT_TRUE(std::isnan(std::imag(getComplexNaN())));
}

TEST(NumericTypesTest, getPosInf) {
  ASSERT_TRUE(std::isinf(getPosInf()));
  ASSERT_EQ(signbit(getPosInf()),0);
}

TEST(NumericTypesTest, getNegInf) {
  ASSERT_TRUE(std::isinf(getNegInf()));
  ASSERT_NE(signbit(getNegInf()),0);
}

TEST(NumericTypesTest, getPosPosComplexInf) {
  real8 r = std::real(getPosPosComplexInf());
  real8 i = std::imag(getPosPosComplexInf());

  ASSERT_TRUE(std::isinf(r));
  ASSERT_EQ(signbit(r),0);
  ASSERT_TRUE(std::isinf(i));
  ASSERT_EQ(signbit(i),0);
}

TEST(NumericTypesTest, getPosNegComplexInf) {
  real8 r = std::real(getPosNegComplexInf());
  real8 i = std::imag(getPosNegComplexInf());

  ASSERT_TRUE(std::isinf(r));
  ASSERT_EQ(signbit(r),0);
  ASSERT_TRUE(std::isinf(i));
  ASSERT_NE(signbit(i),0);
}

TEST(NumericTypesTest, getNegPosComplexInf) {
  real8 r = std::real(getNegPosComplexInf());
  real8 i = std::imag(getNegPosComplexInf());

  ASSERT_TRUE(std::isinf(r));
  ASSERT_NE(signbit(r),0);
  ASSERT_TRUE(std::isinf(i));
  ASSERT_EQ(signbit(i),0);
}

TEST(NumericTypesTest, getNegNegComplexInf) {
  real8 r = std::real(getNegNegComplexInf());
  real8 i = std::imag(getNegNegComplexInf());

  ASSERT_TRUE(std::isinf(r));
  ASSERT_NE(signbit(r),0);
  ASSERT_TRUE(std::isinf(i));
  ASSERT_NE(signbit(i),0);
}