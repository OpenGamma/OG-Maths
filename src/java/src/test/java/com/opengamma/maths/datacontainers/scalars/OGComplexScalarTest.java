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
import com.opengamma.maths.datacontainers.scalar.OGComplexScalar;
import com.opengamma.maths.datacontainers.scalar.OGRealScalar;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.exceptions.MathsExceptionNullPointer;
import com.opengamma.maths.helpers.FuzzyEquals;

/**
 * Tests complex scalar type
 */
@Test
public class OGComplexScalarTest {
  OGComplexScalar defaultVal = new OGComplexScalar(1.2, 3.4);
  OGComplexScalar same = new OGComplexScalar(1.2, 3.4);
  OGComplexDenseMatrix diffclass = new OGComplexDenseMatrix(defaultVal);
  OGComplexScalar diffnumber = new OGComplexScalar(1.2, 5.6);
  OGComplexScalar withindiffnumber = new OGComplexScalar(1.2 + FuzzyEquals.getDefaultTolerance() * 0.9, 3.4);
  OGRealScalar diffdomain = new OGRealScalar(1.4);

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
    assertFalse(defaultVal.mathsequals(diffdomain));
  }

  @Test
  // to prevent false coverage reporting
  public void testToString() {
    defaultVal.toString();
  }

  @Test
  public void testHashCode() {
    assertTrue(defaultVal.hashCode() == same.hashCode());
    assertFalse(defaultVal.hashCode() == diffnumber.hashCode());
  }
}
