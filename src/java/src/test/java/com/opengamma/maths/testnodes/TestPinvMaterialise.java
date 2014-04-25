/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.testnodes;

import static org.testng.AssertJUnit.assertTrue;

import java.util.Arrays;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.OGTerminal;
import com.opengamma.maths.datacontainers.lazy.OGExpr;
import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.datacontainers.other.ComplexArrayContainer;
import com.opengamma.maths.datacontainers.scalar.OGComplexScalar;
import com.opengamma.maths.datacontainers.scalar.OGRealScalar;
import com.opengamma.maths.exceptions.MathsException;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.helpers.FuzzyEquals;
import com.opengamma.maths.materialisers.Materialisers;
import com.opengamma.maths.nodes.MTIMES;
import com.opengamma.maths.nodes.PINV;

public class TestPinvMaterialise {

  @DataProvider
  public Object[][] realDataContainer() {
    Object[][] obj = new Object[9][2];

    obj[0][0] = new PINV(new OGRealScalar(1.0));
    obj[0][1] = new OGRealScalar(1.0);

    obj[1][0] = new PINV(new OGRealScalar(-1.0));
    obj[1][1] = new OGRealScalar(-1.0);

    obj[2][0] = new PINV(new OGRealScalar(0.0));
    obj[2][1] = new OGRealScalar(0.0);

    obj[3][0] = new PINV(new OGRealScalar(10.0));
    obj[3][1] = new OGRealScalar(0.1);

    obj[4][0] = new PINV(new OGRealDenseMatrix(1));
    obj[4][1] = new OGRealScalar(1);

    obj[5][0] = new PINV(new OGRealDenseMatrix(-1));
    obj[5][1] = new OGRealScalar(-1);

    obj[6][0] = new PINV(new OGRealDenseMatrix(0));
    obj[6][1] = new OGRealScalar(0);

    obj[7][0] = new PINV(new OGRealDenseMatrix(new double[] { 1., -4., 7., -12., 2., 2., 9., 4., 3., 1., 11., 7. }, 4, 3));
    obj[7][1] = new OGRealDenseMatrix(new double[] { 0.0142560142560143, -0.1236907236907242, 0.1100683100683104, -0.0598455598455599, 0.5727155727155739, -0.4266409266409274, 0.0299970299970300,
      0.0800118800118803, 0.0024354024354022, -0.0446985446985447, -0.1545391545391548, 0.1528066528066529 }, 3, 4);

    obj[8][0] = new PINV(new OGRealDenseMatrix(new double[] { 1, 4, 7, 10, 2, 5, 8, 11, 3, 6, 9, 12 }, 4, 3));
    obj[8][1] = new OGRealDenseMatrix(new double[] { -0.4833333333333338, -0.0333333333333326, 0.4166666666666664, -0.2444444444444440, -0.0111111111111107, 0.2222222222222215, -0.0055555555555554,
      0.0111111111111111, 0.0277777777777776, 0.2333333333333329, 0.0333333333333330, -0.1666666666666660 }, 3, 4);
    return obj;

  };

  @DataProvider
  public Object[][] complexDataContainer() {
    Object[][] obj = new Object[6][2];
    obj[0][0] = new PINV(new OGComplexScalar(1.0, 0));
    obj[0][1] = new OGComplexDenseMatrix(1.0);

    obj[1][0] = new PINV(new OGComplexScalar(0, 1.0));
    obj[1][1] = new OGComplexScalar(0, -1.0);

    obj[2][0] = new PINV(new OGComplexScalar(0, 0));
    obj[2][1] = new OGComplexScalar(0, 0);

    obj[3][0] = new PINV(new OGComplexScalar(1.0, 1.0));
    obj[3][1] = new OGComplexScalar(0.5, -0.5);

    obj[4][0] = new PINV(new OGComplexDenseMatrix(new double[] { 1, 10, -4, -40, 7, 70, -12, -120, 2, 20, 2, 20, 9, 90, 4, 40, 3, 30, 1, 10, 11, 110, 7, 70 }, 4, 3));
    obj[4][1] = new OGComplexDenseMatrix(new double[] { 0.0001411486560001, -0.0014114865600014, -0.0012246606306012, 0.0122466063060123, 0.0010897852482011, -0.0108978524820109,
      -0.0005925302955006,
      0.0059253029550059, 0.0056704512150057, -0.0567045121500567, -0.0042241675905042, 0.0422416759050422, 0.0002970002970003, -0.0029700029700030, 0.0007921968318008, -0.0079219683180079,
      0.0000241128954000, -0.0002411289540002, -0.0004425598485004, 0.0044255984850044, -0.0015300906390015, 0.0153009063900153, 0.0015129371565015, -0.0151293715650151 }, 3, 4);

    obj[5][0] = new PINV(new OGComplexDenseMatrix(new double[] { 1, 10, 4, 40, 7, 70, 10, 100, 2, 20, 5, 50, 8, 80, 11, 110, 3, 30, 6, 60, 9, 90, 12, 120 }, 4, 3));
    obj[5][1] = new OGComplexDenseMatrix(new double[] { -0.0047854785478551, 0.0478547854785478, -0.0003300330033002, 0.0033003300330034, 0.0041254125412542, -0.0412541254125413,
      -0.0024202420242024, 0.0242024202420241, -0.0001100110011000, 0.0011001100110011, 0.0022002200220021, -0.0220022002200220, -0.0000550055005500, 0.0005500550055006, 0.0001100110011001,
      -0.0011001100110011, 0.0002750275027502, -0.0027502750275028, 0.0023102310231024, -0.0231023102310231, 0.0003300330033003, -0.0033003300330033, -0.0016501650165016, 0.0165016501650165 }, 3, 4);

    return obj;
  };

  @DataProvider
  public Object[][] jointdataContainer() {
    Object[][] obj = new Object[15][];
    int p = 0;
    for (int k = 0; k < realDataContainer().length; k++) {
      obj[p++] = realDataContainer()[k];
    }
    for (int k = 0; k < complexDataContainer().length; k++) {
      obj[p++] = complexDataContainer()[k];
    }
    return obj;
  }

  @Test(dataProvider = "realDataContainer")
  public void materialiseToJDoubleArray(OGNumeric input, OGTerminal expected) {
    double[][] answer = Materialisers.toDoubleArrayOfArrays(input);
    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toDoubleArrayOfArrays(expected), answer)) {
      throw new MathsException("Arrays not equal. Got: " + new OGRealDenseMatrix(answer).toString() + " Expected: " + expected.toString());
    }
  }

  @Test(dataProvider = "complexDataContainer")
  public void materialiseToComplexArrayContainer(OGNumeric input, OGTerminal expected) {
    ComplexArrayContainer answer = Materialisers.toComplexArrayContainer(input);
    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toComplexArrayContainer(expected).getReal(), answer.getReal())) {
      throw new MathsException("Arrays not equal. Got: " + new OGRealDenseMatrix(answer.getReal()).toString() + " Expected: " + Arrays.toString(expected.getData()));
    }
    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toComplexArrayContainer(expected).getImag(), answer.getImag())) {
      throw new MathsException("Arrays not equal. Got: " + new OGRealDenseMatrix(answer.getImag()).toString() + " Expected: " + Arrays.toString(expected.getData()));
    }
  }

  @Test(dataProvider = "jointdataContainer")
  public void materialiseToOGTerminal(OGNumeric input, OGTerminal expected) {
    OGTerminal answer = Materialisers.toOGTerminal(input);
    if (!expected.mathsequals(answer)) {
      throw new MathsException("Terminals not equal. Got: " + answer.toString() + " Expected: " + expected.toString());
    }
  }

  @Test(dataProvider = "jointdataContainer", expectedExceptions = MathsExceptionIllegalArgument.class)
  public void outsideArgBound(OGExpr input, OGTerminal expected) {
    input.getArg(1);
  }

  @DataProvider
  public Object[][] reconstrDataContainer() {
    OGRealDenseMatrix real = new OGRealDenseMatrix(new double[] { 1, -4, 7, 2, 2, 9, 3, 1, 11 }, 3, 3);
    OGComplexDenseMatrix complex = new OGComplexDenseMatrix(new double[] { 1, 10, -4, -40, 7, 70, 2, 20, 2, 20, 9, 90, 3, 30, 1, 10, 11, 110 }, 3, 3);
    Object[][] obj = new Object[2][1];
    obj[0][0] = real;
    obj[1][0] = complex;
    return obj;
  }

  @Test(dataProvider = "reconstrDataContainer")
  public void reconstructionTest(OGTerminal A) {
    OGExpr pinv = new PINV(A);
    OGExpr AmtimesPinvA = new MTIMES(A, pinv);
    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[] { 1, 0, 0, 0, 1, 0, 0, 0, 1 }, 3, 3);
    OGTerminal computed = Materialisers.toOGTerminal(AmtimesPinvA);
    assertTrue(expected.mathsequals(computed, 1e-14, 1e-14));
  }

}
