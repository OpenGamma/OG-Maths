/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths;

import static com.opengamma.maths.DOGMA.C;
import static com.opengamma.maths.DOGMA.D;
import static com.opengamma.maths.DOGMA.acos;
import static com.opengamma.maths.DOGMA.asinh;
import static com.opengamma.maths.DOGMA.atan;
import static com.opengamma.maths.DOGMA.cos;
import static com.opengamma.maths.DOGMA.ctranspose;
import static com.opengamma.maths.DOGMA.disp;
import static com.opengamma.maths.DOGMA.exp;
import static com.opengamma.maths.DOGMA.inv;
import static com.opengamma.maths.DOGMA.lu;
import static com.opengamma.maths.DOGMA.minus;
import static com.opengamma.maths.DOGMA.mldivide;
import static com.opengamma.maths.DOGMA.mtimes;
import static com.opengamma.maths.DOGMA.norm2;
import static com.opengamma.maths.DOGMA.pinv;
import static com.opengamma.maths.DOGMA.plus;
import static com.opengamma.maths.DOGMA.qr;
import static com.opengamma.maths.DOGMA.rdivide;
import static com.opengamma.maths.DOGMA.sin;
import static com.opengamma.maths.DOGMA.sinh;
import static com.opengamma.maths.DOGMA.sum;
import static com.opengamma.maths.DOGMA.svd;
import static com.opengamma.maths.DOGMA.tan;
import static com.opengamma.maths.DOGMA.tanh;
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
import com.opengamma.maths.datacontainers.other.OGLUResult;
import com.opengamma.maths.datacontainers.other.OGQRResult;
import com.opengamma.maths.datacontainers.other.OGSVDResult;
import com.opengamma.maths.datacontainers.scalar.OGComplexScalar;
import com.opengamma.maths.datacontainers.scalar.OGRealScalar;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.exceptions.MathsExceptionNativeComputation;
import com.opengamma.maths.helpers.FuzzyEquals;

/**
 * Test of DOGMA.
 */
@Test
public class DOGMATest {

  // Type from Number Tests
  @Test
  public void DTest() {
    OGRealScalar rs = new OGRealScalar(10);
    assertTrue(rs.mathsequals(D(10)));
  }

  @Test
  public void CTest() {
    OGComplexScalar cs;
    cs = new OGComplexScalar(10);
    assertTrue(cs.mathsequals(C(10)));
    cs = new OGComplexScalar(10, 20);
    assertTrue(cs.mathsequals(C(10, 20)));
  }

  // unary node tests
  @Test
  public void AcosTest() {
    OGTerminal mat;
    mat = new OGRealDenseMatrix(new double[] { 0.5, -0.5 });
    assertTrue(new OGRealDenseMatrix(new double[][] { { 1.0471975511965979, 2.0943951023931957 } }).mathsequals(toOGTerminal(acos(mat)), 1e-14, 1e-14));
    mat = new OGComplexDenseMatrix(new double[] { 3, 6, 4, 8 }, 1, 2);
    assertTrue(new OGComplexDenseMatrix(new double[] { 1.1115489233324776, -2.5998241937784723, 1.1096346915425495, -2.8860395049475405 }, 1, 2).mathsequals(toOGTerminal(acos(mat)), 1e-14, 1e-14));
  }

  @Test
  public void AsinhTest() {
    OGTerminal mat;
    mat = new OGRealDenseMatrix(new double[] { 0.5, -0.5 });
    assertTrue(new OGRealDenseMatrix(new double[][] { { 0.481211825059603, -0.481211825059603 } }).mathsequals(toOGTerminal(asinh(mat)), 1e-14, 1e-14));
    mat = new OGComplexDenseMatrix(new double[] { 3, 6, 4, 8 }, 1, 2);
    assertTrue(new OGComplexDenseMatrix(new double[] { 2.59315859709080, 1.10265963233185, 2.88228969537837, 1.10463461994294 }, 1, 2).mathsequals(toOGTerminal(asinh(mat)), 1e-14, 1e-14));
  }

  @Test
  public void AtanTest() {
    OGTerminal mat;
    mat = new OGRealDenseMatrix(new double[] { 0.5, -0.5 });
    assertTrue(new OGRealDenseMatrix(new double[][] { { 0.463647609000806, -0.463647609000806 } }).mathsequals(toOGTerminal(atan(mat)), 1e-14, 1e-14));
    mat = new OGComplexDenseMatrix(new double[] { 3, 6, 4, 8 }, 1, 2);
    assertTrue(new OGComplexDenseMatrix(new double[] { 1.5030324698021462, 0.1335206214825645, 1.5203354344612496, 0.1000809271519365 }, 1, 2).mathsequals(toOGTerminal(atan(mat)), 1e-14, 1e-14));
  }

  @Test
  public void CosTest() {
    OGTerminal mat;
    mat = new OGRealDenseMatrix(new double[] { 3, 4 });
    assertTrue(new OGRealDenseMatrix(new double[][] { { -0.9899924966004454, -0.6536436208636119 } }).mathsequals(toOGTerminal(cos(mat))));
    mat = new OGComplexDenseMatrix(new double[] { 3, 6, 4, 8 }, 1, 2);
    assertTrue(new OGComplexDenseMatrix(new double[] { -199.6969662082171055, -28.4657623938750675, -974.2421957826329617, 1127.9980945611623611 }, 1, 2).mathsequals(toOGTerminal(cos(mat))));
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
    assertTrue(new OGComplexDenseMatrix(new double[] { 1, 10, 2, -20, 3, 30, 4, -40, 5, 50, 6, -60, 7, 70, 8, -80, 9, 90, 10, -100, 11, 110, 12, -120 }, 3, 4)
        .mathsequals(toOGTerminal(ctranspose(mat))));
  }

  @Test
  public void ExpTest() {
    OGTerminal mat;
    mat = new OGRealDenseMatrix(new double[] { 0.5, -0.5 });
    assertTrue(new OGRealDenseMatrix(new double[][] { { 1.648721270700128, 0.606530659712633 } }).mathsequals(toOGTerminal(exp(mat)), 1e-14, 1e-14));
    mat = new OGComplexDenseMatrix(new double[] { 3, 6, 4, 8 }, 1, 2);
    assertTrue(new OGComplexDenseMatrix(new double[] { 19.2855357450636120, -5.6122103059854025, -7.9440326757102406, 54.0171299856719145 }, 1, 2).mathsequals(toOGTerminal(exp(mat)), 1e-14, 1e-14));
  }

  @Test
  public void InvTest() {
    OGTerminal mat;
    mat = new OGRealDenseMatrix(10);
    assertTrue(D(1.e0 / 10.e0).mathsequals(toOGTerminal(inv(mat))));
    mat = new OGRealDenseMatrix(0);
    assertTrue(D(Double.POSITIVE_INFINITY).mathsequals(toOGTerminal(inv(mat))));

    mat = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { -4, -5, 6 }, { -7, 8, 9 } });
    assertTrue(new OGRealDenseMatrix(new double[][] { { 0.3039215686274509, -0.0196078431372549, -0.0882352941176471 }, { 0.0196078431372549, -0.0980392156862745, 0.0588235294117647 },
      { 0.2189542483660130, 0.0718954248366013, -0.0098039215686275 } }).mathsequals(toOGTerminal(inv(mat))));

    mat = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { -4, -5, 6 } });
    boolean caught = false;
    try {
      toOGTerminal(inv(mat));
    } catch (MathsExceptionNativeComputation e) {
      caught = true;
    }
    assert (caught == true);

    mat = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 1, 2, 3 }, { -4, -5, 6 } });
    // TODO: assert warn raised on singular
    toOGTerminal(inv(mat));

    mat = C(10, 20);
    assertTrue(C(0.02, -0.04).mathsequals(toOGTerminal(inv(mat))));
    mat = C(0, 0);
    assertTrue(C(Double.POSITIVE_INFINITY).mathsequals(toOGTerminal(inv(mat))));

    mat = new OGComplexDenseMatrix(new double[] { 1., 10., -4., -40., 7., 70., 2., 20., 2., 20., 9., 90., 3., 30., 1., 10., 11., 11. }, 3, 3);
    assertTrue(new OGComplexDenseMatrix(new double[] { 0.0015906219620334, -0.0048867825577376, 0.0026942738518561, 0.0088704319348774, 0.0009739401444214, -0.0372879941007054, -0.0019412404140251,
      0.0183104604339916, 0.0011167112286758, -0.0147484293321016, -0.0000973940144421, 0.0037287994100705, 0.0000779152115537, -0.0029830395280564, 0.0002532244375496, -0.0096948784661834,
      -0.0001947880288843, 0.0074575988201411 }, 3, 3).mathsequals(toOGTerminal(inv(mat))));

    mat = new OGComplexDenseMatrix(new double[][] { { 1, 2, 3 }, { 1, 2, 3 }, { -4, -5, 6 } }, new double[][] { { 10, 20, 30 }, { 10, 20, 30 }, { -40, -50, 60 } });
    // TODO: assert warn raised on singular
    toOGTerminal(inv(mat));
  }

  @Test
  public void Norm2Test() {
    OGTerminal mat = new OGRealDenseMatrix(new double[] { 3, 4 });
    assertTrue(D(5).mathsequals(toOGTerminal(norm2(mat))));
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
  public void SinTest() {
    OGTerminal mat;
    mat = new OGRealDenseMatrix(new double[] { 3, 4 });
    assertTrue(new OGRealDenseMatrix(new double[][] { { 0.141120008059867, -0.756802495307928 } }).mathsequals(toOGTerminal(sin(mat))));
    mat = new OGComplexDenseMatrix(new double[] { 3, 6, 4, 8 }, 1, 2);
    assertTrue(new OGComplexDenseMatrix(new double[] { 28.466112195402, -199.694512262161, -1127.998348440116, -974.241976509626 }, 1, 2).mathsequals(toOGTerminal(sin(mat)), 1e-14, 1e-14));
  }

  @Test
  public void SinhTest() {
    OGTerminal mat;
    mat = new OGRealDenseMatrix(new double[] { 3, 4 });
    assertTrue(new OGRealDenseMatrix(new double[][] { { 10.0178749274099, 27.2899171971277 } }).mathsequals(toOGTerminal(sinh(mat))));
    mat = new OGComplexDenseMatrix(new double[] { 3, 6, 4, 8 }, 1, 2);
    assertTrue(new OGComplexDenseMatrix(new double[] { 9.61886584067868, -2.81306079224864, -3.97068387481635, 27.01762535702433 }, 1, 2).mathsequals(toOGTerminal(sinh(mat)), 1e-14, 1e-14));
  }

  @Test
  public void SumTest() {
    OGTerminal mat;
    mat = new OGRealDenseMatrix(new double[][] { {1, 2, 3 }, {4, 5, 6 }, {7, 8, 9 }, {10, 11, 12 } });
    assertTrue(new OGRealDenseMatrix(new double[] {22,26,30}).mathsequals(toOGTerminal(sum(mat))));
    assertTrue(new OGRealDenseMatrix(new double[][] { {6 }, {15 }, {24 }, {33 } }).mathsequals(toOGTerminal(sum(mat, 2))));

    // regression check default summation is sum on dimension 1
    assertTrue(toOGTerminal(sum(mat)).mathsequals(toOGTerminal(sum(mat, 1))));

    mat = new OGComplexDenseMatrix(new double[][] { {1, 2, 3 }, {4, 5, 6 }, {7, 8, 9 }, {10, 11, 12 } }, new double[][] { {10, 8, 6 }, {4, 2, 0 }, {-2, -4, -6 }, {-8, -10, -12 } });
    assertTrue(new OGComplexDenseMatrix(new double[][] {{22, 26, 30 } }, new double[][] {{4, -4, -12 } }).mathsequals(toOGTerminal(sum(mat))));
    assertTrue(new OGComplexDenseMatrix(new double[][] { {6 }, {15 }, {24 }, {33 } }, new double[][] { {24 }, {6 }, {-12 }, {-30 } }).mathsequals(toOGTerminal(sum(mat, 2))));

    // regression check default summation is sum on dimension 1
    assertTrue(toOGTerminal(sum(mat)).mathsequals(toOGTerminal(sum(mat, 1))));

    // check exception is thrown on provision of invalid dimension
    boolean caught = false;
    try {
      toOGTerminal(sum(mat, 3));
    } catch (MathsExceptionIllegalArgument ex) {
      caught = true;
    }
    assertTrue(caught);
  }

  @Test
  public void TanTest() {
    OGTerminal mat;
    mat = new OGRealDenseMatrix(new double[] { 3, 4 });
    assertTrue(new OGRealDenseMatrix(new double[][] { { -0.142546543074278, 1.157821282349577 } }).mathsequals(toOGTerminal(tan(mat))));
    mat = new OGComplexDenseMatrix(new double[] { 3, 6, 4, 8 }, 1, 2);
    assertTrue(new OGComplexDenseMatrix(new double[] { -0.0000034335357991, 0.9999882010834399, 0.0000002226752136, 1.0000000327477192 }, 1, 2).mathsequals(toOGTerminal(tan(mat)), 1e-14, 1e-14));
  }

  @Test
  public void TanhTest() {
    OGTerminal mat;
    mat = new OGRealDenseMatrix(new double[] { 3, 4 });
    assertTrue(new OGRealDenseMatrix(new double[][] { { 0.995054753686730, 0.999329299739067 } }).mathsequals(toOGTerminal(tanh(mat))));
    mat = new OGComplexDenseMatrix(new double[] { 3, 6, 4, 8 }, 1, 2);
    assertTrue(new OGComplexDenseMatrix(new double[] { 0.9958218066674143, -0.0026489645969457, 1.0006427057390959, -0.0001932857742051 }, 1, 2).mathsequals(toOGTerminal(tanh(mat)), 1e-14, 1e-14));
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
  public void UminusTest() {
    OGTerminal mat;
    mat = new OGRealDenseMatrix(10);
    assertTrue(D(-10).mathsequals(toOGTerminal(uminus(mat))));
    mat = new OGRealDenseMatrix(new double[] { 1, 2, 3, 4, 5 });
    assertTrue(new OGRealDenseMatrix(new double[] { -1, -2, -3, -4, -5 }).mathsequals(toOGTerminal(uminus(mat))));
  }

  // binary nodes tests

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
  public void PlusBadDataTest() {
    plus(D(10));
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
  public void MinusBadDataTest() {
    minus(D(10));
  }

  @Test
  public void MldivideTest() {
    OGTerminal rmat, cmat;
    rmat = new OGRealDenseMatrix(new double[][] { { 10., 2., 3. }, { 4., 50., 6. }, { 7., 8., 9. } });
    cmat = new OGComplexDenseMatrix(new double[][] { { 1 }, { 2 }, { 3 } }, new double[][] { { 10 }, { 20 }, { 30 } });
    OGTerminal expected = new OGComplexDenseMatrix(new double[][] { { 0.0000000000000000 }, { 0.0000000000000000 }, { 0.3333333333333333 } }, new double[][] { { 0.0000000000000000 },
      { 0.0000000000000000 }, { 3.3333333333333335 } });
    assertTrue(expected.mathsequals(toOGTerminal(mldivide(rmat, cmat))));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void MldivideBadDataTest() {
    mldivide(D(10));
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

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void MtimesBadDataTest() {
    mtimes(D(10));
  }

  @Test
  public void RdivideTest() {
    OGTerminal rmat, cmat;
    OGNumeric TenTimes;
    rmat = new OGRealDenseMatrix(10);
    cmat = new OGComplexDenseMatrix(new double[] { 10, -7 }, 1, 1);
    TenTimes = rdivide(rmat, rmat, rmat, rmat, rmat, rmat, rmat, rmat, rmat, rmat);
    assertTrue(D(1e-8).mathsequals(toOGTerminal(TenTimes)));
    TenTimes = rdivide(cmat, cmat, cmat, cmat, cmat, cmat, cmat, cmat, cmat, cmat);
    assertTrue(C(0.0000000003500837, -0.0000000019984415).mathsequals(toOGTerminal(TenTimes)));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void RdivideBadDataTest() {
    rdivide(D(10));
  }

  @Test
  public void TimesTest() {
    OGTerminal rmat, cmat;
    OGNumeric TenTimes;
    rmat = new OGRealDenseMatrix(10);
    cmat = new OGComplexDenseMatrix(new double[] { 10, -7 }, 1, 1);
    TenTimes = mtimes(rmat, rmat, rmat, rmat, rmat, rmat, rmat, rmat, rmat, rmat);
    assertTrue(D(1e10).mathsequals(toOGTerminal(TenTimes)));
    TenTimes = mtimes(cmat, cmat, cmat, cmat, cmat, cmat, cmat, cmat, cmat, cmat);
    assertTrue(C(72306229251e0, 12853399300e0).mathsequals(toOGTerminal(TenTimes)));
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void TimesBadDataTest() {
    mtimes(D(10));
  }

  // variadic results test

  @Test
  public void LUTest() {
    OGTerminal mat = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, -5, 6 }, { 7, 8, -9 }, { -10, 11, -12 }, { -13, -14, -15 } });
    OGLUResult res = lu(mat);
    assertTrue(mat.mathsequals(toOGTerminal(mtimes(res.getL(), res.getU())), 1e-14, 1e-14));
  }

  @Test
  public void QRTest() {
    OGTerminal mat = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, -5, 6 }, { 7, 8, -9 }, { -10, 11, -12 }, { -13, -14, -15 } });
    OGQRResult res = qr(mat);
    assertTrue(mat.mathsequals(toOGTerminal(mtimes(res.getQ(), res.getR())), 1e-14, 1e-14));
  }

  @Test
  public void SVDTest() {
    OGTerminal mat = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } });
    OGSVDResult res = svd(mat);
    assertTrue(mat.mathsequals(toOGTerminal(mtimes(res.getU(), res.getS(), res.getVT())), 1e-14, 1e-14));
  }

  // materialiser tests

  @Test
  public void DispTest() {
    OGComplexScalar cs;
    cs = new OGComplexScalar(10);
    disp(cs); // Nothing we can do!
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
