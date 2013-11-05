/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.nativeloader;

/**
 * Holds the values for supported cpu instruction sets
 */
public enum SupportedInstructionSet {

  /**
   * Specifies debug build and instructions.
   */
  DEBUG,

  /**
   * Specifies standard build and instructions (-O2, no debug symbols).
   */
  STANDARD,

  /**
   * Specifies build targeting SSE4.2 instructions.
   */
  SSE42,

  /**
   * Specifies build targeting AVX version 1 instructions.
   */
  AVX1,

  /**
   * Specifies build targeting AVX version 2 instructions.
   */
  AVX2
};
