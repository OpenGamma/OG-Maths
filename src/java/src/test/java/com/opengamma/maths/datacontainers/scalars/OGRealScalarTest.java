/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.maths.datacontainers.scalars;

import static org.testng.Assert.assertTrue;
import static org.testng.AssertJUnit.assertFalse;

import java.util.Arrays;

import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.datacontainers.scalar.OGComplexScalar;
import com.opengamma.maths.datacontainers.scalar.OGRealScalar;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.exceptions.MathsExceptionNullPointer;
import com.opengamma.maths.helpers.FuzzyEquals;

/**
 * Tests the real scalar class
 */
@Test
public class OGRealScalarTest {
  OGRealScalar defaultVal = new OGRealScalar(1.4);
  OGRealScalar same = new OGRealScalar(1.4);
  OGRealDenseMatrix diffclass = new OGRealDenseMatrix(1.4);
  OGRealScalar diffnumber = new OGRealScalar(4.1);
  OGRealScalar withindiffnumber = new OGRealScalar(1.4 + FuzzyEquals.getDefaultTolerance() * 0.9);
  OGComplexScalar diffdomain = new OGComplexScalar(1.4);
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
    assertTrue(defaultVal.getType().equals(ExprEnum.OGRealScalar));
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
    assertFalse(defaultVal.equals(diffclass));
    assertFalse(defaultVal.equals(diffnumber));
  }

  @Test
  public void testFuzzyEquals() {
    assertTrue(defaultVal.fuzzyequals(defaultVal));
    assertTrue(defaultVal.fuzzyequals(same));
    assertTrue(defaultVal.fuzzyequals(withindiffnumber));
    assertFalse(defaultVal.fuzzyequals(diffclass));
    assertFalse(defaultVal.fuzzyequals(diffnumber));
    assertFalse(defaultVal.fuzzyequals(diffdomain));
  }

  @Test
  public void testMathsEquals() {
    assertTrue(defaultVal.mathsequals(defaultVal));
    assertTrue(defaultVal.mathsequals(same));
    assertTrue(defaultVal.mathsequals(withindiffnumber));
    assertTrue(defaultVal.mathsequals(diffclass));
    assertFalse(defaultVal.mathsequals(diffnumber));
    assertTrue(defaultVal.mathsequals(diffdomain));
  }

  @Test
  // to prevent false coverage reporting
  public void testToString() {
    defaultVal.toString();
  }

}
