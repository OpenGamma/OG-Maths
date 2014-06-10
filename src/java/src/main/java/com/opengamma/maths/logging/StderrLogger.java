/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.logging;

/**
 * Logger implementation that prints all messages to stderr.
 */
public class StderrLogger implements ILogger {

  /**
   * Name of the class we're logging for
   */
  private String _name;

  /**
   * Create a standard error logger for the given class
   * @param c The class for which log messages are recorded
   */
  public StderrLogger(Class c) {
    _name = c.getName();
  }

  private void log(String level, String msg) {
    System.err.println(level + " " + _name + ": " + msg);
  }

  /**
   * {@inheritDoc}
   */
  @Override
  public void info(String msg) {
    log("INFO", msg);
  }

  /**
   * {@inheritDoc}
   */
  @Override
  public void debug(String msg) {
    log("DEBUG", msg);
  }

  /**
   * {@inheritDoc}
   */
  @Override
  public void warn(String msg) {
    log("WARN", msg);
  }

  /**
   * {@inheritDoc}
   */
  @Override
  public void error(String msg) {
    log("ERROR", msg);
  }
}
