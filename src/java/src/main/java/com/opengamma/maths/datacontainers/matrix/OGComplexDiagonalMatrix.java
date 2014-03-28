/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.datacontainers.matrix;

import java.util.Arrays;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.scalar.OGComplexScalar;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.helpers.Catchers;
import com.opengamma.maths.helpers.DenseMemoryManipulation;

/**
 * Complex Diagonal matrix type
 */
public class OGComplexDiagonalMatrix extends OGDiagonalMatrix {

  private static ExprEnum s_type = ExprEnum.OGComplexDiagonalMatrix;

  @Override
  public ExprEnum getType() {
    return s_type;
  }

  private double[] _data;
  private int _cols;
  private int _rows;

  /**
   * Places the elements of an array "diag" of length "n" on the diagonal of a matrix of dimension [n * n]
   * @param real the real part of the elements to be placed on the diagonal, the imaginary part is assumed zero
   */
  public OGComplexDiagonalMatrix(double[] real) {
    Catchers.catchNullFromArgList(real, 1);
    int n = real.length;
    _rows = n;
    _cols = n;
    _data = DenseMemoryManipulation.convertSinglePointerToZeroInterleavedSinglePointer(real);
  }

  /**
   * Places the elements of an array "diag" of length "n" on the diagonal of a matrix of dimension [n * n]
   * @param real real part of the elements to be placed on the diagonal
   * @param imag imaginary part of the elements to be placed on the diagonal
   */
  public OGComplexDiagonalMatrix(double[] real, double[] imag) {
    Catchers.catchNullFromArgList(real, 1);
    Catchers.catchNullFromArgList(imag, 2);
    if (real.length != imag.length) {
      throw new MathsExceptionIllegalArgument("On construction data does not commute, real data is length " + real.length + " whilst imaginary data is length " + imag.length);
    }
    int n = real.length;
    _rows = n;
    _cols = n;
    _data = DenseMemoryManipulation.convertTwoSinglePointersToInterleavedSinglePointer(real, imag);

  }

  /**
   * Places the elements of an array "diag" of length "n" on the diagonal of a matrix of dimension [n * n]
   * @param diag the elements to be placed on the diagonal
   */
  public OGComplexDiagonalMatrix(OGComplexScalar[] diag) {
    Catchers.catchNullFromArgList(diag, 1);
    int n = diag.length;
    _rows = n;
    _cols = n;
    _data = new double[2 * n];
    int ptr = 0;
    for (int i = 0; i < n; i++) {
      _data[ptr] = diag[i].getReal();
      _data[ptr + 1] = diag[i].getImag();
      ptr += 2;
    }
  }

  /**
   * Places the elements of an array "diag" of length "n" as the real parts of number on the diagonal of a matrix of dimension [rows * columns]
   * @param diag real part of the elements to be placed on the diagonal
   * @param rows number of rows (if less than "n", diag is truncated to fit)
   * @param columns number of columns (if less than "n", diag is truncated to fit)
   */
  public OGComplexDiagonalMatrix(double[] diag, int rows, int columns) {
    Catchers.catchNullFromArgList(diag, 1);
    if (rows < 1) {
      throw new MathsExceptionIllegalArgument("Illegal number of rows specified. Value given was " + rows);
    }
    if (columns < 1) {
      throw new MathsExceptionIllegalArgument("Illegal number of columns specified. Value given was " + columns);
    }
    _rows = rows;
    _cols = columns;
    int len = Math.max(diag.length, Math.min(rows, columns)); // max as zero fill on short diag
    _data = new double[len * 2];
    double[] tmp = DenseMemoryManipulation.convertSinglePointerToZeroInterleavedSinglePointer(diag);
    System.arraycopy(tmp, 0, _data, 0, tmp.length);
  }

  /**
   * Places the elements of an array "diag" of length "n" as the real parts of number on the diagonal of a matrix of dimension [rows * columns]
   * @param real real part of the elements to be placed on the diagonal
   * @param imag imaginary part of the elements to be placed on the diagonal 
   * @param rows number of rows (if less than "n", diag is truncated to fit)
   * @param columns number of columns (if less than "n", diag is truncated to fit)
   */
  public OGComplexDiagonalMatrix(double[] real, double[] imag, int rows, int columns) {
    Catchers.catchNullFromArgList(real, 1);
    Catchers.catchNullFromArgList(imag, 2);
    if (rows < 1) {
      throw new MathsExceptionIllegalArgument("Illegal number of rows specified. Value given was " + rows);
    }
    if (columns < 1) {
      throw new MathsExceptionIllegalArgument("Illegal number of columns specified. Value given was " + columns);
    }
    if (real.length != imag.length) {
      throw new MathsExceptionIllegalArgument("On construction data does not commute, real data is length " + real.length + " whilst imaginary data is length " + imag.length);
    }
    _rows = rows;
    _cols = columns;
    int len = Math.max(real.length, Math.min(rows, columns)); // max as zero fill on short diag   
    _data = new double[len * 2];
    double[] tmp = DenseMemoryManipulation.convertTwoSinglePointersToInterleavedSinglePointer(real, imag);
    System.arraycopy(tmp, 0, _data, 0, real.length * 2);
  }

  /**
   * Places the "diag" as the first element on the diagonal of a matrix of dimension [rows * columns]
   * @param diag the element to be placed on in the first entry of the diagonal
   * @param rows number of rows
   * @param columns number of columns
   */
  public OGComplexDiagonalMatrix(double diag, int rows, int columns) {
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
   * Places the "diag" as the first element on the diagonal of a matrix of dimension [rows * columns]
   * @param real the real part of the element to be placed on in the first entry of the diagonal
   * @param imag the imaginary part of the element to be placed on in the first entry of the diagonal
   * @param rows number of rows
   * @param columns number of columns
   */
  public OGComplexDiagonalMatrix(double real, double imag, int rows, int columns) {
    if (rows < 1) {
      throw new MathsExceptionIllegalArgument("Illegal number of rows specified. Value given was " + rows);
    }
    if (columns < 1) {
      throw new MathsExceptionIllegalArgument("Illegal number of columns specified. Value given was " + columns);
    }
    _rows = rows;
    _cols = columns;
    _data = new double[2];
    _data[0] = real;
    _data[1] = imag;
  }

  /**
   * Places the "diag" as the first element on the diagonal of a matrix of dimension [1 * 1]
   * The imaginary part is assumed to be zero
   * @param diag the element to be placed on in the first entry of the diagonal
   */
  public OGComplexDiagonalMatrix(double diag) {
    this(diag, 1, 1);
  }

  /**
   * Places the "diag" as the first element on the diagonal of a matrix of dimension [1 * 1]
   * @param real the real part of the element to be placed on in the first entry of the diagonal
   * @param imag the imaginary part of the element to be placed on in the first entry of the diagonal
   */
  public OGComplexDiagonalMatrix(double real, double imag) {
    this(real, imag, 1, 1);
  }

  /**
   * Places the "diag" as the first element on the diagonal of a matrix of dimension [1 * 1]
   * The imaginary part is assumed to be zero
   * @param diag the element to be placed on in the first entry of the diagonal
   */
  public OGComplexDiagonalMatrix(int diag) {
    this((double) diag, 1, 1);
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
  public String toString() {
    String str = "OGComplexDiagonalMatrix:" + "\ndata = " + Arrays.toString(_data) + "\nrows = " + _rows + "\ncols = " + _cols;
    str = str + "\n====Pretty Print====\n";
    String zeroStr = String.format("%24.18f + %24.18fi ", 0.d, 0.d);
    double imag;
    for (int i = 0; i < 2 * _rows; i += 2) {
      for (int j = 0; j < 2 * _cols; j += 2) {
        if (i == j && i < _data.length && j < _data.length) {
          imag = _data[i + 1];
          str += String.format("%24.18f " + (imag >= 0 ? "+" : "-") + "%24.18fi, ", _data[i], Math.abs(imag));
        } else {
          str += zeroStr;
        }
      }
      str += String.format("\n");
    }
    return str;
  }

}
