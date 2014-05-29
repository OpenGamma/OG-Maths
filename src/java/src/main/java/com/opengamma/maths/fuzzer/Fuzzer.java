/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.fuzzer;

/**
 * Fuzzers must implement this interface.
 */
public interface Fuzzer {

  /**
   * Invoke a fuzz for a give length of time.
   * @param time the amount of time the fuzzer should be run for in seconds.
   * @return a fuzzer result holding stats about the fuzz.
   */
  FuzzerResult fuzz(long time);
}
