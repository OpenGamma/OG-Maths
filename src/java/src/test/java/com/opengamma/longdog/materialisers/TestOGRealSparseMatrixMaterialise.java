/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.materialisers;

import java.util.Arrays;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.matrix.OGRealSparseMatrix;
import com.opengamma.longdog.exceptions.MathsException;

public class TestOGRealSparseMatrixMaterialise {

  @DataProvider
  public Object[][] dataContainer() {
    Object[][] obj = new Object[3][5];

    obj[0][0] = new int[] {0, 1};
    obj[0][1] = new int[] {0};
    obj[0][2] = new double[] {5.0};
    obj[0][3] = 1;
    obj[0][4] = 1;

    obj[1][0] = new int[] {0, 2, 2};
    obj[1][1] = new int[] {0, 1};
    obj[1][2] = new double[] {1.0, 2.0};
    obj[1][3] = 2;
    obj[1][4] = 2;

    obj[2][0] = new int[] {0, 3, 6, 10, 12, 12};
    obj[2][1] = new int[] {0, 1, 2, 0, 1, 2, 0, 1, 2, 3, 2, 3};
    obj[2][2] = new double[] { 1.0, 5.0, 90.0, 2.0, 60.0, 10.0, 30.0, 7.0, 11.0, 15.0, 120.0, 160.0};
    obj[2][3] = 4;
    obj[2][4] = 5;

    return obj;
  };

  @Test(dataProvider = "dataContainer")
  public void materialiseToOGTerminal(int[] colPtr, int[] rowIdx, double[] values, int rows, int cols) {
    OGRealSparseMatrix tmp = new OGRealSparseMatrix(colPtr, rowIdx, values, rows, cols);
    OGRealSparseMatrix answer = (OGRealSparseMatrix) Materialisers.toOGTerminal(tmp);
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
