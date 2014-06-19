/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "gtest/gtest.h"
#include "warningmacros.h"
#include "equals.hh"
#include "terminal.hh"
#include <cfloat>
#include <iostream>
#include <cstdint>

using namespace std;
using namespace librdag;

TEST(EqualsTest, SingleValueFuzzyEqualsReal8) {
  real8 NaN = getNaN();
  real8 pinf = getPosInf();
  real8 ninf = getNegInf();

  union
  {
    uint64_t i;
    real8 d;
  } neg0;

  neg0.i = 8000000000000000;

  // NaN branch
  ASSERT_FALSE(SingleValueFuzzyEquals(NaN,NaN));
  ASSERT_FALSE(SingleValueFuzzyEquals(NaN,1));
  ASSERT_FALSE(SingleValueFuzzyEquals(1,NaN));

  // Inf branches
  ASSERT_TRUE(SingleValueFuzzyEquals(pinf,pinf));
  ASSERT_TRUE(SingleValueFuzzyEquals(ninf,ninf));
  ASSERT_FALSE(SingleValueFuzzyEquals(pinf,ninf));
  ASSERT_FALSE(SingleValueFuzzyEquals(ninf,pinf));
  ASSERT_FALSE(SingleValueFuzzyEquals(pinf,DBL_MAX));
  ASSERT_FALSE(SingleValueFuzzyEquals(ninf,-DBL_MAX));

  // val 0 branches
  ASSERT_TRUE(SingleValueFuzzyEquals(0.e0,0.e0));
  ASSERT_TRUE(SingleValueFuzzyEquals(0.e0,neg0.d));
  ASSERT_TRUE(SingleValueFuzzyEquals(neg0.d,0.e0));
  ASSERT_TRUE(SingleValueFuzzyEquals(neg0.d,neg0.d));

  // same value as it trips the return true on "difference less than abs tol" branch
  ASSERT_TRUE(SingleValueFuzzyEquals(1.1e0*DBL_EPSILON,2.e0*DBL_EPSILON));

  // same value as it trips the return true on "difference less than relative error" branch
  ASSERT_TRUE(SingleValueFuzzyEquals(1.e308,9.99999999999999e0*1.e307));

  // fail, just plain different
  ASSERT_FALSE(SingleValueFuzzyEquals(1.e0,2.e0));

}

TEST(EqualsTest, SingleValueFuzzyEqualsComplex16) {
  ASSERT_TRUE(SingleValueFuzzyEquals({1,10},{1,10}));
  ASSERT_FALSE(SingleValueFuzzyEquals({1,10},{5,10}));
  ASSERT_FALSE(SingleValueFuzzyEquals({1,10},{1,50}));
}


TEST(EqualsTest, ArrayBitEquals_real8) {

  constexpr size_t len = 4;
  real8 * data1 = new real8[len] {1.0e0,2.0e0,3.0e0,4.0e0};
  real8 * data2 = new real8[len] {1.0e0,2.0e0,3.0e0,4.0e0};
  real8 * data3 = new real8[len] {-1.0e0,2.0e0,3.0e0,4.0e0};

  ASSERT_TRUE(ArrayBitEquals(data1, data2, len));
  ASSERT_FALSE(ArrayBitEquals(data1, data3, len));

  delete [] data1;
  delete [] data2;
  delete [] data3;

}

TEST(EqualsTest, ArrayFuzzyEquals_real8) {

  constexpr size_t len = 4;
  real8 * data = new real8[len] {1.0e0,2.0e0,3.0e0,4.0e0};
  real8 * same = new real8[len] {1.0e0,2.0e0,3.0e0,4.0e0};
  real8 * diff = new real8[len] {-1.0e0,2.0e0,3.0e0,4.0e0};

  ASSERT_FALSE(ArrayFuzzyEquals(data, diff, len));
  ASSERT_TRUE(ArrayFuzzyEquals(data, same, len));

  delete [] data;
  delete [] same;
  delete [] diff;

}

TEST(EqualsTest, ArrayBitEquals_complex16) {

  constexpr size_t len = 4;
  complex16 * data1 = new complex16[len] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  complex16 * data2 = new complex16[len] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  complex16 * data3 = new complex16[len] {{-1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};

  ASSERT_TRUE(ArrayBitEquals(data1, data2, len));
  ASSERT_FALSE(ArrayBitEquals(data1, data3, len));

  delete [] data1;
  delete [] data2;
  delete [] data3;

}

TEST(EqualsTest, ArrayFuzzyEquals_complex16) {

  constexpr size_t len = 4;
  complex16 * data1 = new complex16[len] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  complex16 * data2 = new complex16[len] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  complex16 * data3 = new complex16[len] {{-1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};

  ASSERT_TRUE(ArrayFuzzyEquals(data1, data2, len));
  ASSERT_FALSE(ArrayFuzzyEquals(data1, data3, len));

  delete [] data1;
  delete [] data2;
  delete [] data3;

}

TEST(EqualsTest, ArrayBitEquals_int) {

  constexpr size_t len = 4;
  int4 * data1 = new int4[len] {1,2,3,4};
  int4 * data2 = new int4[len] {1,2,3,4};
  int4 * data3 = new int4[len] {-1,2,3,4};

  ASSERT_TRUE(ArrayBitEquals(data1, data2, len));
  ASSERT_FALSE(ArrayBitEquals(data1, data3, len));

  delete [] data1;
  delete [] data2;
  delete [] data3;

}


TEST(EqualsTest, OGRealScalar) {

  OGRealScalar::Ptr scalar = OGRealScalar::create(1.0e0);
  OGRealScalar::Ptr same = OGRealScalar::create(1.0e0);
  OGRealScalar::Ptr baddata = OGRealScalar::create(2.0e0);
  OGComplexScalar::Ptr badtype = OGComplexScalar::create({1.0e0,2.0e0});
  OGRealDenseMatrix::Ptr r_comparable = OGRealDenseMatrix::create(new real8[1]{1.e0},1,1,OWNER);
  OGRealDenseMatrix::Ptr r_notcomparable = OGRealDenseMatrix::create(new real8[2]{1.e0,2.e0},1,2,OWNER);
  OGComplexDenseMatrix::Ptr c_comparable = OGComplexDenseMatrix::create(new complex16[1]{{1.e0,0.e0}},1,1, OWNER);
  OGComplexDenseMatrix::Ptr c_notcomparable = OGComplexDenseMatrix::create(new complex16[1]{{1.e0,2.e0}},1,1, OWNER);

  // PR: Really bad API here

  ASSERT_TRUE(scalar->equals(same->asOGTerminal()));
  ASSERT_TRUE(*scalar==same);
  ASSERT_FALSE(scalar->equals(baddata));
  ASSERT_TRUE(*scalar!=baddata);
  ASSERT_FALSE(scalar->equals(badtype));
  ASSERT_TRUE(*scalar!=badtype);

  // PR: Even worse! ==~ requires ptr indirection but == forbids it!

  ASSERT_TRUE(scalar->fuzzyequals(same));
  ASSERT_TRUE(*scalar==~*same);
  ASSERT_FALSE(scalar->fuzzyequals(baddata));
  ASSERT_TRUE(*scalar!=~*baddata);
  ASSERT_FALSE(scalar->fuzzyequals(badtype));
  ASSERT_TRUE(*scalar!=~*badtype);

  ASSERT_FALSE(scalar->mathsequals(c_notcomparable));
  ASSERT_FALSE(*scalar%c_notcomparable);
  ASSERT_TRUE(scalar->mathsequals(c_comparable));
  ASSERT_TRUE(*scalar%c_comparable);

  ASSERT_FALSE(scalar->mathsequals(r_notcomparable));
  ASSERT_FALSE(*scalar%r_notcomparable);
  ASSERT_TRUE(scalar->mathsequals(r_comparable));
  ASSERT_TRUE(*scalar%r_comparable);
}



TEST(EqualsTest, OGComplexScalar) {

  OGComplexScalar::Ptr scalar = OGComplexScalar::create({1.0e0,2.0e0});
  OGComplexScalar::Ptr same = OGComplexScalar::create({1.0e0,2.0e0});
  OGComplexScalar::Ptr baddata = OGComplexScalar::create({-1.0e0,2.0e0});
  OGRealScalar::Ptr badtype = OGRealScalar::create(1.0e0);
  OGRealDenseMatrix::Ptr r_notcomparable = OGRealDenseMatrix::create(new real8[2]{1.e0,2.e0},1,2,OWNER);
  OGComplexDenseMatrix::Ptr c_comparable = OGComplexDenseMatrix::create(new complex16[1]{{1.e0,2.e0}},1,1, OWNER);
  OGComplexDenseMatrix::Ptr c_notcomparable = OGComplexDenseMatrix::create(new complex16[2]{{1.e0,2.e0},{3.e0,4.e0}},2,1, OWNER);


  ASSERT_TRUE(scalar->equals(same));
  ASSERT_TRUE(*scalar==same);
  ASSERT_FALSE(scalar->equals(baddata));
  ASSERT_TRUE(*scalar!=baddata);
  ASSERT_FALSE(scalar->equals(badtype));
  ASSERT_TRUE(*scalar!=badtype);

  ASSERT_TRUE(scalar->fuzzyequals(same));
  ASSERT_TRUE(*scalar==~*same);
  ASSERT_FALSE(scalar->fuzzyequals(baddata));
  ASSERT_TRUE(*scalar!=~*baddata);
  ASSERT_FALSE(scalar->fuzzyequals(badtype));
  ASSERT_TRUE(*scalar!=~*badtype);

  ASSERT_FALSE(scalar->mathsequals(c_notcomparable));
  ASSERT_FALSE(*scalar%c_notcomparable);
  ASSERT_TRUE(scalar->mathsequals(c_comparable));
  ASSERT_TRUE(*scalar%c_comparable);

  ASSERT_FALSE(scalar->mathsequals(r_notcomparable));
  ASSERT_FALSE(*scalar%r_notcomparable);
}

TEST(EqualsTest, OGIntegerScalar) {

  OGIntegerScalar::Ptr scalar = OGIntegerScalar::create(1);
  OGIntegerScalar::Ptr same = OGIntegerScalar::create(1);
  OGIntegerScalar::Ptr baddata = OGIntegerScalar::create(2);
  OGRealScalar::Ptr badtype = OGRealScalar::create(1);
  OGRealDenseMatrix::Ptr r_comparable = OGRealDenseMatrix::create(new real8[1]{1.e0},1,1,OWNER);
  OGRealDenseMatrix::Ptr r_notcomparable = OGRealDenseMatrix::create(new real8[2]{1.e0,2.e0},1,2,OWNER);
  OGComplexDenseMatrix::Ptr c_comparable = OGComplexDenseMatrix::create(new complex16[1]{{1.e0,0.e0}},1,1, OWNER);
  OGComplexDenseMatrix::Ptr c_notcomparable = OGComplexDenseMatrix::create(new complex16[1]{{1.e0,2.e0}},1,1, OWNER);

  ASSERT_TRUE(scalar->equals(same));
  ASSERT_TRUE(*scalar==same);
  ASSERT_FALSE(scalar->equals(baddata));
  ASSERT_TRUE(*scalar!=baddata);
  ASSERT_FALSE(scalar->equals(badtype));
  ASSERT_TRUE(*scalar!=badtype);

  ASSERT_TRUE(scalar->fuzzyequals(same));
  ASSERT_TRUE(*scalar==~*same);
  ASSERT_FALSE(scalar->fuzzyequals(baddata));
  ASSERT_TRUE(*scalar!=~*baddata);
  ASSERT_FALSE(scalar->fuzzyequals(badtype));
  ASSERT_TRUE(*scalar!=~*badtype);

  ASSERT_FALSE(scalar->mathsequals(c_notcomparable));
  ASSERT_FALSE(*scalar%c_notcomparable);
  ASSERT_TRUE(scalar->mathsequals(c_comparable));
  ASSERT_TRUE(*scalar%c_comparable);

  ASSERT_FALSE(scalar->mathsequals(r_notcomparable));
  ASSERT_FALSE(*scalar%r_notcomparable);
  ASSERT_TRUE(scalar->mathsequals(r_comparable));
  ASSERT_TRUE(*scalar%r_comparable);
}

TEST(EqualsTest, OGRealDenseMatrix) {
  real8 * r_data1 = new real8[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  real8 * r_data2 = new real8[4] {-1.0e0,2.0e0,3.0e0,4.0e0};
  complex16 * c_data1 = new complex16[4] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  OGRealDenseMatrix::Ptr matrix = OGRealDenseMatrix::create(r_data1,2,2);
  OGRealDenseMatrix::Ptr same = OGRealDenseMatrix::create(r_data1,2,2);
  OGRealDenseMatrix::Ptr badrows = OGRealDenseMatrix::create(r_data1,4,2);
  OGRealDenseMatrix::Ptr badcols = OGRealDenseMatrix::create(r_data1,2,4);
  OGRealDenseMatrix::Ptr baddata = OGRealDenseMatrix::create(r_data2,2,2);
  OGComplexDenseMatrix::Ptr badtype = OGComplexDenseMatrix::create(c_data1,2,2);

  OGRealDenseMatrix::Ptr r_comparable = OGRealDenseMatrix::create(new real8[4]{1.e0,2.e0,3.e0,4.e0},2,2,OWNER);
  OGRealDenseMatrix::Ptr r_notcomparable = OGRealDenseMatrix::create(new real8[4]{1.e0,2.e0,3.e0,5.e0},2,2,OWNER);
  OGComplexDenseMatrix::Ptr c_comparable = OGComplexDenseMatrix::create(new complex16[4]{{1.e0,0.e0},{2.e0,0.e0},{3.e0,0.e0},{4.e0,0.e0}},2,2, OWNER);
  OGComplexDenseMatrix::Ptr c_notcomparable = OGComplexDenseMatrix::create(new complex16[1]{{1.e0,2.e0}},1,1, OWNER);

  ASSERT_TRUE(matrix->equals(same));
  ASSERT_TRUE(*matrix==same);
  ASSERT_FALSE(matrix->equals(badrows));
  ASSERT_TRUE(*matrix!=badrows);
  ASSERT_FALSE(matrix->equals(badcols));
  ASSERT_TRUE(*matrix!=badcols);
  ASSERT_FALSE(matrix->equals(baddata));
  ASSERT_TRUE(*matrix!=baddata);
  ASSERT_FALSE(matrix->equals(badtype));
  ASSERT_TRUE(*matrix!=badtype);

  ASSERT_TRUE(matrix->fuzzyequals(same));
  ASSERT_TRUE(*matrix==~*same);
  ASSERT_FALSE(matrix->fuzzyequals(badrows));
  ASSERT_TRUE(*matrix!=~*badrows);
  ASSERT_FALSE(matrix->fuzzyequals(badcols));
  ASSERT_TRUE(*matrix!=~*badcols);
  ASSERT_FALSE(matrix->fuzzyequals(baddata));
  ASSERT_TRUE(*matrix!=~*baddata);
  ASSERT_FALSE(matrix->fuzzyequals(badtype));
  ASSERT_TRUE(*matrix!=~*badtype);

  ASSERT_FALSE(matrix->mathsequals(c_notcomparable));
  ASSERT_FALSE(*matrix%c_notcomparable);
  ASSERT_TRUE(matrix->mathsequals(c_comparable));
  ASSERT_TRUE(*matrix%c_comparable);

  ASSERT_FALSE(matrix->mathsequals(r_notcomparable));
  ASSERT_FALSE(*matrix%r_notcomparable);
  ASSERT_TRUE(matrix->mathsequals(r_comparable));
  ASSERT_TRUE(*matrix%r_comparable);

  delete[] r_data1;
  delete[] r_data2;
  delete[] c_data1;
}

TEST(EqualsTest, OGComplexDenseMatrix) {
  complex16 * c_data1 = new complex16[4] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  complex16 * c_data2 = new complex16[4] {{-1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  real8 * r_data1 = new real8[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  OGComplexDenseMatrix::Ptr matrix = OGComplexDenseMatrix::create(c_data1,2,2);
  OGComplexDenseMatrix::Ptr same = OGComplexDenseMatrix::create(c_data1,2,2);
  OGComplexDenseMatrix::Ptr badrows = OGComplexDenseMatrix::create(c_data1,4,2);
  OGComplexDenseMatrix::Ptr badcols = OGComplexDenseMatrix::create(c_data1,2,4);
  OGComplexDenseMatrix::Ptr baddata = OGComplexDenseMatrix::create(c_data2,2,2);
  OGRealDenseMatrix::Ptr badtype = OGRealDenseMatrix::create(r_data1,2,2);
  OGRealDenseMatrix::Ptr r_notcomparable = OGRealDenseMatrix::create(new real8[4]{1.e0,2.e0,3.e0,5.e0},2,2,OWNER);
  OGComplexDenseMatrix::Ptr c_comparable = OGComplexDenseMatrix::create(new complex16[4]{{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}},2,2, OWNER);
  OGComplexDenseMatrix::Ptr c_notcomparable = OGComplexDenseMatrix::create(new complex16[4]{{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{5.0e0,50.0e0}},2,2, OWNER);

  ASSERT_TRUE(matrix->equals(same));
  ASSERT_TRUE(*matrix==same);
  ASSERT_FALSE(matrix->equals(badrows));
  ASSERT_TRUE(*matrix!=badrows);
  ASSERT_FALSE(matrix->equals(badcols));
  ASSERT_TRUE(*matrix!=badcols);
  ASSERT_FALSE(matrix->equals(baddata));
  ASSERT_TRUE(*matrix!=baddata);
  ASSERT_FALSE(matrix->equals(badtype));
  ASSERT_TRUE(*matrix!=badtype);

  ASSERT_TRUE(matrix->fuzzyequals(same));
  ASSERT_TRUE(*matrix==~*same);
  ASSERT_FALSE(matrix->fuzzyequals(badrows));
  ASSERT_TRUE(*matrix!=~*badrows);
  ASSERT_FALSE(matrix->fuzzyequals(badcols));
  ASSERT_TRUE(*matrix!=~*badcols);
  ASSERT_FALSE(matrix->fuzzyequals(baddata));
  ASSERT_TRUE(*matrix!=~*baddata);
  ASSERT_FALSE(matrix->fuzzyequals(badtype));
  ASSERT_TRUE(*matrix!=~*badtype);

  ASSERT_FALSE(matrix->mathsequals(c_notcomparable));
  ASSERT_FALSE(*matrix%c_notcomparable);
  ASSERT_TRUE(matrix->mathsequals(c_comparable));
  ASSERT_TRUE(*matrix%c_comparable);

  ASSERT_FALSE(matrix->mathsequals(r_notcomparable));
  ASSERT_FALSE(*matrix%r_notcomparable);

  delete[] c_data1;
  delete[] c_data2;
  delete[] r_data1;
}

TEST(EqualsTest, OGRealSparseMatrix) {
  real8 * r_data1 = new real8[7] {1.0e0, 3.0e0, 2.0e0, 5.0e0, 4.0e0, 6.0e0, 7.0e0 };
  real8 * r_data2 = new real8[7] {-1.0e0, 3.0e0, 2.0e0, 5.0e0, 4.0e0, 6.0e0, 7.0e0 };
  int4 * colPtr = new int4[6] {0, 2, 4, 7, 7, 7};
  int4 * colPtr2 = new int4[6] {0, 1, 3, 7, 7, 7};
  int4 * rowIdx = new int4[7] {0, 1, 0, 2, 1, 2, 3 };
  int4 * rowIdx2 = new int4[7] {0, 1, 1, 2, 1, 2, 3 };
  size_t rows = 5;
  size_t cols = 4;
  OGRealSparseMatrix::Ptr matrix = OGRealSparseMatrix::create(colPtr, rowIdx, r_data1, rows, cols);
  OGRealSparseMatrix::Ptr same = OGRealSparseMatrix::create(colPtr, rowIdx, r_data1, rows, cols);
  OGRealSparseMatrix::Ptr badrows = OGRealSparseMatrix::create(colPtr, rowIdx, r_data1, 1, cols);
  OGRealSparseMatrix::Ptr badcols = OGRealSparseMatrix::create(colPtr, rowIdx, r_data1, rows, 1);
  OGRealSparseMatrix::Ptr baddata = OGRealSparseMatrix::create(colPtr, rowIdx, r_data2, rows, 1);
  OGRealSparseMatrix::Ptr badcolptr = OGRealSparseMatrix::create(colPtr2, rowIdx, r_data1, rows, cols);
  OGRealSparseMatrix::Ptr badrowidx = OGRealSparseMatrix::create(colPtr, rowIdx2, r_data1, rows, cols);
  OGRealScalar::Ptr badtype = OGRealScalar::create(1.0e0);
  OGRealSparseMatrix::Ptr r_comparable_sparse = OGRealSparseMatrix::create(new int4[6] {0, 2, 4, 7, 7, 7}, new int4[7] {0, 1, 0, 2, 1, 2, 3 }, new real8[7] {1.0e0, 3.0e0, 2.0e0, 5.0e0, 4.0e0, 6.0e0, 7.0e0 }, 5, 4, OWNER);
  OGRealDenseMatrix::Ptr r_notcomparable = OGRealDenseMatrix::create(new real8[4]{1.e0,2.e0,3.e0,5.e0},2,2,OWNER);
  OGComplexSparseMatrix::Ptr c_comparable_sparse = OGComplexSparseMatrix::create(new int4[6] {0, 2, 4, 7, 7, 7}, new int4[7] {0, 1, 0, 2, 1, 2, 3 }, new complex16[7] {{1.0e0,0.e0}, {3.0e0,0.e0}, {2.0e0,0.e0}, {5.0e0,0.e0}, {4.0e0,0.e0}, {6.0e0,0.e0}, {7.0e0,0.e0}}, 5, 4, OWNER);
  OGComplexDiagonalMatrix::Ptr c_notcomparable = OGComplexDiagonalMatrix::create(new complex16[4]{1.e0,2.e0,3.e0,5.e0},5,4, OWNER);

  ASSERT_TRUE(matrix->equals(same));
  ASSERT_TRUE(*matrix==same);
  ASSERT_FALSE(matrix->equals(badrows));
  ASSERT_TRUE(*matrix!=badrows);
  ASSERT_FALSE(matrix->equals(badcols));
  ASSERT_TRUE(*matrix!=badcols);
  ASSERT_FALSE(matrix->equals(baddata));
  ASSERT_TRUE(*matrix!=baddata);
  ASSERT_FALSE(matrix->equals(badcolptr));
  ASSERT_TRUE(*matrix!=badcolptr);
  ASSERT_FALSE(matrix->equals(badrowidx));
  ASSERT_TRUE(*matrix!=badrowidx);
  ASSERT_FALSE(matrix->equals(badtype));
  ASSERT_TRUE(*matrix!=badtype);

  ASSERT_TRUE(matrix->fuzzyequals(same));
  ASSERT_TRUE(*matrix==~*same);
  ASSERT_FALSE(matrix->fuzzyequals(badrows));
  ASSERT_TRUE(*matrix!=~*badrows);
  ASSERT_FALSE(matrix->fuzzyequals(badcols));
  ASSERT_TRUE(*matrix!=~*badcols);
  ASSERT_FALSE(matrix->fuzzyequals(baddata));
  ASSERT_TRUE(*matrix!=~*baddata);
  ASSERT_FALSE(matrix->fuzzyequals(badcolptr));
  ASSERT_TRUE(*matrix!=~*badcolptr);
  ASSERT_FALSE(matrix->fuzzyequals(badrowidx));
  ASSERT_TRUE(*matrix!=~*badrowidx);
  ASSERT_FALSE(matrix->fuzzyequals(badtype));
  ASSERT_TRUE(*matrix!=~*badtype);

  ASSERT_FALSE(matrix->mathsequals(c_notcomparable));
  ASSERT_FALSE(*matrix%c_notcomparable);
  ASSERT_TRUE(matrix->mathsequals(c_comparable_sparse));
  ASSERT_TRUE(*matrix%c_comparable_sparse);

  ASSERT_TRUE(matrix->mathsequals(r_comparable_sparse));
  ASSERT_TRUE(*matrix%r_comparable_sparse);
  ASSERT_FALSE(matrix->mathsequals(r_notcomparable));
  ASSERT_FALSE(*matrix%r_notcomparable);

  delete[] r_data1;
  delete[] r_data2;
  delete[] colPtr;
  delete[] colPtr2;
  delete[] rowIdx;
  delete[] rowIdx2;
}

TEST(EqualsTest, OGComplexSparseMatrix) {
  complex16 * c_data1 = new complex16[7] {{1.0e0,10.0e0}, {3.0e0,30.0e0}, {2.0e0,20.0e0}, {5.0e0,50.0e0}, {4.0e0,40.0e0}, {6.0e0,60.0e0}, {7.0e0,70.0e0} };
  complex16 * c_data2 = new complex16[7] {{-1.0e0,10.0e0}, {3.0e0,30.0e0}, {2.0e0,20.0e0}, {5.0e0,50.0e0}, {4.0e0,40.0e0}, {6.0e0,60.0e0}, {7.0e0,70.0e0} };
  int4 * colPtr = new int4[6] {0, 2, 4, 7, 7, 7};
  int4 * colPtr2 = new int4[6] {0, 1, 3, 7, 7, 7};
  int4 * rowIdx = new int4[7] {0, 1, 0, 2, 1, 2, 3 };
  int4 * rowIdx2 = new int4[7] {0, 1, 1, 2, 1, 2, 3 };
  size_t rows = 5;
  size_t cols = 4;
  OGComplexSparseMatrix::Ptr matrix = OGComplexSparseMatrix::create(colPtr, rowIdx, c_data1, rows, cols);
  OGComplexSparseMatrix::Ptr same = OGComplexSparseMatrix::create(colPtr, rowIdx, c_data1, rows, cols);
  OGComplexSparseMatrix::Ptr badrows = OGComplexSparseMatrix::create(colPtr, rowIdx, c_data1, 1, cols);
  OGComplexSparseMatrix::Ptr badcols = OGComplexSparseMatrix::create(colPtr, rowIdx, c_data1, rows, 1);
  OGComplexSparseMatrix::Ptr baddata = OGComplexSparseMatrix::create(colPtr, rowIdx, c_data2, rows, 1);
  OGComplexSparseMatrix::Ptr badcolptr = OGComplexSparseMatrix::create(colPtr2, rowIdx, c_data1, rows, cols);
  OGComplexSparseMatrix::Ptr badrowidx = OGComplexSparseMatrix::create(colPtr, rowIdx2, c_data1, rows, cols);
  OGRealScalar::Ptr badtype = OGRealScalar::create(1.0e0);
  OGRealDenseMatrix::Ptr r_notcomparable = OGRealDenseMatrix::create(new real8[4]{1.e0,2.e0,3.e0,5.e0},2,2,OWNER);
  OGComplexSparseMatrix::Ptr c_comparable_sparse = OGComplexSparseMatrix::create(new int4[6] {0, 2, 4, 7, 7, 7}, new int4[7] {0, 1, 0, 2, 1, 2, 3 }, new complex16[7] {{1.0e0,10.0e0}, {3.0e0,30.0e0}, {2.0e0,20.0e0}, {5.0e0,50.0e0}, {4.0e0,40.0e0}, {6.0e0,60.0e0}, {7.0e0,70.0e0} }, 5, 4, OWNER);
  OGComplexDiagonalMatrix::Ptr c_notcomparable = OGComplexDiagonalMatrix::create(new complex16[4]{1.e0,2.e0,3.e0,5.e0},5,4, OWNER);


  ASSERT_TRUE(matrix->equals(same));
  ASSERT_TRUE(*matrix==same);
  ASSERT_FALSE(matrix->equals(badrows));
  ASSERT_TRUE(*matrix!=badrows);
  ASSERT_FALSE(matrix->equals(badcols));
  ASSERT_TRUE(*matrix!=badcols);
  ASSERT_FALSE(matrix->equals(baddata));
  ASSERT_TRUE(*matrix!=baddata);
  ASSERT_FALSE(matrix->equals(badcolptr));
  ASSERT_TRUE(*matrix!=badcolptr);
  ASSERT_FALSE(matrix->equals(badrowidx));
  ASSERT_TRUE(*matrix!=badrowidx);
  ASSERT_FALSE(matrix->equals(badtype));
  ASSERT_TRUE(*matrix!=badtype);

  ASSERT_TRUE(matrix->fuzzyequals(same));
  ASSERT_TRUE(*matrix==~*same);
  ASSERT_FALSE(matrix->fuzzyequals(badrows));
  ASSERT_TRUE(*matrix!=~*badrows);
  ASSERT_FALSE(matrix->fuzzyequals(badcols));
  ASSERT_TRUE(*matrix!=~*badcols);
  ASSERT_FALSE(matrix->fuzzyequals(baddata));
  ASSERT_TRUE(*matrix!=~*baddata);
  ASSERT_FALSE(matrix->fuzzyequals(badcolptr));
  ASSERT_TRUE(*matrix!=~*badcolptr);
  ASSERT_FALSE(matrix->fuzzyequals(badrowidx));
  ASSERT_TRUE(*matrix!=~*badrowidx);
  ASSERT_FALSE(matrix->fuzzyequals(badtype));
  ASSERT_TRUE(*matrix!=~*badtype);

  ASSERT_FALSE(matrix->mathsequals(c_notcomparable));
  ASSERT_FALSE(*matrix%c_notcomparable);
  ASSERT_TRUE(matrix->mathsequals(c_comparable_sparse));
  ASSERT_TRUE(*matrix%c_comparable_sparse);

  ASSERT_FALSE(matrix->mathsequals(r_notcomparable));
  ASSERT_FALSE(*matrix%r_notcomparable);

  delete[] c_data1;
  delete[] c_data2;
  delete[] colPtr;
  delete[] colPtr2;
  delete[] rowIdx;
  delete[] rowIdx2;
}

TEST(EqualsTest, OGRealDiagonalMatrix) {
  real8 * r_data1 = new real8[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  real8 * r_data2 = new real8[4] {-1.0e0,2.0e0,3.0e0,4.0e0};
  complex16 * c_data1 = new complex16[4] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  real8 * r_fullmatrix = new real8[16]{1.e0,0.e0,0.e0,0.e0,0.e0,2.e0,0.e0,0.e0,0.e0,0.e0,3.e0,0.e0,0.e0,0.e0,0.e0,4.e0};
  OGRealDiagonalMatrix::Ptr matrix = OGRealDiagonalMatrix::create(r_data1,4,4);
  OGRealDiagonalMatrix::Ptr same = OGRealDiagonalMatrix::create(r_data1,4,4);
  OGRealDiagonalMatrix::Ptr badrows = OGRealDiagonalMatrix::create(r_data1,12,4);
  OGRealDiagonalMatrix::Ptr badcols = OGRealDiagonalMatrix::create(r_data1,4,12);
  OGRealDiagonalMatrix::Ptr baddata = OGRealDiagonalMatrix::create(r_data2,4,4);
  OGRealDenseMatrix::Ptr comparable = OGRealDenseMatrix::create(r_fullmatrix,4,4);
  OGComplexDenseMatrix::Ptr badtype = OGComplexDenseMatrix::create(c_data1,2,2);
  OGRealSparseMatrix::Ptr r_comparable_sparse = OGRealSparseMatrix::create(new int4[5] {0, 1, 2, 3, 4}, new int4[4] {0, 1, 2, 3}, new real8[7] {1.0e0, 2.0e0, 3.0e0, 4.0e0}, 4, 4, OWNER);
  OGRealDenseMatrix::Ptr r_notcomparable = OGRealDenseMatrix::create(new real8[4]{1.e0,2.e0,3.e0,5.e0},2,2,OWNER);
  OGComplexDiagonalMatrix::Ptr c_comparable_diag = OGComplexDiagonalMatrix::create(new complex16[4] {{1.0e0,0.e0}, {2.0e0,0.e0}, {3.0e0,0.e0}, {4.0e0,0.e0}},4, 4, OWNER);
  OGComplexDiagonalMatrix::Ptr c_notcomparable = OGComplexDiagonalMatrix::create(new complex16[4]{1.e0,2.e0,3.e0,5.e0},4,4, OWNER);


  ASSERT_TRUE(matrix->equals(same));
  ASSERT_TRUE(*matrix==same);
  ASSERT_FALSE(matrix->equals(badrows));
  ASSERT_TRUE(*matrix!=badrows);
  ASSERT_FALSE(matrix->equals(badcols));
  ASSERT_TRUE(*matrix!=badcols);
  ASSERT_FALSE(matrix->equals(baddata));
  ASSERT_TRUE(*matrix!=baddata);
  ASSERT_FALSE(matrix->equals(badtype));
  ASSERT_TRUE(*matrix!=badtype);

  ASSERT_TRUE(matrix->fuzzyequals(same));
  ASSERT_TRUE(*matrix==~*same);
  ASSERT_FALSE(matrix->fuzzyequals(badrows));
  ASSERT_TRUE(*matrix!=~*badrows);
  ASSERT_FALSE(matrix->fuzzyequals(badcols));
  ASSERT_TRUE(*matrix!=~*badcols);
  ASSERT_FALSE(matrix->fuzzyequals(baddata));
  ASSERT_TRUE(*matrix!=~*baddata);
  ASSERT_FALSE(matrix->fuzzyequals(badtype));
  ASSERT_TRUE(*matrix!=~*badtype);

  ASSERT_TRUE(matrix->mathsequals(comparable));
  ASSERT_TRUE(*matrix%comparable);

  ASSERT_FALSE(matrix->mathsequals(c_notcomparable));
  ASSERT_FALSE(*matrix%c_notcomparable);
  ASSERT_TRUE(matrix->mathsequals(c_comparable_diag));
  ASSERT_TRUE(*matrix%c_comparable_diag);

  ASSERT_TRUE(matrix->mathsequals(r_comparable_sparse));
  ASSERT_TRUE(*matrix%r_comparable_sparse);
  ASSERT_FALSE(matrix->mathsequals(r_notcomparable));
  ASSERT_FALSE(*matrix%r_notcomparable);

  delete[] r_data1;
  delete[] r_data2;
  delete[] c_data1;
  delete[] r_fullmatrix;
}


TEST(EqualsTest, OGComplexDiagonalMatrix) {
  complex16 * c_data1 = new complex16[4] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  complex16 * c_data2 = new complex16[4] {{-1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  real8 * r_data1 = new real8[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  OGComplexDiagonalMatrix::Ptr matrix = OGComplexDiagonalMatrix::create(c_data1,4,4);
  OGComplexDiagonalMatrix::Ptr same = OGComplexDiagonalMatrix::create(c_data1,4,4);
  OGComplexDiagonalMatrix::Ptr badrows = OGComplexDiagonalMatrix::create(c_data1,12,4);
  OGComplexDiagonalMatrix::Ptr badcols = OGComplexDiagonalMatrix::create(c_data1,4,12);
  OGComplexDiagonalMatrix::Ptr baddata = OGComplexDiagonalMatrix::create(c_data2,4,4);
  OGRealDenseMatrix::Ptr badtype = OGRealDenseMatrix::create(r_data1,2,2);
  OGRealDenseMatrix::Ptr r_notcomparable = OGRealDenseMatrix::create(new real8[4]{1.e0,2.e0,3.e0,5.e0},2,2,OWNER);
  OGComplexDiagonalMatrix::Ptr c_comparable_diag = OGComplexDiagonalMatrix::create(new complex16[4] {{1.0e0,10.e0}, {2.0e0,20.e0}, {3.0e0,30.e0}, {4.0e0,40.e0}},4, 4, OWNER);
  OGComplexDiagonalMatrix::Ptr c_notcomparable = OGComplexDiagonalMatrix::create(new complex16[4]{1.e0,2.e0,3.e0,5.e0},4,4, OWNER);

  ASSERT_TRUE(matrix->equals(same));
  ASSERT_TRUE(*matrix==same);
  ASSERT_FALSE(matrix->equals(badrows));
  ASSERT_TRUE(*matrix!=badrows);
  ASSERT_FALSE(matrix->equals(badcols));
  ASSERT_TRUE(*matrix!=badcols);
  ASSERT_FALSE(matrix->equals(baddata));
  ASSERT_TRUE(*matrix!=baddata);
  ASSERT_FALSE(matrix->equals(badtype));
  ASSERT_TRUE(*matrix!=badtype);

  ASSERT_TRUE(matrix->fuzzyequals(same));
  ASSERT_TRUE(*matrix==~*same);
  ASSERT_FALSE(matrix->fuzzyequals(badrows));
  ASSERT_TRUE(*matrix!=~*badrows);
  ASSERT_FALSE(matrix->fuzzyequals(badcols));
  ASSERT_TRUE(*matrix!=~*badcols);
  ASSERT_FALSE(matrix->fuzzyequals(baddata));
  ASSERT_TRUE(*matrix!=~*baddata);
  ASSERT_FALSE(matrix->fuzzyequals(badtype));
  ASSERT_TRUE(*matrix!=~*badtype);

  ASSERT_FALSE(matrix->mathsequals(c_notcomparable));
  ASSERT_FALSE(*matrix%c_notcomparable);
  ASSERT_TRUE(matrix->mathsequals(c_comparable_diag));
  ASSERT_TRUE(*matrix%c_comparable_diag);

  ASSERT_FALSE(matrix->mathsequals(r_notcomparable));
  ASSERT_FALSE(*matrix%r_notcomparable);

  delete[] c_data1;
  delete[] c_data2;
  delete[] r_data1;
}
