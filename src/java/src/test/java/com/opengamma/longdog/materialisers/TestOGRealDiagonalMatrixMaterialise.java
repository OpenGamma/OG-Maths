/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.materialisers;

import java.util.Arrays;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.matrix.OGRealDiagonalMatrix;
import com.opengamma.longdog.exceptions.MathsException;
import com.opengamma.longdog.testhelpers.ArraysHelpers;

public class TestOGRealDiagonalMatrixMaterialise {

  @DataProvider
  public Object[][] dataContainer() {
    Object[][] obj = new Object[3][4];


    obj[0][0] = new double[] {1};
    obj[0][1] = 1;
    obj[0][2] = 1;
    obj[0][3] = new double[][] {{1}};

    obj[1][0] = new double[] {1,2,3,4};
    obj[1][1] = 4;
    obj[1][2] = 4;
    obj[1][3] = new double[][] {{1,0,0,0},{0,2,0,0},{0,0,3,0},{0,0,0,4}};

    obj[2][0] = new double[] {1,2,3,4};
    obj[2][1] = 6;
    obj[2][2] = 7;
    obj[2][3] = new double[][] {{1,0,0,0,0,0,0},{0,2,0,0,0,0,0},{0,0,3,0,0,0,0},{0,0,0,4,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0}};

    return obj;
  };

  @Test(dataProvider = "dataContainer")
  public void materialiseToJDoubleArray(double[] input, int rows, int cols, double[][] expected) {
    OGRealDiagonalMatrix tmp = new OGRealDiagonalMatrix(input, rows, cols);
    double[][] answer = Materialisers.toDoubleArrayOfArrays(tmp);
    if (!ArraysHelpers.ArraysEquals(expected, answer)) {
      throw new MathsException("Arrays not equal");
    }
  }

  @Test(dataProvider = "dataContainer")
  public void materialiseToRealDiagonalMatrix(double[] input, int rows, int cols, double[][] expected) {
    OGRealDiagonalMatrix tmp = new OGRealDiagonalMatrix(input, rows, cols);
    OGRealDiagonalMatrix answer = (OGRealDiagonalMatrix) Materialisers.toOGTerminal(tmp);
    if (!Arrays.equals(tmp.getData(), answer.getData())) {
      throw new MathsException("Arrays are not equal");
    }
    if (tmp.getRows() != answer.getRows()) {
      throw new MathsException("Rows are not equal");
    }
    if (tmp.getCols() != answer.getCols()) {
      throw new MathsException("Cols are not equal");
    }
  }

}
