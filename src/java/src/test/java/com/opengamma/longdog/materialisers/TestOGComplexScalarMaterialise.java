/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.materialisers;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.other.ComplexArrayContainer;
import com.opengamma.longdog.datacontainers.scalar.OGComplexScalar;
import com.opengamma.longdog.exceptions.MathsException;
import com.opengamma.longdog.testhelpers.ArraysHelpers;

public class TestOGComplexScalarMaterialise {

  @DataProvider
  public Object[][] dataContainer() {
    Object[][] obj = new Object[1][3];

    obj[0][0] = new double[] { 1.d, 10.d };
    obj[0][1] = new double[][] { { 1 } };
    obj[0][2] = new double[][] { { 10 } };

    return obj;
  };

  @Test(dataProvider = "dataContainer")
  public void materialiseToJDoubleArray(double[] input, double[][] expectedReal, double[][] expectedImag) {
    OGComplexScalar tmp = new OGComplexScalar(input);
    ComplexArrayContainer answer = Materialisers.toComplexArrayContainer(tmp);
    if (!ArraysHelpers.ArraysEquals(expectedReal, answer.getReal())) {
      throw new MathsException("REAL: Arrays not equal");
    }
    if (!ArraysHelpers.ArraysEquals(expectedImag, answer.getImag())) {
      throw new MathsException("IMAG: Arrays not equal");
    }
  }

  @Test(dataProvider = "dataContainer")
  public void materialiseToOGTerminal(double[] input, double[][] expectedReal, double[][] expectedImag) {
    OGComplexScalar tmp = new OGComplexScalar(input);
    OGComplexScalar answer = (OGComplexScalar) Materialisers.toOGTerminal(tmp);
    if (tmp.getReal() != answer.getReal()) {
      throw new MathsException("REAL part not equal");
    }
    if (tmp.getImag() != answer.getImag()) {
      throw new MathsException("IMAG part not equal");
    }
  }
}
