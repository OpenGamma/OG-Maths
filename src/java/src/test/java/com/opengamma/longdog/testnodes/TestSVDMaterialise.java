/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.testnodes;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.OGNumeric;
import com.opengamma.longdog.datacontainers.lazy.OGExpr;
import com.opengamma.longdog.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.longdog.datacontainers.scalar.OGRealScalar;
import com.opengamma.longdog.exceptions.MathsException;
import com.opengamma.longdog.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.longdog.helpers.FuzzyEquals;
import com.opengamma.longdog.materialisers.Materialisers;
import com.opengamma.longdog.nodes.SVD;

public class TestSVDMaterialise {

  @DataProvider
  public Object[][] dataContainer() {
    Object[][] obj = new Object[1][4];

    obj[0][0] = new SVD(new OGRealDenseMatrix(new double[][] { { 1, 2 },  { 3, 4 },  { 5, 6 } }));
    obj[0][1] = new OGRealScalar(1.0);
    obj[0][2] = new OGRealScalar(2.0);
    obj[0][3] = new OGRealScalar(3.0);

    return obj;
  };

  @Test(dataProvider = "dataContainer")
  public void materialiseToJDoubleArray(SVD input, OGRealScalar expectedU, OGRealScalar expectedS, OGRealScalar expectedV) {
    double[][] U = Materialisers.toDoubleArrayOfArrays(input.getU());
    double[][] S = Materialisers.toDoubleArrayOfArrays(input.getS());
    double[][] V = Materialisers.toDoubleArrayOfArrays(input.getV());

    if (!FuzzyEquals.ArrayFuzzyEquals(expectedU.getData(), U[0])) {
      throw new MathsException("U not equal. Got: " + U[0][0] + " Expected: " + expectedU.getData()[0]);
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(expectedS.getData(), S[0])) {
      throw new MathsException("S not equal. Got: " + S[0][0] + " Expected: " + expectedS.getData()[0]);
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(expectedV.getData(), V[0])) {
      throw new MathsException("V not equal. Got: " + V[0][0] + " Expected: " + expectedV.getData()[0]);
    }
  }

  @Test(dataProvider = "dataContainer")
  public void materialiseToOGTerminal(SVD input, OGRealScalar expectedU, OGRealScalar expectedS, OGRealScalar expectedV) {
    OGRealScalar U = (OGRealScalar) Materialisers.toOGTerminal(input.getU());
    OGRealScalar S = (OGRealScalar) Materialisers.toOGTerminal(input.getS());
    OGRealScalar V = (OGRealScalar) Materialisers.toOGTerminal(input.getV());

    if (U.getData().length != 1) {
      throw new MathsException("Scalar holding more than one value");
    }
    if (!FuzzyEquals.SingleValueFuzzyEquals(U.getData()[0], expectedU.getData()[0])) {
      throw new MathsException("U not equal. Got: " + U.toString() + " Expected: " + expectedU.getData()[0]);
    }

    if (S.getData().length != 1) {
      throw new MathsException("Scalar holding more than one value");
    }
    if (!FuzzyEquals.SingleValueFuzzyEquals(S.getData()[0], expectedS.getData()[0])) {
      throw new MathsException("S not equal. Got: " + S.toString() + " Expected: " + expectedS.getData()[0]);
    }

    if (V.getData().length != 1) {
      throw new MathsException("Scalar holding more than one value");
    }
    if (!FuzzyEquals.SingleValueFuzzyEquals(V.getData()[0], expectedV.getData()[0])) {
      throw new MathsException("Arrays not equal. Got: " + V.toString() + " Expected: " + expectedV.getData()[0]);
    }
  }

  @Test(dataProvider = "dataContainer", expectedExceptions = MathsExceptionIllegalArgument.class)
  public void outsideArgBound(OGExpr input, OGRealScalar expectedU, OGRealScalar expectedS, OGRealScalar expectedV) {
    OGNumeric n = input.getArg(3);
  }

}
