/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.materialisers;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.scalar.OGRealScalar;
import com.opengamma.longdog.exceptions.MathsException;
import com.opengamma.longdog.testhelpers.ArraysHelpers;

public class TestOGRealScalarMaterialise {

  @DataProvider
  public Object[][] dataContainer() {
    Object[][] obj = new Object[1][2];
    
    
    obj[0][0] = 10.d;
    obj[0][1] = new double[][] {{10}};
   
    return obj;
  };

  @Test(dataProvider = "dataContainer")
  public void materialiseToJDoubleArray(double input, double[][] expected) {
    OGRealScalar tmp = new OGRealScalar(input);
    double[][] answer = Materialisers.toDoubleArrayOfArrays(tmp);
    if (!ArraysHelpers.ArraysEquals(expected, answer)) {
      throw new MathsException("Arrays not equal");
    }
  }

  @Test(dataProvider = "dataContainer")
  public void materialiseToOGTerminal(double input, double[][] expected) {
    OGRealScalar tmp = new OGRealScalar(input);
    OGRealScalar answer = (OGRealScalar) Materialisers.toOGTerminal(tmp);
    if (answer.getData()[0] != expected[0][0]) {
      throw new MathsException("Arrays not equal");
    }
  }

}
