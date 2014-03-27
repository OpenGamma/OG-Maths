/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.maths.datacontainers.matrix;

import static org.testng.Assert.assertTrue;

import java.util.Arrays;

import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.matrix.OGComplexSparseMatrix;
import com.opengamma.maths.datacontainers.scalar.OGComplexScalar;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.exceptions.MathsExceptionNullPointer;

/**
 * Tests the complex sparse matrix class
 */
public class OGComplexSparseMatrixTest {
  double[][] realData = { { 1, 2, 0, 0 }, { 5, 0, 7, 0 }, { 0, 10, 11, 0 }, { 0, 0, 15, 0 } };
  double[][] imagData = { { 10, 20, 30, 0 }, { 0, 60, 70, 0 }, { 90, 100, 0, 120 }, { 0, 0, 0, 160 } };
  double[] compressedRealData = { 1, 0, 5, 0, 2, 0, 10, 0, 7, 0, 11, 0, 15, 0 }; // compressed real part interleaved with zero as no imag given

  double[] compressedRealPartOfMixed = { 1, 5, 0, 2, 0, 10, 0, 7, 11, 15, 0, 0 }; // compressed real part of the full data set, zeros added when imag exists and real doesn't
  double[] compressedImagPartOfMixed = { 10, 0, 90, 20, 60, 100, 30, 70, 0, 0, 120, 160 }; // compressed real part of the full data set, zeros added when imag exists and real doesn't  
  double[] compressedMixedData = { 1, 10, 5, 0, 0, 90, 2, 20, 0, 60, 10, 100, 0, 30, 7, 70, 11, 0, 15, 0, 0, 120, 0, 160 };
  int[] compressedColPtr = { 0, 3, 6, 10, 12 };
  int[] compressedRowIdx = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 3, 2, 3 };
  double[][] dataAsFullTransposed = { { 1, 10, 5, 0, 0, 90, 0, 0 }, { 2, 20, 0, 60, 10, 100, 0, 0 }, { 0, 30, 7, 70, 11, 0, 15, 0 }, { 0, 0, 0, 0, 0, 120, 0, 160 } };
  double[][] dataForEqualsTests = { { 1, 2, 0, 0 }, { 3, 0, 4, 0 }, { 0, 5, 6, 0 }, { 0, 0, 7, 0 } };

  OGComplexScalar[][] complexTData = { //
  { new OGComplexScalar(1, 10), new OGComplexScalar(2, 20), new OGComplexScalar(0, 30), new OGComplexScalar(0, 0) }, //
    { new OGComplexScalar(5, 0), new OGComplexScalar(0, 60), new OGComplexScalar(7, 70), new OGComplexScalar(0, 0) }, //
    { new OGComplexScalar(0, 90), new OGComplexScalar(10, 100), new OGComplexScalar(11, 0), new OGComplexScalar(0, 120) }, //
    { new OGComplexScalar(0, 0), new OGComplexScalar(0, 0), new OGComplexScalar(15, 0), new OGComplexScalar(0, 160) }, };

  // sending in null ptr double[][] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrPtrConstructorNullPtrTest() {
    double[][] tmp = null;
    new OGComplexSparseMatrix(tmp);
  }

  // sending in embedded null ptr double[][] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrPtrConstructorOKThenNullPtrTest() {
    double[][] tmp = new double[2][1];
    tmp[0] = new double[] { 1 };
    tmp[1] = null;
    new OGComplexSparseMatrix(tmp);
  }

  // sending in null ptr double[][],double[][] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrPtrDoublePtrPtrConstructorNullPtrFirstArgTest() {
    double[][] tmp1 = null;
    double[][] tmp2 = imagData;
    new OGComplexSparseMatrix(tmp1, tmp2);
  }

  // sending in embedded null ptr double[][],double[][] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrPtrDoublePtrPtrConstructorOKThenNullPtrFirstArgTest() {
    double[][] tmp1 = new double[2][1];
    tmp1[0] = new double[] { 1 };
    tmp1[1] = null;
    double[][] tmp2 = imagData;
    new OGComplexSparseMatrix(tmp1, tmp2);
  }

  // sending in null ptr double[][],double[][] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrPtrDoublePtrPtrConstructorNullPtrSecondArgTest() {
    double[][] tmp2 = null;
    double[][] tmp1 = realData;
    new OGComplexSparseMatrix(tmp1, tmp2);
  }

  // sending in embedded null ptr double[][],double[][] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrPtrDoublePtrPtrConstructorOKThenNullPtrSecondArgTest() {
    double[][] tmp2 = new double[2][1];
    tmp2[0] = new double[] { 1 };
    tmp2[1] = null;
    double[][] tmp1 = realData;
    new OGComplexSparseMatrix(tmp1, tmp2);
  }

  // sending in null ptr pos1 int[], int[], double[], int, int constructor  
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testCSCSingleInterleavedBlockConstructorNullPtrPos1Test() {
    new OGComplexSparseMatrix(null, new int[1], new double[1], 1, 1);
  }

  // sending in null ptr pos2 int[], int[], double[], int, int constructor  
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testCSCSingleInterleavedBlockConstructorNullPtrPos2Test() {
    new OGComplexSparseMatrix(new int[1], null, new double[1], 1, 1);
  }

  // sending in null ptr pos3 int[], int[], double[], int, int constructor  
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testCSCSingleInterleavedBlockConstructorNullPtrPos3Test() {
    new OGComplexSparseMatrix(new int[1], new int[1], null, 1, 1);
  }

  // sending in null ptr pos1 int[], int[], double[], int, int constructor  
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testCSCSeparateBlockConstructorNullPtrPos1Test() {
    new OGComplexSparseMatrix(null, new int[1], new double[1], new double[1], 1, 1);
  }

  // sending in null ptr pos2 int[], int[], double[], double[], int, int constructor  
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testCSCSeparateBlockConstructorNullPtrPos2Test() {
    new OGComplexSparseMatrix(new int[1], null, new double[1], new double[1], 1, 1);
  }

  // sending in null ptr pos3 int[], int[], double[], double[], int, int constructor  
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testCSCSeparateBlockConstructorNullPtrPos3Test() {
    new OGComplexSparseMatrix(new int[1], new int[1], null, new double[1], 1, 1);
  }

  // sending in null ptr pos4 int[], int[], double[], double[], int, int constructor  
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testCSCSeparateBlockConstructorNullPtrPos4Test() {
    new OGComplexSparseMatrix(new int[1], new int[1], new double[1], null, 1, 1);
  }

  // Raggeds

  // sending in ragged[][], double[][] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrPtrConstructorRaggedTest() {
    double[][] tmp = new double[2][];
    tmp[0] = new double[] { 1 };
    tmp[1] = new double[] { 1, 2, 3 };
    new OGComplexSparseMatrix(tmp);
  }

  // sending in ragged[][] double[][],double[][] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrPtrDoublePtrPtrConstructorRaggedFirstArgTest() {
    double[][] tmp1 = new double[2][];
    tmp1[0] = new double[] { 1 };
    tmp1[1] = new double[] { 1, 2, 3 };
    new OGComplexSparseMatrix(tmp1, imagData);
  }

  // sending in ragged[][] double[][],double[][] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrPtrDoublePtrPtrConstructorRaggedSecondArgTest() {
    double[][] tmp2 = new double[2][];
    tmp2[0] = new double[] { 1 };
    tmp2[1] = new double[] { 1, 2, 3 };
    new OGComplexSparseMatrix(realData, tmp2);
  }

  // sending in ragged[][] OGComplexScalar[][] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testOGComplexScalarPtrPtrConstructorRaggedSecondArgTest() {
    OGComplexScalar[][] tmp = new OGComplexScalar[2][];
    tmp[0] = new OGComplexScalar[] { new OGComplexScalar(1) };
    tmp[1] = new OGComplexScalar[] { new OGComplexScalar(1), new OGComplexScalar(2), new OGComplexScalar(3) };
    new OGComplexSparseMatrix(tmp);
  }

  // Zeros
  //sending in all zeros to double[][] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrPtrConstructorAllZerosTest() {
    new OGComplexSparseMatrix(new double[][] { { 0, 0, 0 }, { 0, 0, 0 } });
  }

  //sending in all zeros first arg to double[][], double[][] constructor
  public void testDoublePtrPtrDoublePtrPtrConstructorAllZerosFirstArgTest() {
    new OGComplexSparseMatrix(new double[][] { { 0, 0, 0 }, { 0, 0, 0 } }, new double[][] { { 1, 2, 3 }, { 0, 0, 6 } });
  }

  //sending in all zeros second arg to double[][], double[][] constructor
  public void testDoublePtrPtrDoublePtrPtrConstructorAllZerosSecondArgTest() {
    new OGComplexSparseMatrix(new double[][] { { 1, 2, 3 }, { 0, 0, 6 } }, new double[][] { { 0, 0, 0 }, { 0, 0, 0 } });
  }

  //sending in all zeros both args to double[][], double[][] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrPtrDoublePtrPtrConstructorAllZerosBothArgsTest() {
    new OGComplexSparseMatrix(new double[][] { { 0, 0, 0 }, { 0, 0, 0 } }, new double[][] { { 0, 0, 0 }, { 0, 0, 0 } });
  }

  //sending in all zeros data  int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorAllZerosTest() {
    new OGComplexSparseMatrix(compressedColPtr, compressedRowIdx, new double[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 4, 4);
  }

  //sending in all zeros data  int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrConstructorAllZerosInBothPartsTest() {
    new OGComplexSparseMatrix(compressedColPtr, compressedRowIdx, new double[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, new double[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 4, 4);
  }

  //sending in all zeros data  int[], int[], double[], int, int  constructor
  // Is fine, just trips other branch for coverage purposes
  public void testDoublePtrDoublePtrConstructorAllZerosInRealPartTest() {
    new OGComplexSparseMatrix(compressedColPtr, compressedRowIdx, new double[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, new double[] { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 4, 4);
  }

  // sending in zeros data OGComplexScalar[][] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testComplexPtrPtrConstructorAllZerosTest() {
    OGComplexScalar[][] tmp = new OGComplexScalar[2][];
    tmp[0] = new OGComplexScalar[] { new OGComplexScalar(0), new OGComplexScalar(0), new OGComplexScalar(0) };
    tmp[1] = new OGComplexScalar[] { new OGComplexScalar(0), new OGComplexScalar(0), new OGComplexScalar(0) };
    new OGComplexSparseMatrix(tmp);
  }

  // diff rows and cols
  // sending in diff rows double[][] double[][] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrPtrDoublePtrPtrConstructorRowsAreDifferentTest() {
    double[][] tmp = new double[2][];
    tmp[0] = new double[] { 1, 2, 3 };
    tmp[1] = new double[] { 1, 2, 3 };
    new OGComplexSparseMatrix(realData, tmp);
  }

  // sending in diff cols double[][] double[][] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrPtrDoublePtrPtrConstructorColumnsAreDifferentTest() {
    double[][] tmp = new double[4][];
    tmp[0] = new double[] { 1, 2 };
    tmp[1] = new double[] { 1, 2 };
    tmp[2] = new double[] { 1, 2 };
    tmp[3] = new double[] { 1, 2 };
    new OGComplexSparseMatrix(realData, tmp);
  }

  // bad rows and cols ptrs
  //sending in bad rows int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorBadRowsDataTest() {
    new OGComplexSparseMatrix(compressedColPtr, compressedRowIdx, compressedMixedData, -1, 4);
  }

  //sending in bad cols int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorBadColsDataTest() {
    new OGComplexSparseMatrix(compressedColPtr, compressedRowIdx, compressedMixedData, 4, -1);
  }

  //sending in too many cols int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorDataCountDataTest() {
    new OGComplexSparseMatrix(compressedColPtr, compressedRowIdx, compressedMixedData, 4, 5);
  }

  // sending in bad ColumnPtr  int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorbadColPtrTooLongTest() {
    new OGComplexSparseMatrix(new int[] { 0, 2, 4, 5, 6, 7 }, compressedRowIdx, compressedMixedData, 4, 4);
  }

  // sending in bad ColumnPtr  int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorbadColPtrValuesAreNotIncreasingTest() {
    new OGComplexSparseMatrix(new int[] { 0, 2, 0, 7, 7 }, compressedRowIdx, compressedMixedData, 4, 4);
  }

  // sending in bad ColumnPtr  int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorbadColPtrValuesIsNegativeTest() {
    new OGComplexSparseMatrix(new int[] { -1, 2, 0, 7, 7 }, compressedRowIdx, compressedMixedData, 4, 4);
  }

  // sending in bad rowIdx  int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorbadRowIdxTooHighTest() {
    new OGComplexSparseMatrix(compressedColPtr, new int[] { 0, -1, 2, 0, 1, 9, 0, 1, 2, 3, 2, 3 }, compressedMixedData, 4, 4);
  }

  // sending in bad rowIdx  int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorbadRowIdxTooLowTest() {
    new OGComplexSparseMatrix(compressedColPtr, new int[] { 0, -1, 2, 0, 1, 2, 0, 1, 2, 3, 2, 3 }, compressedMixedData, 4, 4);
  }

  // sending in bad rowIdx  int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorbadRowIdxLengthTest() {
    new OGComplexSparseMatrix(compressedColPtr, new int[] { 0, 1, 0, 2, 1, 1 }, compressedMixedData, 4, 4);
  }

  //sending in bad rows int[], int[], double[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrConstructorBadRowsDataTest() {
    new OGComplexSparseMatrix(compressedColPtr, compressedRowIdx, compressedRealPartOfMixed, compressedImagPartOfMixed, -1, 4);
  }

  //sending in bad cols int[], int[], double[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrConstructorBadColsDataTest() {
    new OGComplexSparseMatrix(compressedColPtr, compressedRowIdx, compressedRealPartOfMixed, compressedImagPartOfMixed, 4, -1);
  }

  //sending in too many cols int[], int[], double[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrConstructorDataCountDataTest() {
    new OGComplexSparseMatrix(compressedColPtr, compressedRowIdx, compressedRealPartOfMixed, compressedImagPartOfMixed, 4, 5);
  }

  // sending in bad ColumnPtr  int[], int[], double[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrConstructorbadColPtrTooLongTest() {
    new OGComplexSparseMatrix(new int[] { 0, 2, 4, 5, 6, 7 }, compressedRowIdx, compressedRealPartOfMixed, compressedImagPartOfMixed, 4, 4);
  }

  // sending in bad ColumnPtr  int[], int[], double[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrConstructorbadColPtrValuesAreNotIncreasingTest() {
    new OGComplexSparseMatrix(new int[] { 0, 2, 0, 7, 7 }, compressedRowIdx, compressedRealPartOfMixed, compressedImagPartOfMixed, 4, 4);
  }

  // sending in bad ColumnPtr  int[], int[], double[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrConstructorbadColPtrValuesIsNegativeTest() {
    new OGComplexSparseMatrix(new int[] { -1, 2, 0, 7, 7 }, compressedRowIdx, compressedRealPartOfMixed, compressedImagPartOfMixed, 4, 4);
  }

  // sending in bad rowIdx  int[], int[], double[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrConstructorbadRowIdxTooHighTest() {
    new OGComplexSparseMatrix(compressedColPtr, new int[] { 0, 1, 0, 2, 1, 6, 3 }, compressedRealPartOfMixed, compressedImagPartOfMixed, 4, 4);
  }

  // sending in bad rowIdx  int[], int[], double[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrConstructorbadRowIdxTooLowTest() {
    new OGComplexSparseMatrix(compressedColPtr, new int[] { 0, -1, 2, 0, 1, 2, 0, 1, 2, 3, 2, 3 }, compressedRealPartOfMixed, compressedImagPartOfMixed, 4, 4);
  }

  // sending in bad rowIdx  int[], int[], double[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrConstructorbadRowIdxLengthTest() {
    new OGComplexSparseMatrix(compressedColPtr, new int[] { 0, 1, 2, 0, 1, 9, 0, 1, 2, 3, 2, 3 }, compressedRealPartOfMixed, compressedImagPartOfMixed, 4, 4);
  }

  // sending in lengths that don't commute, imag is 1 short. int[], int[], double[], double[], int, int  constructor 
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrConstructorRealAndImagDataDoNotCommuteTest() {
    new OGComplexSparseMatrix(compressedColPtr, compressedRowIdx, compressedRealPartOfMixed, new double[] { 10, 0, 90, 20, 60, 100, 30, 70, 0, 0, 120 }, 4, 4);
  }

  // OKs  
  // sending in ok double[][] double[][] constructor
  @Test
  public void testDoublePtrPtrDoublePtrPtrConstructorInternalDataTest() {
    OGComplexSparseMatrix D = new OGComplexSparseMatrix(realData, imagData);
    assertTrue(D.getClass() == OGComplexSparseMatrix.class);
    assertTrue(Arrays.equals(D.getData(), compressedMixedData));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 4);
  }

  // sending in ok double[][] constructor
  @Test
  public void testDoublePtrPtrConstructorInternalDataTest() {
    OGComplexSparseMatrix D = new OGComplexSparseMatrix(realData);
    assertTrue(D.getClass() == OGComplexSparseMatrix.class);
    assertTrue(Arrays.equals(D.getData(), compressedRealData));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 4);
  }

  @Test
  public void testDoublePtrConstructorInternalDataTest() {
    OGComplexSparseMatrix D = new OGComplexSparseMatrix(compressedColPtr, compressedRowIdx, compressedMixedData, 4, 4);
    assertTrue(D.getClass() == OGComplexSparseMatrix.class);
    assertTrue(Arrays.equals(D.getData(), compressedMixedData));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 4);
  }

  @Test
  public void testDoublePtrDoublePtrConstructorInternalDataTest() {
    OGComplexSparseMatrix D = new OGComplexSparseMatrix(compressedColPtr, compressedRowIdx, compressedRealPartOfMixed, compressedImagPartOfMixed, 4, 4);
    assertTrue(D.getClass() == OGComplexSparseMatrix.class);
    assertTrue(Arrays.equals(D.getData(), compressedMixedData));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 4);
  }

  @Test
  public void testDoublePtrDoublePtrConstructorInternalDataZerosInRealPartTest() {
    OGComplexSparseMatrix D = new OGComplexSparseMatrix(compressedColPtr, compressedRowIdx, new double[compressedImagPartOfMixed.length], compressedImagPartOfMixed, 4, 4);
    assertTrue(D.getClass() == OGComplexSparseMatrix.class);
    assertTrue(Arrays.equals(D.getData(), new double[] { 0, 10, 0, 0, 0, 90, 0, 20, 0, 60, 0, 100, 0, 30, 0, 70, 0, 0, 0, 0, 0, 120, 0, 160 }));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 4);
  }

  @Test
  public void testOGComplexScalarPtrPtrConstructorInternalDataTest() {
    OGComplexSparseMatrix D = new OGComplexSparseMatrix(complexTData);
    assertTrue(D.getClass() == OGComplexSparseMatrix.class);
    assertTrue(Arrays.equals(D.getData(), compressedMixedData));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 4);
  }

  // functions
  // test get rows
  @Test
  public void testGetNumRowsTest() {
    OGComplexSparseMatrix D = new OGComplexSparseMatrix(realData, imagData);
    assertTrue(D.getRows() == 4);
  }

  // test get cols
  @Test
  public void testGetNumColumnsTest() {
    OGComplexSparseMatrix D = new OGComplexSparseMatrix(realData, imagData);
    assertTrue(D.getCols() == 4);
  }

  // test get row idx
  @Test
  public void testGetRowIndexTest() {
    OGComplexSparseMatrix D = new OGComplexSparseMatrix(realData, imagData);
    assertTrue(Arrays.equals(D.getRowIdx(), compressedRowIdx));
  }

  // test get col ptr
  @Test
  public void testGetColPtrTest() {
    OGComplexSparseMatrix D = new OGComplexSparseMatrix(realData, imagData);
    assertTrue(Arrays.equals(D.getColPtr(), compressedColPtr));
  }

  @Test
  public void testGetTypeEnum() {
    OGComplexSparseMatrix D = new OGComplexSparseMatrix(realData, imagData);
    assertTrue(D.getType().equals(ExprEnum.OGComplexSparseMatrix));
  }

  @Test
  public void toStringTest() {
    OGComplexSparseMatrix D = new OGComplexSparseMatrix(new double[][] { { 1, -1 } }, new double[][] { { 1, -1 } });
    D.toString();
  }

}
