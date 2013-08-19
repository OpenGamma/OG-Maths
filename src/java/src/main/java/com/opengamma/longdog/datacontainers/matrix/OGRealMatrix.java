/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.datacontainers.matrix;

import java.util.Arrays;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;
import com.opengamma.longdog.datacontainers.OGArray;
import com.opengamma.longdog.helpers.Catchers;
import com.opengamma.longdog.helpers.DenseMemoryManipulation;

/**
 * Dense real matrix
 */
public class OGRealMatrix extends OGArray {
  private static ExprTypeEnum s_type = ExprTypeEnum.OGRealMatrix;

  private double[] _data;
  private int _rows, _cols;

  public OGRealMatrix(double[] data, int rows, int cols) {
    _rows = rows;
    _cols = cols;
    _data = data;
  }

  /**
   * Takes a row major java double[][] and turns it into an OGDoubleArray
   * @param dataIn a row major java double[][] 
   */
  public OGRealMatrix(double[][] dataIn) {
    Catchers.catchNullFromArgList(dataIn, 1);
    _data = DenseMemoryManipulation.convertRowMajorDoublePointerToColumnMajorSinglePointer(dataIn);
    _rows = dataIn.length;
    _cols = dataIn[0].length;
  }

  @Override
  public double[] getData() {
    return _data;
  }

  @Override
  public ExprTypeEnum getType() {
    return s_type;
  }

  /**
   * Gets the rows.
   * @return the rows
   */
  @Override
  public int getRows() {
    return _rows;
  }

  /**
   * Gets the cols.
   * @return the cols
   */
  @Override
  public int getCols() {
    return _cols;
  }

  @Override
  public String toString() {
    String str = "OGRealMatrix:" + "\ndata = " + Arrays.toString(_data) + "\nrows = " + _rows + "\ncols = " + _cols;
    str = str + "\n====Pretty Print====\n";
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < _cols; j++) {
        str += String.format("%24.18f ", _data[j * _rows + i]);
      }
      str += String.format("\n");
    }
    return str;
  }

}
