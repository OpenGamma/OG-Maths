/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.maths.helpers;

import static org.testng.Assert.assertTrue;

import java.util.Arrays;

import org.testng.annotations.Test;

import com.opengamma.maths.exceptions.MathsException;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.exceptions.MathsExceptionNullPointer;
import com.opengamma.maths.helpers.DenseMemoryManipulation;

/**
 * Tests the DenseMemoryManipulation class
 */
@Test
public class DenseMemoryManipulationTest {
  private static double[][] DDragged = new double[2][];
  private static double[][] DDdata1 = new double[2][];
  private static double[][] DDdata2 = new double[2][];
  private static double[] DNULL = null;
  private static double[] Ddata1 = new double[] { 1, 2, 3, 4 };
  private static double[] Ddata2 = new double[] { 10, 20, 30, 40 };
  private static double[] DComplex = new double[] { 1, 10 };
  static {
    DDragged[0] = new double[1];
    DDragged[1] = new double[2];
    DDdata1[0] = new double[] { 1, 2, 3, 4 };
    DDdata1[1] = new double[] { 5, 6, 7, 8 };
    DDdata2[0] = new double[] { 10, 20, 30, 40 };
    DDdata2[1] = new double[] { 50, 60, 70, 80 };
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void convertRowMajorDoublePointerToColumnMajorSinglePointerRaggedInTest() {
    DenseMemoryManipulation.convertRowMajorDoublePointerToColumnMajorSinglePointer(DDragged);
  }

  @Test
  public void convertRowMajorDoublePointerToColumnMajorSinglePointerOKDataTest() {
    double[] result = DenseMemoryManipulation.convertRowMajorDoublePointerToColumnMajorSinglePointer(DDdata1);
    assertTrue(Arrays.equals(result, new double[] { 1, 5, 2, 6, 3, 7, 4, 8 }));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void convertRowMajorDoublePointerToColumnMajorZeroInterleavedSinglePointerRaggedInTest() {
    DenseMemoryManipulation.convertRowMajorDoublePointerToColumnMajorZeroInterleavedSinglePointer(DDragged);
  }

  @Test
  public void convertRowMajorDoublePointerToColumnMajorZeroInterleavedSinglePointerOKDataTest() {
    double[] result = DenseMemoryManipulation.convertRowMajorDoublePointerToColumnMajorZeroInterleavedSinglePointer(DDdata1);
    assertTrue(Arrays.equals(result, new double[] { 1, 0, 5, 0, 2, 0, 6, 0, 3, 0, 7, 0, 4, 0, 8, 0 }));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void convertTwoRowMajorDoublePointerToColumnMajorInterleavedSinglePointerRaggedFirstTest() {
    DenseMemoryManipulation.convertTwoRowMajorDoublePointerToColumnMajorInterleavedSinglePointer(DDragged, DDdata2);
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void convertTwoRowMajorDoublePointerToColumnMajorInterleavedSinglePointerRaggedSecondTest() {
    DenseMemoryManipulation.convertTwoRowMajorDoublePointerToColumnMajorInterleavedSinglePointer(DDdata1, DDragged);
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void convertTwoRowMajorDoublePointerToColumnMajorInterleavedSinglePointerNonCommuteRowsTest() {
    DenseMemoryManipulation.convertTwoRowMajorDoublePointerToColumnMajorInterleavedSinglePointer(new double[1][4], DDdata2);
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void convertTwoRowMajorDoublePointerToColumnMajorInterleavedSinglePointerNonCommuteColsTest() {
    DenseMemoryManipulation.convertTwoRowMajorDoublePointerToColumnMajorInterleavedSinglePointer(DDdata1, new double[2][3]);
  }

  @Test
  public void convertTwoRowMajorDoublePointerToColumnMajorInterleavedSinglePointerOKDataTest() {
    double[] result = DenseMemoryManipulation.convertTwoRowMajorDoublePointerToColumnMajorInterleavedSinglePointer(DDdata1, DDdata2);
    assertTrue(Arrays.equals(result, new double[] { 1, 10, 5, 50, 2, 20, 6, 60, 3, 30, 7, 70, 4, 40, 8, 80 }));
  }

  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void convertSinglePointerToZeroInterleavedSinglePointerNullInTest() {
    DenseMemoryManipulation.convertSinglePointerToZeroInterleavedSinglePointer(DNULL);
  }

  @Test
  public void convertSinglePointerToZeroInterleavedSinglePointerOKDataTest() {
    double[] result = DenseMemoryManipulation.convertSinglePointerToZeroInterleavedSinglePointer(Ddata1);
    assertTrue(Arrays.equals(result, new double[] { 1, 0, 2, 0, 3, 0, 4, 0 }));
  }

  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void convertTwoSinglePointersToInterleavedSinglePointerNullFirstTest() {
    DenseMemoryManipulation.convertTwoSinglePointersToInterleavedSinglePointer(DNULL, Ddata2);
  }

  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void convertTwoSinglePointersToInterleavedSinglePointerNullSecondTest() {
    DenseMemoryManipulation.convertTwoSinglePointersToInterleavedSinglePointer(Ddata1, DNULL);
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void convertTwoSinglePointersToInterleavedSinglePointerNonCommuteTest() {
    DenseMemoryManipulation.convertTwoSinglePointersToInterleavedSinglePointer(Ddata1, new double[2]);
  }

  @Test
  public void convertTwoSinglePointersToInterleavedSinglePointerOKDataTest() {
    double[] result = DenseMemoryManipulation.convertTwoSinglePointersToInterleavedSinglePointer(Ddata1, Ddata2);
    assertTrue(Arrays.equals(result, new double[] { 1, 10, 2, 20, 3, 30, 4, 40 }));
  }

  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void fillArrayWithInterleavedComplexValueNullFirstTest() {
    DenseMemoryManipulation.fillArrayWithInterleavedComplexValue(DNULL, DComplex);
  }

  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void fillArrayWithInterleavedComplexValueNullSecondTest() {
    DenseMemoryManipulation.fillArrayWithInterleavedComplexValue(DComplex, DNULL);
  }

  @Test(expectedExceptions = MathsException.class)
  public void fillArrayWithInterleavedComplexValueOddLengthTest() {
    DenseMemoryManipulation.fillArrayWithInterleavedComplexValue(new double[3], DComplex);
  }

  @Test(expectedExceptions = MathsException.class)
  public void fillArrayWithInterleavedComplexValueNotSingleValueComplexTest() {
    DenseMemoryManipulation.fillArrayWithInterleavedComplexValue(new double[4], new double[3]);
  }

  @Test
  public void fillArrayWithInterleavedComplexValueOKDataTest() {
    double[] result = new double[10];
    DenseMemoryManipulation.fillArrayWithInterleavedComplexValue(result, DComplex);
    assertTrue(Arrays.equals(result, new double[] { 1, 10, 1, 10, 1, 10, 1, 10, 1, 10 }));
  }

}
