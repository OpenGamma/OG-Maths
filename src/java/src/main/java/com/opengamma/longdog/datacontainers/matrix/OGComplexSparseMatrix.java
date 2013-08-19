/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.datacontainers.matrix;

import java.util.Arrays;
import java.util.Formatter;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;
import com.opengamma.longdog.helpers.Catchers;
import com.opengamma.longdog.helpers.MathsException;
import com.opengamma.longdog.helpers.MatrixPrimitiveUtils;

/**
 * Complex sparse matrix type
 */
public class OGComplexSparseMatrix extends OGSparseMatrix {

  private static ExprTypeEnum s_type = ExprTypeEnum.OGComplexSparseMatrix;

  private double[] _data;
  private int[] _colPtr;
  private int[] _rowIdx;
  private int _els;
  private int _rows;
  private int _cols;
  private int _maxEntriesInAColumn;

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
      throw new MathsException("Backing real array is ragged");
    }
    if (MatrixPrimitiveUtils.isRagged(imaginaryPart)) {
      throw new MathsException("Backing imaginary array is ragged");
    }
    int rows = realPart.length;
    int cols = realPart[0].length;

    if (imaginaryPart.length != rows) {
      throw new MathsException("Number of rows in first array (" + rows + ") does not match number of rows in second (" + imaginaryPart.length + ")");
    }
    if (imaginaryPart[0].length != cols) {
      throw new MathsException("Number of columns in first array (" + cols + ") does not match number of columns in second (" + imaginaryPart[0].length + ")");
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
      throw new MathsException("Matrix representation has no non-zero values. Blank matrices are not allowed");
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

  @Override
  public ExprTypeEnum getType() {
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
