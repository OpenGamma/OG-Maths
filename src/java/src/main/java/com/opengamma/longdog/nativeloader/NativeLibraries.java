/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.nativeloader;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.nio.file.Path;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.BufferedOutputStream;
import com.opengamma.longdog.exceptions.LongdogInitializationException;
import com.opengamma.longdog.exceptions.LongdogUnsupportedPlatformException;

import static java.nio.file.Files.createTempDirectory;

/**
 * Used for extracting and loading native libraries from the JAR file.
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
    propsFile = NativeLibraries.class.getResourceAsStream("/config/NativeLibraries.properties");
    if (propsFile == null) {
      throw new LongdogInitializationException("Cannot find NativeLibraries properties file");
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

    String libPath = s_tmpDir + File.separatorChar + lib;

    try {
      if (s_debug) {
        System.out.println("Loading " + lib + " from " + libPath);
      }
      System.load(libPath);
    } catch (Exception e) {
      throw new LongdogInitializationException("Cannot load " + lib + " as " + libPath, e);
    }
  }

  private NativeLibraries() {
  };

}
