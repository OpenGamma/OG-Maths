OpenGamma Maths Installation
========

This document is split into two sections: the first describes "installing" OpenGamma Maths (OG-Maths) as a user, the second describes "installing" OG-Maths as an OG-Maths developer. Most people will want to install from the user perspective.

[![OpenGamma](http://developers.opengamma.com/res/display/default/chrome/masthead_logo.png "OpenGamma")](http://developers.opengamma.com)

Installation as a User
----------------------

## For Java Users

To use OG-Maths in a Java project, assuming you are using Maven as your package manager, include OG-Maths in the `pom.xml` as you would any other dependency:

```
<dependencies>
        <dependency>
                <groupId>com.opengamma.maths</groupId>
                <artifactId>og-maths</artifactId>
                <version>0.1.0-SNAPSHOT</version>
        </dependency>
</dependencies>
```

That's literally it! The jar has all the code included necessary to work out which platform you are on and what your CPU is like, you can just get on and use the maths library!


## For Developers/C++ Users

Note: To get a JAR like the one we distribute you'll need to build on all of Linux, OSX and Windows and then combine the jars produced into a single JAR. If you just want a JAR for a single platform, this step can be omitted.

### Requirements ####

Generally recent versions of the requirements are preferred:

#### Tools

* CMake 2.8.12+ (we test with 2.8.12)
* gfortran (we test with GCC 4.8.2)
* A C++ compiler supporting C++11 (we test with GCC 4.8.2)
* A C compiler supporting C99 (we test with GCC 4.8.2)
* Python 2.6 or 2.7 with argparse and yaml modules (we test with Python 2.6 on Linux and 2.7 on Windows/OS X)
* JDK 7 (we test with Oracle JDK 1.7.0_51 and 1.7.0_u21)
* Maven (we test with 3.0.5)

#### Operating Systems

* Windows 7/8 (we test on Windows 7)
* Mac OS X 10.9
* Linux (we test on CentOS 6, Debian 7, and Fedora 18 and 19)

#### Dependencies
* OG-Lapack, our OpenGamma LAPACK build, available [here](https://github.com/OpenGamma/OG-Lapack/).
* jcommander 1.17+ (we test with 1.17)
* TestNG 6.3.1+ (we test with 6.3.1)

#### Obtaining the OG-Maths source

The source code can be cloned from Github:

```
git clone https://github.com/OpenGamma/OG-Maths.git
```

Building
--------

The code uses the CMake toolchain to build. First, create an out-of-tree build dir:

```
mkdir build
cd build
```

****
**Before going any further, make sure your build environment `CLASSPATH` variable contains the paths to your `jcommander` and `TestNG` jars.**
****

Next run CMake. It is required that the path to the OG-Lapack exports file is provided ([build OG-Lapack locally first!](https://github.com/OpenGamma/OG-Lapack/)) so that the build can link against the LAPACK libraries and include them in the built JAR. This is done with the `LAPACK_EXPORT` CMake variable. Invocation of CMake:

```
cmake .. -DLAPACK_EXPORT=<path_to_og_lapack>/build/LAPACK_EXPORTS.cmake -DGCC_LIB_FOLDER=<path to gcc RTLs>
```

The build process includes the GCC runtime libraries, which are copied from `GCC_LIB_FOLDER`. As an alternative to specifying the location as an argument to CMake, the environment variable `GCC_LIB_FOLDER` may also be set to specify the location. Specifying the location on the command line overrides the value stored in the environment variable.

Build with:

```
make -j
```

We strongly recommend that you test your build, since different platforms, compilers, or system math libraries may have an effect on computed results. Test with:

```
export NPROCS=4
make test ARGS=-j${NPROCS}
```

#### Troubleshooting native library extraction

The JAR's main class runs an extremely simple test that forces the extraction and linking of native code. If you encounter problems with the native code tests passing (those prefixed `check_`) and the TestNG tests failing due to UnsatisfiedLinkError, you can use the main class as a test of linking whilst working through the issue:

```
java -jar jars/og-maths-0.1.0-SNAPSHOT.jar
```

Should this test succeed, the final line of the output will be:
```
Native library linkage appears to be working correctly.
```

If there are link errors, these will be uncaught and therefore printed to standard output. It can be easier to tell what the problem is this way than using the TestNG tests.

#### Working with the Java side in Eclipse
The project can be imported into Eclipse, but this is not necessary unless you intend to develop the actual Java library itself - most people will just want the pre-built JAR. If you are sure you want to work on the Java part of the library, first ensure your Eclipse install has the Maven toolchain installed and then:

1. Go to `"File -> Import"`.
2. Select the `"Maven -> Existing Maven Projects"` option.
3. In the popup, click the `"Browse..."` button.
4. Choose the `src/java` subdirectory of the OG-Maths source code (`OG-Maths`).
5. Click `"Finish"`

#### Refreshing the expression enum

Since it is difficult to work with generated Java code in eclipse, we generate and then
commit the expression enum for the Java side. In order to regenerate it, use the
`exprenum_java` target:

```
$ make exprenum_java
[100%] Regenerating ExprEnum.java
[100%] Built target exprenum_java
```



