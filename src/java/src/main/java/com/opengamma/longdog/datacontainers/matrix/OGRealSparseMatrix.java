/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.datacontainers.matrix;

import java.util.Arrays;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;
import com.opengamma.longdog.helpers.Catchers;
import com.opengamma.longdog.helpers.MathsException;
import com.opengamma.longdog.helpers.MatrixPrimitiveUtils;

/**
 * Real sparse matrix type
 */
public class OGRealSparseMatrix extends OGSparseMatrix {

  private static ExprTypeEnum s_type = ExprTypeEnum.OGRealSparseMatrix;

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
      throw new MathsException("Matrix representation must not be ragged, i.e. all rows must be the same length");
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
      throw new MathsException("Matrix representation has no non-zero values. Blank matrices are not allowed");
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

  @Override
  public ExprTypeEnum getType() {
    return s_type;
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

}
