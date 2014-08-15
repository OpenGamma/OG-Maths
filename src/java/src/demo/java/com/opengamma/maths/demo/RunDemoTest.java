/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.demo;

import java.util.List;

import org.testng.annotations.Test;

import com.beust.jcommander.ParameterException;

/**
 * Test the entry point for running demos (mainly checking the CLI).
 */
@Test
public class RunDemoTest {

  @Test(expectedExceptions = ParameterException.class)
  public void TestUnknownArg() {
    String[] ArgString = new String[] { "--trombone", "76" };
    RunDemo.getInstance().mainEntry(ArgString);
  }

  @Test
  public void TestProduceListOfDemosFires() {
    String[] ArgString = new String[] { "-l" };
    RunDemo.getInstance().mainEntry(ArgString);
  }

  @Test
  public void TestHelpCommandFires() {
    String[] ArgString = new String[] { "-h" };
    RunDemo.getInstance().mainEntry(ArgString);
  }

  @Test(expectedExceptions = ParameterException.class)
  public void TestNullClassName() {
    String[] ArgString = new String[] { "-n" };
    RunDemo.getInstance().mainEntry(ArgString);
  }

  @Test(expectedExceptions = RuntimeException.class)
  public void TestInvalidClassName() {
    String[] ArgString = new String[] { "-n", "ostrich" };
    RunDemo.getInstance().mainEntry(ArgString);
  }

  @Test
  public void TestValidClassName() {
    String[] ArgString = new String[] { "-n", "MtimesVsTimesDemo" };
    RunDemo.getInstance().mainEntry(ArgString);
  }

  @Test
  public void TestInvokeAllDemosDeclaredAvailable() {
    List<String> demos = RunDemo.getAvailableDemos();
    for (String demo : demos) {
      String[] ArgString = new String[] { "-n", demo };
      RunDemo.getInstance().mainEntry(ArgString);
    }
  }
}
