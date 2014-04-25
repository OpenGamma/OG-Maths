/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths;

import static com.opengamma.maths.DOGMA.C;
import static com.opengamma.maths.DOGMA.D;
import static com.opengamma.maths.DOGMA.ctranspose;
import static com.opengamma.maths.DOGMA.disp;
import static com.opengamma.maths.DOGMA.minus;
import static com.opengamma.maths.DOGMA.mtimes;
import static com.opengamma.maths.DOGMA.norm2;
import static com.opengamma.maths.DOGMA.pinv;
import static com.opengamma.maths.DOGMA.plus;
import static com.opengamma.maths.DOGMA.svd;
import static com.opengamma.maths.DOGMA.toComplexArrayContainer;
import static com.opengamma.maths.DOGMA.toDoubleArrayOfArrays;
import static com.opengamma.maths.DOGMA.toOGTerminal;
import static com.opengamma.maths.DOGMA.transpose;
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

  @Test
  public void PinvTest() {
    OGTerminal mat;
    mat = new OGRealDenseMatrix(10);
    assertTrue(D(1.e0 / 10.e0).mathsequals(toOGTerminal(pinv(mat))));
    mat = new OGRealDenseMatrix(0);
    assertTrue(D(0.e0).mathsequals(toOGTerminal(pinv(mat))));
    mat = new OGRealDenseMatrix(new double[] { 1., -4., 7., -12., 2., 2., 9., 4., 3., 1., 11., 7. }, 4, 3);
    assertTrue(new OGRealDenseMatrix(new double[] { 0.0142560142560143, -0.1236907236907242, 0.1100683100683104, -0.0598455598455599, 0.5727155727155739, -0.4266409266409274, 0.0299970299970300,
      0.0800118800118803, 0.0024354024354022, -0.0446985446985447, -0.1545391545391548, 0.1528066528066529 }, 3, 4).mathsequals(toOGTerminal(pinv(mat))));

    mat = C(10, 20);
    assertTrue(C(0.02, -0.04).mathsequals(toOGTerminal(pinv(mat))));
    mat = C(0, 0);
    assertTrue(C(0.e0).mathsequals(toOGTerminal(pinv(mat))));
    mat = new OGComplexDenseMatrix(new double[] { 1, 10, -4, -40, 7, 70, -12, -120, 2, 20, 2, 20, 9, 90, 4, 40, 3, 30, 1, 10, 11, 110, 7, 70 }, 4, 3);
    assertTrue(new OGComplexDenseMatrix(new double[] { 0.0001411486560001, -0.0014114865600014, -0.0012246606306012, 0.0122466063060123, 0.0010897852482011, -0.0108978524820109, -0.0005925302955006,
      0.0059253029550059, 0.0056704512150057, -0.0567045121500567, -0.0042241675905042, 0.0422416759050422, 0.0002970002970003, -0.0029700029700030, 0.0007921968318008, -0.0079219683180079,
      0.0000241128954000, -0.0002411289540002, -0.0004425598485004, 0.0044255984850044, -0.0015300906390015, 0.0153009063900153, 0.0015129371565015, -0.0151293715650151 }, 3, 4)
        .mathsequals(toOGTerminal(pinv(mat))));
  }

  @Test
  public void TransposeTest() {
    OGTerminal mat;
    mat = new OGRealDenseMatrix(10);
    assertTrue(D(10.e0).mathsequals(toOGTerminal(transpose(mat))));
    mat = new OGRealDenseMatrix(new double[] { 1, 2, 3 }, 1, 3);
    assertTrue(new OGRealDenseMatrix(new double[] { 1, 2, 3 }, 3, 1).mathsequals(toOGTerminal(transpose(mat))));
    mat = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } });
    assertTrue(new OGRealDenseMatrix(new double[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }, 3, 4).mathsequals(toOGTerminal(transpose(mat))));

    mat = new OGComplexScalar(10, 100);
    assertTrue(C(10.e0, 100e0).mathsequals(toOGTerminal(transpose(mat))));
    mat = new OGComplexDenseMatrix(new double[] { 1, 10, 2, 20, 3, 30 }, 1, 3);
    assertTrue(new OGComplexDenseMatrix(new double[] { 1, 10, 2, 20, 3, 30 }, 3, 1).mathsequals(toOGTerminal(transpose(mat))));
    mat = new OGComplexDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } }, new double[][] { { 10, 20, 30 }, { 40, 50, 60 }, { 70, 80, 90 }, { 100, 110, 120 } });
    assertTrue(new OGComplexDenseMatrix(new double[] { 1, 10, 2, 20, 3, 30, 4, 40, 5, 50, 6, 60, 7, 70, 8, 80, 9, 90, 10, 100, 11, 110, 12, 120 }, 3, 4).mathsequals(toOGTerminal(transpose(mat))));
  }

  @Test
  public void CtransposeTest() {
    OGTerminal mat;
    mat = new OGRealDenseMatrix(10);
    assertTrue(D(10.e0).mathsequals(toOGTerminal(ctranspose(mat))));
    mat = new OGRealDenseMatrix(new double[] { 1, 2, 3 }, 1, 3);
    assertTrue(new OGRealDenseMatrix(new double[] { 1, 2, 3 }, 3, 1).mathsequals(toOGTerminal(ctranspose(mat))));
    mat = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } });
    assertTrue(new OGRealDenseMatrix(new double[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }, 3, 4).mathsequals(toOGTerminal(ctranspose(mat))));

    mat = new OGComplexScalar(10, -100);
    assertTrue(C(10.e0, 100e0).mathsequals(toOGTerminal(ctranspose(mat))));
    mat = new OGComplexDenseMatrix(new double[] { 1, -10, 2, 20, 3, -30 }, 1, 3);
    assertTrue(new OGComplexDenseMatrix(new double[] { 1, 10, 2, -20, 3, 30 }, 3, 1).mathsequals(toOGTerminal(ctranspose(mat))));
    mat = new OGComplexDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } }, new double[][] { { -10, 20, -30 }, { 40, -50, 60 }, { -70, 80, -90 }, { 100, -110, 120 } });
    assertTrue(new OGComplexDenseMatrix(new double[] { 1, 10, 2, -20, 3, 30, 4, -40, 5, 50, 6, -60, 7, 70, 8, -80, 9, 90, 10, -100, 11, 110, 12, -120 }, 3, 4).mathsequals(toOGTerminal(ctranspose(mat))));
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
