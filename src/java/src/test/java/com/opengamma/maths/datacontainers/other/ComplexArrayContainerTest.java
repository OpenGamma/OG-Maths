/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.datacontainers.other;

import static org.testng.Assert.assertTrue;
import static org.testng.Assert.assertFalse;

import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.other.ComplexArrayContainer;
import com.opengamma.maths.exceptions.MathsException;
import com.opengamma.maths.exceptions.MathsExceptionNullPointer;
import com.opengamma.maths.testhelpers.ArraysHelpers;

public class ComplexArrayContainerTest {

  double[][] real = new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } };
  double[][] imag = new double[][] { { 10, 20, 30 }, { 40, 50, 60 }, { 70, 80, 90 }, { 100, 110, 120 } };

  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void NullFirstPtrRealArrTest() {
    new ComplexArrayContainer(null, imag);
  }

  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void NullSecondPtrRealArrTest() {
    new ComplexArrayContainer(new double[][] { { 1 }, null }, imag);
  }

  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void NullFirstPtrImagArrTest() {
    new ComplexArrayContainer(real, null);
  }

  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void NullSecondPtrImagArrTest() {
    new ComplexArrayContainer(real, new double[][] { { 1 }, null });
  }

  @Test(expectedExceptions = MathsException.class)
  public void RaggedRealArrTest() {
    new ComplexArrayContainer(new double[][] { { 1 }, { 2, 3 }, { 4, 5, 6 } }, imag);
  }

  @Test(expectedExceptions = MathsException.class)
  public void RaggedImagArrTest() {
    new ComplexArrayContainer(real, new double[][] { { 1 }, { 2, 3 }, { 4, 5, 6 } });
  }

  @Test(expectedExceptions = MathsException.class)
  public void BadCommuteRowsTest() {
    new ComplexArrayContainer(real, new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } });
  }

  @Test(expectedExceptions = MathsException.class)
  public void BadCommuteColsTest() {
    new ComplexArrayContainer(real, new double[][] { { 1, 2 }, { 3, 4 }, { 5, 6 }, { 7, 8 } });
  }

  @Test
  public void OKCtorTest() {
    new ComplexArrayContainer(real, imag);
  }

  @Test
  public void getRealTest() {
    assertTrue(ArraysHelpers.ArraysEquals(new ComplexArrayContainer(real, imag).getReal(), real));
  }

  @Test
  public void getImagTest() {
    assertTrue(ArraysHelpers.ArraysEquals(new ComplexArrayContainer(real, imag).getImag(), imag));
  }

  @Test
  public void getAnyImagTest() {
    assertTrue(new ComplexArrayContainer(real, imag).anyImaginary());
  }

  @Test
  public void getAnyImagWZeroImagTest() {
    assertFalse(new ComplexArrayContainer(real, new double[4][3]).anyImaginary());
  }

  @Test
  public void getAnyImagWNegZeroImagTest() {
    double[][] tmp = new double[4][3];
    tmp[0][0] = Double.longBitsToDouble(2L << 63);
    assertFalse(new ComplexArrayContainer(real, tmp).anyImaginary());
  }

  @Test
  public void toStringTest() {
    new ComplexArrayContainer(real, imag).toString();
  }
}
