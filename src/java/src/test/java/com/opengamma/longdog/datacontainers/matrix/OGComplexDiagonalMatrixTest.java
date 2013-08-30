/**
 * Copyright (C) 2012 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.longdog.datacontainers.matrix;

import static org.testng.Assert.assertTrue;

import java.util.Arrays;

import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;
import com.opengamma.longdog.datacontainers.scalar.OGComplexScalar;
import com.opengamma.longdog.helpers.DenseMemoryManipulation;
import com.opengamma.longdog.helpers.MathsExceptionIllegalArgument;
import com.opengamma.longdog.helpers.MathsExceptionNullPointer;

/**
 * Tests the {@link OGComplexDiagonalMatrix} Class
 */
public class OGComplexDiagonalMatrixTest {

  double[] data4x3diagdreal = new double[] { 1, 2, 3 };
  double[] data4x3diagdimag = new double[] { 10, 20, 30 };
  double[] interleavedfull4x3 = DenseMemoryManipulation.convertTwoSinglePointersToInterleavedSinglePointer(data4x3diagdreal, data4x3diagdimag);
  double[] interleavedreal4x3 = DenseMemoryManipulation.convertSinglePointerToZeroInterleavedSinglePointer(data4x3diagdreal);
  OGComplexScalar[][] data4x3full = new OGComplexScalar[][] {//
  { new OGComplexScalar(1, 10), new OGComplexScalar(0.00), new OGComplexScalar(0.00) },//
    { new OGComplexScalar(0.00), new OGComplexScalar(2, 20), new OGComplexScalar(0.00) }, //
    { new OGComplexScalar(0.00), new OGComplexScalar(0.00), new OGComplexScalar(3, 30) },//
    { new OGComplexScalar(0.00), new OGComplexScalar(0.00), new OGComplexScalar(0.00) } };

  // sending in null ptr double[] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrConstructorNullPtrTest() {
    double[] tmp = null;
    new OGComplexDiagonalMatrix(tmp);
  }

  // sending in null ptr double[], int, int constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrIntIntConstructorNullPtrTest() {
    double[] tmp = null;
    new OGComplexDiagonalMatrix(tmp, 1, 1);
  }

  // sending in null ptr double[] double[] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrDoublePtrConstructorNullPtrFirstArgTest() {
    double[] tmp = null;
    new OGComplexDiagonalMatrix(tmp, data4x3diagdimag);
  }

  // sending in null ptr double[] double[] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrDoublePtrConstructorNullPtrSecondArgTest() {
    double[] tmp = null;
    new OGComplexDiagonalMatrix(data4x3diagdreal, tmp);
  }

  // sending in null ptr OGComplexScalar[] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testOGComplexScalarPtrConstructorNullPtrTest() {
    OGComplexScalar[] tmp = null;
    new OGComplexDiagonalMatrix(tmp);
  }

  // sending in null ptr double[] double[] int int  constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrDoublePtrIntIntConstructorNullPtrFirstArgTest() {
    double[] tmp = null;
    new OGComplexDiagonalMatrix(tmp, data4x3diagdimag, 4, 3);
  }

  // sending in null ptr double[] double[] int int  constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrDoublePtrIntIntConstructorNullPtrSecondArgTest() {
    double[] tmp = null;
    new OGComplexDiagonalMatrix(data4x3diagdreal, tmp, 4, 3);
  }

  // sending in non commuting args double[] double[]  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrConstructorNonConformantArgTest() {
    new OGComplexDiagonalMatrix(data4x3diagdreal, new double[] { 1, 2 });
  }

  // sending in non commuting args double[] double[] int int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrIntIntConstructorNonConformantArgTest() {
    new OGComplexDiagonalMatrix(data4x3diagdreal, new double[] { 1, 2 }, 4, 3);
  }

  // sending in bad rows double[], int, int constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrIntIntConstructorBadRowsDataTest() {
    new OGComplexDiagonalMatrix(data4x3diagdreal, -1, 3);
  }

  // sending in bad rows double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrIntIntConstructorBadColsDataTest() {
    new OGComplexDiagonalMatrix(data4x3diagdreal, 3, -1);
  }

  // sending in bad rows double[], double[], int, int constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrIntIntConstructorBadRowsDataTest() {
    new OGComplexDiagonalMatrix(data4x3diagdreal, data4x3diagdimag, -1, 3);
  }

  // sending in bad rows double[], double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrIntIntConstructorBadColsDataTest() {
    new OGComplexDiagonalMatrix(data4x3diagdreal, data4x3diagdimag, 3, -1);
  }

  // sending in bad rows double, int, int constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoubleIntIntConstructorBadRowsDataTest() {
    new OGComplexDiagonalMatrix(1, -1, 3);
  }

  // sending in bad cols double, int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoubleIntIntConstructorBadColsDataTest() {
    new OGComplexDiagonalMatrix(1, 3, -1);
  }

  // sending in bad rows double, int, int constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoubleDoubleIntIntConstructorBadRowsDataTest() {
    new OGComplexDiagonalMatrix(1, 2, -1, 3);
  }

  // sending in bad cols double, int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoubleDoubleIntIntConstructorBadColsDataTest() {
    new OGComplexDiagonalMatrix(1, 2, 3, -1);
  }

  // sending in ok OGComplexScalar[] constructor
  @Test
  public void testOGComplexScalarPtrConstructorInternalDataTest() {
    OGComplexScalar[] tmp = new OGComplexScalar[] { new OGComplexScalar(1, 10), new OGComplexScalar(2, 20), new OGComplexScalar(3, 30) };
    new OGComplexDiagonalMatrix(tmp);
  }

  // sending in ok double[] constructor
  @Test
  public void testDoublePtrConstructorInternalDataTest() {
    OGComplexDiagonalMatrix D = new OGComplexDiagonalMatrix(data4x3diagdreal);
    assertTrue(D.getClass() == OGComplexDiagonalMatrix.class);
    assertTrue(Arrays.equals(D.getData(), interleavedreal4x3));
    assertTrue(D.getRows() == 3);
    assertTrue(D.getCols() == 3);
  }

  // sending in ok double[] double[] constructor
  @Test
  public void testDoublePtrDoublePtrConstructorInternalDataTest() {
    OGComplexDiagonalMatrix D = new OGComplexDiagonalMatrix(data4x3diagdreal, data4x3diagdimag);
    assertTrue(D.getClass() == OGComplexDiagonalMatrix.class);
    assertTrue(Arrays.equals(D.getData(), interleavedfull4x3));
    assertTrue(D.getRows() == 3);
    assertTrue(D.getCols() == 3);
  }

  // sending in ok double[] double[] constructor
  @Test
  public void testDoublePtrDoublePtrIntIntConstructorInternalDataTest() {
    OGComplexDiagonalMatrix D = new OGComplexDiagonalMatrix(data4x3diagdreal, data4x3diagdimag, 4, 3);
    assertTrue(D.getClass() == OGComplexDiagonalMatrix.class);
    assertTrue(Arrays.equals(D.getData(), interleavedfull4x3));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 3);
  }

  // sending in ok double[], int, int constructor
  @Test
  public void testDoublePtrIntIntConstructorInternalDataTest() {
    OGComplexDiagonalMatrix D = new OGComplexDiagonalMatrix(data4x3diagdreal, 16, 32);
    assertTrue(D.getClass() == OGComplexDiagonalMatrix.class);
    assertTrue(Arrays.equals(D.getData(), interleavedreal4x3));
    assertTrue(D.getRows() == 16);
    assertTrue(D.getCols() == 32);
  }

  // sending in ok double, int, int  constructor
  @Test
  public void testDoubleIntIntConstructorInternalDataTest() {
    OGComplexDiagonalMatrix D = new OGComplexDiagonalMatrix(3, 13, 37);
    assertTrue(D.getClass() == OGComplexDiagonalMatrix.class);
    assertTrue(Arrays.equals(D.getData(), new double[] { 3 }));
    assertTrue(D.getRows() == 13);
    assertTrue(D.getCols() == 37);
  }

  // sending in ok double constructor
  @Test
  public void testDoubleConstructorInternalDataTest() {
    OGComplexDiagonalMatrix D = new OGComplexDiagonalMatrix(3.d);
    assertTrue(D.getClass() == OGComplexDiagonalMatrix.class);
    assertTrue(Arrays.equals(D.getData(), new double[] { 3 }));
    assertTrue(D.getRows() == 1);
    assertTrue(D.getCols() == 1);
  }

  // sending in ok int constructor
  @Test
  public void testIntConstructorInternalDataTest() {
    OGComplexDiagonalMatrix D = new OGComplexDiagonalMatrix((int) 3);
    assertTrue(D.getClass() == OGComplexDiagonalMatrix.class);
    assertTrue(Arrays.equals(D.getData(), new double[] { 3 }));
    assertTrue(D.getRows() == 1);
    assertTrue(D.getCols() == 1);
  }

  // sending in ok real imag constructor
  @Test
  public void testDoubleDoubleConstructorInternalDataTest() {
    OGComplexDiagonalMatrix D = new OGComplexDiagonalMatrix((double) 3, (double) 4);
    assertTrue(D.getClass() == OGComplexDiagonalMatrix.class);
    assertTrue(Arrays.equals(D.getData(), new double[] { 3, 4 }));
    assertTrue(D.getRows() == 1);
    assertTrue(D.getCols() == 1);
  }

  // test get rows
  @Test
  public void testGetRowsTest() {
    OGComplexDiagonalMatrix D = new OGComplexDiagonalMatrix(data4x3diagdreal, 4, 3);
    assertTrue(D.getRows() == 4);
  }

  // test get cols
  @Test
  public void testGetColumnsTest() {
    OGComplexDiagonalMatrix D = new OGComplexDiagonalMatrix(data4x3diagdreal, 4, 3);
    assertTrue(D.getCols() == 3);
  }

  @Test
  public void testGetTypeEnum() {
    OGComplexDiagonalMatrix D = new OGComplexDiagonalMatrix(data4x3diagdreal, data4x3diagdimag);
    assertTrue(D.getType().equals(ExprTypeEnum.OGComplexDiagonalMatrix));
  }

}
