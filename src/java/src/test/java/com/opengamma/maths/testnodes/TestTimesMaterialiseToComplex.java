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
import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.datacontainers.other.ComplexArrayContainer;
import com.opengamma.maths.datacontainers.scalar.OGComplexScalar;
import com.opengamma.maths.exceptions.MathsException;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.exceptions.MathsExceptionNativeComputation;
import com.opengamma.maths.helpers.FuzzyEquals;
import com.opengamma.maths.materialisers.Materialisers;
import com.opengamma.maths.nodes.PLUS;
import com.opengamma.maths.nodes.TIMES;

public class TestTimesMaterialiseToComplex {

  /**
   * Computes the real part of complex multiply.
   * @param data1 first data
   * @param data2 second data
   * @return real(first * second)
   */
  private double cmplxMulRP(double[] data1, double[] data2) {
    return data1[0] * data2[0] - data1[1] * data2[1];
  }

  /**
   * Computes the imaginary part of complex multiply.
   * @param data1 first data
   * @param data2 second data
   * @return imag(first * second)
   */
  private double cmplxMulIP(double[] data1, double[] data2) {
    return data1[0] * data2[1] + data1[1] * data2[0];
  }

  /**
   * Computes complex multiply.
   * @param data1 first data
   * @param data2 second data
   * @return (first * second)
   */
  private double[] cmplxMul(double[] data1, double[] data2) {
    double[] ret = new double[2];
    ret[0] = cmplxMulRP(data1, data2);
    ret[1] = cmplxMulIP(data1, data2);
    return ret;
  }

  @DataProvider
  public Object[][] dataContainer() {
    Object[][] obj = new Object[10][2];

    obj[0][0] = new TIMES(new OGComplexScalar(2.0, 4.0), new OGComplexScalar(3.0, 5.0));
    obj[0][1] = new OGComplexScalar(cmplxMul(new double[] { 2, 4 }, new double[] { 3, 5 }));

    obj[1][0] = new TIMES(new TIMES(new OGComplexScalar(1.0, 4.0), new OGComplexScalar(2.0, 5.0)), new OGComplexScalar(3.0, 6.0));
    obj[1][1] = new OGComplexScalar(cmplxMul(cmplxMul(new double[] { 1, 4 }, new double[] { 2, 5 }), new double[] { 3, 6 }));

    OGNumeric bigScalarTree = new OGComplexScalar(1.0, 2.0);
    double[] ans = new double[] { 1, 2 };
    for (int i = 1; i < 40; i++) {
      if (i % 2 == 0) {
        bigScalarTree = new TIMES(new OGComplexScalar(i, i * 2), bigScalarTree);
      }
      else {
        bigScalarTree = new TIMES(bigScalarTree, new OGComplexScalar(i, i * 2));
      }
      ans = cmplxMul(ans, new double[] { i, i * 2 });
    }

    obj[2][0] = bigScalarTree;
    obj[2][1] = new OGComplexScalar(ans);

    OGNumeric m1 = new OGComplexDenseMatrix(new double[][] { { 1.0, 2.0 }, { 3.0, 4.0 } }, new double[][] { { 7.0, 4.5 }, { 6.0, 8.0 } });
    OGNumeric m2 = new OGComplexDenseMatrix(new double[][] { { 3.0, 1.0 }, { 5.5, 1.1 } }, new double[][] { { 6.0, 3.5 }, { 2.3, 4.6 } });
    OGNumeric m3 = new OGComplexDenseMatrix(new double[][] { { -39., -13.75 }, { 2.7, -32.4 } }, new double[][] { { 27.0, 11.5 }, { 39.9, 27.2 } });

    obj[3][0] = new TIMES(m1, m2);
    obj[3][1] = m3;

    OGNumeric m4 = new OGComplexDenseMatrix(new double[][] { { 6.0, 5.0 }, { 2.0, 1.0 } }, new double[][] { { 2.2, 7.1 }, { 3.0, 5.0 } });
    OGNumeric m5 = new OGComplexDenseMatrix(new double[][] { { -293.4, -150.4 }, { -114.3, -168.4 } }, new double[][] { { 76.2, -40.125 }, { 87.9, -134.8 } });

    obj[4][0] = new TIMES(new TIMES(m1, m2), m4);
    obj[4][1] = m5;

    double[][] baseReal = new double[][] { { 1.0, 2.0 }, { 4.0, 5.0 } };
    double[][] baseImag = new double[][] { { 1.0, 2.0 }, { 4.0, 5.0 } };
    OGNumeric bigRealMatrixTree = new OGComplexDenseMatrix(new double[][] { { 1, 2 }, { 4, 5 } }, new double[][] { { 1, 2 }, { 4, 5 } });
    double[][] realMatrixProd = new double[][] { { 1.0, 2.0 }, { 4.0, 5.0 } };
    double[][] imagMatrixProd = new double[][] { { 1.0, 2.0 }, { 4.0, 5.0 } };
    for (int i = 1; i < 40; i++) {
      if (i % 2 == 0) {
        bigRealMatrixTree = new TIMES(new OGComplexDenseMatrix(baseReal, baseImag), bigRealMatrixTree);
      }
      else {
        bigRealMatrixTree = new TIMES(bigRealMatrixTree, new OGComplexDenseMatrix(baseReal, baseImag));
      }
      for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
          double[] tmpbase = new double[] { baseReal[j][k], baseImag[j][k] };
          double[] tmpprod = new double[] { realMatrixProd[j][k], imagMatrixProd[j][k] };
          realMatrixProd[j][k] = cmplxMulRP(tmpprod, tmpbase);
          imagMatrixProd[j][k] = cmplxMulIP(tmpprod, tmpbase);
        }
      }
    }
    obj[5][0] = bigRealMatrixTree;
    obj[5][1] = new OGComplexDenseMatrix(realMatrixProd, imagMatrixProd);

    OGNumeric m6 = new OGComplexDenseMatrix(new double[][] { { -19, -9.5 }, { -12.0, -16.0 } }, new double[][] { { 17.0, 15.0 }, { 21.0, 28.0 } });

    obj[6][0] = new TIMES(new OGComplexScalar(2.0, 3.0), m1);
    obj[6][1] = m6;

    obj[7][0] = new TIMES(m1, new OGComplexScalar(2.0, 3.0));
    obj[7][1] = m6;

    OGNumeric m7 = new OGComplexDenseMatrix(new double[][] { { 2.0 } }, new double[][] { { 3.0 } });

    obj[8][0] = new TIMES(m1, m7);
    obj[8][1] = m6;

    obj[9][0] = new TIMES(m7, m1);
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
      throw new MathsException("Terminals not equal.\n Got: " + answer.toString() + "\nExpected: " + expected.toString());
    }
  }

  @Test(expectedExceptions = MathsExceptionNativeComputation.class)
  public void invalidDimensions() {
    OGNumeric m1 = new OGComplexDenseMatrix(new double[][] { { 1.0, 2.0 } }, new double[][] { { 3.0, 4.0 } });
    OGNumeric m2 = new OGComplexDenseMatrix(new double[][] { { 1.0, 2.0, 3.0 } }, new double[][] { { 4.0, 5.0, 6.0 } });
    OGNumeric p = new PLUS(m1, m2);
    Materialisers.toOGTerminal(p);
  }

  @Test(dataProvider = "dataContainer", expectedExceptions = MathsExceptionIllegalArgument.class)
  public void outsideArgBound(OGExpr input, OGTerminal expected) {
    OGNumeric n = input.getArg(2);
  }

}
