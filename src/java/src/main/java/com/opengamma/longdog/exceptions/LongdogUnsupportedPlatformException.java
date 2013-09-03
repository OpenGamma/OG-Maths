/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.exceptions;

/**
 * Exception thrown if the discovered platform is unsupported.
 */
public class LongdogUnsupportedPlatformException extends RuntimeException {

  private static final long serialVersionUID = 1L;

  public LongdogUnsupportedPlatformException() {
    super();
  }

  public LongdogUnsupportedPlatformException(final String s) {
    super(s);
  }

  public LongdogUnsupportedPlatformException(final String s, final Throwable cause) {
    super(s, cause);
  }

  public LongdogUnsupportedPlatformException(final Throwable cause) {
    super(cause);
  }

  public LongdogUnsupportedPlatformException(String message, Throwable cause,
      boolean enableSuppression, boolean writableStackTrace) {
    super(message, cause, enableSuppression, writableStackTrace);
  }
}
