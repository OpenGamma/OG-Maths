/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.logging;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Logger implementation that logs all messages using SLF4J.
 */
public class Slf4jLogger implements ILogger {

  /**
   * The SLF4J Logger instance
   */
  private Logger _log;

  /**
   * Create an SLF4J logger for a given class
   * @param c The class for which log messages are recorded
   */
  public Slf4jLogger(Class c) {
    _log = LoggerFactory.getLogger(c);
  }

  /**
   * {@inheritDoc}
   */
  @Override
  public void info(String msg) {
    _log.info(msg);
  }

  /**
   * {@inheritDoc}
   */
  @Override
  public void debug(String msg) {
    _log.debug(msg);
  }

  /**
   * {@inheritDoc}
   */
  @Override
  public void warn(String msg) {
    _log.warn(msg);
  }

  /**
   * {@inheritDoc}
   */
  @Override
  public void error(String msg) {
    _log.error(msg);
  }
}
