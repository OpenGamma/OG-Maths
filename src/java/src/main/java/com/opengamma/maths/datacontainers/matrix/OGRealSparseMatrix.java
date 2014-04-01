/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.datacontainers.matrix;

import java.util.Arrays;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.helpers.Catchers;
import com.opengamma.maths.helpers.MatrixPrimitiveUtils;

/**
 * Real sparse matrix type
 */
public class OGRealSparseMatrix extends OGSparseMatrix {

  private static ExprEnum s_type = ExprEnum.OGRealSparseMatrix;

  private double[] _data;
  private int[] _colPtr;
  private int[] _rowIdx;
  private int _els;
  private int _rows;
  private int _cols;
  private int _maxEntriesInAColumn;

  /* constructors */

  /**
   * Construct from double[][]
   * @param matrix is a double[][]
   */
  public OGRealSparseMatrix(double[][] matrix) {
    Catchers.catchNullFromArgList(matrix, 1);
    if (MatrixPrimitiveUtils.isRagged(matrix)) {
      throw new MathsExceptionIllegalArgument("Matrix representation must not be ragged, i.e. all rows must be the same length");
    }

    // check there is at least some data in the matrix.
    boolean dataOK = false;
    for (int i = 0; i < matrix.length; i++) {
      for (int j = 0; j < matrix[i].length; j++) {
        if (matrix[i][j] != 0.e0) {
          dataOK = true;
        }
      }
    }
    if (!dataOK) {
      throw new MathsExceptionIllegalArgument("Matrix representation has no non-zero values. Blank matrices are not allowed");
    }

    final int rows = matrix.length;
    final int cols = matrix[0].length;

    //get number of elements
    _els = rows * cols;

    // tmp arrays, in case we get in a fully populated matrix, intelligent design upstream should ensure that this is overkill!
    double[] dataTmp = new double[_els];
    int[] colPtrTmp = new int[_els + 1];
    int[] rowIdxTmp = new int[_els];

    // we need unwind the array m into coordinate form
    int ptr = 0;
    int i;
    int localMaxEntrisInACol;
    _maxEntriesInAColumn = -1;
    double entry = 0;
    for (i = 0; i < cols; i++) {
      colPtrTmp[i] = ptr;
      localMaxEntrisInACol = 0;
      for (int j = 0; j < rows; j++) {
        entry = matrix[j][i];
        if (entry != 0.0e0) {
          rowIdxTmp[ptr] = j;
          dataTmp[ptr] = entry;
          ptr++;
          localMaxEntrisInACol++;
        }
      }
      if (localMaxEntrisInACol > _maxEntriesInAColumn) {
        _maxEntriesInAColumn = localMaxEntrisInACol;
      }
    }
    colPtrTmp[i] = ptr;

    // return correct 0 to correct length of the vector buffers
    _data = Arrays.copyOfRange(dataTmp, 0, ptr);
    _colPtr = Arrays.copyOfRange(colPtrTmp, 0, i + 1); // yes, the +1 is correct, it allows the computation of the number of elements in the final row!
    _rowIdx = Arrays.copyOfRange(rowIdxTmp, 0, ptr);
    _rows = rows;
    _cols = cols;
  }

  /**
   * Construct from underlying CSC representation
   * @param colPtr the columns pointers
   * @param rowIdx the row indexes
   * @param values the underlying values
   * @param rows the number of rows
   * @param columns the number of columns
   */
  public OGRealSparseMatrix(int[] colPtr, int[] rowIdx, double[] values, int rows, int columns) {
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

    if (rowIdx.length != values.length) {
      throw new MathsExceptionIllegalArgument("Insufficient data or rowIdx values given, they should be the same but got rowIdx.length=" + rowIdx.length + "values.length=" + values.length + " .");
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

  @Override
  public ExprEnum getType() {
    return s_type;
  }

  /**
   * Gets the colPtr.
   * @return the colPtr
   */
  @Override
  public int[] getColPtr() {
    return _colPtr;
  }

  /**
   * Gets the rowIdx.
   * @return the rowIdx
   */
  @Override
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

  /**
   * Gets the data.
   * @return the data
   */
  public double[] getData() {
    return _data;
  }

  @Override
  public String toString() {
    String str = "OGRealSparseMatrix:\n";
    return str + "\nvalues=" + Arrays.toString(_data) + "\nrowInd=" + Arrays.toString(_rowIdx) + "\ncolPtr=" + Arrays.toString(_colPtr) + "\ncols=" + _cols + "\nrows=" + _rows + "\nels=" + _els;
  }

  @Override
  protected OGRealDenseMatrix asOGRealDenseMatrix() {
    final int rows = this.getRows();
    final int cols = this.getCols();
    final int[] colPtr = this.getColPtr();
    final int[] rowIdx = this.getRowIdx();
    final double[] data = this.getData();
    double[] tmp = new double[rows * cols];
    for (int ir = 0; ir < cols; ir++) {
      for (int i = colPtr[ir]; i <= colPtr[ir + 1] - 1; i++) {
        tmp[rowIdx[i] + ir * rows] = data[i];
      }
    }
    return new OGRealDenseMatrix(tmp, rows, cols);

  }

  @Override
  protected OGComplexDenseMatrix asOGComplexDenseMatrix() {
    final int rows = this.getRows();
    final int cols = this.getCols();
    final int[] colPtr = this.getColPtr();
    final int[] rowIdx = this.getRowIdx();
    final double[] data = this.getData();
    double[] tmp = new double[2 * rows * cols];
    for (int ir = 0; ir < cols; ir++) {
      for (int i = colPtr[ir]; i < colPtr[ir + 1]; i++) {
        tmp[2 * (rowIdx[i] + ir * rows)] = data[i];
      }
    }
    return new OGComplexDenseMatrix(tmp, rows, cols);

  }

}
