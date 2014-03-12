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

  /**
   *  Serial ID
   */
  private static final long serialVersionUID = -5343624439409841980L;

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
