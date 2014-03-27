/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.materialisers;

import java.util.Arrays;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.datacontainers.other.ComplexArrayContainer;
import com.opengamma.maths.exceptions.MathsException;
import com.opengamma.maths.helpers.DenseMemoryManipulation;
import com.opengamma.maths.materialisers.Materialisers;
import com.opengamma.maths.testhelpers.ArraysHelpers;

public class TestOGComplexDenseMatrixMaterialise {

  @DataProvider
  public Object[][] dataContainer() {
    Object[][] obj = new Object[4][4];

    obj[0][0] = new double[][] { { 1 } };
    obj[0][1] = new double[][] { { 10 } };
    obj[0][2] = new double[][] { { 1 } };
    obj[0][3] = new double[][] { { 10 } };

    obj[1][0] = new double[][] { { 1, 2, 3, 4 } };
    obj[1][1] = new double[][] { { 10, 20, 30, 40 } };
    obj[1][2] = new double[][] { { 1, 2, 3, 4 } };
    obj[1][3] = new double[][] { { 10, 20, 30, 40 } };

    obj[2][0] = new double[][] { { 1 }, { 2 }, { 3 }, { 4 } };
    obj[2][1] = new double[][] { { 10 }, { 20 }, { 30 }, { 40 } };
    obj[2][2] = new double[][] { { 1 }, { 2 }, { 3 }, { 4 } };
    obj[2][3] = new double[][] { { 10 }, { 20 }, { 30 }, { 40 } };

    obj[3][0] = new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } };
    obj[3][1] = new double[][] { { 10, 20, 30 }, { 40, 50, 60 }, { 70, 80, 90 }, { 100, 110, 120 } };
    obj[3][2] = new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } };
    obj[3][3] = new double[][] { { 10, 20, 30 }, { 40, 50, 60 }, { 70, 80, 90 }, { 100, 110, 120 } };

    return obj;
  };

  @Test(dataProvider = "dataContainer")
  public void materialiseToJComplexContainer(double[][] inputReal, double[][] inputImag, double[][] expectedReal, double[][] expectedImag) {
    OGComplexDenseMatrix tmp = new OGComplexDenseMatrix(inputReal, inputImag);
    ComplexArrayContainer answer = Materialisers.toComplexArrayContainer(tmp);
    if (!ArraysHelpers.ArraysEquals(expectedReal, answer.getReal())) {
      throw new MathsException("REAL: Arrays not equal");
    }
    if (!ArraysHelpers.ArraysEquals(expectedImag, answer.getImag())) {
      throw new MathsException("IMAG: Arrays not equal");
    }
  }

  @Test(dataProvider = "dataContainer")
  public void materialisetoOGTerminal(double[][] inputReal, double[][] inputImag, double[][] expectedReal, double[][] expectedImag) {
    OGComplexDenseMatrix tmp = new OGComplexDenseMatrix(inputReal, inputImag);
    OGComplexDenseMatrix answer = (OGComplexDenseMatrix) Materialisers.toOGTerminal(tmp);
    double[] expected = DenseMemoryManipulation.convertTwoRowMajorDoublePointerToColumnMajorInterleavedSinglePointer(expectedReal, expectedImag);
    if (!Arrays.equals(expected, answer.getData())) {
      throw new MathsException("Arrays not equal");
    }
  }

}
