/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.fuzzer;

import org.testng.annotations.Test;

import com.beust.jcommander.ParameterException;

/**
 * Tests the Fuzzer main entry point. 
 * Mainly just makes sure the parameters given don't throw, there's little to assert.
 */
@Test
public class FuzzerMainTest {

  String defaultTime = "1";

  FuzzerMain fm = FuzzerMain.getInstance();
  
  @Test(expectedExceptions = ParameterException.class)
  public void TestUnknownArg() {
    String[] ArgString = new String[] { "--trombone", "76" };
    fm.mainEntry(ArgString);
  }

  @Test
  public void TestTime() {
    String[] ArgString = new String[] { "-t", "2" };
    fm.mainEntry(ArgString);
  }

  @Test(expectedExceptions = ParameterException.class)
  public void TestBadTime() {
    String[] ArgString = new String[] { "-t", "-10" };
    fm.mainEntry(ArgString);
  }

  @Test
  public void TestDataSize() {
    String[] ArgString = new String[] { "-t", defaultTime, "-d", "20" };
    fm.mainEntry(ArgString);
    ArgString = new String[] { "-t", defaultTime, "--maxDataSize", "20" };
    fm.mainEntry(ArgString);
  }

  @Test(expectedExceptions = ParameterException.class)
  public void TestBadDataSize() {
    String[] ArgString = new String[] { "-t", defaultTime, "-d", "-10" };
    fm.mainEntry(ArgString);
    ArgString = new String[] { "-t", defaultTime, "-maxDataSize", "-10" };
    fm.mainEntry(ArgString);
  }

  @Test
  public void TestDataRefs() {
    String[] ArgString = new String[] { "-t", defaultTime, "-r", "20" };
    fm.mainEntry(ArgString);
    ArgString = new String[] { "-t", defaultTime, "-r", "20" };
    fm.mainEntry(ArgString);
  }

  @Test(expectedExceptions = ParameterException.class)
  public void TestBadDataRefs() {
    String[] ArgString = new String[] { "-t", defaultTime, "-r", "-10" };
    fm.mainEntry(ArgString);
    ArgString = new String[] { "-t", defaultTime, "-maxDataRefs", "-10" };
    fm.mainEntry(ArgString);
  }

  @Test
  public void TestThreads() {
    String[] ArgString = new String[] { "-t", defaultTime, "-c", "4" };
    fm.mainEntry(ArgString);
    ArgString = new String[] { "-t", defaultTime, "--nthreads", "4" };
    fm.mainEntry(ArgString);
  }

  @Test(expectedExceptions = ParameterException.class)
  public void TestBadThreads() {
    String[] ArgString = new String[] { "-t", defaultTime, "-c", "-1" };
    fm.mainEntry(ArgString);
    ArgString = new String[] { "-t", defaultTime, "--nthreads", "-1" };
    fm.mainEntry(ArgString);
  }

  @Test
  public void TestSeed() {
    String[] ArgString = new String[] { "-t", defaultTime, "-s", "4" };
    fm.mainEntry(ArgString);
    ArgString = new String[] { "-t", defaultTime, "--seed", "4" };
    fm.mainEntry(ArgString);
  }

  @Test
  public void TestSensibleCombo() {
    String[] ArgString = new String[] { "-t", "5", "-c", "4", "-l", "-p" };
    fm.mainEntry(ArgString);
  }

}
