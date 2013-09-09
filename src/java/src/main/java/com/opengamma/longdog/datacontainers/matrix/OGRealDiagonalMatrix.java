/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.datacontainers.matrix;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;
import com.opengamma.longdog.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.longdog.exceptions.MathsExceptionNullPointer;
import com.opengamma.longdog.helpers.Catchers;

/**
 * Real Diagonal matrix type
 */
public class OGRealDiagonalMatrix extends OGDiagonalMatrix {
  private static ExprTypeEnum s_type = ExprTypeEnum.OGRealDiagonalMatrix;

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
    int len = Math.min(diag.length, Math.min(rows, columns));
    _data = new double[len];
    System.arraycopy(diag, 0, _data, 0, len);
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
  public ExprTypeEnum getType() {
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
}
