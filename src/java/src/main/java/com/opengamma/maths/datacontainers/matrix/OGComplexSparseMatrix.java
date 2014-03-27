/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.datacontainers.matrix;

import java.util.Arrays;
import java.util.Formatter;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.scalar.OGComplexScalar;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.helpers.Catchers;
import com.opengamma.maths.helpers.DenseMemoryManipulation;
import com.opengamma.maths.helpers.MatrixPrimitiveUtils;

/**
 * Complex sparse matrix type
 */
public class OGComplexSparseMatrix extends OGSparseMatrix {

  private static ExprEnum s_type = ExprEnum.OGComplexSparseMatrix;

  private double[] _data;
  private int[] _colPtr;
  private int[] _rowIdx;
  private int _els;
  private int _rows;
  private int _cols;
  private int _maxEntriesInAColumn;

  /**
   * Construct from double[][]
   * @param realPart is a double[][]
   */
  public OGComplexSparseMatrix(double[][] realPart) {
    Catchers.catchNullFromArgList(realPart, 1);
    if (MatrixPrimitiveUtils.isRagged(realPart)) {
      throw new MathsExceptionIllegalArgument("Matrix representation must not be ragged, i.e. all rows must be the same length");
    }

    // check there is at least some data in the matrix.
    boolean dataOK = false;
    for (int i = 0; i < realPart.length; i++) {
      for (int j = 0; j < realPart[i].length; j++) {
        if (realPart[i][j] != 0.e0) {
          dataOK = true;
        }
      }
    }
    if (!dataOK) {
      throw new MathsExceptionIllegalArgument("Matrix representation has no non-zero values. Blank matrices are not allowed");
    }

    final int rows = realPart.length;
    final int cols = realPart[0].length;

    //get number of elements
    _els = rows * cols;

    // tmp arrays, in case we get in a fully populated matrix, intelligent design upstream should ensure that this is overkill!
    double[] dataTmp = new double[_els * 2];
    int[] colPtrTmp = new int[_els + 1];
    int[] rowIdxTmp = new int[_els];

    // we need unwind the array m into coordinate form
    int ptr = 0, dataptr = 0;
    int i;
    int localMaxEntrisInACol;
    _maxEntriesInAColumn = -1;
    double entry = 0;
    for (i = 0; i < cols; i++) {
      colPtrTmp[i] = ptr;
      localMaxEntrisInACol = 0;
      for (int j = 0; j < rows; j++) {
        entry = realPart[j][i];
        if (entry != 0.0e0) {
          rowIdxTmp[ptr] = j;
          dataTmp[dataptr] = entry;
          ptr++;
          dataptr += 2;
          localMaxEntrisInACol++;
        }
      }
      if (localMaxEntrisInACol > _maxEntriesInAColumn) {
        _maxEntriesInAColumn = localMaxEntrisInACol;
      }
    }
    colPtrTmp[i] = ptr;

    // return correct 0 to correct length of the vector buffers
    _data = Arrays.copyOfRange(dataTmp, 0, dataptr);
    _colPtr = Arrays.copyOfRange(colPtrTmp, 0, i + 1); // yes, the +1 is correct, it allows the computation of the number of elements in the final row!
    _rowIdx = Arrays.copyOfRange(rowIdxTmp, 0, ptr);
    _rows = rows;
    _cols = cols;
  }

  /**
   * Construct from two double[][] matrices, one containing the real part of the numbers the other containing the imaginary.
   * The arrays must not be ragged and must be of identical dimension. 
   * @param realPart a double[][] matrix containing the real part of the numbers
   * @param imaginaryPart a double[][] matrix containing the imaginary part of the numbers
   */
  public OGComplexSparseMatrix(double[][] realPart, double[][] imaginaryPart) {
    Catchers.catchNullFromArgList(realPart, 1);
    Catchers.catchNullFromArgList(imaginaryPart, 2);
    if (MatrixPrimitiveUtils.isRagged(realPart)) {
      throw new MathsExceptionIllegalArgument("Backing real array is ragged");
    }
    if (MatrixPrimitiveUtils.isRagged(imaginaryPart)) {
      throw new MathsExceptionIllegalArgument("Backing imaginary array is ragged");
    }
    int rows = realPart.length;
    int cols = realPart[0].length;

    if (imaginaryPart.length != rows) {
      throw new MathsExceptionIllegalArgument("Number of rows in first array (" + rows + ") does not match number of rows in second (" + imaginaryPart.length + ")");
    }
    if (imaginaryPart[0].length != cols) {
      throw new MathsExceptionIllegalArgument("Number of columns in first array (" + cols + ") does not match number of columns in second (" + imaginaryPart[0].length + ")");
    }

    // check there is at least some data in the matrix.
    boolean dataOK = false;
    for (int i = 0; i < realPart.length; i++) {
      for (int j = 0; j < realPart[i].length; j++) {
        if (realPart[i][j] != 0.e0) {
          dataOK = true;
          break;
        }
        if (imaginaryPart[i][j] != 0.e0) {
          dataOK = true;
          break;
        }
      }
    }
    if (!dataOK) {
      throw new MathsExceptionIllegalArgument("Matrix representation has no non-zero values. Blank matrices are not allowed");
    }

    //get number of elements
    _els = rows * cols;
    double[] dataTmp = new double[2 * _els];
    int[] colPtrTmp = new int[_els + 1];
    int[] rowIdxTmp = new int[_els];

    // we need unwind the array m into CSC form, note the rowIdx reflects the notion that 64bit complex representation is considered a single 128bit index
    // this is so the same data can be shoved into C without problem and the internal representation of a CSC real and CSC complex matrix is the same if the nonzero pattern is the same in each case 
    int ptr = 0, dataptr = 0;
    int i;
    int localMaxEntrisInACol;
    _maxEntriesInAColumn = -1;
    double realEntry = 0, imagEntry = 0;
    for (i = 0; i < cols; i++) {
      colPtrTmp[i] = ptr;
      localMaxEntrisInACol = 0;
      for (int j = 0; j < rows; j++) {
        realEntry = realPart[j][i];
        imagEntry = imaginaryPart[j][i];
        if (realEntry != 0.0e0 || imagEntry != 0.0e0) {
          rowIdxTmp[ptr] = j;
          dataTmp[dataptr] = realEntry;
          dataTmp[dataptr + 1] = imagEntry;
          ptr++;
          dataptr += 2;
          localMaxEntrisInACol++;
        }
      }
      if (localMaxEntrisInACol > _maxEntriesInAColumn) {
        _maxEntriesInAColumn = localMaxEntrisInACol;
      }
    }
    colPtrTmp[i] = ptr;

    // return correct 0 to correct length of the vector buffers
    _data = Arrays.copyOfRange(dataTmp, 0, dataptr);
    _colPtr = Arrays.copyOfRange(colPtrTmp, 0, i + 1); // yes, the +1 is correct, it allows the computation of the number of elements in the final row!
    _rowIdx = Arrays.copyOfRange(rowIdxTmp, 0, ptr);
    _rows = rows;
    _cols = cols;
  }

  /**
   * Construct from ComplexType[][] array
   * @param data a ComplexType[][] array
   */
  public OGComplexSparseMatrix(OGComplexScalar[][] data) {
    Catchers.catchNullFromArgList(data, 1);
    if (MatrixPrimitiveUtils.isRagged(data)) {
      throw new MathsExceptionIllegalArgument("Backing array is ragged");
    }
    int rows = data.length;
    int cols = data[0].length;

    // check there is at least some data in the matrix.
    boolean dataOK = false;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        if (data[i][j].getReal() != 0.e0) {
          dataOK = true;
          break;
        }
        if (data[i][j].getImag() != 0.e0) {
          dataOK = true;
          break;
        }
      }
    }
    if (!dataOK) {
      throw new MathsExceptionIllegalArgument("Matrix representation has no non-zero values. Blank matrices are not allowed");
    }

    //get number of elements
    _els = rows * cols;
    double[] dataTmp = new double[2 * _els];
    int[] colPtrTmp = new int[_els + 1];
    int[] rowIdxTmp = new int[_els];

    // we need unwind the array m into CSC form, note the rowIdx reflects the notion that 64bit complex representation is considered a single 128bit index
    // this is so the same data can be shoved into C without problem and the internal representation of a CSC real and CSC complex matrix is the same if the nonzero pattern is the same in each case 
    int ptr = 0, dataptr = 0;
    int i;
    int localMaxEntrisInACol;
    _maxEntriesInAColumn = -1;
    double realEntry = 0, imagEntry = 0;
    for (i = 0; i < cols; i++) {
      colPtrTmp[i] = ptr;
      localMaxEntrisInACol = 0;
      for (int j = 0; j < rows; j++) {
        realEntry = data[j][i].getReal();
        imagEntry = data[j][i].getImag();
        if (realEntry != 0.0e0 || imagEntry != 0.0e0) {
          rowIdxTmp[ptr] = j;
          dataTmp[dataptr] = realEntry;
          dataTmp[dataptr + 1] = imagEntry;
          ptr++;
          dataptr += 2;
          localMaxEntrisInACol++;
        }
      }
      if (localMaxEntrisInACol > _maxEntriesInAColumn) {
        _maxEntriesInAColumn = localMaxEntrisInACol;
      }
    }
    colPtrTmp[i] = ptr;

    // return correct 0 to correct length of the vector buffers
    _data = Arrays.copyOfRange(dataTmp, 0, dataptr);
    _colPtr = Arrays.copyOfRange(colPtrTmp, 0, i + 1); // yes, the +1 is correct, it allows the computation of the number of elements in the final row!
    _rowIdx = Arrays.copyOfRange(rowIdxTmp, 0, ptr);
    _rows = rows;
    _cols = cols;
  }

  /**
   * Construct from underlying CSC representation
   * @param colPtr the columns pointers
   * @param rowIdx the row indexes
   * @param values the underlying values, interleaved [Real,Complex] pairs
   * @param rows the number of rows
   * @param columns the number of columns
   */
  public OGComplexSparseMatrix(int[] colPtr, int[] rowIdx, double[] values, int rows, int columns) {
    Catchers.catchNullFromArgList(colPtr, 1);
    Catchers.catchNullFromArgList(rowIdx, 2);
    Catchers.catchNullFromArgList(values, 3);
    if (rows < 1) {
      throw new MathsExceptionIllegalArgument("Illegal number of rows specified. Value given was " + rows);
    }
    if (columns < 1) {
      throw new MathsExceptionIllegalArgument("Illegal number of columns specified. Value given was " + columns);
    }

    if (columns != colPtr.length - 1) {
      throw new MathsExceptionIllegalArgument("Columns specified does not commute with length of colPtr. Length colPtr (-1)= " + (colPtr.length - 1) + ", columns given = " + columns + ".");
    }

    if (rowIdx.length * 2 != values.length) {
      throw new MathsExceptionIllegalArgument("Insufficient data or rowIdx values given, in complex data layout rowIdx.length*2 = data.length, however the data passed as rowIdx.length*2 = " + 2 *
          rowIdx.length + "values.length = " + values.length + " .");
    }

    // check there is at least some data in the matrix.
    boolean dataOK = false;
    for (int i = 0; i < values.length; i++) {
      if (values[i] != 0.e0) {
        dataOK = true;
        break;
      }
    }
    if (!dataOK) {
      throw new MathsExceptionIllegalArgument("Matrix representation has no non-zero values. Blank matrices are not allowed");
    }

    final int vlen = values.length;
    _data = new double[vlen];
    System.arraycopy(values, 0, _data, 0, vlen);

    final int clen = colPtr.length;
    _colPtr = new int[clen];
    // check colptr is ascending

    if (colPtr[0] < 0) {
      throw new MathsExceptionIllegalArgument("Illegal value in colPtr[0], values should be zero or greater, bad value was " + colPtr[0]);
    }
    _colPtr[0] = colPtr[0];

    for (int i = 1; i < clen; i++) {
      if (colPtr[i] < colPtr[i - 1]) {
        throw new MathsExceptionIllegalArgument("Illegal value in colPtr, values should be ascending or static, descending value referenced at position " + i + ", bad value was " + colPtr[i]);
      }
      _colPtr[i] = colPtr[i];
    }

    final int rlen = rowIdx.length;
    _rowIdx = new int[rlen];
    // check rowIdx isn't out of range whilst copying in
    for (int i = 0; i < rlen; i++) {
      if (rowIdx[i] > rows || rowIdx[i] < 0) {
        throw new MathsExceptionIllegalArgument("Illegal value in rowIdx, row out of range referenced at position " + i + ", bad value was " + rowIdx[i]);
      }
      _rowIdx[i] = rowIdx[i];
    }

    _rows = rows;

    _cols = columns;

    _els = rows * columns;
  }

  /**
   * Construct from underlying CSC representation
   * @param colPtr the columns pointers
   * @param rowIdx the row indexes
   * @param realValues the underlying real part of the values
   * @param imagValues the underlying imaginary part of the values
   * @param rows the number of rows
   * @param columns the number of columns
   */
  public OGComplexSparseMatrix(int[] colPtr, int[] rowIdx, double[] realValues, double[] imagValues, int rows, int columns) {
    Catchers.catchNullFromArgList(colPtr, 1);
    Catchers.catchNullFromArgList(rowIdx, 2);
    Catchers.catchNullFromArgList(realValues, 3);
    Catchers.catchNullFromArgList(imagValues, 4);
    if (rows < 1) {
      throw new MathsExceptionIllegalArgument("Illegal number of rows specified. Value given was " + rows);
    }
    if (columns < 1) {
      throw new MathsExceptionIllegalArgument("Illegal number of columns specified. Value given was " + columns);
    }

    if (columns != colPtr.length - 1) {
      throw new MathsExceptionIllegalArgument("Columns specified does not commute with length of colPtr. Length colPtr (-1)= " + (colPtr.length - 1) + ", columns given = " + columns + ".");
    }

    if (rowIdx.length != realValues.length) {
      throw new MathsExceptionIllegalArgument("Insufficient data or rowIdx values given, they should be the same but got rowIdx.length=" + rowIdx.length + "values.length=" + realValues.length + " .");
    }

    if (realValues.length != imagValues.length) {
      throw new MathsExceptionIllegalArgument("The number of real values passed as \"realValues\" must match the number of imaginary values, passed as \"imagValues\"");
    }

    // check there is at least some data in the matrix.
    boolean dataOK = false;
    for (int i = 0; i < realValues.length; i++) {
      if (realValues[i] != 0.e0) {
        dataOK = true;
        break;
      }
      if (imagValues[i] != 0.e0) {
        dataOK = true;
        break;
      }
    }
    if (!dataOK) {
      throw new MathsExceptionIllegalArgument("Matrix representation has no non-zero values. Blank matrices are not allowed");
    }

    _data = DenseMemoryManipulation.convertTwoSinglePointersToInterleavedSinglePointer(realValues, imagValues);

    final int clen = colPtr.length;
    _colPtr = new int[clen];
    // check colptr is ascending

    if (colPtr[0] < 0) {
      throw new MathsExceptionIllegalArgument("Illegal value in colPtr[0], values should be zero or greater, bad value was " + colPtr[0]);
    }
    _colPtr[0] = colPtr[0];

    for (int i = 1; i < clen; i++) {
      if (colPtr[i] < colPtr[i - 1]) {
        throw new MathsExceptionIllegalArgument("Illegal value in colPtr, values should be ascending or static, descending value referenced at position " + i + ", bad value was " + colPtr[i]);
      }
      _colPtr[i] = colPtr[i];
    }

    final int rlen = rowIdx.length;
    _rowIdx = new int[rlen];
    // check rowIdx isn't out of range whilst copying in
    for (int i = 0; i < rlen; i++) {
      if (rowIdx[i] > rows || rowIdx[i] < 0) {
        throw new MathsExceptionIllegalArgument("Illegal value in rowIdx, row out of range referenced at position " + i + ", bad value was " + rowIdx[i]);
      }
      _rowIdx[i] = rowIdx[i];
    }

    _rows = rows;

    _cols = columns;

    _els = rows * columns;
  }

  @Override
  public ExprEnum getType() {
    return s_type;
  }

  /**
   * Gets the data.
   * @return the data
   */
  public double[] getData() {
    return _data;
  }

  /**
   * Gets the colPtr.
   * @return the colPtr
   */
  public int[] getColPtr() {
    return _colPtr;
  }

  /**
   * Gets the rowIdx.
   * @return the rowIdx
   */
  public int[] getRowIdx() {
    return _rowIdx;
  }

  /**
   * Gets the rows.
   * @return the rows
   */
  public int getRows() {
    return _rows;
  }

  /**
   * Gets the cols.
   * @return the cols
   */
  public int getCols() {
    return _cols;
  }

  @Override
  public String toString() {
    StringBuffer sb = new StringBuffer();
    Formatter formatter = new Formatter(sb);
    double abstmp;
    sb.append("OGComplexSparseMatrix:\n");
    for (int ir = 0; ir < _cols; ir++) {
      for (int i = _colPtr[ir]; i < _colPtr[ir + 1]; i++) {
        sb.append("(");
        sb.append(_rowIdx[i]);
        sb.append(",");
        sb.append(ir);
        sb.append(") = ");
        formatter.format("%24.18f", _data[2 * i]);
        abstmp = (_data[2 * i + 1]);
        if (abstmp < 0) {
          sb.append("-");
        } else {
          sb.append("    +");
        }
        formatter.format("%24.18f", Math.abs(abstmp));
        sb.append("i ");
        sb.append("\n");
      }
    }
    formatter.close();
    return "\nvalues=" + Arrays.toString(_data) + "\nrowInd=" + Arrays.toString(_rowIdx) + "\ncolPtr=" + Arrays.toString(_colPtr) + "\ncols=" + _cols + "\nrows=" + _rows + "\nels=" + _els + "\n" +
        sb.toString();

  }

}
