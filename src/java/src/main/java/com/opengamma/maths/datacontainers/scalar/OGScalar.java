/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.datacontainers.scalar;

import java.util.Arrays;

import com.opengamma.maths.datacontainers.OGTerminal;
import com.opengamma.maths.helpers.FuzzyEquals;

/**
 * Scalar Numeric class
 */
public abstract class OGScalar extends OGTerminal {

  @Override
  public final boolean equals(Object obj) {
    if (this == obj)
      return true;
    if (getClass() != obj.getClass())
      return false;
    OGScalar other = (OGScalar) obj;
    if (!Arrays.equals(this.getData(), other.getData()))
      return false;
    return true;
  }

  @Override
  public final boolean fuzzyequals(OGTerminal term, double maxabserror, double maxrelerror) {
    if (this == term)
      return true;
    if (getClass() != term.getClass())
      return false;
    OGScalar other = (OGScalar) term;
    if (!FuzzyEquals.ArrayFuzzyEquals(this.getData(), other.getData(), maxabserror, maxrelerror))
      return false;
    return true;
  }

  @Override
  public final int hashCode() {
    final int prime = 31;
    int result = 1;
    result = prime * result + Arrays.hashCode(this.getData());
    return result;
  }

}
