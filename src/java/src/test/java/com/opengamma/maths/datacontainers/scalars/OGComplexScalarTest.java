/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.maths.datacontainers.scalars;

import static org.testng.Assert.assertTrue;

import java.util.Arrays;

import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.scalar.OGComplexScalar;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.exceptions.MathsExceptionNullPointer;

/**
 * Tests complex scalar type
 */
@Test
public class OGComplexScalarTest {
  OGComplexScalar defaultVal = new OGComplexScalar(1.2, 3.4);
  double dataForEqualsTests = 1.6;
  Double NULLbigD = null;
  double[] NULLdptr = null;
  double[] dptrData = new double[] { 1.2, 3.4 };

  @Test
  public void constructFromNumberTest() {
    new OGComplexScalar(1.4);
    new OGComplexScalar(1);
  }

  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void constructFromNullNumberTest() {
    new OGComplexScalar(NULLbigD);
  }

  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void twoArgCtorConstructFromNullNumberFirstTest() {
    new OGComplexScalar(NULLbigD, 1.4);
  }

  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void twoArgCtorConstructFromNullNumberSecondTest() {
    new OGComplexScalar(1.4, NULLbigD);
  }

  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void dblePtrCtorConstructFromNullTest() {
    new OGComplexScalar(NULLdptr);
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void dblePtrCtorConstructFromTooShortTest() {
    new OGComplexScalar(new double[1]);
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void dblePtrCtorConstructFromTooLongTest() {
    new OGComplexScalar(new double[3]);
  }

  @Test
  public void dblePtrCtorConstructOkDataTest() {
    OGComplexScalar res = new OGComplexScalar(dptrData);
    assertTrue(res.getReal() == dptrData[0]);
    assertTrue(res.getImag() == dptrData[1]);
    assertTrue(Arrays.equals(res.getData(), dptrData));
  }

  @Test
  public void getTypeTest() {
    assertTrue(defaultVal.getType().equals(ExprEnum.OGComplexScalar));
  }

  @Test
  public void getCmplxZero() {
    assertTrue(OGComplexScalar.ZERO.getReal() == 0);
    assertTrue(OGComplexScalar.ZERO.getImag() == 0);
  }

  @Test
  public void getCmplxI() {
    assertTrue(OGComplexScalar.I.getReal() == 0);
    assertTrue(OGComplexScalar.I.getImag() == 1);
  }

  @Test
  public void getCmplxNegI() {
    assertTrue(OGComplexScalar.NEGATIVE_I.getReal() == 0);
    assertTrue(OGComplexScalar.NEGATIVE_I.getImag() == -1);
  }

}
