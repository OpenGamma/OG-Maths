/**
 * Copyright (C) 2012 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.longdog.helpers;

/**
 * Provides a manner in which maths exceptions relating illegal arguments can be thrown
 */
public class MathsExceptionIllegalArgument extends MathsException {
  private static final long serialVersionUID = 1L;

  public MathsExceptionIllegalArgument() {
    super();
  }

  public MathsExceptionIllegalArgument(final String s) {
    super(s + " points to null.");
  }

  public MathsExceptionIllegalArgument(final String s, final Throwable cause) {
    super(s + " points to null.", cause);
  }

  public MathsExceptionIllegalArgument(final Throwable cause) {
    super(cause);
  }
}
