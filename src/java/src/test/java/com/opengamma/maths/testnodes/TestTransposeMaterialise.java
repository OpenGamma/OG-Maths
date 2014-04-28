/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.testnodes;

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
import com.opengamma.maths.nodes.TRANSPOSE;

public class TestTransposeMaterialise {

  @DataProvider
  public Object[][] realDataContainer() {
    Object[][] obj = new Object[8][2];

    obj[0][0] = new TRANSPOSE(new OGRealScalar(1.0));
    obj[0][1] = new OGRealScalar(1.0);

    obj[1][0] = new TRANSPOSE(new OGRealScalar(-1.0));
    obj[1][1] = new OGRealScalar(-1.0);

    obj[2][0] = new TRANSPOSE(new OGRealScalar(0.0));
    obj[2][1] = new OGRealScalar(0.0);

    obj[3][0] = new TRANSPOSE(new OGRealDenseMatrix(1));
    obj[3][1] = new OGRealScalar(1);

    obj[4][0] = new TRANSPOSE(new OGRealDenseMatrix(-1));
    obj[4][1] = new OGRealScalar(-1);

    obj[5][0] = new TRANSPOSE(new OGRealDenseMatrix(0));
    obj[5][1] = new OGRealScalar(0);

    obj[6][0] = new TRANSPOSE(new OGRealDenseMatrix(new double[] { 1, 2, 3 }, 1, 3));
    obj[6][1] = new OGRealDenseMatrix(new double[] { 1, 2, 3 }, 3, 1);

    obj[7][0] = new TRANSPOSE(new OGRealDenseMatrix(new double[] { 1., -4., 7., -12., 2., 2., 9., 4., 3., 1., 11., 7. }, 4, 3));
    obj[7][1] = new OGRealDenseMatrix(new double[] { 1, 2, 3, -4, 2, 1, 7, 9, 11, -12, 4, 7 }, 3, 4);
    return obj;
  };

  @DataProvider
  public Object[][] complexDataContainer() {
    Object[][] obj = new Object[6][2];
    obj[0][0] = new TRANSPOSE(new OGComplexScalar(1.0, 0));
    obj[0][1] = new OGComplexDenseMatrix(1.0);

    obj[1][0] = new TRANSPOSE(new OGComplexScalar(0, 1.0));
    obj[1][1] = new OGComplexScalar(0, 1.0);

    obj[2][0] = new TRANSPOSE(new OGComplexScalar(0, 0));
    obj[2][1] = new OGComplexScalar(0, 0);

    obj[3][0] = new TRANSPOSE(new OGComplexScalar(1.0, 1.0));
    obj[3][1] = new OGComplexScalar(1, 1);

    obj[4][0] = new TRANSPOSE(new OGComplexDenseMatrix(new double[] { 1, 10, 2, 20, 3, 30 }, 1, 3));
    obj[4][1] = new OGComplexDenseMatrix(new double[] { 1, 10, 2, 20, 3, 30 }, 3, 1);

    obj[5][0] = new TRANSPOSE(new OGComplexDenseMatrix(new double[] { 1, -10, -4, 40, 7, -70, -12, 120, 2, -20, 2, -20, 9, -90, 4, -40, 3, -30, 1, -10, 11, -110, 7, -70 }, 4, 3));
    obj[5][1] = new OGComplexDenseMatrix(new double[] { 1, -10, 2, -20, 3, -30, -4, 40, 2, -20, 1, -10, 7, -70, 9, -90, 11, -110, -12, 120, 4, -40, 7, -70 }, 3, 4);

    return obj;
  };

  @DataProvider
  public Object[][] jointdataContainer() {
    Object[][] obj = new Object[14][];
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
      throw new MathsException("Terminals not equal. Got: " + answer.toString() + " Expected: " + expected.getData()[0]);
    }
  }

  @Test(dataProvider = "jointdataContainer", expectedExceptions = MathsExceptionIllegalArgument.class)
  public void outsideArgBound(OGExpr input, OGTerminal expected) {
    input.getArg(1);
  }

  
  @Test(dataProvider = "jointdataContainer")
  public void reconstructionTest(OGExpr input, OGTerminal UNUSED) {
    // twice transposing the input should equal input.
    OGTerminal expected = Materialisers.toOGTerminal(input);
    OGTerminal answer = Materialisers.toOGTerminal(new TRANSPOSE(new TRANSPOSE(input)));
    if (!expected.mathsequals(answer)) {
      throw new MathsException("Terminals not equal. Got: " + answer.toString() + " Expected: " + expected.toString());
    }
  }
}
