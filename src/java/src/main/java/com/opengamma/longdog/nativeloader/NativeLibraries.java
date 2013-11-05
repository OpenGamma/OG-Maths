/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.nativeloader;

import static java.nio.file.Files.createTempDirectory;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.opengamma.longdog.exceptions.LongdogInitializationException;
import com.opengamma.longdog.exceptions.LongdogUnsupportedPlatformException;

/**
 * Used for extracting and loading native libraries.
 * 
 * Default behaviour is to extract and load libraries from the JAR file. 
 * Optionally, the location of the config file that specifies the libraries to be loaded can
 * be given on the command line as -Dconfigfileloc="location/file" in which case the
 * libraries are loaded from system specific library path based locations.
 *
 * The correct libraries are extracted and loaded depending on the architecture
 * that we are running on.
 */
public final class NativeLibraries {

  /* Location that native libraries are extracted to */
  private static String s_tmpDir;
  /* We initialize once only. Are we already initialized? */
  private static boolean s_initialized;
  /* print debug info? */
  private static boolean s_debug = true;
  private static List<String> s_libsToExtract = new ArrayList<String>();
  private static List<String> s_libsToLoad = new ArrayList<String>();
  private static List<String> s_libsForInitialise = new ArrayList<String>();
  /* properties file location, defaults to jar, can be overridden on command line */
  private static String s_configFileLocation = "/config/NativeLibraries.properties";
  /* true if config was supplied on the command line */
  private static boolean s_configFileOnCommandline;
  /* properties file location, defaults to jar, can be overridden on command line */
  private static String s_instrSet;
  /* true if instr set was supplied on the command line */
  private static boolean s_instrSetOnCommandline;

  /**
   * Check whether the current platform is supported.
   *
   * We support Mac OS X, Windows, and Linux running on x86 64-bit architectures.
   */
  private static void checkPlatformSupported() {
    String arch = System.getProperty("os.arch");
    if (!arch.equals("amd64") && !arch.equals("x86_64")) {
      throw new LongdogUnsupportedPlatformException(
          "Your architecture " + arch + " is unsupported. Only 64-bit systems are supported.");
    }

    Set<String> supportedPlatforms = new HashSet<String>() {
      /**
       * SID
       */
      private static final long serialVersionUID = 7866031787682964784L;

      {
        add("lin");
        add("win");
        add("mac");
      }
    };
    String platform = getShortPlatform();
    if (!supportedPlatforms.contains(platform)) {
      String err = "Your platform, " + getPlatform() + ", is not supported.";
      throw new LongdogUnsupportedPlatformException(err);
    }
  }

  /**
   * Get the current platform name.
   * @return the platform
   */
  private static String getPlatform() {
    return System.getProperty("os.name");
  }

  /**
   * Return the lower-case first three letters of the platform name.
   * 
   * These will match up with the names referred to in the NativeLibraries.properties file
   */
  private static String getShortPlatform() {
    return getPlatform().toLowerCase().substring(0, 3);
  }

  /**
    * Load configuration of native library loader from within the Longdog JAR.
    *
    * Configuration includes the names of libraries that must be extracted,
    * and the subset of those that will be explicitly loaded.
    */
  private static void getConfigFromProperties() {
    Properties nativeLibrariesProperties = new Properties();

    InputStream propsFile = null;

    // Is the instructionSet overridden?
    String commandLineInstrSet = System.getProperty("instructionSet");
    if (commandLineInstrSet != null) { // specified instruction set given
      s_instrSetOnCommandline = true;
      try {
        SupportedInstructionSet.valueOf(commandLineInstrSet);
      } catch (Exception e) {
        throw new LongdogInitializationException("Invalid instruction set specified on command line (value given was):" + commandLineInstrSet);
      }
      s_instrSet = commandLineInstrSet;
    }

    // Is the config file location overridden?
    String commandLineConfig = System.getProperty("configFile");

    if (commandLineConfig != null) { // yes we are using a config file specified on the command line
      s_configFileOnCommandline = true;
      if (s_debug) {
        System.out.println("Using command line supplied configfileloc information");
      }
      s_configFileLocation = commandLineConfig;
      try {
        if (s_debug) {
          System.out.println("Attempting load from " + s_configFileLocation);
        }
        propsFile = new FileInputStream(s_configFileLocation);
      } catch (IOException ex) {
        throw new LongdogInitializationException("Cannot find NativeLibraries properties file in command line specified location: " + s_configFileLocation);
      }
    } else { // we are using the default config file location in the JAR
      propsFile = NativeLibraries.class.getResourceAsStream(s_configFileLocation);
      if (propsFile == null) {
        throw new LongdogInitializationException("Cannot find NativeLibraries properties file in location: " + s_configFileLocation);
      }
    }
    try {
      nativeLibrariesProperties.load(propsFile);
    } catch (IOException e) {
      throw new LongdogInitializationException("Cannot load native library properties.");
    }
    try {
      propsFile.close();
    } catch (IOException e) {
      throw new LongdogInitializationException("Failed to close properties file stream.");
    }

    // look for NativeLibraries specific entries
    String nlString = "NativeLibraries." + getShortPlatform() + ".*";
    Pattern nlPattern = Pattern.compile(nlString, Pattern.CASE_INSENSITIVE);
    for (Entry<Object, Object> entry : nativeLibrariesProperties.entrySet()) {
      String key = entry.getKey().toString();
      Matcher nlMatch = nlPattern.matcher(key);
      if (nlMatch.matches()) {
        if (key.endsWith("libraries")) {
          String value = (String) entry.getValue();
          String[] libs = value.split(",");
          for (String lib : libs) {
            s_libsToExtract.add(lib.trim());
          }
        }
        if (key.endsWith("load")) {
          String value = (String) entry.getValue();
          String[] libs = value.split(",");
          for (String lib : libs) {
            s_libsToLoad.add(lib.trim());
          }
        }
        if (key.endsWith("initialise")) {
          String value = (String) entry.getValue();
          String[] libs = value.split(",");
          for (String lib : libs) {
            s_libsForInitialise.add(lib.trim());
          }
        }
      }
    }
  }

  /**
   * Prepare for loading of native libraries.
   *
   * A temporary directory is created for holding native libraries, and they
   * are extracted to this directory. If a library cannot be found in the JAR,
   * we continue anyway since we may be in a testing environment before the
   * construction of the JAR containing the native libraries.
   */
  public static synchronized void initialize() {

    if (s_initialized) {
      // Initialise once only.
      if (s_debug) {
        System.out.println("Skipping initialization - already initialized");
      }
      return;
    }
    if (s_debug) {
      System.out.println("Initializing native libraries.");
    }

    checkPlatformSupported();
    getConfigFromProperties();

    if (!s_configFileOnCommandline) {
      Path libDir;
      try {
        libDir = createTempDirectory("og-mathswrappers-");
        libDir.toFile().deleteOnExit();
      } catch (IOException e) {
        throw new LongdogInitializationException("Could not create temp directory for native libaries", e);
      }
      s_tmpDir = libDir.toString();

      String url = "/lib";

      Class<?> c = NativeLibraries.class;
      for (String name : s_libsToExtract) {
        if (s_debug) {
          System.out.println("Extracting " + name);
        }
        String fsPath = libDir + "/" + name;
        String jarPath = url + "/" + getShortPlatform() + "/" + name;

        URL jarURL = c.getResource(jarPath);
        if (jarURL == null) {
          throw new LongdogInitializationException("Resource " + jarPath + " not found in jar file");
        }

        InputStream source = null;
        OutputStream destination = null;
        try {
          source = c.getResourceAsStream(jarPath);
          destination = new BufferedOutputStream(new FileOutputStream(fsPath));
          byte[] buffer = new byte[1024 * 1024];
          int len;
          boolean haveReadBytes = false;
          while ((len = source.read(buffer)) != -1) {
            if (!haveReadBytes && (len == 0)) {
              throw new LongdogInitializationException("0 bytes read for resource" + name);
            }
            haveReadBytes = true;
            destination.write(buffer, 0, len);
          }
        } catch (Exception e) {
          throw new LongdogInitializationException("Error extracting resource " + name, e);
        } finally {
          if (source != null) {
            try {
              source.close();
            } catch (IOException e) {
              throw new LongdogInitializationException("Error closing source stream for " + name, e);
            }
          }
          if (destination != null) {
            try {
              destination.close();
            } catch (IOException e) {
              throw new LongdogInitializationException("Error closing destination stream for " + name, e);
            }
          }
        }

        File file = new File(fsPath);

        try {
          file.deleteOnExit();
        } catch (SecurityException e) {
          throw new LongdogInitializationException("Security settings prevent deletion of native libraries on exit", e);
        }
      }
      if (s_debug) {
        System.out.println("Native libraries extracted");
      }
    } // end if(!s_commandlineconfig)

    // load initialisation libraries
    for (String name : s_libsForInitialise) {
      load(name);
    }

    // instruction set was specified on command line
    SupportedInstructionSet instructionSet;
    if (s_instrSetOnCommandline) { // we know its valid, check was performed in getConfigFromProperties()
      instructionSet = SupportedInstructionSet.valueOf(s_instrSet);
    } else {
      instructionSet = GetSupportedInstructionSet.getSupportedInstructionSet();
      if (s_debug) {
        System.out.println("Probed instruction set is: " + instructionSet.toString());
      }
    }

    if (s_debug) {
      System.out.println("Running with instruction set as: " + instructionSet.toString());
    }

    // load the libraries that do the heavy lifting
    for (String name : s_libsToLoad) {
      load(name);
    }
    s_initialized = true;
  }

  /**
   * Load a native library.
   *
   * Native libraries must be extracted before they are loaded.
   *
   * @param lib the name of the library to be loaded
   */
  private static synchronized void load(String lib) {

    if (!s_configFileOnCommandline) { // we are using the config from the file in the jar
      String libPath = s_tmpDir + File.separatorChar + lib;
      try {
        if (s_debug) {
          System.out.println("Loading " + lib + " from " + libPath);
        }
        System.load(libPath);
      } catch (Exception e) {
        throw new LongdogInitializationException("Cannot load " + lib + " as " + libPath, e);
      }
    } else { // we are using a command line specified config location
      Pattern pat = Pattern.compile("lib(\\w+)..*"); // compile pattern to strip lib and .* from library name
      Matcher m = pat.matcher(lib);
      if (!m.matches()) {
        throw new LongdogInitializationException("Could not extract library name from lib string.");
      }
      String libsimplename = m.group(1); // single group
      if (libsimplename == null) {
        throw new LongdogInitializationException("Could not extract system invariant library name from library name string.");
      }
      try {
        if (s_debug) {
          System.out.println("Loading " + libsimplename + " from native library locations.");
        }
        System.loadLibrary(libsimplename);
      } catch (Exception e) {
        throw new LongdogInitializationException("Cannot load " + libsimplename + " from native library locations", e);
      }

    }

  }

  private NativeLibraries() {
  };

}
