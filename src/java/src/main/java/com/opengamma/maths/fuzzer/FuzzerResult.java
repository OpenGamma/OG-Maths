/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.fuzzer;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.TimeZone;

/**
 * Holds the results of a fuzz.
 */
public class FuzzerResult {
  /**
   * The number of threads used.
   */
  private int _nthreads;
  /**
   * The number of trees executed by each thread.
   */
  private long[] _ntreesPerThread;
  /**
   * The seeds used by each thread.
   */
  private long[] _seeds;
  /**
   * The elapsed execution time.
   */
  private long _elapsedTime;
  /**
   * The maximum number of tree refs specified.
   */
  private int _maxTreeRefs;
  /**
   * The maximum data size specified.
   */
  private int _maxDataSize;

  /**
   * Construct a new fuzzer result, this is just a POD container.
   * @param ntrees the number of trees executed by each thread.
   * @param seed the seeds used by each thread.
   * @param elapasedTime the elapsed execution time (in seconds).
   * @param maxTreeRefs the maximum number of tree refs specified.
   * @param maxDataSize the maximum data size specified.
   */
  public FuzzerResult(long ntrees, long seed, long elapasedTime, int maxTreeRefs, int maxDataSize) {
    _nthreads = 1;
    _ntreesPerThread = new long[] { ntrees };
    _seeds = new long[] { seed };
    _elapsedTime = elapasedTime;
    _maxTreeRefs = maxTreeRefs;
    _maxDataSize = maxDataSize;
  }

  /**
   * Construct a new fuzzer result, this is just a POD container.
   * @param nthreads the number of threads used to generate this data.
   * @param ntreesPerThread the number of trees executed by each thread.
   * @param seeds the seeds used by each thread.
   * @param elapasedTime the elapsed execution time (in seconds).
   * @param maxTreeRefs the maximum number of tree refs specified.
   * @param maxDataSize the maximum data size specified.
   */
  public FuzzerResult(int nthreads, long[] ntreesPerThread, long[] seeds, long elapasedTime, int maxTreeRefs, int maxDataSize) {
    _nthreads = nthreads;
    _ntreesPerThread = Arrays.copyOf(ntreesPerThread, ntreesPerThread.length);
    _seeds = Arrays.copyOf(seeds, seeds.length);
    _elapsedTime = elapasedTime;
    _maxTreeRefs = maxTreeRefs;
    _maxDataSize = maxDataSize;
  }

  /**
   * Gets the the number of threads used to generate this data.
   * @return the number of threads used to generate this data.
   */
  public int getNthreads() {
    return _nthreads;
  }

  /**
   * Gets the number of trees executed by each thread.
   * @return the number of trees executed by each thread.
   */
  public long[] getNtreesPerThread() {
    return _ntreesPerThread;
  }

  /**
   * Gets the seeds used by each thread.
   * @return the seeds used by each thread.
   */
  public long[] getSeeds() {
    return _seeds;
  }

  /**
   * Gets the elapsed execution time.
   * @return the elapsed execution time.
   */
  public long getElapsedTime() {
    return _elapsedTime;
  }

  /**
   * Gets the maximum number of tree refs specified.
   * @return the maximum number of tree refs specified.
   */
  public int getMaxTreeRefs() {
    return _maxTreeRefs;
  }

  /**
   * Gets the maximum data size specified.
   * @return the maximum data size specified.
   */
  public int getMaxDataSize() {
    return _maxDataSize;
  }

  @Override
  public String toString() {
    StringBuffer buf = new StringBuffer();
    buf.append("___Fuzzer Results___\n");
    buf.append("_Execution Parameters_\n");
    buf.append("Threads: " + _nthreads + "\n");
    buf.append("Thread RNG seeds: " + Arrays.toString(_seeds) + "\n");
    buf.append("Max Tree Refs: " + _maxTreeRefs + "\n");
    buf.append("Max Data Size: " + _maxDataSize + "\n");
    buf.append("\n_Execution Results_\n");
    SimpleDateFormat fmt = new SimpleDateFormat("HH:mm:ss");
    fmt.setTimeZone(TimeZone.getTimeZone("UTC")); // UNIX epoch is UTC
    buf.append("ElapsedTime: " + fmt.format(new Date(1000L * _elapsedTime)) + "\n");
    long texec = 0;
    for (int k = 0; k < _nthreads; k++) {
      texec += _ntreesPerThread[k];
    }
    buf.append("Trees executed total: " + texec + "\n");
    buf.append("Trees per second: " + ((double) texec / (double) _elapsedTime) + "\n");
    buf.append("Trees executed per thread: " + Arrays.toString(_ntreesPerThread) + "\n");
    return buf.toString();
  }
}
