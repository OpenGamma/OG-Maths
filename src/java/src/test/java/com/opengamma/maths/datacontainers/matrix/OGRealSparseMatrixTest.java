/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.maths.datacontainers.matrix;

import static org.testng.Assert.assertTrue;
import static org.testng.AssertJUnit.assertFalse;

import java.util.Arrays;

import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealSparseMatrix;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.exceptions.MathsExceptionNullPointer;
import com.opengamma.maths.helpers.FuzzyEquals;

/**
 * Tests the OGRealSparseMatrix Class
 */
public class OGRealSparseMatrixTest {

  double[][] data = { { 1, 2, 0, 0 }, { 3, 0, 4, 0 }, { 0, 5, 6, 0 }, { 0, 0, 7, 0 } };
  double[] compressedData = { 1.0, 3.0, 2.0, 5.0, 4.0, 6.0, 7.0 };
  int[] compressedColPtr = { 0, 2, 4, 7, 7 };
  int[] compressedRowIdx = { 0, 1, 0, 2, 1, 2, 3 };

  OGRealDenseMatrix getRow = new OGRealDenseMatrix(new double[] { 0, 5, 6, 0 }, 1, 4);
  OGRealDenseMatrix getCol = new OGRealDenseMatrix(new double[] { 2, 0, 5, 0 }, 4, 1);

  OGRealSparseMatrix defaultVal = new OGRealSparseMatrix(compressedColPtr, compressedRowIdx, compressedData, 4, 4);
  OGRealSparseMatrix same = new OGRealSparseMatrix(compressedColPtr, compressedRowIdx, compressedData, 4, 4);
  OGRealSparseMatrix withindiffnumber = new OGRealSparseMatrix(compressedColPtr, compressedRowIdx, new double[] { 1.0 + FuzzyEquals.getDefaultTolerance() / 2, 3.0, 2.0, 5.0, 4.0, 6.0, 7.0 }, 4, 4);
  OGRealDenseMatrix diffclass = new OGRealDenseMatrix(1);
  OGRealSparseMatrix diffrows = new OGRealSparseMatrix(compressedColPtr, compressedRowIdx, compressedData, 5, 4);
  OGRealSparseMatrix diffcols = new OGRealSparseMatrix(new int[] { 0, 2, 4, 7, 7, 7 }, compressedRowIdx, compressedData, 4, 5);
  OGRealSparseMatrix diffcolptr = new OGRealSparseMatrix(new int[] { 0, 2, 3, 7, 7 }, compressedRowIdx, compressedData, 4, 4); // NOTE: invalid data layout
  OGRealSparseMatrix diffrowidx = new OGRealSparseMatrix(compressedColPtr, new int[] { 0, 1, 1, 2, 1, 2, 3 }, compressedData, 4, 4);
  OGRealSparseMatrix diffnumber = new OGRealSparseMatrix(compressedColPtr, compressedRowIdx, new double[] { 1337, 3.0, 2.0, 5.0, 4.0, 6.0, 7.0 }, 4, 4);
  OGComplexDenseMatrix diffdomain = new OGComplexDenseMatrix(data);

  // sending in null ptr double[][] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrPtrConstructorNullPtrTest() {
    double[][] tmp = null;
    new OGRealSparseMatrix(tmp);
  }

  // sending in embedded null ptr double[][] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrPtrConstructorOKThenNullPtrTest() {
    double[][] tmp = new double[2][1];
    tmp[0] = new double[] { 1 };
    tmp[1] = null;
    new OGRealSparseMatrix(tmp);
  }

  // sending in null ptr pos1 int[], int[], double[], int, int constructor  
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testCSCConstructorNullPtrPos1Test() {
    new OGRealSparseMatrix(null, new int[1], new double[1], 1, 1);
  }

  // sending in null ptr pos1 int[], int[], double[], int, int constructor  
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testCSCConstructorNullPtrPos2Test() {
    new OGRealSparseMatrix(new int[1], null, new double[1], 1, 1);
  }

  // sending in null ptr pos1 int[], int[], double[], int, int constructor  
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testCSCConstructorNullPtrPos3Test() {
    new OGRealSparseMatrix(new int[1], new int[1], null, 1, 1);
  }

  // sending in ragged[][] double[][] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrPtrConstructorRaggedTest() {
    double[][] tmp = new double[2][];
    tmp[0] = new double[] { 1 };
    tmp[1] = new double[] { 1, 2, 3 };
    new OGRealSparseMatrix(tmp);
  }

  //sending in all zeros to double[][] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrPtrConstructorAllZerosTest() {
    new OGRealSparseMatrix(new double[][] { { 0, 0, 0 }, { 0, 0, 0 } });
  }

  // sending in ok double[][] constructor
  @Test
  public void testDoublePtrPtrConstructorInternalDataTest() {
    OGRealSparseMatrix D = new OGRealSparseMatrix(data);
    assertTrue(D.getClass() == OGRealSparseMatrix.class);
    assertTrue(Arrays.equals(D.getData(), compressedData));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 4);
  }

  //sending in bad rows int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorBadRowsDataTest() {
    new OGRealSparseMatrix(compressedColPtr, compressedRowIdx, compressedData, -1, 4);
  }

  //sending in bad cols int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorBadColsDataTest() {
    new OGRealSparseMatrix(compressedColPtr, compressedRowIdx, compressedData, 4, -1);
  }

  //sending in too many cols int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorDataCountDataTest() {
    new OGRealSparseMatrix(compressedColPtr, compressedRowIdx, compressedData, 4, 5);
  }

  // sending in bad ColumnPtr  int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorbadColPtrTooLongTest() {
    new OGRealSparseMatrix(new int[] { 0, 2, 4, 5, 6, 7 }, compressedRowIdx, compressedData, 4, 4);
  }

  // sending in bad ColumnPtr  int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorbadColPtrValuesAreNotIncreasingTest() {
    new OGRealSparseMatrix(new int[] { 0, 2, 0, 7, 7 }, compressedRowIdx, compressedData, 4, 4);
  }

  // sending in bad ColumnPtr  int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorbadColPtrValuesIsNegativeTest() {
    new OGRealSparseMatrix(new int[] { -1, 2, 0, 7, 7 }, compressedRowIdx, compressedData, 4, 4);
  }

  // sending in bad rowIdx  int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorbadRowIdxTooHighTest() {
    new OGRealSparseMatrix(compressedColPtr, new int[] { 0, 1, 0, 2, 1, 6, 3 }, compressedData, 4, 4);
  }

  // sending in bad rowIdx  int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorbadRowIdxTooLowTest() {
    new OGRealSparseMatrix(compressedColPtr, new int[] { 0, 1, 0, 2, 1, -1, 3 }, compressedData, 4, 4);
  }

  // sending in bad rowIdx  int[], int[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorbadRowIdxLengthTest() {
    new OGRealSparseMatrix(compressedColPtr, new int[] { 0, 1, 0, 2, 1, 1 }, compressedData, 4, 4);
  }

  // sending in ok  int[], int[], double[], int, int  constructor
  @Test
  public void testDoublePtrConstructorInternalDataTest() {
    OGRealSparseMatrix D = new OGRealSparseMatrix(compressedColPtr, compressedRowIdx, compressedData, 4, 4);
    assertTrue(D.getClass() == OGRealSparseMatrix.class);
    assertTrue(Arrays.equals(D.getData(), compressedData));
    assertTrue(Arrays.equals(D.getColPtr(), compressedColPtr));
    assertTrue(Arrays.equals(D.getRowIdx(), compressedRowIdx));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 4);
  }

  // test get rows
  @Test
  public void testGetRowsTest() {
    OGRealSparseMatrix D = new OGRealSparseMatrix(data);
    assertTrue(D.getRows() == 4);
  }

  // test get cols
  @Test
  public void testGetColumnsTest() {
    OGRealSparseMatrix D = new OGRealSparseMatrix(data);
    assertTrue(D.getCols() == 4);
  }

  @Test
  public void testGetTypeEnum() {
    OGRealSparseMatrix D = new OGRealSparseMatrix(data);
    assertTrue(D.getType().equals(ExprEnum.OGRealSparseMatrix));
  }

  @Test
  public void toDebugStringTest() {
    OGRealSparseMatrix D = new OGRealSparseMatrix(data);
    D.toDebugString();
  }

  @Test
  public void toStringTest() {
    OGRealSparseMatrix D = new OGRealSparseMatrix(data);
    D.toString();
  }
  
  @Test
  public void testHashCode() {
    assertTrue(defaultVal.hashCode() == same.hashCode());
    assertFalse(defaultVal.hashCode() == diffnumber.hashCode());
  }

  @Test
  public void testEquals() {
    assertTrue(defaultVal.equals(defaultVal));
    assertTrue(defaultVal.equals(same));
    assertFalse(defaultVal.equals(diffrows));
    assertFalse(defaultVal.equals(diffcols));
    assertFalse(defaultVal.equals(diffcolptr));
    assertFalse(defaultVal.equals(diffrowidx));
    assertFalse(defaultVal.equals(diffclass));
    assertFalse(defaultVal.equals(diffnumber));
  }

  @Test
  public void testFuzzyEquals() {
    assertTrue(defaultVal.fuzzyequals(defaultVal));
    assertTrue(defaultVal.fuzzyequals(same));
    assertTrue(defaultVal.fuzzyequals(withindiffnumber));
    assertFalse(defaultVal.fuzzyequals(diffrows));
    assertFalse(defaultVal.fuzzyequals(diffcols));
    assertFalse(defaultVal.fuzzyequals(diffcolptr));
    assertFalse(defaultVal.fuzzyequals(diffrowidx));
    assertFalse(defaultVal.fuzzyequals(diffclass));
    assertFalse(defaultVal.fuzzyequals(diffnumber));
    assertFalse(defaultVal.fuzzyequals(diffdomain));
  }

  @Test
  public void testMathsEquals() {
    assertTrue(defaultVal.mathsequals(defaultVal));
    assertTrue(defaultVal.mathsequals(same));
    assertTrue(defaultVal.mathsequals(withindiffnumber));
    assertFalse(defaultVal.mathsequals(diffclass));
    assertFalse(defaultVal.mathsequals(diffrows));
    assertFalse(defaultVal.mathsequals(diffcols));
    assertFalse(defaultVal.mathsequals(diffcolptr));
    assertFalse(defaultVal.mathsequals(diffrowidx));
    assertFalse(defaultVal.mathsequals(diffnumber));
    assertTrue(defaultVal.mathsequals(diffdomain));
  }

}
