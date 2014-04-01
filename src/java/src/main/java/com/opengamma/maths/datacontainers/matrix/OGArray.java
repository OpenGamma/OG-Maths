/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.datacontainers.matrix;

import java.util.Arrays;

import com.opengamma.maths.datacontainers.OGTerminal;
import com.opengamma.maths.helpers.FuzzyEquals;

/**
 * Array base class from which all arrays inherit.
 */
public abstract class OGArray extends OGTerminal {

  /**
   * Gets the rows.
   * @return the rows
   */
  public abstract int getRows();

  /**
   * Gets the columns.
   * @return the columns
   */
  public abstract int getCols();

  @Override
  public boolean equals(Object obj) {
    if (this == obj) {
      return true;
    }
    if (getClass() != obj.getClass()) {
      return false;
    }
    OGArray other = (OGArray) obj;
    if (other.getRows() != this.getRows()) {
      return false;
    }
    if (other.getCols() != this.getCols()) {
      return false;
    }
    if (!Arrays.equals(this.getData(), other.getData())) {
      return false;
    }
    return true;
  }

  @Override
  public int hashCode() {
    final int prime = 31;
    int result = 1;
    result = prime * result + this.getCols();
    result = prime * result + Arrays.hashCode(this.getData());
    result = prime * result + this.getRows();
    return result;
  }

  @Override
  public boolean fuzzyequals(OGTerminal term, double maxabserror, double maxrelerror) {
    if (this == term) {
      return true;
    }
    if (getClass() != term.getClass()) {
      return false;
    }
    OGArray other = (OGArray) term;
    if (other.getRows() != this.getRows()) {
      return false;
    }
    if (other.getCols() != this.getCols()) {
      return false;
    }
    if (!FuzzyEquals.ArrayFuzzyEquals(this.getData(), other.getData(), maxabserror, maxrelerror)) {
      return false;
    }
    return true;
  }
  
}
