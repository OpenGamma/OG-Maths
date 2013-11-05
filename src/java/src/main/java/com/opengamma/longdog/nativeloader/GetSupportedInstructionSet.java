/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.nativeloader;

/**
 * Gets the supported instruction set, helper class with logical separation from loader
 */
public class GetSupportedInstructionSet {

  /**
   * Probes the CPU and gets the maximum supported instruction set.
   * @return maximum supported instruction set.
   */
  public static native SupportedInstructionSet getSupportedInstructionSet();

}
