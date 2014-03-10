/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.exceptions;

/**
 * Provides a manner in which maths exceptions relating to execution in native code can be thrown.
 */
public class MathsExceptionNativeComputation extends MathsExceptionNative {
  public MathsExceptionNativeComputation(String msg, String[] backtraceInfo) {
    super(msg, backtraceInfo);
  }
}
