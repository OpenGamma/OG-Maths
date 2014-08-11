/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.testnodes;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.OGTerminal;
import com.opengamma.maths.datacontainers.lazy.OGExpr;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.datacontainers.scalar.OGRealScalar;
import com.opengamma.maths.exceptions.MathsException;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.exceptions.MathsExceptionNativeComputation;
import com.opengamma.maths.helpers.FuzzyEquals;
import com.opengamma.maths.materialisers.Materialisers;
import com.opengamma.maths.nodes.TIMES;

public class TestTimesMaterialise {

  @DataProvider
  public Object[][] dataContainer() {
    Object[][] obj = new Object[10][2];

    obj[0][0] = new TIMES(new OGRealScalar(2.0), new OGRealScalar(3.0));
    obj[0][1] = new OGRealScalar(2.0 * 3.0);

    obj[1][0] = new TIMES(new TIMES(new OGRealScalar(1.0), new OGRealScalar(2.0)), new OGRealScalar(3.0));
    obj[1][1] = new OGRealScalar(1.0 * 2.0 * 3.0);

    OGNumeric bigScalarTree = new OGRealScalar(0.0);
    double prod = 0.0;
    for (int i = 1; i < 500; i++) {
      if (i % 2 == 0) {
        bigScalarTree = new TIMES(new OGRealScalar(i), bigScalarTree);
      }
      else {
        bigScalarTree = new TIMES(bigScalarTree, new OGRealScalar(i));
      }
      prod *= i;
    }

    obj[2][0] = bigScalarTree;
    obj[2][1] = new OGRealScalar(prod);

    OGNumeric m1 = new OGRealDenseMatrix(new double[][] { { 1.0, 2.0 }, { 3.0, 4.0 } });
    OGNumeric m2 = new OGRealDenseMatrix(new double[][] { { 7.0, 4.5 }, { 6.0, 8.0 } });
    OGNumeric m3 = new OGRealDenseMatrix(new double[][] { { 7.0, 9.0 }, { 18.0, 32.0 } });

    obj[3][0] = new TIMES(m1, m2);
    obj[3][1] = m3;

    OGNumeric m4 = new OGRealDenseMatrix(new double[][] { { 6.0, 5.0 }, { 2.0, 1.0 } });
    OGNumeric m5 = new OGRealDenseMatrix(new double[][] { { 42.0, 45.0 }, { 36.0, 32.0 } });

    obj[4][0] = new TIMES(new TIMES(m1, m2), m4);
    obj[4][1] = m5;

    double[][] baseMatrix = new double[][] { { 1.0, 2.0 }, { 4.0, 5.0 } };
    OGNumeric bigRealMatrixTree = new OGRealDenseMatrix(baseMatrix);
    double[][] realMatrixProd = new double[2][2];
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        realMatrixProd[j][k] = baseMatrix[j][k];
      }
    }
    for (int i = 1; i < 10; i++) {
      if (i % 2 == 0) {
        bigRealMatrixTree = new TIMES(new OGRealDenseMatrix(baseMatrix), bigRealMatrixTree);
      }
      else {
        bigRealMatrixTree = new TIMES(bigRealMatrixTree, new OGRealDenseMatrix(baseMatrix));
      }
      for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
          realMatrixProd[j][k] *= baseMatrix[j][k];
        }
      }
    }
    obj[5][0] = bigRealMatrixTree;
    obj[5][1] = new OGRealDenseMatrix(realMatrixProd);

    OGNumeric m6 = new OGRealDenseMatrix(new double[][] { { 2.0, 4.0 }, { 6.0, 8.0 } });
    
    obj[6][0] = new TIMES(m1, new OGRealScalar(2.0));
    obj[6][1] = m6;

    obj[7][0] = new TIMES(new OGRealScalar(2.0), m1);
    obj[7][1] = m6;

    OGNumeric m7 = new OGRealDenseMatrix(new double[][] { { 2.0 } });

    obj[8][0] = new TIMES(m1, m7);
    obj[8][1] = m6;

    obj[9][0] = new TIMES(m7, m1);
    obj[9][1] = m6;

    return obj;
  };

  @Test(dataProvider = "dataContainer")
  public void materialiseToJDoubleArray(OGNumeric input, OGTerminal expected) {
    double[][] answer = Materialisers.toDoubleArrayOfArrays(input);
    double[][] reference = Materialisers.toDoubleArrayOfArrays(expected);
    if (!FuzzyEquals.ArrayFuzzyEquals(reference, answer)) {
      throw new MathsException("Arrays not equal");
    }
  }

  @Test(dataProvider = "dataContainer")
  public void materialiseToOGTerminal(OGNumeric input, OGTerminal expected) {
    OGTerminal answer = Materialisers.toOGTerminal(input);
    if (!FuzzyEquals.ArrayFuzzyEquals(answer.getData(), expected.getData())) {
      throw new MathsException("Terminals not equal.\n Got: " + answer.toString() + "\nExpected: " + expected.toString());
    }
  }

  @Test(expectedExceptions = MathsExceptionNativeComputation.class)
  public void invalidDimensions() {
    OGNumeric m1 = new OGRealDenseMatrix(new double[][] { { 1.0, 2.0 } });
    OGNumeric m2 = new OGRealDenseMatrix(new double[][] { { 1.0, 2.0, 3.0 } });
    OGNumeric p = new TIMES(m1, m2);
    Materialisers.toOGTerminal(p);
  }

  @Test(dataProvider = "dataContainer", expectedExceptions = MathsExceptionIllegalArgument.class)
  public void outsideArgBound(OGExpr input, OGTerminal expected) {
    OGNumeric n = input.getArg(2);
  }

}
