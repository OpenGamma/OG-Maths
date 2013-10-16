/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.materialisers;

import java.util.Arrays;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.matrix.OGComplexSparseMatrix;
import com.opengamma.longdog.exceptions.MathsException;

public class TestOGComplexSparseMatrixMaterialise {

  @DataProvider
  public Object[][] dataContainer() {
    Object[][] obj = new Object[2][6];

    obj[0][0] = new int[] {0, 1};
    obj[0][1] = new int[] {0};
    obj[0][2] = new double[] {5.0};
    obj[0][3] = new double[] {50.0};
    obj[0][4] = 1;
    obj[0][5] = 1;

    obj[1][0] = new int[] {0, 2, 2};
    obj[1][1] = new int[] {0, 1};
    obj[1][2] = new double[] {1.0, 2.0};
    obj[1][3] = new double[] {10.0, 20.0};
    obj[1][4] = 2;
    obj[1][5] = 2;

    return obj;
  };

  @Test(dataProvider = "dataContainer")
  public void materialiseToOGTerminal(int[] colIdx, int[] rowPtr, double[] realpart, double[] imagpart, int rows, int cols) {
    OGComplexSparseMatrix tmp = new OGComplexSparseMatrix(colIdx, rowPtr, realpart, imagpart, rows, cols);
    OGComplexSparseMatrix answer = (OGComplexSparseMatrix) Materialisers.toOGTerminal(tmp);
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
