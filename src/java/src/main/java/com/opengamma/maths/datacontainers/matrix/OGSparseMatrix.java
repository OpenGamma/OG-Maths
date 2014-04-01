/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.datacontainers.matrix;

import java.util.Arrays;

import com.opengamma.maths.datacontainers.OGTerminal;

/**
 * Sparse matrix super type
 */
public abstract class OGSparseMatrix extends OGArray {

  /**
   * Gets the colPtr.
   * @return the colPtr
   */
  public abstract int[] getColPtr();

  /**
   * Gets the rowIdx.
   * @return the rowIdx
   */
  public abstract int[] getRowIdx();

  @Override
  public int hashCode() {
    final int prime = 31;
    int result = super.hashCode();
    result = prime * result + Arrays.hashCode(this.getColPtr());
    result = prime * result + Arrays.hashCode(this.getRowIdx());
    return result;
  }

  @Override
  public boolean equals(final Object obj) {
    if (!super.equals(obj)) {
      return false;
    } else {
      OGSparseMatrix other = (OGSparseMatrix) obj;
      if (!Arrays.equals(this.getColPtr(), other.getColPtr())) {
        return false;
      }
      if (!Arrays.equals(this.getRowIdx(), other.getRowIdx())) {
        return false;
      }
    }
    return true;
  }

  @Override
  public boolean fuzzyequals(OGTerminal term, double maxabserror, double maxrelerror) {
    if (!super.fuzzyequals(term, maxabserror, maxrelerror)) {
      return false;
    } else {
      OGSparseMatrix other = (OGSparseMatrix) term;
      if (!Arrays.equals(this.getColPtr(), other.getColPtr())) {
        return false;
      }
      if (!Arrays.equals(this.getRowIdx(), other.getRowIdx())) {
        return false;
      }
    }
    return true;
  }
}
