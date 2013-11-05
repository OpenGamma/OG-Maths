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
  DEBUG("dbg"),

  /**
   * Specifies standard build and instructions (-O2, no debug symbols).
   */
  STANDARD("std"),

  /**
   * Specifies build targeting SSE4.2 instructions.
   */
  SSE42("sse42"),

  /**
   * Specifies build targeting AVX version 1 instructions.
   */
  AVX1("avx"),

  /**
   * Specifies build targeting AVX version 2 instructions.
   */
  AVX2("avx"); // currently there's no differentiation specified between AVX1 and AVX2

  private SupportedInstructionSet(String tagline) {
    _tagline = tagline;
  }

  private String _tagline;

  /**
   * Gets the tag line for libraries with respect to instruction set
   * @return the tag line for library loading purposes
   */
  public String getTagline() {
    return _tagline;
  }

};
