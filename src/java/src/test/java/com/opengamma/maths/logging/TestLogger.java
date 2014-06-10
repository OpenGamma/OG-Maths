/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.logging;

import org.testng.annotations.BeforeClass;
import org.testng.annotations.Test;

public class TestLogger {

  private Logger _logger;

  @BeforeClass
  public void setUp() {
    _logger = new Logger(TestLogger.class);
  }

  @Test
  public void testDebug() {
    _logger.debug("Debug message");
  }

  @Test
  public void testInfo() {
    _logger.info("Info message");
  }

  @Test
  public void testWarn() {
    _logger.warn("Warn message");
  }

  @Test
  public void testError() {
    _logger.error("Error message");
  }
}
