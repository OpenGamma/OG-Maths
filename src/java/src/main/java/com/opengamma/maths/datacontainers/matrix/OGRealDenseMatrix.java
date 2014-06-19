/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.datacontainers.matrix;

import java.util.Arrays;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.OGTerminal;
import com.opengamma.maths.datacontainers.scalar.OGRealScalar;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.helpers.Catchers;
import com.opengamma.maths.helpers.DenseMemoryManipulation;
import com.opengamma.maths.helpers.FuzzyEquals;

/**
 * Dense real matrix
 */
public class OGRealDenseMatrix extends OGDenseMatrix {
  private static ExprEnum s_type = ExprEnum.OGRealDenseMatrix;

  private double[] _data;
  private int _rows, _cols;

  /**
   * Takes a row major java double[][] and turns it into an OGRealDenseMatrix
   * @param dataIn a row major java double[][] 
   */
  public OGRealDenseMatrix(double[][] dataIn) {
    Catchers.catchNullFromArgList(dataIn, 1);
    _data = DenseMemoryManipulation.convertRowMajorDoublePointerToColumnMajorSinglePointer(dataIn);
    _rows = dataIn.length;
    _cols = dataIn[0].length;
  }

  /**
   * Takes a column major double[] and turns it into an OGRealDenseMatrix
   * @param dataIn the backing data
   * @param rows number of rows
   * @param columns number of columns
   */
  public OGRealDenseMatrix(double[] dataIn, int rows, int columns) {
    Catchers.catchNullFromArgList(dataIn, 1);
    if (rows < 1) {
      throw new MathsExceptionIllegalArgument("Illegal number of rows specified. Value given was " + rows);
    }
    if (columns < 1) {
      throw new MathsExceptionIllegalArgument("Illegal number of columns specified. Value given was " + columns);
    }
    int len = rows * columns;
    if (len != dataIn.length) {
      throw new MathsExceptionIllegalArgument("Number of rows and columns specified does not commute with the quantity of data supplied");
    }
    _data = new double[len];
    System.arraycopy(dataIn, 0, _data, 0, len);
    _rows = rows;
    _cols = columns;
  }

  /**
   * @param number the single number in this array
   */
  public OGRealDenseMatrix(double number) {
    _cols = 1;
    _rows = 1;
    _data = new double[1];
    _data[0] = number;
  }

  /**
   * Takes a double[] and turns it into an OGMatrix as a single row
   * @param dataIn the backing data
   */
  public OGRealDenseMatrix(double[] dataIn) {
    Catchers.catchNullFromArgList(dataIn, 1);
    int len = dataIn.length;
    _data = new double[len];
    System.arraycopy(dataIn, 0, _data, 0, len);
    _rows = 1;
    _cols = len;
  }

  @Override
  public double[] getData() {
    return _data;
  }

  @Override
  public ExprEnum getType() {
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
    String str = "OGRealDenseMatrix:" + "\ndata = " + Arrays.toString(_data) + "\nrows = " + _rows + "\ncols = " + _cols;
    str = str + "\n====Pretty Print====\n";
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < _cols; j++) {
        str += String.format("%24.18f ", _data[j * _rows + i]);
      }
      str += String.format("\n");
    }
    return str;
  }

  @Override
  protected OGRealDenseMatrix asOGRealDenseMatrix() {
    return new OGRealDenseMatrix(this.getData(), _rows, _cols);
  }

  @Override
  protected OGComplexDenseMatrix asOGComplexDenseMatrix() {
    return new OGComplexDenseMatrix(DenseMemoryManipulation.convertSinglePointerToZeroInterleavedSinglePointer(this.getData()), _rows, _cols);
  }

}
