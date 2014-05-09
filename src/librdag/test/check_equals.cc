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

TEST(EqualsTest, SingleValueFuzzyEqualsReal16) {
  real16 NaN = getNaN();
  real16 pinf = getPosInf();
  real16 ninf = getNegInf();

  union
  {
    uint64_t i;
    real16 d;
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


TEST(EqualsTest, ArrayBitEquals_real16) {

  int len = 4;
  real16 * data1 = new real16[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  real16 * data2 = new real16[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  real16 * data3 = new real16[4] {-1.0e0,2.0e0,3.0e0,4.0e0};

  ASSERT_TRUE(ArrayBitEquals(data1, data2, len));
  ASSERT_FALSE(ArrayBitEquals(data1, data3, len));

  delete [] data1;
  delete [] data2;
  delete [] data3;

}

TEST(EqualsTest, ArrayFuzzyEquals_real16) {

  int len = 4;
  real16 * data = new real16[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  real16 * same = new real16[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  real16 * diff  = new  real16[4]{-1.0e0,2.0e0,3.0e0,4.0e0};

  ASSERT_FALSE(ArrayFuzzyEquals(data, diff, len));
  ASSERT_TRUE(ArrayFuzzyEquals(data, same, len));

  delete [] data;
  delete [] same;
  delete [] diff;

}

TEST(EqualsTest, ArrayBitEquals_complex16) {

  int len = 4;
  complex16 * data1 = new complex16[4] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  complex16 * data2 = new complex16[4] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  complex16 * data3 = new complex16[4] {{-1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};

  ASSERT_TRUE(ArrayBitEquals(data1, data2, len));
  ASSERT_FALSE(ArrayBitEquals(data1, data3, len));

  delete [] data1;
  delete [] data2;
  delete [] data3;

}

TEST(EqualsTest, ArrayFuzzyEquals_complex16) {

  int len = 4;
  complex16 * data1 = new complex16[4] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  complex16 * data2 = new complex16[4] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  complex16 * data3 = new complex16[4] {{-1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};

  ASSERT_TRUE(ArrayFuzzyEquals(data1, data2, len));
  ASSERT_FALSE(ArrayFuzzyEquals(data1, data3, len));

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

  pOGRealScalar scalar = pOGRealScalar{new OGRealScalar(1.0e0)};
  pOGRealScalar same = pOGRealScalar{new OGRealScalar(1.0e0)};
  pOGRealScalar baddata = pOGRealScalar{new OGRealScalar(2.0e0)};
  pOGComplexScalar badtype = pOGComplexScalar{new OGComplexScalar({1.0e0,2.0e0})};
  pOGRealMatrix r_comparable = pOGRealMatrix{new OGRealMatrix(new real16[1]{1.e0},1,1,OWNER)};
  pOGRealMatrix r_notcomparable = pOGRealMatrix{new OGRealMatrix(new real16[2]{1.e0,2.e0},1,2,OWNER)};
  pOGComplexMatrix c_comparable = pOGComplexMatrix{new OGComplexMatrix(new complex16[1]{{1.e0,0.e0}},1,1, OWNER)};
  pOGComplexMatrix c_notcomparable = pOGComplexMatrix{new OGComplexMatrix(new complex16[1]{{1.e0,2.e0}},1,1, OWNER)};

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
  ASSERT_FALSE(*scalar%*c_notcomparable);
  ASSERT_TRUE(scalar->mathsequals(c_comparable));
  ASSERT_TRUE(*scalar%*c_comparable);

  ASSERT_FALSE(scalar->mathsequals(r_notcomparable));
  ASSERT_FALSE(*scalar%*r_notcomparable);
  ASSERT_TRUE(scalar->mathsequals(r_comparable));
  ASSERT_TRUE(*scalar%*r_comparable);
}



TEST(EqualsTest, OGComplexScalar) {

  pOGComplexScalar scalar = pOGComplexScalar{new OGComplexScalar({1.0e0,2.0e0})};
  pOGComplexScalar same = pOGComplexScalar{new OGComplexScalar({1.0e0,2.0e0})};
  pOGComplexScalar baddata = pOGComplexScalar{new OGComplexScalar({-1.0e0,2.0e0})};
  pOGRealScalar badtype = pOGRealScalar{new OGRealScalar(1.0e0)};
  pOGRealMatrix r_notcomparable = pOGRealMatrix{new OGRealMatrix(new real16[2]{1.e0,2.e0},1,2,OWNER)};
  pOGComplexMatrix c_comparable = pOGComplexMatrix{new OGComplexMatrix(new complex16[1]{{1.e0,2.e0}},1,1, OWNER)};
  pOGComplexMatrix c_notcomparable = pOGComplexMatrix{new OGComplexMatrix(new complex16[2]{{1.e0,2.e0},{3.e0,4.e0}},2,1, OWNER)};


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
  ASSERT_FALSE(*scalar%*c_notcomparable);
  ASSERT_TRUE(scalar->mathsequals(c_comparable));
  ASSERT_TRUE(*scalar%*c_comparable);

  ASSERT_FALSE(scalar->mathsequals(r_notcomparable));
  ASSERT_FALSE(*scalar%*r_notcomparable);
}

TEST(EqualsTest, OGIntegerScalar) {

  pOGIntegerScalar scalar = pOGIntegerScalar{new OGIntegerScalar(1)};
  pOGIntegerScalar same = pOGIntegerScalar{new OGIntegerScalar(1)};
  pOGIntegerScalar baddata = pOGIntegerScalar{new OGIntegerScalar(2)};
  pOGRealScalar badtype = pOGRealScalar{new OGRealScalar(1)};
  pOGRealMatrix r_comparable = pOGRealMatrix{new OGRealMatrix(new real16[1]{1.e0},1,1,OWNER)};
  pOGRealMatrix r_notcomparable = pOGRealMatrix{new OGRealMatrix(new real16[2]{1.e0,2.e0},1,2,OWNER)};
  pOGComplexMatrix c_comparable = pOGComplexMatrix{new OGComplexMatrix(new complex16[1]{{1.e0,0.e0}},1,1, OWNER)};
  pOGComplexMatrix c_notcomparable = pOGComplexMatrix{new OGComplexMatrix(new complex16[1]{{1.e0,2.e0}},1,1, OWNER)};

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
  ASSERT_FALSE(*scalar%*c_notcomparable);
  ASSERT_TRUE(scalar->mathsequals(c_comparable));
  ASSERT_TRUE(*scalar%*c_comparable);

  ASSERT_FALSE(scalar->mathsequals(r_notcomparable));
  ASSERT_FALSE(*scalar%*r_notcomparable);
  ASSERT_TRUE(scalar->mathsequals(r_comparable));
  ASSERT_TRUE(*scalar%*r_comparable);
}

TEST(EqualsTest, OGRealMatrix) {
  real16 * r_data1 = new real16[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  real16 * r_data2 = new real16[4] {-1.0e0,2.0e0,3.0e0,4.0e0};
  complex16 * c_data1 = new complex16[4] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  pOGRealMatrix matrix = pOGRealMatrix{new OGRealMatrix(r_data1,2,2)};
  pOGRealMatrix same = pOGRealMatrix{new OGRealMatrix(r_data1,2,2)};
  pOGRealMatrix badrows = pOGRealMatrix{new OGRealMatrix(r_data1,4,2)};
  pOGRealMatrix badcols = pOGRealMatrix{new OGRealMatrix(r_data1,2,4)};
  pOGRealMatrix baddata = pOGRealMatrix{new OGRealMatrix(r_data2,2,2)};
  pOGComplexMatrix badtype = pOGComplexMatrix{new OGComplexMatrix(c_data1,2,2)};

  pOGRealMatrix r_comparable = pOGRealMatrix{new OGRealMatrix(new real16[4]{1.e0,2.e0,3.e0,4.e0},2,2,OWNER)};
  pOGRealMatrix r_notcomparable = pOGRealMatrix{new OGRealMatrix(new real16[4]{1.e0,2.e0,3.e0,5.e0},2,2,OWNER)};
  pOGComplexMatrix c_comparable = pOGComplexMatrix{new OGComplexMatrix(new complex16[4]{{1.e0,0.e0},{2.e0,0.e0},{3.e0,0.e0},{4.e0,0.e0}},2,2, OWNER)};
  pOGComplexMatrix c_notcomparable = pOGComplexMatrix{new OGComplexMatrix(new complex16[1]{{1.e0,2.e0}},1,1, OWNER)};

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
  ASSERT_FALSE(*matrix%*c_notcomparable);
  ASSERT_TRUE(matrix->mathsequals(c_comparable));
  ASSERT_TRUE(*matrix%*c_comparable);

  ASSERT_FALSE(matrix->mathsequals(r_notcomparable));
  ASSERT_FALSE(*matrix%*r_notcomparable);
  ASSERT_TRUE(matrix->mathsequals(r_comparable));
  ASSERT_TRUE(*matrix%*r_comparable);

  delete[] r_data1;
  delete[] r_data2;
  delete[] c_data1;
}

TEST(EqualsTest, OGComplexMatrix) {
  complex16 * c_data1 = new complex16[4] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  complex16 * c_data2 = new complex16[4] {{-1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  real16 * r_data1 = new real16[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  pOGComplexMatrix matrix = pOGComplexMatrix{new OGComplexMatrix(c_data1,2,2)};
  pOGComplexMatrix same = pOGComplexMatrix{new OGComplexMatrix(c_data1,2,2)};
  pOGComplexMatrix badrows = pOGComplexMatrix{new OGComplexMatrix(c_data1,4,2)};
  pOGComplexMatrix badcols = pOGComplexMatrix{new OGComplexMatrix(c_data1,2,4)};
  pOGComplexMatrix baddata = pOGComplexMatrix{new OGComplexMatrix(c_data2,2,2)};
  pOGRealMatrix badtype = pOGRealMatrix{new OGRealMatrix(r_data1,2,2)};
  pOGRealMatrix r_notcomparable = pOGRealMatrix{new OGRealMatrix(new real16[4]{1.e0,2.e0,3.e0,5.e0},2,2,OWNER)};
  pOGComplexMatrix c_comparable = pOGComplexMatrix{new OGComplexMatrix(new complex16[4]{{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}},2,2, OWNER)};
  pOGComplexMatrix c_notcomparable = pOGComplexMatrix{new OGComplexMatrix(new complex16[4]{{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{5.0e0,50.0e0}},2,2, OWNER)};

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
  ASSERT_FALSE(*matrix%*c_notcomparable);
  ASSERT_TRUE(matrix->mathsequals(c_comparable));
  ASSERT_TRUE(*matrix%*c_comparable);

  ASSERT_FALSE(matrix->mathsequals(r_notcomparable));
  ASSERT_FALSE(*matrix%*r_notcomparable);

  delete[] c_data1;
  delete[] c_data2;
  delete[] r_data1;
}

TEST(EqualsTest, OGRealSparseMatrix) {
  real16 * r_data1 = new real16[7] {1.0e0, 3.0e0, 2.0e0, 5.0e0, 4.0e0, 6.0e0, 7.0e0 };
  real16 * r_data2 = new real16[7] {-1.0e0, 3.0e0, 2.0e0, 5.0e0, 4.0e0, 6.0e0, 7.0e0 };
  int * colPtr = new int[6] {0, 2, 4, 7, 7, 7};
  int * colPtr2 = new int[6] {0, 1, 3, 7, 7, 7};
  int * rowIdx = new int[7] {0, 1, 0, 2, 1, 2, 3 };
  int * rowIdx2 = new int[7] {0, 1, 1, 2, 1, 2, 3 };
  int rows = 5;
  int cols = 4;
  pOGRealSparseMatrix matrix = pOGRealSparseMatrix{new OGRealSparseMatrix(colPtr, rowIdx, r_data1, rows, cols)};
  pOGRealSparseMatrix same = pOGRealSparseMatrix{new OGRealSparseMatrix(colPtr, rowIdx, r_data1, rows, cols)};
  pOGRealSparseMatrix badrows = pOGRealSparseMatrix{new OGRealSparseMatrix(colPtr, rowIdx, r_data1, 1, cols)};
  pOGRealSparseMatrix badcols = pOGRealSparseMatrix{new OGRealSparseMatrix(colPtr, rowIdx, r_data1, rows, 1)};
  pOGRealSparseMatrix baddata = pOGRealSparseMatrix{new OGRealSparseMatrix(colPtr, rowIdx, r_data2, rows, 1)};
  pOGRealSparseMatrix badcolptr = pOGRealSparseMatrix{new OGRealSparseMatrix(colPtr2, rowIdx, r_data1, rows, cols)};
  pOGRealSparseMatrix badrowidx = pOGRealSparseMatrix{new OGRealSparseMatrix(colPtr, rowIdx2, r_data1, rows, cols)};
  pOGRealScalar badtype = pOGRealScalar{new OGRealScalar(1.0e0)};
  pOGRealSparseMatrix r_comparable_sparse = pOGRealSparseMatrix{new OGRealSparseMatrix(new int[6] {0, 2, 4, 7, 7, 7}, new int[7] {0, 1, 0, 2, 1, 2, 3 }, new real16[7] {1.0e0, 3.0e0, 2.0e0, 5.0e0, 4.0e0, 6.0e0, 7.0e0 }, 5, 4, OWNER)};
  pOGRealMatrix r_notcomparable = pOGRealMatrix{new OGRealMatrix(new real16[4]{1.e0,2.e0,3.e0,5.e0},2,2,OWNER)};
  pOGComplexSparseMatrix c_comparable_sparse = pOGComplexSparseMatrix{new OGComplexSparseMatrix(new int[6] {0, 2, 4, 7, 7, 7}, new int[7] {0, 1, 0, 2, 1, 2, 3 }, new complex16[7] {{1.0e0,0.e0}, {3.0e0,0.e0}, {2.0e0,0.e0}, {5.0e0,0.e0}, {4.0e0,0.e0}, {6.0e0,0.e0}, {7.0e0,0.e0}}, 5, 4, OWNER)};
  pOGComplexDiagonalMatrix c_notcomparable = pOGComplexDiagonalMatrix{new OGComplexDiagonalMatrix(new complex16[4]{1.e0,2.e0,3.e0,5.e0},5,4, OWNER)};

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
  ASSERT_FALSE(*matrix%*c_notcomparable);
  ASSERT_TRUE(matrix->mathsequals(c_comparable_sparse));
  ASSERT_TRUE(*matrix%*c_comparable_sparse);

  ASSERT_TRUE(matrix->mathsequals(r_comparable_sparse));
  ASSERT_TRUE(*matrix%*r_comparable_sparse);
  ASSERT_FALSE(matrix->mathsequals(r_notcomparable));
  ASSERT_FALSE(*matrix%*r_notcomparable);

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
  int * colPtr = new int[6] {0, 2, 4, 7, 7, 7};
  int * colPtr2 = new int[6] {0, 1, 3, 7, 7, 7};
  int * rowIdx = new int[7] {0, 1, 0, 2, 1, 2, 3 };
  int * rowIdx2 = new int[7] {0, 1, 1, 2, 1, 2, 3 };
  int rows = 5;
  int cols = 4;
  pOGComplexSparseMatrix matrix = pOGComplexSparseMatrix{new OGComplexSparseMatrix(colPtr, rowIdx, c_data1, rows, cols)};
  pOGComplexSparseMatrix same = pOGComplexSparseMatrix{new OGComplexSparseMatrix(colPtr, rowIdx, c_data1, rows, cols)};
  pOGComplexSparseMatrix badrows = pOGComplexSparseMatrix{new OGComplexSparseMatrix(colPtr, rowIdx, c_data1, 1, cols)};
  pOGComplexSparseMatrix badcols = pOGComplexSparseMatrix{new OGComplexSparseMatrix(colPtr, rowIdx, c_data1, rows, 1)};
  pOGComplexSparseMatrix baddata = pOGComplexSparseMatrix{new OGComplexSparseMatrix(colPtr, rowIdx, c_data2, rows, 1)};
  pOGComplexSparseMatrix badcolptr = pOGComplexSparseMatrix{new OGComplexSparseMatrix(colPtr2, rowIdx, c_data1, rows, cols)};
  pOGComplexSparseMatrix badrowidx = pOGComplexSparseMatrix{new OGComplexSparseMatrix(colPtr, rowIdx2, c_data1, rows, cols)};
  pOGRealScalar badtype = pOGRealScalar{new OGRealScalar(1.0e0)};
  pOGRealMatrix r_notcomparable = pOGRealMatrix{new OGRealMatrix(new real16[4]{1.e0,2.e0,3.e0,5.e0},2,2,OWNER)};
  pOGComplexSparseMatrix c_comparable_sparse = pOGComplexSparseMatrix{new OGComplexSparseMatrix(new int[6] {0, 2, 4, 7, 7, 7}, new int[7] {0, 1, 0, 2, 1, 2, 3 }, new complex16[7] {{1.0e0,10.0e0}, {3.0e0,30.0e0}, {2.0e0,20.0e0}, {5.0e0,50.0e0}, {4.0e0,40.0e0}, {6.0e0,60.0e0}, {7.0e0,70.0e0} }, 5, 4, OWNER)};
  pOGComplexDiagonalMatrix c_notcomparable = pOGComplexDiagonalMatrix{new OGComplexDiagonalMatrix(new complex16[4]{1.e0,2.e0,3.e0,5.e0},5,4, OWNER)};


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
  ASSERT_FALSE(*matrix%*c_notcomparable);
  ASSERT_TRUE(matrix->mathsequals(c_comparable_sparse));
  ASSERT_TRUE(*matrix%*c_comparable_sparse);

  ASSERT_FALSE(matrix->mathsequals(r_notcomparable));
  ASSERT_FALSE(*matrix%*r_notcomparable);

  delete[] c_data1;
  delete[] c_data2;
  delete[] colPtr;
  delete[] colPtr2;
  delete[] rowIdx;
  delete[] rowIdx2;
}

TEST(EqualsTest, OGRealDiagonalMatrix) {
  real16 * r_data1 = new real16[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  real16 * r_data2 = new real16[4] {-1.0e0,2.0e0,3.0e0,4.0e0};
  complex16 * c_data1 = new complex16[4] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  real16 * r_fullmatrix = new real16[16]{1.e0,0.e0,0.e0,0.e0,0.e0,2.e0,0.e0,0.e0,0.e0,0.e0,3.e0,0.e0,0.e0,0.e0,0.e0,4.e0};
  pOGRealDiagonalMatrix matrix = pOGRealDiagonalMatrix{new OGRealDiagonalMatrix(r_data1,4,4)};
  pOGRealDiagonalMatrix same = pOGRealDiagonalMatrix{new OGRealDiagonalMatrix(r_data1,4,4)};
  pOGRealDiagonalMatrix badrows = pOGRealDiagonalMatrix{new OGRealDiagonalMatrix(r_data1,12,4)};
  pOGRealDiagonalMatrix badcols = pOGRealDiagonalMatrix{new OGRealDiagonalMatrix(r_data1,4,12)};
  pOGRealDiagonalMatrix baddata = pOGRealDiagonalMatrix{new OGRealDiagonalMatrix(r_data2,4,4)};
  pOGRealMatrix comparable = pOGRealMatrix{new OGRealMatrix(r_fullmatrix,4,4)};
  pOGComplexMatrix badtype = pOGComplexMatrix{new OGComplexMatrix(c_data1,2,2)};
  pOGRealSparseMatrix r_comparable_sparse = pOGRealSparseMatrix{new OGRealSparseMatrix(new int[5] {0, 1, 2, 3, 4}, new int[4] {0, 1, 2, 3}, new real16[7] {1.0e0, 2.0e0, 3.0e0, 4.0e0}, 4, 4, OWNER)};
  pOGRealMatrix r_notcomparable = pOGRealMatrix{new OGRealMatrix(new real16[4]{1.e0,2.e0,3.e0,5.e0},2,2,OWNER)};
  pOGComplexDiagonalMatrix c_comparable_diag = pOGComplexDiagonalMatrix{new OGComplexDiagonalMatrix(new complex16[4] {{1.0e0,0.e0}, {2.0e0,0.e0}, {3.0e0,0.e0}, {4.0e0,0.e0}},4, 4, OWNER)};
  pOGComplexDiagonalMatrix c_notcomparable = pOGComplexDiagonalMatrix{new OGComplexDiagonalMatrix(new complex16[4]{1.e0,2.e0,3.e0,5.e0},4,4, OWNER)};


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
  ASSERT_TRUE(*matrix%*comparable);

  ASSERT_FALSE(matrix->mathsequals(c_notcomparable));
  ASSERT_FALSE(*matrix%*c_notcomparable);
  ASSERT_TRUE(matrix->mathsequals(c_comparable_diag));
  ASSERT_TRUE(*matrix%*c_comparable_diag);

  ASSERT_TRUE(matrix->mathsequals(r_comparable_sparse));
  ASSERT_TRUE(*matrix%*r_comparable_sparse);
  ASSERT_FALSE(matrix->mathsequals(r_notcomparable));
  ASSERT_FALSE(*matrix%*r_notcomparable);

  delete[] r_data1;
  delete[] r_data2;
  delete[] c_data1;
  delete[] r_fullmatrix;
}


TEST(EqualsTest, OGComplexDiagonalMatrix) {
  complex16 * c_data1 = new complex16[4] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  complex16 * c_data2 = new complex16[4] {{-1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  real16 * r_data1 = new real16[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  pOGComplexDiagonalMatrix matrix = pOGComplexDiagonalMatrix{new OGComplexDiagonalMatrix(c_data1,4,4)};
  pOGComplexDiagonalMatrix same = pOGComplexDiagonalMatrix{new OGComplexDiagonalMatrix(c_data1,4,4)};
  pOGComplexDiagonalMatrix badrows = pOGComplexDiagonalMatrix{new OGComplexDiagonalMatrix(c_data1,12,4)};
  pOGComplexDiagonalMatrix badcols = pOGComplexDiagonalMatrix{new OGComplexDiagonalMatrix(c_data1,4,12)};
  pOGComplexDiagonalMatrix baddata = pOGComplexDiagonalMatrix{new OGComplexDiagonalMatrix(c_data2,4,4)};
  pOGRealMatrix badtype = pOGRealMatrix{new OGRealMatrix(r_data1,2,2)};
  pOGRealMatrix r_notcomparable = pOGRealMatrix{new OGRealMatrix(new real16[4]{1.e0,2.e0,3.e0,5.e0},2,2,OWNER)};
  pOGComplexDiagonalMatrix c_comparable_diag = pOGComplexDiagonalMatrix{new OGComplexDiagonalMatrix(new complex16[4] {{1.0e0,10.e0}, {2.0e0,20.e0}, {3.0e0,30.e0}, {4.0e0,40.e0}},4, 4, OWNER)};
  pOGComplexDiagonalMatrix c_notcomparable = pOGComplexDiagonalMatrix{new OGComplexDiagonalMatrix(new complex16[4]{1.e0,2.e0,3.e0,5.e0},4,4, OWNER)};

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
  ASSERT_FALSE(*matrix%*c_notcomparable);
  ASSERT_TRUE(matrix->mathsequals(c_comparable_diag));
  ASSERT_TRUE(*matrix%*c_comparable_diag);

  ASSERT_FALSE(matrix->mathsequals(r_notcomparable));
  ASSERT_FALSE(*matrix%*r_notcomparable);

  delete[] c_data1;
  delete[] c_data2;
  delete[] r_data1;
}
