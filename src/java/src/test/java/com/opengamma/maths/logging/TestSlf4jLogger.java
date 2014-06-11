/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.logging;

import org.testng.annotations.BeforeClass;
import org.testng.annotations.Test;

public class TestSlf4jLogger {

  private Slf4jLogger _slf4jLogger;

  @BeforeClass
  public void setUp() {
    _slf4jLogger = new Slf4jLogger(TestSlf4jLogger.class);
  }

  @Test
  public void testDebug() {
    _slf4jLogger.debug("Debug message");
  }

  @Test
  public void testInfo() {
    _slf4jLogger.info("Info message");
  }

  @Test
  public void testWarn() {
    _slf4jLogger.warn("Warn message");
  }

  @Test
  public void testError() {
    _slf4jLogger.error("Error message");
  }
}
