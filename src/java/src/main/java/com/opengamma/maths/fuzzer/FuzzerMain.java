/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.fuzzer;

import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import static java.nio.file.Files.createTempDirectory;

import com.beust.jcommander.IParameterValidator;
import com.beust.jcommander.JCommander;
import com.beust.jcommander.Parameter;
import com.beust.jcommander.ParameterException;

/**
 * Entry point to run the maths fuzzer.
 */
public class FuzzerMain {

  /**
   * Hidden default ctor
   */
  private FuzzerMain() {
  }

  /**
   * Singleton reference holder
   */
  private static class FuzzerMainRefHolder {
    private static final FuzzerMain s_ref = new FuzzerMain();
  }

  /**
   * Gets the singleton instance of this class/
   */
  public static FuzzerMain getInstance() {
    return FuzzerMainRefHolder.s_ref;
  }

  /**
   * Private class to hold ref to and parse the CLI args.
   */
  private class CLIArgs {

    @Parameter(names = { "--seed", "-s" }, description = "The random seed to use.")
    private Long seed = 0L;

    @Parameter(names = { "--nthreads", "-c" }, description = "The number of threads to use.", validateWith = ValidateIntPositiveNonZero.class)
    private Integer nthreads = 1;

    @Parameter(names = { "--maxRefs", "-r" }, description = "The maxiumum number of refs in a tree.", validateWith = ValidateIntPositiveNonZero.class)
    private Integer maxRefs = 50;

    @Parameter(names = { "--maxDataSize", "-d" }, description = "The maxiumum data size.", validateWith = ValidateIntPositiveNonZero.class)
    private Integer maxDataSize = 10;

    @Parameter(names = { "--time", "-t" }, description = "The length of time in seconds to run.", validateWith = ValidateLongPositiveNonZero.class)
    private Long maxtime = Long.MAX_VALUE;

    @Parameter(names = { "--logging", "-l" }, description = "Flag to turn on logging.")
    private boolean loggingOn = false;

    @Parameter(names = { "--logDirectory", "-ld" }, description = "The directory in which to place logs. Provision of this flag automatically switches on logging (-l).")
    private String logDir;

    @Parameter(names = { "--printStats", "-p" }, description = "Provision of this flag automatically switches on printing stats to STDOUT.")
    private boolean printStats;

    @Parameter(names = { "--help", "-h" }, help = true)
    private boolean help;
  }

  /**
   * Ensures a Long is > 0;
   */
  public static class ValidateLongPositiveNonZero implements IParameterValidator {
    @Override
    public void validate(String param, String value) throws ParameterException {
      long val = Long.parseLong(value);
      if (val < 1) {
        throw new ParameterException("Argument " + param + " should be >= 1. Value given was " + val + ".");
      }
    }
  }

  /**
   * Ensures an Integer is > 0;
   */
  public static class ValidateIntPositiveNonZero implements IParameterValidator {
    @Override
    public void validate(String param, String value) throws ParameterException {
      int val = Integer.parseInt(value);
      if (val < 1) {
        throw new ParameterException("Argument " + param + " should be >= 1. Value given was " + val + ".");
      }
    }
  }

  private CLIArgs _argParser;

  /**
   * Parses the args to the main and mainEntry methods
   * @param args the args to parse
   * @param killOnUsage if true, the executing with the "-h" flag will halt execution and print usage.
   */
  private void parseArgs(String[] args, boolean killOnUsage) {
    // parse the args
    _argParser = getInstance().new CLIArgs();
    JCommander jcommanderRef = new JCommander(_argParser, args);
    jcommanderRef.setProgramName("FuzzerMain");

    // see if -h was invoked, or no args were given
    if (_argParser.help) {
      jcommanderRef.usage();
      if (killOnUsage) {
        Runtime.getRuntime().halt(0);
      }
    }
  }

  /**
   * Invoke the tree fuzzer as an application.
   * @param args, for options invoke with "-h".
   */
  public static void main(String[] args) {
    getInstance().mainEntry(args, true);
    System.exit(0);
    Runtime.getRuntime().halt(0);
  }

  /**
   * Invoke the tree fuzzer with command line style options, but as a function.
   * @param args to parse, will not die on "-h" flag.
   */
  public void mainEntry(String[] args) {
    mainEntry(args, false);
  }

  /**
   * Invoke the tree fuzzer with command line style options, but as a function.
   * @param args for options invoke with "-h".
   * @param killOnUsage if true, the executing with the "-h" flag will halt execution and print usage.
    */
  public void mainEntry(String[] args, boolean killOnUsage) {
    parseArgs(args, killOnUsage);
    // deal with the rest
    long rngSeed = _argParser.seed;
    int maxDataRefs = _argParser.maxRefs;
    int maxDataSize = _argParser.maxDataSize;
    int nthreads = _argParser.nthreads;
    long time = _argParser.maxtime;
    boolean loggingOn = _argParser.loggingOn;
    String logDirStr = _argParser.logDir;
    boolean printStats = _argParser.printStats;
    File logDir = null;

    // provision of logging dir switches on logging
    if (loggingOn || logDirStr != null) {
      loggingOn = true;
      // -l switches on logging, but log dir may not have been provided, create one
      try {
        if (logDirStr == null) {
          Path tmpDir;
          tmpDir = createTempDirectory("og-maths-logs-");
          logDir = tmpDir.toFile();
        }
        System.out.println("Sending fuzzer logs to " + logDir.getCanonicalPath() + ".");
      } catch (IOException ex) {
        throw new RuntimeException(ex);
      }
    }

    Fuzzer fuzzer = new ThreadedTreeFuzzer(rngSeed, maxDataRefs, maxDataSize, nthreads, loggingOn, logDir);
    FuzzerResult result = fuzzer.fuzz(time);
    if (printStats) {
      System.out.println(result.toString());
    }
  }
}
