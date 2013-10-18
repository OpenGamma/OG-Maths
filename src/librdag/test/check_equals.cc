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

TEST(EqualsTest, ArrayBitEquals_real16) {

  int len = 4;
  real16 * data1 = new real16[4] {1,2,3,4};
  real16 * data2 = new real16[4] {1,2,3,4};
  real16 * data3 = new real16[4] {-1,2,3,4};

  ASSERT_TRUE(ArrayBitEquals(data1, data2, len));
  ASSERT_FALSE(ArrayBitEquals(data1, data3, len));

  delete [] data1;
  delete [] data2;
  delete [] data3;

}

TEST(EqualsTest, ArrayBitEquals_complex16) {

  int len = 4;
  complex16 * data1 = new complex16[4] {{1,10},{2,20},{3,30},{4,40}};
  complex16 * data2 = new complex16[4] {{1,10},{2,20},{3,30},{4,40}};
  complex16 * data3 = new complex16[4] {{-1,10},{2,20},{3,30},{4,40}};

  ASSERT_TRUE(ArrayBitEquals(data1, data2, len));
  ASSERT_FALSE(ArrayBitEquals(data1, data3, len));

  delete [] data1;
  delete [] data2;
  delete [] data3;

}

TEST(EqualsTest, ArrayBitEquals_int) {

  int len = 4;
  int * data1 = new int[4] {1,2,3,4};
  int * data2 = new int[4] {1,2,3,4};
  int * data3 = new int[4] {-1,2,3,4};

  ASSERT_TRUE(ArrayBitEquals(data1, data2, len));
  ASSERT_FALSE(ArrayBitEquals(data1, data3, len));

  delete [] data1;
  delete [] data2;
  delete [] data3;

}


TEST(EqualsTest, OGRealScalar) {

  OGRealScalar * scalar = new OGRealScalar(1);
  OGRealScalar * same = new OGRealScalar(1);
  OGRealScalar * baddata = new OGRealScalar(2);
  OGComplexScalar * badtype = new OGComplexScalar({1,2});

  ASSERT_TRUE(scalar->equals(same));
  ASSERT_FALSE(scalar->equals(baddata));
  ASSERT_FALSE(scalar->equals(badtype));

  delete scalar;
  delete same;
  delete baddata;
  delete badtype;
}


TEST(EqualsTest, OGComplexScalar) {

  OGComplexScalar * scalar = new OGComplexScalar({1,2});
  OGComplexScalar * same = new OGComplexScalar({1,2});
  OGComplexScalar * baddata = new OGComplexScalar({-1,2});
  OGRealScalar * badtype = new OGRealScalar(1);

  ASSERT_TRUE(scalar->equals(same));
  ASSERT_FALSE(scalar->equals(baddata));
  ASSERT_FALSE(scalar->equals(badtype));

  delete scalar;
  delete same;
  delete baddata;
  delete badtype;
}

TEST(EqualsTest, OGIntegerScalar) {

  OGIntegerScalar * scalar = new OGIntegerScalar(1);
  OGIntegerScalar * same = new OGIntegerScalar(1);
  OGIntegerScalar * baddata = new OGIntegerScalar(2);
  OGRealScalar * badtype = new OGRealScalar(1);

  ASSERT_TRUE(scalar->equals(same));
  ASSERT_FALSE(scalar->equals(baddata));
  ASSERT_FALSE(scalar->equals(badtype));

  delete scalar;
  delete same;
  delete baddata;
  delete badtype;
}

TEST(EqualsTest, OGRealMatrix) {
  real16 * r_data1 = new real16[4] {1,2,3,4};
  real16 * r_data2 = new real16[4] {-1,2,3,4};
  complex16 * c_data1 = new complex16[4] {{1,10},{2,20},{3,30},{4,40}};
  OGRealMatrix * matrix = new OGRealMatrix(r_data1,2,2);
  OGRealMatrix * same = new OGRealMatrix(r_data1,2,2);
  OGRealMatrix * badrows = new OGRealMatrix(r_data1,4,2);
  OGRealMatrix * badcols = new OGRealMatrix(r_data1,2,4);
  OGRealMatrix * baddata = new OGRealMatrix(r_data2,2,2);
  OGComplexMatrix * badtype = new OGComplexMatrix(c_data1,2,2);

  ASSERT_TRUE(matrix->equals(same));
  ASSERT_FALSE(matrix->equals(badrows));
  ASSERT_FALSE(matrix->equals(badcols));
  ASSERT_FALSE(matrix->equals(baddata));
  ASSERT_FALSE(matrix->equals(badtype));

  delete[] r_data1;
  delete[] r_data2;
  delete[] c_data1;
  delete matrix;
  delete same;
  delete badrows;
  delete badcols;
  delete baddata;
  delete badtype;

}

TEST(EqualsTest, OGComplexMatrix) {
  complex16 * c_data1 = new complex16[4] {{1,10},{2,20},{3,30},{4,40}};
  complex16 * c_data2 = new complex16[4] {{-1,10},{2,20},{3,30},{4,40}};
  real16 * r_data1 = new real16[4] {1,2,3,4};
  OGComplexMatrix * matrix = new OGComplexMatrix(c_data1,2,2);
  OGComplexMatrix * same = new OGComplexMatrix(c_data1,2,2);
  OGComplexMatrix * badrows = new OGComplexMatrix(c_data1,4,2);
  OGComplexMatrix * badcols = new OGComplexMatrix(c_data1,2,4);
  OGComplexMatrix * baddata = new OGComplexMatrix(c_data2,2,2);
  OGRealMatrix * badtype = new OGRealMatrix(r_data1,2,2);

  ASSERT_TRUE(matrix->equals(same));
  ASSERT_FALSE(matrix->equals(badrows));
  ASSERT_FALSE(matrix->equals(badcols));
  ASSERT_FALSE(matrix->equals(baddata));
  ASSERT_FALSE(matrix->equals(badtype));

  delete[] c_data1;
  delete[] c_data2;
  delete[] r_data1;
  delete matrix;
  delete same;
  delete badrows;
  delete badcols;
  delete baddata;
  delete badtype;

}