/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.longdog.exceptions;

/**
 * Provides a manner in which maths exceptions relating to null pointer access can be thrown.
 */
public class MathsException extends RuntimeException {
  private static final long serialVersionUID = 1L;

  public MathsException() {
    super();
  }

  public MathsException(final String s) {
    super(s);
  }

  public MathsException(final String s, final Throwable cause) {
    super(s, cause);
  }

  public MathsException(final Throwable cause) {
    super(cause);
  }
}
