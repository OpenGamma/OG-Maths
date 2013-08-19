/**
 * Copyright (C) 2012 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.longdog.helpers;

/**
 * Provides a manner in which maths exceptions relating to null pointer access can be thrown
 */
public class MathsException extends RuntimeException {
  private static final long serialVersionUID = 1L;

  public MathsException() {
    super();
  }

  public MathsException(final String s) {
    super(s + " points to null.");
  }

  public MathsException(final String s, final Throwable cause) {
    super(s + " points to null.", cause);
  }

  public MathsException(final Throwable cause) {
    super(cause);
  }
}
