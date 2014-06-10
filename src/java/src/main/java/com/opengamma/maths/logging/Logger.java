/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.logging;

/**
 * The main logging class for OG-Maths.
 *
 * In order to allow OG-Maths to have no dependencies, we provide a wrapper
 * around logging functionality in this class. If SLF4J is available, it is
 * used. Otherwise, logging messages are written to stderr.
 */
public class Logger {

  /**
   * The logger implementation
   */
  private ILogger _logger;
  /**
   * Whether we managed to find SLF4J on the classpath
   */
  static boolean s_slf4j_available;

  static {
    ClassLoader classLoader = Logger.class.getClassLoader();
    try {
      Class slf4jLogger = classLoader.loadClass("org.slf4j.Logger");
      Class slf4jLoggerFactory = classLoader.loadClass("org.slf4j.LoggerFactory");
      s_slf4j_available = true;
    } catch (ClassNotFoundException e) {
      System.err.println("WARN: SLF4J not found - logging to stderr");
    }
  }

  /**
   * Construct a logger for the specified class
   * @param c The class from which log messages will appear.
   */
  public Logger(Class c) {
    if (s_slf4j_available) {
      _logger = new Slf4jLogger(c);
    } else {
      _logger = new StderrLogger(c);
    }
  }

  /**
   * Log at the DEBUG log level
   * @param msg The log message
   */
  public void debug(String msg) {
    _logger.debug(msg);
  }

  /**
   * Log at the INFO log level
   * @param msg The log message
   */
  public void info(String msg) {
    _logger.info(msg);
  }

  /**
   * Log at the WARN log level
   * @param msg The log message
   */
  public void warn(String msg) {
    _logger.warn(msg);
  }

  /**
   * Log at the ERROR log level
   * @param msg The log message
   */
  public void error(String msg) {
    _logger.error(msg);
  }
}
