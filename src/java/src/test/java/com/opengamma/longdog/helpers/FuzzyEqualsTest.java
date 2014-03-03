/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.helpers;

import static org.testng.AssertJUnit.assertFalse;
import static org.testng.AssertJUnit.assertTrue;

import org.testng.annotations.Test;

/**
 * Tests for values being equal allowing for a level of floating point fuzz
 * Based on our nyqwk2 C++ code.
 */
@Test
public class FuzzyEqualsTest {

  double getNaN()
  {
    return Double.longBitsToDouble(0x7FF1010101010101L);
  }

  double getPosInf()
  {
    return Double.longBitsToDouble(0x7FF0000000000000L);
  }

  double getNegInf()
  {
    return Double.longBitsToDouble(0xFFF0000000000000L);
  }

  double getNegZero()
  {
    return Double.longBitsToDouble(8000000000000000L);
  }

  @Test
  public void EqualsTest_SingleValueFuzzyEqualsDouble() {
    double NaN = getNaN();
    double pinf = getPosInf();
    double ninf = getNegInf();
    double neg0 = getNegZero();

    // NaN branch
    assertFalse(FuzzyEquals.SingleValueFuzzyEquals(NaN, NaN));
    assertFalse(FuzzyEquals.SingleValueFuzzyEquals(NaN, 1));
    assertFalse(FuzzyEquals.SingleValueFuzzyEquals(1, NaN));

    // Inf branches
    assertTrue(FuzzyEquals.SingleValueFuzzyEquals(pinf, pinf));
    assertTrue(FuzzyEquals.SingleValueFuzzyEquals(ninf, ninf));
    assertFalse(FuzzyEquals.SingleValueFuzzyEquals(pinf, ninf));
    assertFalse(FuzzyEquals.SingleValueFuzzyEquals(ninf, pinf));
    assertFalse(FuzzyEquals.SingleValueFuzzyEquals(pinf, Double.MAX_VALUE));
    assertFalse(FuzzyEquals.SingleValueFuzzyEquals(ninf, -Double.MAX_VALUE));

    // val 0 branches
    assertTrue(FuzzyEquals.SingleValueFuzzyEquals(0.e0, 0.e0));
    assertTrue(FuzzyEquals.SingleValueFuzzyEquals(0.e0, neg0));
    assertTrue(FuzzyEquals.SingleValueFuzzyEquals(neg0, 0.e0));
    assertTrue(FuzzyEquals.SingleValueFuzzyEquals(neg0, neg0));

    // same value as it trips the return true on "difference less than abs tol" branch
    assertTrue(FuzzyEquals.SingleValueFuzzyEquals(FuzzyEquals.getEps(), 2.e0 * FuzzyEquals.getEps()));

    // same value as it trips the return true on "difference less than relative error" branch
    assertTrue(FuzzyEquals.SingleValueFuzzyEquals(1.e308, 9.99999999999999e0 * 1.e307));

    // fail, just plain different
    assertFalse(FuzzyEquals.SingleValueFuzzyEquals(1.e0, 2.e0));

  }

  @Test
  public void EqualsTest_ArrayFuzzyEqualsDouble() {

    double data[] = { 1.0e0, 2.0e0, 3.0e0, 4.0e0 };
    double same[] = { 1.0e0, 2.0e0, 3.0e0, 4.0e0 };
    double diff[] = { -1.0e0, 2.0e0, 3.0e0, 4.0e0 };
    double lendiff[] = { -1.0e0, 2.0e0, 3.0e0 };

    assertFalse(FuzzyEquals.ArrayFuzzyEquals(data, lendiff));
    assertFalse(FuzzyEquals.ArrayFuzzyEquals(data, diff));
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(data, same));
  }

  @Test
  public void EqualsTest_CheckEPSIsAppropriatelySmall()
  {
    assertTrue(FuzzyEquals.getEps() < 5e-16);
  }

  @Test
  public void EqualsTest_CheckDefaultToleranceAppropriatelySmall()
  {
    assertTrue(FuzzyEquals.getEps() < 10 * 5e-16);
  }

}
