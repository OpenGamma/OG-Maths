/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.exceptions;

/**
 * Provides a manner in which maths exceptions relating to native code errors that occur
 * outside of those that are part of DOGMA - e.g. bad_alloc, etc.
 */
public class MathsExceptionNativeUnspecified extends MathsExceptionNative {

  /**
   * Serial ID
   */
  private static final long serialVersionUID = 1873841862632698981L;

  /**
   * This form of the constructor is used on all OSes, since backtraces cannot be
   * provided for unspecified exceptions.
   * @param msg the error message
   */
  public MathsExceptionNativeUnspecified(String msg) {
    super(msg);
  }


}
