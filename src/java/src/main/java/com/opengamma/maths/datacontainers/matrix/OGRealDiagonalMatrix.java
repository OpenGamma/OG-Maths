/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.datacontainers.matrix;

import java.util.Arrays;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.exceptions.MathsExceptionNullPointer;
import com.opengamma.maths.helpers.Catchers;

/**
 * Real Diagonal matrix type
 */
public class OGRealDiagonalMatrix extends OGDiagonalMatrix {
  private static ExprEnum s_type = ExprEnum.OGRealDiagonalMatrix;

  private double[] _data;
  private int _cols;
  private int _rows;

  /**
   * Places the elements of an array "diag" of length "n" on the diagonal of a matrix of dimension [n * n]
   * @param diag the elements to be placed on the diagonal
   */
  public OGRealDiagonalMatrix(double[] diag) {
    Catchers.catchNullFromArgList(diag, 1);
    int n = diag.length;
    _rows = n;
    _cols = n;
    _data = new double[n];
    System.arraycopy(diag, 0, _data, 0, n);
  }

  /**
   * Places the elements of an array "diag" of length "n" on the diagonal of a matrix of dimension [rows * columns]
   * @param diag the elements to be placed on the diagonal
   * @param rows number of rows (if less than "n", diag is truncated to fit)
   * @param columns number of columns (if less than "n", diag is truncated to fit)
   */
  public OGRealDiagonalMatrix(double[] diag, int rows, int columns) {
    if (diag == null) {
      throw new MathsExceptionNullPointer("dataIn is null");
    }
    if (rows < 1) {
      throw new MathsExceptionIllegalArgument("Illegal number of rows specified. Value given was " + rows);
    }
    if (columns < 1) {
      throw new MathsExceptionIllegalArgument("Illegal number of columns specified. Value given was " + columns);
    }
    Catchers.catchNullFromArgList(diag, 1);
    _rows = rows;
    _cols = columns;
    int len = Math.max(diag.length, Math.min(rows, columns)); // max as zero fill on short diag
    _data = new double[len];
    System.arraycopy(diag, 0, _data, 0, diag.length);
  }

  /**
   * Places the "diag" as the first element on the diagonal of a matrix of dimension [rows * columns]
   * @param diag the element to be placed on in the first entry of the diagonal
   * @param rows number of rows
   * @param columns number of columns
   */
  public OGRealDiagonalMatrix(double diag, int rows, int columns) {
    if (rows < 1) {
      throw new MathsExceptionIllegalArgument("Illegal number of rows specified. Value given was " + rows);
    }
    if (columns < 1) {
      throw new MathsExceptionIllegalArgument("Illegal number of columns specified. Value given was " + columns);
    }
    _rows = rows;
    _cols = columns;
    int len = 1;
    _data = new double[len];
    _data[0] = diag;
  }

  /**
   * Places the "diag" as the first element on the diagonal of a matrix of dimension [1 * 1]
   * @param diag the element to be placed on in the first entry of the diagonal
   */
  public OGRealDiagonalMatrix(double diag) {
    this(diag, 1, 1);
  }

  /**
   * Places the "diag" as the first element on the diagonal of a matrix of dimension [1 * 1]
   * @param diag the element to be placed on in the first entry of the diagonal
   */
  public OGRealDiagonalMatrix(int diag) {
    this((double) diag, 1, 1);
  }

  @Override
  public ExprEnum getType() {
    return s_type;
  }

  @Override
  public double[] getData() {
    return _data;
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
  public String toDebugString() {
    String str = "OGRealDiagonalMatrix:" + "\ndata = " + Arrays.toString(_data) + "\nrows = " + _rows + "\ncols = " + _cols;
    str = str + "\n====Pretty Print====\n";
    String zeroStr = String.format("%24.18f ", 0.d);
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < _cols; j++) {
        if (i == j && i < _data.length && j < _data.length) {
          str += String.format("%24.18f ", _data[i]);
        } else {
          str += zeroStr;
        }
      }
      str += String.format("\n");
    }
    return str;
  }

  @Override
  public String toString() {
    String str = "Diagonal Matrix:\n";
    String zeroStr = String.format("%24.18f ", 0.d);
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < _cols; j++) {
        if (i == j && i < _data.length && j < _data.length) {
          str += String.format("%24.18f ", _data[i]);
        } else {
          str += zeroStr;
        }
      }
      str += String.format("\n");
    }
    return str;
  }

  @Override
  protected OGRealDenseMatrix asOGRealDenseMatrix() {
    double[] data = new double[_rows * _cols];
    for (int k = 0; k < this.getData().length; k++) {
      data[k * (_rows + 1)] = this.getData()[k];
    }
    return new OGRealDenseMatrix(data, _rows, _cols);
  }

  @Override
  protected OGComplexDenseMatrix asOGComplexDenseMatrix() {
    double[] data = new double[2 * _rows * _cols];
    for (int k = 0; k < this.getData().length; k++) {
      data[2 * k * (_rows + 1)] = this.getData()[k];
    }
    return new OGComplexDenseMatrix(data, _rows, _cols);
  }
}
