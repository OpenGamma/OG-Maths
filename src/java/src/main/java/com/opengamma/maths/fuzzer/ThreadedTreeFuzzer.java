/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.fuzzer;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Generates pseudo-random trees and runs them
 */
public class ThreadedTreeFuzzer implements Fuzzer {

  /**
   * The start seed for the RNG.
   */
  private long _startSeed;

  /**
   * Maximum number of local reference objects in the tree.
   */
  private int _maxTreeRefs;

  /**
   * Maximum "size" of terminal data (rows and cols guaranteed to be < this number).
   */
  private int _maxDataSize;

  /**
   * The number of threads to use in the fuzzing process.
   */
  private int _nthreads;

  /**
   * The thread executor.
   */
  private ExecutorService _executor;

  /**
   * One second in quantity of nanoseconds.
   */
  private static final long S_SECOND = 1000000000L;

  /**
   * Logging status, true for logging is on.
   */
  private boolean _logging;

  /**
   * log directory
   */
  private File _logDir;

  /**
   * The per thread logs
   */
  private RandomAccessFile[] _theLogs;

  /**
   * Sets up internal state of dogma methods via reflection.
   * @param startSeed the start seed for the RNG
   * @param maxTreeRefs
   * @param maxDataSize
   */
  public ThreadedTreeFuzzer(long startSeed, int maxTreeRefs, int maxDataSize, int nthreads, boolean logging, File logDir) {
    _startSeed = startSeed;
    _maxTreeRefs = maxTreeRefs;
    _maxDataSize = maxDataSize;
    _nthreads = nthreads;
    _executor = Executors.newFixedThreadPool(_nthreads);
    _logging = logging;

    if (_logging) {
      // try and set up log directory.
      _logDir = logDir;

      // check that directory is indeed a directory
      if (!_logDir.isDirectory()) {
        throw new RuntimeException("Directory required, but path found. Given: " + _logDir.toString());
      }

      // if the dir doesn't exist, create it
      if (!_logDir.exists()) {
        boolean dirCreationOk = _logDir.mkdirs();
        if (!dirCreationOk) {
          throw new RuntimeException("Could not create logging directory \'" + logDir + "\'");
        }
      }
      // spin up a log file per thread
      _theLogs = new RandomAccessFile[nthreads];
      for (int k = 0; k < nthreads; k++) {
        String filename = _logDir.getPath() + File.separatorChar + "FuzzerLog_thread" + k + ".log";
        try {
          _theLogs[k] = new RandomAccessFile(filename, "rw");
        } catch (FileNotFoundException ex) {
          throw new RuntimeException(ex);
        }
      }
      // exit of branch, each fuzzer thread should have it's own logging file
    }

  }

  /**
   * {@inheritDoc}
   */
  public final void fuzz(long runtime) {
    RandomAccessFile f = null;
    for (int k = 0; k < _nthreads; k++) {
      if (_logging) {
        f = _theLogs[k];
      }
      FuzzingThread fuzzythread = new FuzzingThread(_startSeed + k, _maxTreeRefs, _maxDataSize, _logging, f);
      _executor.execute(fuzzythread);
    }
    long startTime = System.nanoTime();
    long test = runtime * S_SECOND;
    long stopTime = startTime + test > startTime ? startTime + test : Long.MAX_VALUE;
    while (System.nanoTime() < stopTime) {
    }
    long elapsed = System.nanoTime() - startTime;
    // kill threads
    _executor.shutdownNow();
    System.out.println("Time elapsed = " + elapsed / S_SECOND + " seconds.");
  }

  @Override
  protected void finalize() throws Throwable {
    // try and clean up
    for (int k = 0; k < _nthreads; k++) {
      _theLogs[k].close();
    }
    super.finalize();
  }
}
