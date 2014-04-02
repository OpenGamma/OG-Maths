/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths;

import static com.opengamma.maths.DOGMA.C;
import static com.opengamma.maths.DOGMA.D;
import static com.opengamma.maths.DOGMA.disp;
import static com.opengamma.maths.DOGMA.minus;
import static com.opengamma.maths.DOGMA.mtimes;
import static com.opengamma.maths.DOGMA.norm2;
import static com.opengamma.maths.DOGMA.plus;
import static com.opengamma.maths.DOGMA.svd;
import static com.opengamma.maths.DOGMA.toComplexArrayContainer;
import static com.opengamma.maths.DOGMA.toDoubleArrayOfArrays;
import static com.opengamma.maths.DOGMA.toOGTerminal;
import static com.opengamma.maths.DOGMA.uminus;
import static org.testng.AssertJUnit.assertFalse;
import static org.testng.AssertJUnit.assertTrue;

import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.OGTerminal;
import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.datacontainers.other.OGSVDResult;
import com.opengamma.maths.datacontainers.scalar.OGComplexScalar;
import com.opengamma.maths.datacontainers.scalar.OGRealScalar;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.helpers.FuzzyEquals;

/**
 * Test of DOGMA.
 */
public class DOGMATest {

  @Test
  public void DShortCutTest() {
    OGRealScalar rs = new OGRealScalar(10);
    assertTrue(rs.mathsequals(D(10)));
  }

  @Test
  public void CShortCutTest() {
    OGComplexScalar cs;
    cs = new OGComplexScalar(10);
    assertTrue(cs.mathsequals(C(10)));
    cs = new OGComplexScalar(10, 20);
    assertTrue(cs.mathsequals(C(10, 20)));
  }

  @Test
  public void DispTest() {
    OGComplexScalar cs;
    cs = new OGComplexScalar(10);
    disp(cs); // Nothing we can do!
  }

  @Test
  public void Norm2Test() {
    OGTerminal mat = new OGRealDenseMatrix(new double[] { 3, 4 });
    assertTrue(D(5).mathsequals(toOGTerminal(norm2(mat))));
  }

  @Test
  public void UminusTest() {
    OGTerminal mat;
    mat = new OGRealDenseMatrix(10);
    assertTrue(D(-10).mathsequals(toOGTerminal(uminus(mat))));
    mat = new OGRealDenseMatrix(new double[] { 1, 2, 3, 4, 5 });
    assertTrue(new OGRealDenseMatrix(new double[] { -1, -2, -3, -4, -5 }).mathsequals(toOGTerminal(uminus(mat))));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void PlusBadDataTest() {
    plus(D(10));
  }

  @Test
  public void PlusTest() {
    OGTerminal rmat, cmat;
    OGNumeric TenTimes;
    rmat = new OGRealDenseMatrix(10);
    cmat = new OGComplexDenseMatrix(new double[] { 10, -7 }, 1, 1);
    TenTimes = plus(rmat, rmat, rmat, rmat, rmat, rmat, rmat, rmat, rmat, rmat);
    assertTrue(D(100).mathsequals(toOGTerminal(TenTimes)));
    TenTimes = plus(cmat, cmat, cmat, cmat, cmat, cmat, cmat, cmat, cmat, cmat);
    assertTrue(C(100, -70).mathsequals(toOGTerminal(TenTimes)));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void MinusBadDataTest() {
    minus(D(10));
  }

  @Test
  public void MinusTest() {
    OGTerminal rmat, cmat;
    OGNumeric TenTimes;
    rmat = new OGRealDenseMatrix(10);
    cmat = new OGComplexDenseMatrix(new double[] { 10, -7 }, 1, 1);
    TenTimes = minus(rmat, rmat, rmat, rmat, rmat, rmat, rmat, rmat, rmat, rmat);
    assertTrue(D(-80).mathsequals(toOGTerminal(TenTimes)));
    TenTimes = minus(cmat, cmat, cmat, cmat, cmat, cmat, cmat, cmat, cmat, cmat);
    assertTrue(C(-80, 56).mathsequals(toOGTerminal(TenTimes)));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void MtimesBadDataTest() {
    mtimes(D(10));
  }

  @Test
  public void MtimesTest() {
    OGTerminal rmat, cmat;
    OGNumeric TenTimes;
    rmat = new OGRealDenseMatrix(10);
    cmat = new OGComplexDenseMatrix(new double[] { 10, -7 }, 1, 1);
    TenTimes = mtimes(rmat, rmat, rmat, rmat, rmat, rmat, rmat, rmat, rmat, rmat);
    assertTrue(D(1e10).mathsequals(toOGTerminal(TenTimes)));
    TenTimes = mtimes(cmat, cmat, cmat, cmat, cmat, cmat, cmat, cmat, cmat, cmat);
    assertTrue(C(72306229251e0, 12853399300e0).mathsequals(toOGTerminal(TenTimes)));
  }

  @Test
  public void SVDTest() {
    OGTerminal mat = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } });
    OGSVDResult res = svd(mat);
    assertTrue(mat.mathsequals(toOGTerminal(mtimes(res.getU(), res.getS(), res.getVT())), 1e-14, 1e-14));
  }

  @Test
  public void toDoubleArrayOfArraysTest() {
    double[][] data = new double[][] { { 1, 2 }, { 3, 4 } };
    OGTerminal mat = new OGRealDenseMatrix(data);
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(data, toDoubleArrayOfArrays(mat)));
  }

  @Test
  public void toComplexArrayContainerTest() {
    double[][] rdata = new double[][] { { 1, 2 }, { 3, 4 } };
    double[][] cdata = new double[][] { { 10, 20 }, { 30, 40 } };

    OGTerminal rmat = new OGRealDenseMatrix(rdata);
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(rdata, toComplexArrayContainer(rmat).getReal()));
    assertFalse(toComplexArrayContainer(rmat).anyImaginary());

    OGTerminal cmat = new OGComplexDenseMatrix(rdata, cdata);
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(rdata, toComplexArrayContainer(cmat).getReal()));
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(cdata, toComplexArrayContainer(cmat).getImag()));
    assertTrue(toComplexArrayContainer(cmat).anyImaginary());
  }

  @Test
  public void toOGTerminalTest() {
    double[][] data = new double[][] { { 1, 2 }, { 3, 4 } };
    OGTerminal mat = new OGRealDenseMatrix(data);
    OGTerminal expected = new OGRealDenseMatrix(data);
    assertTrue(expected.mathsequals(toOGTerminal(mat)));
  }

}
