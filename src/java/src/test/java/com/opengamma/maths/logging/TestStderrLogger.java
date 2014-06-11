/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.logging;

import org.testng.annotations.BeforeClass;
import org.testng.annotations.Test;

public class TestStderrLogger {

  private StderrLogger _stderrLogger;

  @BeforeClass
  public void setUp() {
    _stderrLogger = new StderrLogger(TestStderrLogger.class);
  }

  @Test
  public void testDebug() {
    _stderrLogger.debug("Debug message");
  }

  @Test
  public void testInfo() {
    _stderrLogger.info("Info message");
  }

  @Test
  public void testWarn() {
    _stderrLogger.warn("Warn message");
  }

  @Test
  public void testError() {
    _stderrLogger.error("Error message");
  }
}
