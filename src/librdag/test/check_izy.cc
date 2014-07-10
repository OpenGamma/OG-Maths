/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "gtest/gtest.h"
#include "warningmacros.h"
#include "equals.hh"
#include "izy.hh"
#include "exceptions.hh"
#include <memory>

using namespace std;
using namespace librdag;
using namespace izy;

// Check successful templating of vx_cos.
TEST(IZYTest_vx_cos, vd_cos) {

real8 vector[] = {3.1415926535897931,-1.5707963267948966,1.0,-1.0,0.0};
const int count = 5;
std::unique_ptr<real8[]> answer = vx_cos(count, vector);
real8 expected[] = {-1.0,0.0000000000000001,0.5403023058681398,0.5403023058681398,1.0};
EXPECT_TRUE(ArrayFuzzyEquals(expected,answer.get(),count));

}

TEST(IZYTest_vx_cos, vz_cos) {

complex16 vector[] = {{2.3561944901923448,1.5707963267948966},{1.0,1.0},{3.1415926535897931,-1.0},{0.0,1.0}};
const int count = 4;
std::unique_ptr<complex16[]> answer = vx_cos(count, vector);
complex16 expected[] = {{-1.7742571174664565,-1.6272640593586463},{0.8337300251311491,-0.9888977057628651},{-1.5430806348152437,0.0000000000000001},{1.5430806348152437,0.0}};
EXPECT_TRUE(ArrayFuzzyEquals(expected,answer.get(),count));

}