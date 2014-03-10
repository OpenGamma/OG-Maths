/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.exceptions;

/**
 * Provides a manner in which maths exceptions relating to native code errors during the conversion from Java ASTs to C++ ASTs
 */
public class MathsExceptionNativeConversion extends MathsExceptionNative {
  public MathsExceptionNativeConversion(String msg, String[] backtraceInfo) {
    super(msg, backtraceInfo);
  }

  public MathsExceptionNativeConversion() {
    super();
  }

  public MathsExceptionNativeConversion(final String s) {
    super(s);
  }

  public MathsExceptionNativeConversion(final String s, final Throwable cause) {
    super(s, cause);
  }

  public MathsExceptionNativeConversion(final Throwable cause) {
    super(cause);
  }
}
