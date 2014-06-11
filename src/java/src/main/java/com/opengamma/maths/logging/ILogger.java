/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.logging;

/**
 * Interface for logging implementations in OG-Maths
 */
public interface ILogger {
  /**
   * Log at the DEBUG log level
   * @param msg The log message
   */
  public void debug(String msg);

  /**
   * Log at the INFO log level
   * @param msg The log message
   */
  public void info(String msg);

  /**
   * Log at the WARN log level
   * @param msg The log message
   */
  public void warn(String msg);

  /**
   * Log at the ERROR log level
   * @param msg The log message
   */
  public void error(String msg);

}
