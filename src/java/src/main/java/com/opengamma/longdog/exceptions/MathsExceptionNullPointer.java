/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.longdog.exceptions;

/**
 * Provides a manner in which maths exceptions relating to null pointer access can be thrown
 */
public class MathsExceptionNullPointer extends MathsException {

  /**
   * Serial ID
   */
  private static final long serialVersionUID = 192180225138562639L;

  public MathsExceptionNullPointer() {
    super();
  }

  public MathsExceptionNullPointer(final String s) {
    super(s + " points to null.");
  }

  public MathsExceptionNullPointer(final String s, final Throwable cause) {
    super(s + " points to null.", cause);
  }

  public MathsExceptionNullPointer(final Throwable cause) {
    super(cause);
  }
}
