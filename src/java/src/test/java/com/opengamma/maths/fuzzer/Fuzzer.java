/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.fuzzer;

import org.testng.annotations.Test;

import com.opengamma.maths.DOGMA;
import com.opengamma.maths.datacontainers.OGNumeric;

public class Fuzzer {

  /**
   * Invoke the tree fuzzer.
   * Args
   * - exec time.
   * - seed
   * @param args
   */
  //  public static void main(String[] args) {
  //    TreeFuzzer fuzzer = new TreeFuzzer();
  //    fuzzer.fuzz();
  //  }

  @Test
  public void fuzzer() {
    long rngSeed = 0L;
    int maxNewNodes = 10;
    int maxDataSize = 5;
    TreeFuzzer fuzzer = new TreeFuzzer(rngSeed, maxNewNodes, maxDataSize);
    fuzzer.fuzz(2l);
  }

}
