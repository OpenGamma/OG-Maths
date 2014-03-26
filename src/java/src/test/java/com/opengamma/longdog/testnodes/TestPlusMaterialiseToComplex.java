/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.testnodes;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.OGNumeric;
import com.opengamma.longdog.datacontainers.OGTerminal;
import com.opengamma.longdog.datacontainers.lazy.OGExpr;
import com.opengamma.longdog.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.longdog.datacontainers.other.ComplexArrayContainer;
import com.opengamma.longdog.datacontainers.scalar.OGComplexScalar;
import com.opengamma.longdog.exceptions.MathsException;
import com.opengamma.longdog.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.longdog.exceptions.MathsExceptionNativeComputation;
import com.opengamma.longdog.helpers.FuzzyEquals;
import com.opengamma.longdog.materialisers.Materialisers;
import com.opengamma.longdog.nodes.PLUS;

public class TestPlusMaterialiseToComplex {

  @DataProvider
  public Object[][] dataContainer() {
    Object[][] obj = new Object[10][2];


    obj[0][0] = new PLUS(new OGComplexScalar(2.0, 4.0), new OGComplexScalar(3.0, 5.0));
    obj[0][1] = new OGComplexScalar(2.0 + 3.0, 4.0 + 5.0);

    obj[1][0] = new PLUS(new PLUS(new OGComplexScalar(1.0, 4.0), new OGComplexScalar(2.0, 5.0)), new OGComplexScalar(3.0, 6.0));
    obj[1][1] = new OGComplexScalar(1.0 + 2.0 + 3.0, 4.0 + 5.0 + 6.0);

    OGNumeric bigScalarTree = new OGComplexScalar(0.0, 0.0);
    double real = 0.0;
    double imag = 0.0;
    for (int i = 1; i < 500; i++) {
      if (i%2 == 0) {
        bigScalarTree = new PLUS(new OGComplexScalar(i, i*2), bigScalarTree);
      }
      else {
        bigScalarTree = new PLUS(bigScalarTree, new OGComplexScalar(i, i*2));
      }
      real += i;
      imag += i * 2;
    }

    obj[2][0] = bigScalarTree;
    obj[2][1] = new OGComplexScalar(real, imag);

    OGNumeric m1 = new OGComplexDenseMatrix(new double[][]{ { 1.0, 2.0 }, { 3.0, 4.0 } }, new double[][]{ { 7.0, 4.5 }, { 6.0, 8.0 } });
    OGNumeric m2 = new OGComplexDenseMatrix(new double[][]{ { 3.0, 1.0 }, { 5.5, 1.1 } }, new double[][]{ { 6.0, 3.5 }, { 2.3, 4.6 } });
    OGNumeric m3 = new OGComplexDenseMatrix(new double[][]{ { 4.0, 3.0 }, { 8.5, 5.1 } }, new double[][]{ { 13.0, 8.0 }, { 8.3, 12.6 } });

    obj[3][0] = new PLUS(m1, m2);
    obj[3][1] = m3;

    OGNumeric m4 = new OGComplexDenseMatrix(new double[][] { { 6.0, 5.0 }, { 2.0, 1.0 } }, new double[][] { { 2.2, 7.1 }, { 3.0, 5.0 } });
    OGNumeric m5 = new OGComplexDenseMatrix(new double[][] { { 10.0, 8.0 }, { 10.5, 6.1 } }, new double[][] { { 15.2, 15.1 }, { 11.3, 17.6 } });

    obj[4][0] = new PLUS(new PLUS(m1, m2), m4);
    obj[4][1] = m5;

    double[][] baseReal = new double[][]{ { 1.0, 2.0 }, { 4.0, 5.0 } };
    double[][] baseImag = new double[][]{ { 1.0, 2.0 }, { 4.0, 5.0 } };
    OGNumeric bigRealMatrixTree = new OGComplexDenseMatrix(new double[][] { { 0, 0 }, { 0, 0 } }, new double[][] { { 0, 0 }, { 0, 0 } });
    double[][] realMatrixSum = new double[2][2];
    double[][] imagMatrixSum = new double[2][2];
    for (int i = 1; i < 500; i++) {
      if (i%2 == 0) {
        bigRealMatrixTree = new PLUS(new OGComplexDenseMatrix(baseReal, baseImag), bigRealMatrixTree);
      }
      else {
        bigRealMatrixTree = new PLUS(bigRealMatrixTree, new OGComplexDenseMatrix(baseReal, baseImag));
      }
      for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
          realMatrixSum[j][k] += baseReal[j][k];
          imagMatrixSum[j][k] += baseImag[j][k];
        }
      }
    }
    obj[5][0] = bigRealMatrixTree;
    obj[5][1] = new OGComplexDenseMatrix(realMatrixSum, imagMatrixSum);

    OGNumeric m6 = new OGComplexDenseMatrix(new double[][]{ { 2.0, 3.0 }, { 4.0, 5.0 } }, new double[][]{ { 9.0, 6.5 }, { 8.0, 10.0 } });

    obj[6][0] = new PLUS(new OGComplexScalar(1.0, 2.0), m1);
    obj[6][1] = m6;

    obj[7][0] = new PLUS(m1, new OGComplexScalar(1.0, 2.0));
    obj[7][1] = m6;

    OGNumeric m7 = new OGComplexDenseMatrix(new double[][] { { 1.0 } }, new double[][] { { 2.0 } });

    obj[8][0] = new PLUS(m1, m7);
    obj[8][1] = m6;

    obj[9][0] = new PLUS(m7, m1);
    obj[9][1] = m6;

    return obj;
  };

  @Test(dataProvider = "dataContainer")
  public void materialiseToJDoubleArray(OGNumeric input, OGTerminal expected) {
    ComplexArrayContainer answer = Materialisers.toComplexArrayContainer(input);
    ComplexArrayContainer reference = Materialisers.toComplexArrayContainer(expected);
    if (!FuzzyEquals.ArrayFuzzyEquals(reference.getReal(), answer.getReal())) {
      throw new MathsException("Arrays not equal");
    }
    if (!FuzzyEquals.ArrayFuzzyEquals(reference.getImag(), answer.getImag())) {
      throw new MathsException("Arrays not equal");
    }
  }

  @Test(dataProvider = "dataContainer")
  public void materialiseToOGTerminal(OGNumeric input, OGTerminal expected) {
    OGTerminal answer = Materialisers.toOGTerminal(input);

    if (!FuzzyEquals.ArrayFuzzyEquals(answer.getData(), expected.getData())) {
      throw new MathsException("Arrays not equal");
    }
  }

  @Test(expectedExceptions = MathsExceptionNativeComputation.class)
  public void invalidDimensions() {
    OGNumeric m1 = new OGComplexDenseMatrix(new double[][] { { 1.0, 2.0} }, new double[][] { { 3.0, 4.0 } });
    OGNumeric m2 = new OGComplexDenseMatrix(new double[][] { { 1.0, 2.0, 3.0 } }, new double[][] { { 4.0, 5.0, 6.0 } });
    OGNumeric p = new PLUS(m1, m2);
    Materialisers.toOGTerminal(p);
  }

  @Test(dataProvider = "dataContainer", expectedExceptions = MathsExceptionIllegalArgument.class)
  public void outsideArgBound(OGExpr input, OGTerminal expected) {
    OGNumeric n = input.getArg(2);
  }

}
