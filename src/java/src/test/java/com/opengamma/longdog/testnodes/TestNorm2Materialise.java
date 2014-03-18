/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.testnodes;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.OGNumeric;
import com.opengamma.longdog.datacontainers.lazy.OGExpr;
import com.opengamma.longdog.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.longdog.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.longdog.datacontainers.scalar.OGComplexScalar;
import com.opengamma.longdog.datacontainers.scalar.OGRealScalar;
import com.opengamma.longdog.exceptions.MathsException;
import com.opengamma.longdog.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.longdog.helpers.FuzzyEquals;
import com.opengamma.longdog.materialisers.Materialisers;
import com.opengamma.longdog.nodes.NORM2;

public class TestNorm2Materialise {

  @DataProvider
  public Object[][] dataContainer() {
    Object[][] obj = new Object[13][2];

    obj[0][0] = new NORM2(new OGRealScalar(1.0));
    obj[0][1] = new OGRealScalar(1.0);

    obj[1][0] = new NORM2(new OGRealScalar(-1.0));
    obj[1][1] = new OGRealScalar(1.0);

    obj[2][0] = new NORM2(new OGRealScalar(0.0));
    obj[2][1] = new OGRealScalar(0.0);

    obj[3][0] = new NORM2(new OGRealDenseMatrix(-1));
    obj[3][1] = new OGRealScalar(1);

    obj[4][0] = new NORM2(new OGRealDenseMatrix(new double[][] { { 1, 2, 3 } }));
    obj[4][1] = new OGRealScalar(3.74165738677394);

    obj[5][0] = new NORM2(new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } }));
    obj[5][1] = new OGRealScalar(25.4624074360364);

    obj[6][0] = new NORM2(new OGComplexScalar(1.0, 0));
    obj[6][1] = new OGRealScalar(1.0);

    obj[7][0] = new NORM2(new OGComplexScalar(0, 1.0));
    obj[7][1] = new OGRealScalar(1.0);

    obj[8][0] = new NORM2(new OGComplexScalar(1.0, 1.0));
    obj[8][1] = new OGRealScalar(Math.sqrt(2.e0));

    obj[9][0] = new NORM2(new OGComplexScalar(-1.0, 1.0));
    obj[9][1] = new OGRealScalar(Math.sqrt(2.e0));

    obj[10][0] = new NORM2(new OGComplexScalar(0.0, 0.0));
    obj[10][1] = new OGRealScalar(0.e0);

    obj[11][0] = new NORM2(new OGComplexDenseMatrix(new double[][] {{1,2,3}},new double[][] {{10,20,30}}));
    obj[11][1] = new OGRealScalar(37.6031913539263);

    obj[12][0] = new NORM2(new OGComplexDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } },new double[][] { { 10, 20, 30 }, { 40, 50, 60 }, { 70, 80, 90 }, { 100, 110, 120 } }));
    obj[12][1] = new OGRealScalar(255.894027746469);

    return obj;
  };

  @Test(dataProvider = "dataContainer")
  public void materialiseToJDoubleArray(OGNumeric input, OGRealScalar expected) {
    double[][] answer = Materialisers.toDoubleArrayOfArrays(input);
    if (!FuzzyEquals.ArrayFuzzyEquals(expected.getData(), answer[0])) {
      throw new MathsException("Arrays not equal. Got: " + answer[0][0] + " Expected: " + expected.getData()[0]);
    }
  }

  @Test(dataProvider = "dataContainer")
  public void materialiseToOGTerminal(OGNumeric input, OGRealScalar expected) {
    OGRealScalar answer = (OGRealScalar) Materialisers.toOGTerminal(input);
    if (answer.getData().length != 1) {
      throw new MathsException("Scalar holding more than one value");
    }
    if (!FuzzyEquals.SingleValueFuzzyEquals(answer.getData()[0], expected.getData()[0])) {
      throw new MathsException("Arrays not equal. Got: " + answer.toString() + " Expected: " + expected.getData()[0]);
    }
  }

  @Test(dataProvider = "dataContainer", expectedExceptions = MathsExceptionIllegalArgument.class)
  public void outsideArgBound(OGExpr input, OGRealScalar expected) {
    OGNumeric n = input.getArg(1);
  }

}
