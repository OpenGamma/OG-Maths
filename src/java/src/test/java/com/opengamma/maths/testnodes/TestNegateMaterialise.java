/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.testnodes;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.lazy.OGExpr;
import com.opengamma.maths.datacontainers.scalar.OGRealScalar;
import com.opengamma.maths.exceptions.MathsException;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.materialisers.Materialisers;
import com.opengamma.maths.nodes.NEGATE;
import com.opengamma.maths.testhelpers.ArraysHelpers;

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

  @Test(dataProvider = "dataContainer", expectedExceptions = MathsExceptionIllegalArgument.class)
  public void outsideArgBound(OGExpr input, OGRealScalar expected) {
    OGNumeric n = input.getArg(1);
  }
}
