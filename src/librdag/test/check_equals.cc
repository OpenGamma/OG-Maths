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
  real16 * data1 = new real16[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  real16 * data2 = new real16[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  real16 * data3 = new real16[4] {-1.0e0,2.0e0,3.0e0,4.0e0};

  ASSERT_TRUE(ArrayBitEquals(data1, data2, len));
  ASSERT_FALSE(ArrayBitEquals(data1, data3, len));

  delete [] data1;
  delete [] data2;
  delete [] data3;

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

  OGRealScalar * scalar = new OGRealScalar(1.0e0);
  OGRealScalar * same = new OGRealScalar(1.0e0);
  OGRealScalar * baddata = new OGRealScalar(2.0e0);
  OGComplexScalar * badtype = new OGComplexScalar({1.0e0,2.0e0});

  ASSERT_TRUE(scalar->equals(same));
  ASSERT_TRUE(*scalar==*same);
  ASSERT_FALSE(scalar->equals(baddata));
  ASSERT_FALSE(scalar->equals(badtype));
  ASSERT_TRUE(*scalar!=*badtype);
  ASSERT_TRUE(*scalar!=*badtype);

  delete scalar;
  delete same;
  delete baddata;
  delete badtype;
}


TEST(EqualsTest, OGComplexScalar) {

  OGComplexScalar * scalar = new OGComplexScalar({1.0e0,2.0e0});
  OGComplexScalar * same = new OGComplexScalar({1.0e0,2.0e0});
  OGComplexScalar * baddata = new OGComplexScalar({-1.0e0,2.0e0});
  OGRealScalar * badtype = new OGRealScalar(1.0e0);

  ASSERT_TRUE(scalar->equals(same));
  ASSERT_TRUE(*scalar==*same);
  ASSERT_FALSE(scalar->equals(baddata));
  ASSERT_FALSE(scalar->equals(badtype));
  ASSERT_TRUE(*scalar!=*badtype);

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
  ASSERT_TRUE(*scalar==*same);
  ASSERT_FALSE(scalar->equals(baddata));
  ASSERT_FALSE(scalar->equals(badtype));
  ASSERT_TRUE(*scalar!=*badtype);

  delete scalar;
  delete same;
  delete baddata;
  delete badtype;
}

TEST(EqualsTest, OGRealMatrix) {
  real16 * r_data1 = new real16[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  real16 * r_data2 = new real16[4] {-1.0e0,2.0e0,3.0e0,4.0e0};
  complex16 * c_data1 = new complex16[4] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  OGRealMatrix * matrix = new OGRealMatrix(r_data1,2,2);
  OGRealMatrix * same = new OGRealMatrix(r_data1,2,2);
  OGRealMatrix * badrows = new OGRealMatrix(r_data1,4,2);
  OGRealMatrix * badcols = new OGRealMatrix(r_data1,2,4);
  OGRealMatrix * baddata = new OGRealMatrix(r_data2,2,2);
  OGComplexMatrix * badtype = new OGComplexMatrix(c_data1,2,2);

  ASSERT_TRUE(matrix->equals(same));
  ASSERT_TRUE(*matrix==*same);
  ASSERT_FALSE(matrix->equals(badrows));
  ASSERT_FALSE(matrix->equals(badcols));
  ASSERT_FALSE(matrix->equals(baddata));
  ASSERT_FALSE(matrix->equals(badtype));
  ASSERT_TRUE(*matrix!=*badtype);

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
  complex16 * c_data1 = new complex16[4] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  complex16 * c_data2 = new complex16[4] {{-1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  real16 * r_data1 = new real16[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  OGComplexMatrix * matrix = new OGComplexMatrix(c_data1,2,2);
  OGComplexMatrix * same = new OGComplexMatrix(c_data1,2,2);
  OGComplexMatrix * badrows = new OGComplexMatrix(c_data1,4,2);
  OGComplexMatrix * badcols = new OGComplexMatrix(c_data1,2,4);
  OGComplexMatrix * baddata = new OGComplexMatrix(c_data2,2,2);
  OGRealMatrix * badtype = new OGRealMatrix(r_data1,2,2);

  ASSERT_TRUE(matrix->equals(same));
  ASSERT_TRUE(*matrix==*same);
  ASSERT_FALSE(matrix->equals(badrows));
  ASSERT_FALSE(matrix->equals(badcols));
  ASSERT_FALSE(matrix->equals(baddata));
  ASSERT_FALSE(matrix->equals(badtype));
  ASSERT_TRUE(*matrix!=*badtype);

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

TEST(EqualsTest, OGRealSparseMatrix) {
  real16 * r_data1 = new real16[7] {1.0e0, 3.0e0, 2.0e0, 5.0e0, 4.0e0, 6.0e0, 7.0e0 };
  real16 * r_data2 = new real16[7] {-1.0e0, 3.0e0, 2.0e0, 5.0e0, 4.0e0, 6.0e0, 7.0e0 };
  int * colPtr = new int[6] {0, 2, 4, 7, 7, 7};
  int * colPtr2 = new int[6] {0, 1, 3, 7, 7, 7};
  int * rowIdx = new int[7] {0, 1, 0, 2, 1, 2, 3 };
  int * rowIdx2 = new int[7] {0, 1, 1, 2, 1, 2, 3 };
  int rows = 5;
  int cols = 4;
  OGRealSparseMatrix * matrix = new OGRealSparseMatrix(colPtr, rowIdx, r_data1, rows, cols);
  OGRealSparseMatrix * same = new OGRealSparseMatrix(colPtr, rowIdx, r_data1, rows, cols);
  OGRealSparseMatrix * badrows = new OGRealSparseMatrix(colPtr, rowIdx, r_data1, 1, cols);
  OGRealSparseMatrix * badcols = new OGRealSparseMatrix(colPtr, rowIdx, r_data1, rows, 1);
  OGRealSparseMatrix * baddata = new OGRealSparseMatrix(colPtr, rowIdx, r_data2, rows, 1);
  OGRealSparseMatrix * badcolptr = new OGRealSparseMatrix(colPtr2, rowIdx, r_data1, rows, cols);
  OGRealSparseMatrix * badrowidx = new OGRealSparseMatrix(colPtr, rowIdx2, r_data1, rows, cols);
  OGRealScalar * badtype = new OGRealScalar(1.0e0);

  ASSERT_TRUE(matrix->equals(same));
  ASSERT_TRUE(*matrix==*same);
  ASSERT_FALSE(matrix->equals(badrows));
  ASSERT_FALSE(matrix->equals(badcols));
  ASSERT_FALSE(matrix->equals(baddata));
  ASSERT_FALSE(matrix->equals(badcolptr));
  ASSERT_FALSE(matrix->equals(badrowidx));
  ASSERT_FALSE(matrix->equals(badtype));
  ASSERT_TRUE(*matrix!=*badtype);

  delete matrix;
  delete same;
  delete badrows;
  delete badcols;
  delete badcolptr;
  delete badrowidx;
  delete baddata;
  delete badtype;
  delete [] r_data1;
  delete [] r_data2;
  delete [] colPtr;
  delete [] rowIdx;
  delete [] colPtr2;
  delete [] rowIdx2;
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
  OGComplexSparseMatrix * matrix = new OGComplexSparseMatrix(colPtr, rowIdx, c_data1, rows, cols);
  OGComplexSparseMatrix * same = new OGComplexSparseMatrix(colPtr, rowIdx, c_data1, rows, cols);
  OGComplexSparseMatrix * badrows = new OGComplexSparseMatrix(colPtr, rowIdx, c_data1, 1, cols);
  OGComplexSparseMatrix * badcols = new OGComplexSparseMatrix(colPtr, rowIdx, c_data1, rows, 1);
  OGComplexSparseMatrix * baddata = new OGComplexSparseMatrix(colPtr, rowIdx, c_data2, rows, 1);
  OGComplexSparseMatrix * badcolptr = new OGComplexSparseMatrix(colPtr2, rowIdx, c_data1, rows, cols);
  OGComplexSparseMatrix * badrowidx = new OGComplexSparseMatrix(colPtr, rowIdx2, c_data1, rows, cols);
  OGRealScalar * badtype = new OGRealScalar(1.0e0);

  ASSERT_TRUE(matrix->equals(same));
  ASSERT_TRUE(*matrix==*same);
  ASSERT_FALSE(matrix->equals(badrows));
  ASSERT_FALSE(matrix->equals(badcols));
  ASSERT_FALSE(matrix->equals(baddata));
  ASSERT_FALSE(matrix->equals(badcolptr));
  ASSERT_FALSE(matrix->equals(badrowidx));
  ASSERT_FALSE(matrix->equals(badtype));
  ASSERT_TRUE(*matrix!=*badtype);

  delete matrix;
  delete same;
  delete badrows;
  delete badcols;
  delete badcolptr;
  delete badrowidx;
  delete baddata;
  delete badtype;
  delete [] c_data1;
  delete [] c_data2;
  delete [] colPtr;
  delete [] rowIdx;
  delete [] colPtr2;
  delete [] rowIdx2;
}

TEST(EqualsTest, OGRealDiagonalMatrix) {
  real16 * r_data1 = new real16[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  real16 * r_data2 = new real16[4] {-1.0e0,2.0e0,3.0e0,4.0e0};
  complex16 * c_data1 = new complex16[4] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  OGRealDiagonalMatrix * matrix = new OGRealDiagonalMatrix(r_data1,4,4);
  OGRealDiagonalMatrix * same = new OGRealDiagonalMatrix(r_data1,4,4);
  OGRealDiagonalMatrix * badrows = new OGRealDiagonalMatrix(r_data1,12,4);
  OGRealDiagonalMatrix * badcols = new OGRealDiagonalMatrix(r_data1,4,12);
  OGRealDiagonalMatrix * baddata = new OGRealDiagonalMatrix(r_data2,4,4);
  OGComplexMatrix * badtype = new OGComplexMatrix(c_data1,2,2);

  ASSERT_TRUE(matrix->equals(same));
  ASSERT_TRUE(*matrix==*same);
  ASSERT_FALSE(matrix->equals(badrows));
  ASSERT_FALSE(matrix->equals(badcols));
  ASSERT_FALSE(matrix->equals(baddata));
  ASSERT_FALSE(matrix->equals(badtype));
  ASSERT_TRUE(*matrix!=*badtype);

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


TEST(EqualsTest, OGComplexDiagonalMatrix) {
  complex16 * c_data1 = new complex16[4] {{1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  complex16 * c_data2 = new complex16[4] {{-1.0e0,10.0e0},{2.0e0,20.0e0},{3.0e0,30.0e0},{4.0e0,40.0e0}};
  real16 * r_data1 = new real16[4] {1.0e0,2.0e0,3.0e0,4.0e0};
  OGComplexDiagonalMatrix * matrix = new OGComplexDiagonalMatrix(c_data1,4,4);
  OGComplexDiagonalMatrix * same = new OGComplexDiagonalMatrix(c_data1,4,4);
  OGComplexDiagonalMatrix * badrows = new OGComplexDiagonalMatrix(c_data1,12,4);
  OGComplexDiagonalMatrix * badcols = new OGComplexDiagonalMatrix(c_data1,4,12);
  OGComplexDiagonalMatrix * baddata = new OGComplexDiagonalMatrix(c_data2,4,4);
  OGRealMatrix * badtype = new OGRealMatrix(r_data1,2,2);

  ASSERT_TRUE(matrix->equals(same));
  ASSERT_TRUE(*matrix==*same);
  ASSERT_FALSE(matrix->equals(badrows));
  ASSERT_FALSE(matrix->equals(badcols));
  ASSERT_FALSE(matrix->equals(baddata));
  ASSERT_FALSE(matrix->equals(badtype));
  ASSERT_TRUE(*matrix!=*badtype);

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
