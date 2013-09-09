/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.exceptions;

/**
 * Exception thrown if native code initialisation fails.
 */
public class LongdogInitializationException extends RuntimeException {

  /**
   * SID
   */
  private static final long serialVersionUID = 1L;

  public LongdogInitializationException() {
    super();
  }

  public LongdogInitializationException(String message) {
    super(message);
  }

  public LongdogInitializationException(Throwable cause) {
    super(cause);
  }

  public LongdogInitializationException(String message, Throwable cause) {
    super(message, cause);
  }

  public LongdogInitializationException(String message, Throwable cause,
      boolean enableSuppression, boolean writableStackTrace) {
    super(message, cause, enableSuppression, writableStackTrace);
  }

}
