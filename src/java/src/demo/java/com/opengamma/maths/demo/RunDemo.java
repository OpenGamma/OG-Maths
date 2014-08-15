/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.demo;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import com.beust.jcommander.JCommander;
import com.beust.jcommander.Parameter;

/**
 * Entry point for running demos.
 */
public class RunDemo {

  /**
   * List contain the names of valid demos.
   */
  private static List<String> s_availableDemos = new ArrayList<>();
  static {
    s_availableDemos.add("MtimesVsTimesDemo");
    s_availableDemos.add("AdditionDemo");
  }

  /**
   * Hidden default ctor.
   */
  private RunDemo() {
  }

  /**
  * Singleton reference holder.
  */
  private static class RunDemosRefHolder {
    private static final RunDemo s_ref = new RunDemo();
  }

  /**
   * Gets the singleton instance of this class.
   */
  public static RunDemo getInstance() {
    return RunDemosRefHolder.s_ref;
  }

  /**
   * The main entry point. For usage run with args = "-h".
   * This is intended to be run as a java application.
   * @param args the arguments to the main method.
   */
  public static void main(String[] args) {
    getInstance().mainEntry(args, true);
    System.exit(0);
    Runtime.getRuntime().halt(0);
  }

  /**
   * The accepted command line arguments.
   */
  private class CLIArgs {
    @Parameter(names = { "--demoName", "-n" }, description = "The name of the demo class you wish to run.")
    private String name;

    @Parameter(names = { "--listDemos", "-l" }, description = "Lists the available demos.")
    private boolean listDemos;

    @Parameter(names = { "--help", "-h" }, help = true)
    private boolean help;
  }

  /**
   * Ref to the arg parser class.
   */
  private CLIArgs _argParser;

  /**
   * Parses the args to the main method and invokes operations for the helper flags based on them (list and help flags).
   * @param args the args to parse
   * @param killOnUsageOrList if true, the executing with the "-h" or "-l" flag 
   * will halt execution and print usage or list demos available respectively.
   * @return true if the parseArgs has handled the requirements of the flag, else false.
    */
  private boolean parseHelperFlags(String[] args, boolean killOnUsageOrList) {
    // parse the args
    JCommander jcommanderRef = new JCommander(_argParser, args);
    jcommanderRef.setProgramName("RunDemos");

    // see if -h was invoked, or no args were given
    if (_argParser.help) {
      jcommanderRef.usage();
      if (killOnUsageOrList) {
        Runtime.getRuntime().halt(0);
      }
      return true;
    }

    // see if -l was invoked, print demos if so
    if (_argParser.listDemos) {
      System.out.println("_Available Demos_");
      for (String str : s_availableDemos) {
        System.out.println(" * " + str);
      }
      if (killOnUsageOrList) {
        Runtime.getRuntime().halt(0);
      }
      return true;
    }

    // no helper flags found, continue
    return false;
  }

  /**
   * Invoke the demo application with command line style options, but as a function.
   * @param args to parse, will not die on "-h" or "-l" flag.
   */
  public void mainEntry(String[] args) {
    mainEntry(args, false);
  }

  public void mainEntry(String[] args, boolean killOnUsageOrList) {
    _argParser = this.new CLIArgs(); // force a new CLI instance
    boolean helperFlagPresent = parseHelperFlags(args, killOnUsageOrList);

    // if there's no helper type flag present then we need to try running an actual demo.
    if (!helperFlagPresent) {
      String demoName = _argParser.name; // will be not null, jCommander checks
      Class<?> demoClazz = null;
      try {
        demoClazz = Class.forName("com.opengamma.maths.demo." + demoName);
        DemoInterface demo = (DemoInterface) demoClazz.newInstance();
        demo.runDemo();
      } catch (ClassNotFoundException | InstantiationException | IllegalAccessException ex) {
        throw new RuntimeException("Problem with instantiating demo class.", ex);
      }
      if (killOnUsageOrList) {
        Runtime.getRuntime().halt(0);
      }

    }
  }

  /**
   * Gets the available demos.
   * @return the available demos
   */
  public static List<String> getAvailableDemos() {
    return s_availableDemos;
  }

}
