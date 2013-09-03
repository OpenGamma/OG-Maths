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
import com.opengamma.longdog.datacontainers.scalar.OGRealScalar;
import com.opengamma.longdog.helpers.MathsExceptionIllegalArgument;
import com.opengamma.longdog.helpers.MathsExceptionNullPointer;

/**
 * Tests the real scalar class
 */
@Test
public class OGRealScalarTest {
  OGRealScalar defaultVal = new OGRealScalar(1.4);
  Double NULL = null;

  @Test
  public void constructFromNumberTest() {
    new OGRealScalar(1.4);
    new OGRealScalar(1);
  }

  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void constructFromNullNumberTest() {
    new OGRealScalar(NULL);
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void constructFromIllegalNumberTest() {
    Number illegalNum = new Number() {

      private static final long serialVersionUID = 1L;

      @Override
      public long longValue() {
        return 0;
      }

      @Override
      public int intValue() {
        return 0;
      }

      @Override
      public float floatValue() {
        return 0;
      }

      @Override
      public double doubleValue() {
        throw new MathsExceptionIllegalArgument();
      }
    };
    new OGRealScalar(illegalNum);
  }

  @Test
  public void getDataTest() {
    assertTrue(Arrays.equals(defaultVal.getData(), new double[] { 1.4 }));
  }

  @Test
  public void testGetTypeEnum() {
    assertTrue(defaultVal.getType().equals(ExprTypeEnum.OGRealScalar));
  }

  @Test
  public void testToString() {
    defaultVal.toString();
  }

}
