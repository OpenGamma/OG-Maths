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
import com.opengamma.maths.exceptions.MathsException;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.helpers.FuzzyEquals;
import com.opengamma.maths.materialisers.Materialisers;
import com.opengamma.maths.nodes.SUMROWS;

/**
 * Tests the sumrows() works as expected. 
 */
@Test
public class TestSumrowsMaterialise {

  @DataProvider
  public Object[][] realDataContainer() {
    Object[][] obj = new Object[1][2];
    obj[0][0] = new SUMROWS(new OGRealDenseMatrix(new double[][] { {1, 2, 3 }, {4, 5, 6 }, {7, 8, 9 }, {10, 11, 12 } }));
    obj[0][1] = new OGRealDenseMatrix(new double[][] { {6 }, {15 }, {24 }, {33 } });
    return obj;
  };

  @DataProvider
  public Object[][] complexDataContainer() {
    Object[][] obj = new Object[1][2];
    obj[0][0] = new SUMROWS(new OGComplexDenseMatrix(new double[][] { {1, 2, 3 }, {4, 5, 6 }, {7, 8, 9 }, {10, 11, 12 } }, new double[][] { {10, 8, 6 }, {4, 2, 0 }, {-2, -4, -6 }, {-8, -10, -12 } }));
    obj[0][1] = new OGComplexDenseMatrix(new double[][] { {6 }, {15 }, {24 }, {33 } }, new double[][] { {24 }, {6 }, {-12 }, {-30 } });
    return obj;
  };

  @DataProvider
  public Object[][] jointdataContainer() {
    Object[][] obj = new Object[2][];
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

}
