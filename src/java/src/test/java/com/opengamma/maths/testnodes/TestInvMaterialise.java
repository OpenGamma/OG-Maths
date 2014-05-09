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
import com.opengamma.maths.exceptions.MathsExceptionNativeComputation;
import com.opengamma.maths.helpers.FuzzyEquals;
import com.opengamma.maths.materialisers.Materialisers;
import com.opengamma.maths.nodes.INV;
import com.opengamma.maths.nodes.MTIMES;

/**
 * Tests the INV node
 */
public class TestInvMaterialise {

  @DataProvider
  public Object[][] realDataContainer() {
    Object[][] obj = new Object[8][2];

    obj[0][0] = new INV(new OGRealScalar(1.0));
    obj[0][1] = new OGRealScalar(1.0);

    obj[1][0] = new INV(new OGRealScalar(-1.0));
    obj[1][1] = new OGRealScalar(-1.0);

    obj[2][0] = new INV(new OGRealScalar(0.0));
    obj[2][1] = new OGRealScalar(Double.POSITIVE_INFINITY);

    obj[3][0] = new INV(new OGRealScalar(10.0));
    obj[3][1] = new OGRealScalar(0.1);

    obj[4][0] = new INV(new OGRealDenseMatrix(1));
    obj[4][1] = new OGRealScalar(1);

    obj[5][0] = new INV(new OGRealDenseMatrix(-1));
    obj[5][1] = new OGRealScalar(-1);

    obj[6][0] = new INV(new OGRealDenseMatrix(0));
    obj[6][1] = new OGRealScalar(Double.POSITIVE_INFINITY);

    obj[7][0] = new INV(new OGRealDenseMatrix(new double[] { 1, -4, 7, 2, 2, 9, 3, 1, -4 }, 3, 3));
    obj[7][1] = new OGRealDenseMatrix(new double[] { 0.0918918918918919, 0.0486486486486487, 0.2702702702702703, -0.1891891891891892, 0.1351351351351351, -0.0270270270270270, 0.0216216216216216,
      0.0702702702702703, -0.0540540540540541 }, 3, 3);

    return obj;

  };

  @DataProvider
  public Object[][] complexDataContainer() {
    Object[][] obj = new Object[5][2];
    obj[0][0] = new INV(new OGComplexScalar(1.0, 0));
    obj[0][1] = new OGComplexDenseMatrix(1.0);

    obj[1][0] = new INV(new OGComplexScalar(0, 1.0));
    obj[1][1] = new OGComplexScalar(0, -1.0);

    obj[2][0] = new INV(new OGComplexScalar(0, 0));
    obj[2][1] = new OGComplexScalar(Double.POSITIVE_INFINITY, 0);

    obj[3][0] = new INV(new OGComplexScalar(1.0, 1.0));
    obj[3][1] = new OGComplexScalar(0.5, -0.5);

    obj[4][0] = new INV(new OGComplexDenseMatrix(new double[] { 1., 10., -4., -40., 7., 70., 2., 20., 2., 20., 9., 90., 3., 30., 1., 10., 11., 11. }, 3, 3));
    obj[4][1] = new OGComplexDenseMatrix(new double[] { 0.0015906219620334, -0.0048867825577376, 0.0026942738518561, 0.0088704319348774, 0.0009739401444214, -0.0372879941007054, -0.0019412404140251,
      0.0183104604339916, 0.0011167112286758, -0.0147484293321016, -0.0000973940144421, 0.0037287994100705, 0.0000779152115537, -0.0029830395280564, 0.0002532244375496, -0.0096948784661834,
      -0.0001947880288843, 0.0074575988201411 }, 3, 3);

    return obj;
  };

  @DataProvider
  public Object[][] jointdataContainer() {
    Object[][] obj = new Object[13][];
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
    OGComplexDenseMatrix complex = new OGComplexDenseMatrix(new double[] { 1, 10, -4, -40, 7, 70, 2, 20, 2, 20, 9, 90, 3, 30, 1, 10, 11, 11 }, 3, 3);
    Object[][] obj = new Object[2][1];
    obj[0][0] = real;
    obj[1][0] = complex;
    return obj;
  }

  @Test(dataProvider = "reconstrDataContainer")
  public void reconstructionTest(OGTerminal A) {
    OGExpr INV = new INV(A);
    OGExpr AmtimesINVA = new MTIMES(A, INV);
    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[] { 1, 0, 0, 0, 1, 0, 0, 0, 1 }, 3, 3);
    OGTerminal computed = Materialisers.toOGTerminal(AmtimesINVA);
    assertTrue(expected.mathsequals(computed, 1e-14, 1e-14));
  }

  @DataProvider
  public Object[][] singularDataContainer() {
    OGRealDenseMatrix real = new OGRealDenseMatrix(new double[] { 1, 10, 1, 2, 20, 2, 3, 30, 3 }, 3, 3);
    OGComplexDenseMatrix complex = new OGComplexDenseMatrix(new double[] { 1, 10, 10, 100, 1, 10, 2, 20, 20, 200, 2, 20, 3, 30, 30, 300, 3, 30 }, 3, 3);
    Object[][] obj = new Object[2][1];
    obj[0][0] = real;
    obj[1][0] = complex;
    return obj;
  }

  @Test(dataProvider = "singularDataContainer")
  public void warnOnSingularTest(OGTerminal A) {
    // TODO: This is placeholder code to assert a warning is logged on singular
    Materialisers.toOGTerminal(new INV(A));
  }

  @DataProvider
  public Object[][] nonSquareDataContainer() {
    OGRealDenseMatrix real0 = new OGRealDenseMatrix(new double[] { 1, 2, 3, 4, 5, 6 }, 2, 3);
    OGRealDenseMatrix real1 = new OGRealDenseMatrix(new double[] { 1, 2, 3, 4, 5, 6 }, 3, 2);
    OGComplexDenseMatrix complex0 = new OGComplexDenseMatrix(new double[] { 1, 10, 2, 20, 3, 30, 4, 40, 5, 50, 6, 60 }, 2, 3);
    OGComplexDenseMatrix complex1 = new OGComplexDenseMatrix(new double[] { 1, 10, 2, 20, 3, 30, 4, 40, 5, 50, 6, 60 }, 3, 2);
    Object[][] obj = new Object[4][1];
    obj[0][0] = real0;
    obj[1][0] = real1;
    obj[2][0] = complex0;
    obj[3][0] = complex1;
    return obj;
  }

  @Test(dataProvider = "nonSquareDataContainer", expectedExceptions=MathsExceptionNativeComputation.class)
  public void throwOnNonSquareTest(OGTerminal A) {
    Materialisers.toOGTerminal(new INV(A));
  }

}
