/**
 * Copyright (C) 2012 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.longdog.datacontainers.scalars;

import static org.testng.Assert.assertTrue;

import java.util.Arrays;

import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;
import com.opengamma.longdog.datacontainers.scalar.OGComplexScalar;
import com.opengamma.longdog.helpers.MathsExceptionIllegalArgument;
import com.opengamma.longdog.helpers.MathsExceptionNullPointer;

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
    assertTrue(defaultVal.getType().equals(ExprTypeEnum.OGComplexScalar));
  }

}
