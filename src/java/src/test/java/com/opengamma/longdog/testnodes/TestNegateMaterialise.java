/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.testnodes;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.OGNumeric;
import com.opengamma.longdog.datacontainers.scalar.OGRealScalar;
import com.opengamma.longdog.exceptions.MathsException;
import com.opengamma.longdog.materialisers.Materialisers;
import com.opengamma.longdog.nodes.NEGATE;
import com.opengamma.longdog.testhelpers.ArraysHelpers;

public class TestNegateMaterialise {

  @DataProvider
  public Object[][] dataContainer() {
    Object[][] obj = new Object[3][2];


    obj[0][0] = new NEGATE(new OGRealScalar(1.0));
    obj[0][1] = new OGRealScalar(-1.0);

    obj[1][0] = new NEGATE(new OGRealScalar(-1.0));
    obj[1][1] = new OGRealScalar(1.0);

    obj[2][0] = new NEGATE(new OGRealScalar(0.0));
    obj[2][1] = new OGRealScalar(-0.0);

    return obj;
  };

  @Test(dataProvider = "dataContainer")
  public void materialiseToJDoubleArray(OGNumeric input, OGRealScalar expected) {
    double[][] answer = Materialisers.toDoubleArrayOfArrays(input);
    if (!ArraysHelpers.ArraysEquals(new double[][] {expected.getData()}, answer)) {
      throw new MathsException("Arrays not equal");
    }
  }

  @Test(dataProvider = "dataContainer")
  public void materialiseToOGTerminal(OGNumeric input, OGRealScalar expected) {
    OGRealScalar answer = (OGRealScalar) Materialisers.toOGTerminal(input);
    if (answer.getData().length != 1) {
      throw new MathsException("Scalar holding more than one value");
    }
    if (answer.getData()[0] != expected.getData()[0]) {
      throw new MathsException("Arrays not equal");
    }
  }

}
