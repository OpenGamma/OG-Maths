/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.exceptions;

/**
 * Exception thrown if native code initialisation fails.
 */
public class MathsExceptionOnInitialization extends RuntimeException {

  /**
   * Serial ID
   */
  private static final long serialVersionUID = -8519882973848004168L;

  public MathsExceptionOnInitialization() {
    super();
  }

  public MathsExceptionOnInitialization(String message) {
    super(message);
  }

  public MathsExceptionOnInitialization(Throwable cause) {
    super(cause);
  }

  public MathsExceptionOnInitialization(String message, Throwable cause) {
    super(message, cause);
  }

  public MathsExceptionOnInitialization(String message, Throwable cause,
      boolean enableSuppression, boolean writableStackTrace) {
    super(message, cause, enableSuppression, writableStackTrace);
  }

}
