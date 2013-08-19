/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.datacontainers.matrix;

import java.util.Arrays;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;
import com.opengamma.longdog.helpers.Catchers;
import com.opengamma.longdog.helpers.DenseMemoryManipulation;
import com.opengamma.longdog.helpers.MathsException;
import com.opengamma.longdog.helpers.MatrixPrimitiveUtils;

/**
 * Dense complex matrix
 */
public class OGComplexMatrix extends OGMatrix {
  private static ExprTypeEnum s_type = ExprTypeEnum.OGComplexMatrix;

  private double[] _data;
  private int _rows, _cols;

  /**
   * Takes a row major java double[][] and turns it into an OGComplexArray
   * @param dataIn a row major java double[][] 
   */
  public OGComplexMatrix(double[][] dataIn) {
    Catchers.catchNullFromArgList(dataIn, 1);
    _data = DenseMemoryManipulation.convertRowMajorDoublePointerToColumnMajorZeroInterleavedSinglePointer(dataIn);
    _rows = dataIn.length;
    _cols = dataIn[0].length;
  }

  /**
   * Takes a row major java double[][] and turns it into an OGComplexArray
   * @param realPart a row major java double[][] that is to be the real part of a complex array
   * @param imaginaryPart a row major java double[][] that is to be the imaginary part of a complex array
   */
  public OGComplexMatrix(double[][] realPart, double[][] imaginaryPart) {
    Catchers.catchNullFromArgList(realPart, 1);
    Catchers.catchNullFromArgList(imaginaryPart, 2);
    if (MatrixPrimitiveUtils.isRagged(realPart)) {
      throw new MathsException("Backing real array is ragged");
    }
    if (MatrixPrimitiveUtils.isRagged(imaginaryPart)) {
      throw new MathsException("Backing imaginary array is ragged");
    }
    _data = DenseMemoryManipulation.convertTwoRowMajorDoublePointerToColumnMajorInterleavedSinglePointer(realPart, imaginaryPart);
    _rows = realPart.length;
    _cols = realPart[0].length;
  }

  /**
   * Takes a column major double[] and turns it into an OGComplexArray
   * The length of dataIn must be either:
   * <li>rows*columns in which case it is assumed the double[] provided is the real part of the data (imaginary part assumed zero)
   * <li>2*rows*columns in which case it is assumed the double[] provided is formed of interleaved real and imaginary values
   * @param dataIn the real part data backing data
   * @param rows number of rows
   * @param columns number of columns
   */
  public OGComplexMatrix(double[] dataIn, int rows, int columns) {
    if (dataIn == null) {
      throw new MathsException("dataIn is null");
    }
    if (rows < 1) {
      throw new MathsException("Illegal number of rows specified. Value given was " + rows);
    }
    if (columns < 1) {
      throw new MathsException("Illegal number of columns specified. Value given was " + columns);
    }
    int len = rows * columns;
    if (!(len == dataIn.length || 2 * len == dataIn.length)) {
      throw new MathsException("Number of rows and columns specified does not commute with the quantity of data supplied.\n Rows=" + rows + " Columns=" + columns + " Data length=" +
          dataIn.length);
    }

    _data = new double[2 * rows * columns];
    if (len == dataIn.length) { // constructing from assumed real array
      _data = DenseMemoryManipulation.convertSinglePointerToZeroInterleavedSinglePointer(dataIn);
    } else {
      System.arraycopy(dataIn, 0, _data, 0, 2 * len);
    }
    _rows = rows;
    _cols = columns;
  }

  /**
   * Takes two separate column major double[] one for real part, one for imag part, and turns it into an OGComplexArray
   * @param realData the real components of the backing data
   * @param imagData the imaginary components of the backing data 
   * @param rows number of rows
   * @param columns number of columns
   */
  public OGComplexMatrix(double[] realData, double[] imagData, int rows, int columns) {
    Catchers.catchNullFromArgList(realData, 1);
    Catchers.catchNullFromArgList(imagData, 2);
    Catchers.catchValueShouldNotBeNegativeOrZeroFromArgList(rows, 3);
    Catchers.catchValueShouldNotBeNegativeOrZeroFromArgList(columns, 4);
    final int realDatalen = realData.length;
    final int imagDatalen = imagData.length;
    if (realDatalen != imagDatalen) {
      throw new MathsException("The lengths of the data provided by realData and imagData are not the same.");
    }
    int len = rows * columns;
    if (realDatalen != len) {
      throw new MathsException("Number of rows and columns specified does not commute with the quantity of data supplied.\n Rows=" + rows + " Columns=" + columns + " Data length=" +
          realDatalen);
    }
    _data = DenseMemoryManipulation.convertTwoSinglePointersToInterleavedSinglePointer(realData, imagData);
    _rows = rows;
    _cols = columns;
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
    String str = "\nOGComplexMatrix:" + "\ndata = " + Arrays.toString(_data) + "\nrows = " + _rows + "\ncols = " + _cols;
    str = str + "\n====Pretty Print====\n";
    double imag;
    for (int i = 0; i < 2 * _rows; i += 2) {
      for (int j = 0; j < _cols - 1; j++) {
        imag = _data[j * 2 * _rows + i + 1];
        str += String.format("%24.18f " + (imag >= 0 ? "+" : "-") + "%24.18fi, ", _data[j * 2 * _rows + i], Math.abs(imag));
      }
      imag = _data[(_cols - 1) * 2 * _rows + i + 1];
      str += String.format("%24.18f " + (imag >= 0 ? "+" : "-") + "%24.18fi, ", _data[(_cols - 1) * 2 * _rows + i], Math.abs(imag));
      str += String.format("\n");
    }
    return str;
  }

}
