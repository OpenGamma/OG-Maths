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
import com.opengamma.longdog.datacontainers.scalar.OGRealScalar;
import com.opengamma.longdog.exceptions.MathsException;
import com.opengamma.longdog.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.longdog.materialisers.Materialisers;
import com.opengamma.longdog.nodes.PLUS;
import com.opengamma.longdog.testhelpers.ArraysHelpers;

public class TestPlusMaterialise {

  @DataProvider
  public Object[][] dataContainer() {
    Object[][] obj = new Object[3][2];


    obj[0][0] = new PLUS(new OGRealScalar(2.0), new OGRealScalar(3.0));
    obj[0][1] = new OGRealScalar(2.0 + 3.0);

    obj[1][0] = new PLUS(new PLUS(new OGRealScalar(1.0), new OGRealScalar(2.0)), new OGRealScalar(3.0));
    obj[1][1] = new OGRealScalar(1.0 + 2.0 + 3.0);

    OGNumeric bigtree = new OGRealScalar(0.0);
    double sum = 0.0;
    for (int i = 1; i < 500; i++) {
      if (i%2 == 0) {
        bigtree = new PLUS(new OGRealScalar(i), bigtree);
      }
      else {
        bigtree = new PLUS(bigtree, new OGRealScalar(i));
      }
      sum += i;
    }

    obj[2][0] = bigtree;
    obj[2][1] = new OGRealScalar(sum);

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
    if (answer.getData()[0] != expected.getData()[0]) {
      throw new MathsException("Arrays not equal");
    }
  }

  @Test(dataProvider = "dataContainer", expectedExceptions = MathsExceptionIllegalArgument.class)
  public void outsideArgBound(OGExpr input, OGRealScalar expected) {
    OGNumeric n = input.getArg(2);
  }

}
