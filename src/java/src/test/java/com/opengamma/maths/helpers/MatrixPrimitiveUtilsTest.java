/**
 * Copyright (C) 2011 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.helpers;

import static org.testng.Assert.assertFalse;
import static org.testng.Assert.assertTrue;

import java.util.Arrays;

import org.testng.annotations.Test;

import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.exceptions.MathsExceptionNullPointer;
import com.opengamma.maths.helpers.MatrixPrimitiveUtils;

/**
 * Tests for MatrixPrimitiveUtils methods
 */
public class MatrixPrimitiveUtilsTest {
  double _array[][] = { { 1, 2, 3, 4, 5 }, { 6, 7, 8, 9, 10 }, { 11, 12, 13, 14, 15 } };
  double _ragged[][] = { { 1, 2, 3, 4 }, { 5, 6, 7 }, { 11, 12, 13, 14, 15 } };
  double _square[][] = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
  double _vectorwithzeros[] = { 0, 1, 0, 2, 3, 0, 4, 0, 5, 0, 0, 6 };
  double _arraywithzeros[][] = { { 1, 2, 0, 4, 5 }, { 6, 0, 8, 9, 10 }, { 11, 12, 13, 0, 15 } };

  int _intarray[][] = { { 1, 2, 3, 4, 5 }, { 6, 7, 8, 9, 10 }, { 11, 12, 13, 14, 15 } };
  int _intragged[][] = { { 1, 2, 3, 4 }, { 5, 6, 7 }, { 11, 12, 13, 14, 15 } };
  int _intsquare[][] = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
  int _intvectorwithzeros[] = { 0, 1, 0, 2, 3, 0, 4, 0, 5, 0, 0, 6 };
  int _intarraywithzeros[][] = { { 1, 2, 0, 4, 5 }, { 6, 0, 8, 9, 10 }, { 11, 12, 13, 0, 15 } };

  Object[][] _objragged = { { new Object(), new Object(), new Object() }, { new Object(), new Object() } };
  Object[][] _objarray = { { new Object(), new Object(), new Object() }, { new Object(), new Object(), new Object() } };

  @Test
  public void testIsRagged() {
    assertTrue(MatrixPrimitiveUtils.isRagged(_ragged));
    assertFalse(MatrixPrimitiveUtils.isRagged(_array));
    assertTrue(MatrixPrimitiveUtils.isRagged(_intragged));
    assertFalse(MatrixPrimitiveUtils.isRagged(_intarray));
    assertTrue(MatrixPrimitiveUtils.isRagged(_objragged));
    assertFalse(MatrixPrimitiveUtils.isRagged(_objarray));
  }

  @Test
  public void testIsSquare() {
    assertTrue(MatrixPrimitiveUtils.isSquare(_square));
    assertFalse(MatrixPrimitiveUtils.isSquare(_array));
    assertFalse(MatrixPrimitiveUtils.isSquare(_ragged));
    assertTrue(MatrixPrimitiveUtils.isSquare(_intsquare));
    assertFalse(MatrixPrimitiveUtils.isSquare(_intarray));
    assertFalse(MatrixPrimitiveUtils.isSquare(_intragged));
  }

  @Test
  public void testGetNumberOfElementsInArray() {
    assertTrue(MatrixPrimitiveUtils.getNumberOfElementsInArray(_array) == 15);
    assertTrue(MatrixPrimitiveUtils.getNumberOfElementsInArray(_ragged) == 12);
    assertTrue(MatrixPrimitiveUtils.getNumberOfElementsInArray(_square) == 9);
    assertTrue(MatrixPrimitiveUtils.getNumberOfElementsInArray(_intarray) == 15);
    assertTrue(MatrixPrimitiveUtils.getNumberOfElementsInArray(_intragged) == 12);
    assertTrue(MatrixPrimitiveUtils.getNumberOfElementsInArray(_intsquare) == 9);
  }

  @Test
  public void testNumberOfNonZeroElementsInVector() {
    assertTrue(MatrixPrimitiveUtils.numberOfNonZeroElementsInVector(_vectorwithzeros) == 6);
    assertTrue(MatrixPrimitiveUtils.numberOfNonZeroElementsInVector(_intvectorwithzeros) == 6);
  }

  @Test
  public void testNumberOfNonZeroElementsInMatrix() {
    assertTrue(MatrixPrimitiveUtils.numberOfNonZeroElementsInMatrix(_arraywithzeros) == 12);
    assertTrue(MatrixPrimitiveUtils.numberOfNonZeroElementsInMatrix(_intarraywithzeros) == 12);
  }

  @Test
  public void testArrayHasContiguousNonZeros() {
    double[] data0 = { 0, 0, 0, 0, 0 };
    double[] data1 = { 0, 1, 1, 1, 0 };
    double[] data2 = { 0, 1, 0, 0, 0 };
    double[] data3 = { 1, 0, 0, 0, 0 };
    double[] data4 = { 1, 0, 0, 0, 1 };
    double[] data5 = { 0, 0, 1, 1, 1, 0, 0, 1 };
    assertTrue(MatrixPrimitiveUtils.arrayHasContiguousRowEntries(data0));
    assertTrue(MatrixPrimitiveUtils.arrayHasContiguousRowEntries(data1));
    assertTrue(MatrixPrimitiveUtils.arrayHasContiguousRowEntries(data2));
    assertTrue(MatrixPrimitiveUtils.arrayHasContiguousRowEntries(data3));
    assertFalse(MatrixPrimitiveUtils.arrayHasContiguousRowEntries(data4));
    assertFalse(MatrixPrimitiveUtils.arrayHasContiguousRowEntries(data5));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testRemoveLowerTriangleThrowsOnRagged() {
    double[][] ragged = { { 1, 2 }, { 3, 4, 5 } };
    MatrixPrimitiveUtils.removeLowerTriangle(ragged);
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testRemoveLowerTriangleThrowsOnNotSquare() {
    double[][] notsq = { { 1, 2, 3 }, { 4, 5, 6 } };
    MatrixPrimitiveUtils.removeLowerTriangle(notsq);
  }

  @Test
  public void testRemoveLowerTriangle() {
    double[][] array = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
    double[][] expected = { { 1, 2, 3 }, { 0, 5, 6 }, { 0, 0, 9 } };
    assertTrue(Arrays.deepEquals(expected, MatrixPrimitiveUtils.removeLowerTriangle(array)));
  }

  @Test
  public void testIsUpperTriangular() {
    double[][] UT = { { 1, 2, 3 }, { 0, 4, 5 }, { 0, 0, 6 } };
    double[][] notUT = { { 1, 2, 3 }, { 0, 4, 5 }, { 0, 1, 6 } };
    assertTrue(MatrixPrimitiveUtils.isUpperTriangular(UT));
    assertFalse(MatrixPrimitiveUtils.isUpperTriangular(notUT));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testIsUpperTriangularThrowsOnRagged() {
    double[][] ragged = { { 1, 2 }, { 3, 4, 5 } };
    assertTrue(MatrixPrimitiveUtils.isUpperTriangular(ragged));
  }

  @Test
  public void testCheckIsUpperTriangular() {
    double[][] UT = { { 1, 2, 3 }, { 0, 4, 5 }, { 0, 0, 6 } };
    assertTrue(Arrays.deepEquals(UT, MatrixPrimitiveUtils.checkIsUpperTriangular(UT)));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testCheckIsUpperTriangularExceptions() {
    double[][] notUT = { { 1, 2, 3 }, { 0, 4, 5 }, { 0, 1, 6 } };
    MatrixPrimitiveUtils.checkIsUpperTriangular(notUT);
  }

  @Test
  public void testIsLowerTriangular() {
    double[][] LT = { { 1, 0, 0 }, { 2, 3, 0 }, { 4, 5, 6 } };
    double[][] notLT = { { 1, 1, 0 }, { 2, 3, 0 }, { 4, 5, 6 } };
    assertTrue(MatrixPrimitiveUtils.isLowerTriangular(LT));
    assertFalse(MatrixPrimitiveUtils.isLowerTriangular(notLT));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testIsLowerTriangularThrowsOnRagged() {
    double[][] ragged = { { 1, 2 }, { 3, 4, 5 } };
    MatrixPrimitiveUtils.isLowerTriangular(ragged);
  }

  @Test
  public void testCheckIsLowerTriangular() {
    double[][] LT = { { 1, 0, 0 }, { 2, 3, 0 }, { 4, 5, 6 } };
    assertTrue(Arrays.deepEquals(LT, MatrixPrimitiveUtils.checkIsLowerTriangular(LT)));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testCheckIsLowerTriangularExceptions() {
    double[][] notLT = { { 1, 1, 0 }, { 2, 3, 0 }, { 4, 5, 6 } };
    MatrixPrimitiveUtils.checkIsLowerTriangular(notLT);
  }

  @Test
  public void testCheckIsUpperHessenberg() {
    double[][] UH = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 0, 9, 10, 11 }, { 0, 0, 12, 13 } };
    assertTrue(Arrays.deepEquals(UH, MatrixPrimitiveUtils.checkIsUpperHessenberg(UH)));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testCheckIsUpperHessenbergExceptions() {
    double[][] notUH = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 1, 9, 10, 11 }, { 0, 0, 12, 13 } };
    MatrixPrimitiveUtils.checkIsUpperHessenberg(notUH);
  }

  @Test
  public void testIsUpperHessenberg() {
    double[][] UH = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 0, 9, 10, 11 }, { 0, 0, 12, 13 } };
    assertTrue(MatrixPrimitiveUtils.isUpperHessenberg(UH));
    double[][] notUH = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 1, 9, 10, 11 }, { 0, 0, 12, 13 } };
    assertFalse(MatrixPrimitiveUtils.isUpperHessenberg(notUH));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testIsUpperHessenbergThrowsOnRagged() {
    double[][] ragged = { { 1, 2 }, { 3, 4, 5 } };
    MatrixPrimitiveUtils.isUpperHessenberg(ragged);
  }

  @Test
  public void testCheckIsLowerHessenberg() {
    double[][] LH = { { 1, 2, 0, 0 }, { 3, 4, 5, 0 }, { 6, 7, 8, 9 }, { 10, 11, 12, 13 } };
    assertTrue(Arrays.deepEquals(LH, MatrixPrimitiveUtils.checkIsLowerHessenberg(LH)));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testCheckIsLowerHessenbergExceptions() {
    double[][] notLH = { { 1, 2, 0, 1 }, { 3, 4, 5, 0 }, { 6, 7, 8, 9 }, { 10, 11, 12, 13 } };
    MatrixPrimitiveUtils.checkIsLowerHessenberg(notLH);
  }

  @Test
  public void testIsLowerHessenberg() {
    double[][] LH = { { 1, 2, 0, 0 }, { 3, 4, 5, 0 }, { 6, 7, 8, 9 }, { 10, 11, 12, 13 } };
    double[][] notLH = { { 1, 2, 0, 1 }, { 3, 4, 5, 0 }, { 6, 7, 8, 9 }, { 10, 11, 12, 13 } };
    assertTrue(MatrixPrimitiveUtils.isLowerHessenberg(LH));
    assertFalse(MatrixPrimitiveUtils.isLowerHessenberg(notLH));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testIsLowerHessenbergThrowsOnRagged() {
    double[][] ragged = { { 1, 2 }, { 3, 4, 5 } };
    MatrixPrimitiveUtils.isLowerHessenberg(ragged);
  }

  @Test
  public void testIsTriDiag() {
    double[][] trid = { { 1, 2, 0, 0, 0, 0, 0 }, { 3, 4, 5, 0, 0, 0, 0 }, { 0, 6, 7, 8, 0, 0, 0 }, { 0, 0, 9, 10, 11, 0, 0 }, { 0, 0, 0, 12, 13, 14, 0 }, { 0, 0, 0, 0, 15, 16, 17 },
      { 0, 0, 0, 0, 0, 18, 19 } };
    double[][] ntrid = { { 1, 2, 1, 0, 0, 0, 0 }, { 3, 4, 5, 0, 0, 0, 0 }, { 0, 6, 7, 8, 0, 0, 0 }, { 0, 0, 9, 10, 11, 0, 0 }, { 0, 0, 0, 12, 13, 14, 0 }, { 0, 0, 0, 0, 15, 16, 17 },
      { 0, 0, 0, 0, 0, 18, 19 } };
    double[][] ntrid_LHS_escape = { { 1, 2, 0, 0, 0, 0, 0 }, { 3, 4, 5, 0, 0, 0, 0 }, { -1, 6, 7, 8, 0, 0, 0 }, { 0, 0, 9, 10, 11, 0, 0 }, { 0, 0, 0, 12, 13, 14, 0 }, { 0, 0, 0, 0, 15, 16, 17 },
      { 0, 0, 0, 0, 0, 18, 19 } };
    double[][] ntrid_RHS_escape = { { 1, 2, 0, 0, 0, 0, 0 }, { 3, 4, 5, 0, 0, 0, 0 }, { 0, 6, 7, 8, -1, 0, 0 }, { 0, 0, 9, 10, 11, 0, 0 }, { 0, 0, 0, 12, 13, 14, 0 }, { 0, 0, 0, 0, 15, 16, 17 },
      { 0, 0, 0, 0, 0, 18, 19 } };
    double[][] ntrid_lastrow_escape = { { 1, 2, 0, 0, 0, 0, 0 }, { 3, 4, 5, 0, 0, 0, 0 }, { 0, 6, 7, 8, 0, 0, 0 }, { 0, 0, 9, 10, 11, 0, 0 }, { 0, 0, 0, 12, 13, 14, 0 }, { 0, 0, 0, 0, 15, 16, 17 },
      { 0, 0, 0, 0, -1, 18, 19 } };
    assertTrue(MatrixPrimitiveUtils.isTriDiag(trid));
    assertFalse(MatrixPrimitiveUtils.isTriDiag(ntrid));
    assertFalse(MatrixPrimitiveUtils.isTriDiag(ntrid_LHS_escape));
    assertFalse(MatrixPrimitiveUtils.isTriDiag(ntrid_RHS_escape));
    assertFalse(MatrixPrimitiveUtils.isTriDiag(ntrid_lastrow_escape));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testIsTriDiagThrowsOnRagged() {
    double[][] ragged = { { 1, 2 }, { 3, 4, 5 } };
    MatrixPrimitiveUtils.isTriDiag(ragged);
  }

  @Test
  public void testCheckIsTriDiag() {
    double[][] trid = { { 1, 2, 0, 0, 0, 0, 0 }, { 3, 4, 5, 0, 0, 0, 0 }, { 0, 6, 7, 8, 0, 0, 0 }, { 0, 0, 9, 10, 11, 0, 0 }, { 0, 0, 0, 12, 13, 14, 0 }, { 0, 0, 0, 0, 15, 16, 17 },
      { 0, 0, 0, 0, 0, 18, 19 } };
    assertTrue(Arrays.deepEquals(trid, MatrixPrimitiveUtils.checkIsTriDiag(trid)));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testCheckIsTriDiagExceptions() {
    double[][] notTrid = { { 1, 2, 1, 0, 0, 0, 0 }, { 3, 4, 5, 0, 0, 0, 0 }, { 0, 6, 7, 8, 0, 0, 0 }, { 0, 0, 9, 10, 11, 0, 0 }, { 0, 0, 0, 12, 13, 14, 0 }, { 0, 0, 0, 0, 15, 16, 17 },
      { 0, 0, 0, 0, 0, 18, 19 } };
    MatrixPrimitiveUtils.checkIsTriDiag(notTrid);
  }

  @Test
  public void testIsNDiag() {
    double[][] diag = { { 1, 0, 0, 0 }, { 0, 2, 0, 0 }, { 0, 0, 3, 0 }, { 0, 0, 0, 4 } };
    double[][] ndiag = { { 1, 1, 0, 0 }, { 0, 2, 0, 0 }, { 0, 0, 3, 0 }, { 0, 0, 0, 4 } };
    double[][] trid = { { 1, 2, 0, 0, 0, 0, 0 }, { 3, 4, 5, 0, 0, 0, 0 }, { 0, 6, 7, 8, 0, 0, 0 }, { 0, 0, 9, 10, 11, 0, 0 }, { 0, 0, 0, 12, 13, 14, 0 }, { 0, 0, 0, 0, 15, 16, 17 },
      { 0, 0, 0, 0, 0, 18, 19 } };
    double[][] ntrid = { { 1, 2, 0, 0, 0, 0, 0 }, { 3, 4, 5, 1, 0, 0, 0 }, { 0, 6, 7, 8, 0, 0, 0 }, { 0, 0, 9, 10, 11, 0, 0 }, { 0, 0, 0, 12, 13, 14, 0 }, { 0, 0, 0, 0, 15, 16, 17 },
      { 0, 0, 0, 0, 0, 18, 19 } };
    double[][] pentd = { { 1, 2, 3, 0, 0, 0, 0, 0, 0 }, { 4, 5, 6, 7, 0, 0, 0, 0, 0 }, { 8, 9, 10, 11, 12, 0, 0, 0, 0 }, { 0, 13, 14, 15, 16, 17, 0, 0, 0 }, { 0, 0, 18, 19, 20, 21, 22, 0, 0 },
      { 0, 0, 0, 23, 24, 25, 26, 27, 0 }, { 0, 0, 0, 0, 28, 29, 30, 31, 32 }, { 0, 0, 0, 0, 0, 33, 34, 35, 36 }, { 0, 0, 0, 0, 0, 0, 37, 38, 39 } };
    double[][] npentd = { { 1, 2, 3, 0, 0, 0, 0, 0, 0 }, { 4, 5, 6, 7, 0, 0, 0, 0, 0 }, { 8, 9, 10, 11, 12, 0, 0, 0, 0 }, { 0, 13, 14, 15, 16, 17, 0, 0, 0 }, { 0, 0, 18, 19, 20, 21, 22, 0, 0 },
      { 0, 0, 0, 23, 24, 25, 26, 27, 0 }, { 0, 0, 0, 0, 28, 29, 30, 31, 32 }, { 0, 0, 0, 0, 0, 33, 34, 35, 36 }, { 0, 0, 0, 0, 0, 1, 37, 38, 39 } };
    double[][] trid_tripLHS = { { 1, 2, 0, 0, 0, 0, 0 }, { 3, 4, 5, 0, 0, 0, 0 }, { -1, 6, 7, 8, 0, 0, 0 }, { 0, 0, 9, 10, 11, 0, 0 }, { 0, 0, 0, 12, 13, 14, 0 }, { 0, 0, 0, 0, 15, 16, 17 },
      { 0, 0, 0, 0, 0, 18, 19 } };

    assertTrue(MatrixPrimitiveUtils.isNDiag(diag, 1));
    assertTrue(MatrixPrimitiveUtils.isNDiag(trid, 3));
    assertTrue(MatrixPrimitiveUtils.isNDiag(pentd, 5));
    assertFalse(MatrixPrimitiveUtils.isNDiag(ndiag, 1));
    assertFalse(MatrixPrimitiveUtils.isNDiag(ntrid, 3));
    assertFalse(MatrixPrimitiveUtils.isNDiag(npentd, 5));
    assertFalse(MatrixPrimitiveUtils.isNDiag(trid_tripLHS, 3));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testIsNDiagThrowsEven() {
    double[][] even = { { 1, -1, 0, 0 }, { 0, 2, -1, 0 }, { 0, 0, 3, -1 }, { 0, 0, 0, 4 } };
    MatrixPrimitiveUtils.isNDiag(even, 2);
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testIsNDiagThrowsOnRagged() {
    double[][] ragged = { { 1, 2 }, { 3, 4, 5 } };
    MatrixPrimitiveUtils.isNDiag(ragged, 3);
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testIsNDiagThrowsOnBadBandWidth() {
    double[][] diag = { { 1, 0, 0, 0 }, { 0, 2, 0, 0 }, { 0, 0, 3, 0 }, { 0, 0, 0, 4 } };
    MatrixPrimitiveUtils.isNDiag(diag, 11);
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testIsNDiagThrowsOnZeroBandWidth() {
    double[][] diag = { { 1, 0, 0, 0 }, { 0, 2, 0, 0 }, { 0, 0, 3, 0 }, { 0, 0, 0, 4 } };
    MatrixPrimitiveUtils.isNDiag(diag, 0);
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testIsNDiagPositiveEvenBandwidth() {
    double[][] diag = { { 1, 0, 0, 0 }, { 0, 2, 0, 0 }, { 0, 0, 3, 0 }, { 0, 0, 0, 4 } };
    MatrixPrimitiveUtils.isNDiag(diag, 6); // test positive even bandwidth
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testIsNDiagNegativeOddBandwidth() {
    double[][] diag = { { 1, 0, 0, 0 }, { 0, 2, 0, 0 }, { 0, 0, 3, 0 }, { 0, 0, 0, 2 } };
    MatrixPrimitiveUtils.isNDiag(diag, -1); // test negative odd bandwidth
  }

  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testIsNDiagNotNull() {
    MatrixPrimitiveUtils.isNDiag(null, 1);
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testIsNDiagImpossibleBandwidth() {
    double[][] diag = { { 1, 0, 0, 0 }, { 0, 2, 0, 0 }, { 0, 0, 3, 0 }, { 0, 0, 0, 2 } };
    MatrixPrimitiveUtils.isNDiag(diag, 0);
    MatrixPrimitiveUtils.isNDiag(diag, 27);
  }

  @Test
  public void testCheckIsNDiag() {
    double[][] trid = { { 1, 2, 0, 0, 0, 0, 0 }, { 3, 4, 5, 0, 0, 0, 0 }, { 0, 6, 7, 8, 0, 0, 0 }, { 0, 0, 9, 10, 11, 0, 0 }, { 0, 0, 0, 12, 13, 14, 0 }, { 0, 0, 0, 0, 15, 16, 17 },
      { 0, 0, 0, 0, 0, 18, 19 } };
    assertTrue(Arrays.deepEquals(trid, MatrixPrimitiveUtils.checkIsNDiag(trid, 3)));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testCheckIsNDiagExceptions() {
    double[][] notTrid = { { 1, 2, 1, 0, 0, 0, 0 }, { 3, 4, 5, 0, 0, 0, 0 }, { 0, 6, 7, 8, 0, 0, 0 }, { 0, 0, 9, 10, 11, 0, 0 }, { 0, 0, 0, 12, 13, 14, 0 }, { 0, 0, 0, 0, 15, 16, 17 },
      { 0, 0, 0, 0, 0, 18, 19 } };
    MatrixPrimitiveUtils.checkIsNDiag(notTrid, 3);
  }

  @Test
  public void testIsEven() {
    assertTrue(MatrixPrimitiveUtils.isEven(0));
    assertTrue(MatrixPrimitiveUtils.isEven(-2));
    assertTrue(MatrixPrimitiveUtils.isEven(2));
    assertFalse(MatrixPrimitiveUtils.isEven(-1));
    assertFalse(MatrixPrimitiveUtils.isEven(1));
  }

  @Test
  public void testIsOdd() {
    assertFalse(MatrixPrimitiveUtils.isOdd(0));
    assertFalse(MatrixPrimitiveUtils.isOdd(-2));
    assertFalse(MatrixPrimitiveUtils.isOdd(2));
    assertTrue(MatrixPrimitiveUtils.isOdd(-1));
    assertTrue(MatrixPrimitiveUtils.isOdd(1));
  }

} // class end

