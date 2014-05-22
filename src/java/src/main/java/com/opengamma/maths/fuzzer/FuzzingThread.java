/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.fuzzer;

import java.io.RandomAccessFile;

/**
 * Threaded fuzzing class.
 * Objects of this class will fuzz trees based on parameters given at construction time.
 */
public class FuzzingThread implements Runnable {
  /**
   * The tree fuzzer.
   */
  private TreeFuzzer _fuzzer;
  /**
   * The seed for the RNG used in the fuzzer.
   */
  private long _seed;

  /**
   * Set to true when logging
   */
  private boolean _logging;

  /**
   * The log file.
   */
  private RandomAccessFile _logFile;

  /**
   * 
   * @param seed
   * @param maxTreeRefs
   * @param maxDataSize
   */
  public FuzzingThread(long seed, int maxTreeRefs, int maxDataSize) {
    this(seed, maxTreeRefs, maxDataSize, false, null);
  }

  public FuzzingThread(long seed, int maxTreeRefs, int maxDataSize, boolean logging, RandomAccessFile log) {
    _fuzzer = new TreeFuzzer(seed, maxTreeRefs, maxDataSize, logging, log);
    _seed = seed;
    _logging = logging;
    _logFile = log;
  }

  @Override
  public void run() {
    // run indefinitely
    System.out.println("Running thread with seed: " + _seed + ".");
    _fuzzer.fuzz(Long.MAX_VALUE);
  }
}
