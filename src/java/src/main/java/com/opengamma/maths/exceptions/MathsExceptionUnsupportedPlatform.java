/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.exceptions;

/**
 * Exception thrown if the discovered platform is unsupported.
 */
public class MathsExceptionUnsupportedPlatform extends RuntimeException {

  /**
   * Serial ID
   */
  private static final long serialVersionUID = -1105413990531662184L;

  public MathsExceptionUnsupportedPlatform() {
    super();
  }

  public MathsExceptionUnsupportedPlatform(final String s) {
    super(s);
  }

  public MathsExceptionUnsupportedPlatform(final String s, final Throwable cause) {
    super(s, cause);
  }

  public MathsExceptionUnsupportedPlatform(final Throwable cause) {
    super(cause);
  }

  public MathsExceptionUnsupportedPlatform(String message, Throwable cause,
      boolean enableSuppression, boolean writableStackTrace) {
    super(message, cause, enableSuppression, writableStackTrace);
  }
}
