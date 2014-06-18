/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.helpers;

import com.opengamma.maths.datacontainers.OGTerminal;

/**
 * Contains is() methods.
 */
public class Iss {

  /**
   * Default ctor.
   */
  private Iss() {
  };

  /**
   * Check if a terminal is a type that can represent numbers in complex space.
   * @param terminal the terminal to check.
   * @return true if {@code terminal} can support complex numbers, false else.
   */
  public static boolean isComplex(final OGTerminal terminal) {
    return !isReal(terminal);
  }

  /**
   * Check if a terminal is a type that can only represent real numbers.
   * @param terminal the terminal to check.
   * @return true if {@code terminal} can only support real numbers, false else.
   */
  public static boolean isReal(final OGTerminal terminal) {
    switch (terminal.getType()) {
      case OGRealScalar:
      case OGLogicalMatrix:
      case OGRealDenseMatrix:
      case OGRealDiagonalMatrix:
      case OGRealSparseMatrix:
      case OGIntegerScalar:
        return true;
      default:
        return false;
    }
  }
}
