/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.longdog.exceptions;

/**
 * Provides a manner in which maths exceptions relating to data non conformance can be thrown
 */
public class MathsExceptionNonConformance extends MathsException {
  private static final long serialVersionUID = 1L;

  public MathsExceptionNonConformance() {
    super();
  }

  public MathsExceptionNonConformance(final String s) {
    super(s + " points to null.");
  }

  public MathsExceptionNonConformance(final String s, final Throwable cause) {
    super(s + " points to null.", cause);
  }

  public MathsExceptionNonConformance(final Throwable cause) {
    super(cause);
  }
}
