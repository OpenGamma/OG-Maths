/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.datacontainers;

import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.helpers.FuzzyEquals;
import com.opengamma.maths.helpers.Iss;

/**
 * Holds a terminal node in the AST (OGArray or OGScalar)
 */
public abstract class OGTerminal implements OGNumeric {

  /**
   * Gets the data held in the OGTerminal node
   * @return the data
   */
  public abstract double[] getData();

  /**
   * Compare this with another terminal to see if they are "fuzzy" equal, this
   * kind of equals is intended to account for floating point error.
   *
   * Default values are used for "fuzzy" equals on floating point numbers.
   *
   * @param term the terminal against which the comparison should be made.
   * @return true if this object is fuzzy equal to the other.
   */
  public final boolean fuzzyequals(OGTerminal term) {
    return this.fuzzyequals(term, FuzzyEquals.getDefaultTolerance(), FuzzyEquals.getDefaultTolerance());
  }

  /**
   * Compare this with another terminal to see if they are "fuzzy" equal, this
   * kind of equals is intended to account for floating point error.
   * @param term the terminal against which the comparison should be made.
   * @param maxabserror determines the minimum threshold for "equal" in terms of
   * the two numbers being very small in magnitude.
   * @param maxrelerror determines the minimum threshold for "equal" in terms of
   * the relative magnitude of the numbers.
   * @return true if this terminal is fuzzy equal to the other.
   */
  public abstract boolean fuzzyequals(OGTerminal term, double maxabserror, double maxrelerror);

  /**
   * Compare this with another terminal to see if they are mathematically equal,
   * this kind of equals is intended to account for floating point error AND to
   * perform type conversion if necessary. The type conversion is to account for
   * different data layouts that mean objects are mathematically the same but
   * different in their representation. An example of this would be a real dense
   * matrix with only the diagonal nonzero being mathematically equal to a real
   * diagonal matrix with the same numbers on the diagonal.
   * @param term the terminal against which the comparison should be made.
   * @return true if this object is mathematically equal to the other.
   */
  public final boolean mathsequals(OGTerminal term) {
    return this.mathsequals(term, FuzzyEquals.getDefaultTolerance(), FuzzyEquals.getDefaultTolerance());
  }

  /**
   * Compare this with another terminal to see if they are mathematically equal,
   * this kind of equals is intended to account for floating point error AND to
   * perform type conversion if necessary. The type conversion is to account for
   * different data layouts that mean objects are mathematically the same but
   * different in their representation. An example of this would be a real dense
   * matrix with only the diagonal nonzero being mathematically equal to a real
   * diagonal matrix with the same numbers on the diagonal.
   *
   * Default values are used for "fuzzy" equals on floating point numbers.
   *
   * @param term the terminal against which the comparison should be made.
   * @param maxabserror determines the minimum threshold for "equal" in terms of
   * the two numbers being very small in magnitude.
   * @param maxrelerror determines the minimum threshold for "equal" in terms of
   * the relative magnitude of the numbers.
   * @return true if this object is mathematically equal to the other.
   */
  public boolean mathsequals(OGTerminal term, double maxabserror, double maxrelerror) {
    OGTerminal convthis = null;
    OGTerminal convother = null;
    if (Iss.isComplex(this) || Iss.isComplex(term)) {
      convthis = this.asOGComplexDenseMatrix();
      convother = term.asOGComplexDenseMatrix();
    } else {
      convthis = this.asOGRealDenseMatrix();
      convother = term.asOGRealDenseMatrix();
    }
    return convthis.fuzzyequals(convother, maxabserror, maxrelerror);
  }

  /**
   * Convert this terminal type to an OGRealDenseMatrix.
   * @return the OGRealDenseMatrix representation of this type.
   */
  protected OGRealDenseMatrix asOGRealDenseMatrix() {
    throw new MathsExceptionIllegalArgument("Cannot convert complex space type to real space");
  }

  /**
   * Convert this terminal type to an OGComplexDenseMatrix.
   * @return the OGComplexDenseMatrix representation of this type.
   */
  protected abstract OGComplexDenseMatrix asOGComplexDenseMatrix();

  @Override
  public abstract boolean equals(final Object obj);

  /**
   * Functionality similar to {@code toString()}, but with added debugging
   * output appropriate for the class.
   * @return a string containing debugging output.
   */
  public abstract String toDebugString();

}
