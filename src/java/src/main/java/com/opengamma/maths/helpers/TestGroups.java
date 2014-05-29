/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */

package com.opengamma.maths.helpers;

/**
 * Constants for the TestNG groups.
 */
public final class TestGroups {

  /**
   * TestNG 'fuzzer' group.
   * Will only run if "-DrunFuzzerTests" is supplied on the command line.
   */
  public static final String FUZZER = "fuzzer";

  /**
   * TestNG 'all' group.
   * Will run a regression test anywhere.
   */
  public static final String ALL = "all";

  /**
   * Restricted constructor.
   */
  private TestGroups() {
    throw new UnsupportedOperationException("No instances");
  }

}
