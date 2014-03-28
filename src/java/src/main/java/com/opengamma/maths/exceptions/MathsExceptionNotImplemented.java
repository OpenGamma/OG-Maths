/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.maths.exceptions;

/**
 * Provides a manner in which maths exceptions relating to not implemented features can be thrown
 */
public class MathsExceptionNotImplemented extends MathsException {

  /**
   * Serial ID
   */
  private static final long serialVersionUID = -4513146658517029099L;

  public MathsExceptionNotImplemented() {
    super();
  }

  public MathsExceptionNotImplemented(final String s) {
    super(s + " points to null.");
  }

  public MathsExceptionNotImplemented(final String s, final Throwable cause) {
    super(s + " points to null.", cause);
  }

  public MathsExceptionNotImplemented(final Throwable cause) {
    super(cause);
  }
}
