/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.materialisers;

import java.util.Arrays;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.matrix.OGComplexDiagonalMatrix;
import com.opengamma.longdog.datacontainers.other.ComplexArrayContainer;
import com.opengamma.longdog.exceptions.MathsException;
import com.opengamma.longdog.testhelpers.ArraysHelpers;

public class TestOGComplexDiagonalMatrixMaterialise {

  @DataProvider
  public Object[][] dataContainer() {
    Object[][] obj = new Object[3][6];

    obj[0][0] = new double[] { 1 };
    obj[0][1] = new double[] { 10 };    
    obj[0][2] = 1;
    obj[0][3] = 1;
    obj[0][4] = new double[][] { { 1 } };
    obj[0][5] = new double[][] { { 10 } };

    obj[1][0] = new double[] { 1, 2, 3, 4 };
    obj[1][1] = new double[] { 10, 20, 30, 40 };    
    obj[1][2] = 4;
    obj[1][3] = 4;
    obj[1][4] = new double[][] { { 1, 0, 0, 0 }, { 0, 2, 0, 0 }, { 0, 0, 3, 0 }, { 0, 0, 0, 4 } };
    obj[1][5] = new double[][] { { 10, 0, 0, 0 }, { 0, 20, 0, 0 }, { 0, 0, 30, 0 }, { 0, 0, 0, 40 } };    

    obj[2][0] = new double[] { 1, 2, 3, 4 };
    obj[2][1] = new double[] { 10, 20, 30, 40 };    
    obj[2][2] = 6;
    obj[2][3] = 7;
    obj[2][4] = new double[][] { { 1, 0, 0, 0, 0, 0, 0 }, { 0, 2, 0, 0, 0, 0, 0 }, { 0, 0, 3, 0, 0, 0, 0 }, { 0, 0, 0, 4, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 } };
    obj[2][5] = new double[][] { { 10, 0, 0, 0, 0, 0, 0 }, { 0, 20, 0, 0, 0, 0, 0 }, { 0, 0, 30, 0, 0, 0, 0 }, { 0, 0, 0, 40, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 } };    

    return obj;
  };

  @Test(dataProvider = "dataContainer")
  public void materialiseToJDoubleArray(double[] inputReal, double[] inputComplex, int rows, int cols, double[][] expectedReal, double[][] expectedImag) {
    OGComplexDiagonalMatrix tmp = new OGComplexDiagonalMatrix(inputReal, inputComplex, rows, cols);
    ComplexArrayContainer answer = Materialisers.toComplexArrayContainer(tmp);
    if (!ArraysHelpers.ArraysEquals(expectedReal, answer.getReal())) {
      throw new MathsException("REAL: Arrays not equal");
    }
    if (!ArraysHelpers.ArraysEquals(expectedImag, answer.getImag())) {
      throw new MathsException("IMAG: Arrays not equal");
    }
  }

  @Test(dataProvider = "dataContainer")
  public void materialiseToOGTerminal(double[] inputReal, double[] inputComplex, int rows, int cols, double[][] expectedReal, double[][] expectedImag) {
    OGComplexDiagonalMatrix tmp = new OGComplexDiagonalMatrix(inputReal, inputComplex, rows, cols);
    OGComplexDiagonalMatrix answer = (OGComplexDiagonalMatrix) Materialisers.toOGTerminal(tmp);
    if (answer.getRows() != tmp.getRows()) {
      throw new MathsException("Rows not equal");
    }
    if (answer.getCols() != tmp.getCols()) {
      throw new MathsException("Cols not equal");
    }
    if (!Arrays.equals(tmp.getData(), answer.getData())) {
      throw new MathsException("Arrays not equal");
    }
  }
}
