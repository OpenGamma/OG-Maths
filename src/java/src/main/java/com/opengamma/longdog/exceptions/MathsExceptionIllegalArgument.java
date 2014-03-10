/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.longdog.exceptions;

/**
 * Provides a manner in which maths exceptions relating illegal arguments can be thrown
 */
public class MathsExceptionIllegalArgument extends MathsException {

  /**
   * Serial ID
   */
  private static final long serialVersionUID = -5130462724654601466L;

  public MathsExceptionIllegalArgument() {
    super();
  }

  public MathsExceptionIllegalArgument(final String s) {
    super(s);
  }

  public MathsExceptionIllegalArgument(final String s, final Throwable cause) {
    super(s, cause);
  }

  public MathsExceptionIllegalArgument(final Throwable cause) {
    super(cause);
  }
}
