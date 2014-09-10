#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

cmake_minimum_required (VERSION 2.8.6)

include(CMakeForceCompiler)

if(APPLE OR WIN32)
  # On Apple, we don't want to use Clang.
  # On Windows, we don't want to end up building with Visual Studio.
  CMAKE_FORCE_C_COMPILER(gcc GNU)
endif(APPLE OR WIN32)


project (og_maths)

# check command line is sensible
if(LAPACK_BUILDDIR)
  set(LAPACK_BUILDINFO ${LAPACK_BUILDDIR}/LAPACK_EXPORTS.cmake)
else()
#This is a temporary thing. Expect later to add in ExternalProject_add to get the code!!
  message( FATAL_ERROR "LAPACK_BUILDDIR must be defined: e.g. -DLAPACK_BUILDDIR=\"/path/to/OG-Lapack/build\"" )
endif()
if(IZY_BUILDDIR)
  set(IZY_BUILDINFO ${IZY_BUILDDIR}/IZY_EXPORTS.cmake)
else()
  message( FATAL_ERROR "IZY_BUILDDIR must be defined: e.g. -DIZY_BUILDDIR=\"/path/to/OG-Izy/build\"" )
endif()


enable_language(C)
enable_language(Fortran)

find_package(PythonInterp)
message(STATUS "Looking for Python found - ${PYTHONINTERP_FOUND}")
if (PYTHONINTERP_FOUND)
  message(STATUS "Using Python version ${PYTHON_VERSION_STRING}")
endif()

if(WIN32)
  set(CMAKE_THREAD_LIBS_INIT -mthreads)
else()
  # we force -pthread in preference to -lpthread because it sets -D_REENTRANT
  set(THREADS_HAVE_PTHREAD_ARG "THREADS_HAVE_PTHREAD_ARG")
  find_package(Threads)
endif()

set(og_maths_VERSION_MAJOR 0)
set(og_maths_VERSION_MINOR 0)
set(og_maths_VERSION_PATCH 1)
set(og_maths_VERSION
  ${og_maths_VERSION_MAJOR}.${og_maths_VERSION_MINOR}.${og_maths_VERSION_PATCH})

set(maven_VERSION_MAJOR 0)
set(maven_VERSION_MINOR 1)
set(maven_VERSION_PATCH 0)
set(maven_VERSION
  ${maven_VERSION_MAJOR}.${maven_VERSION_MINOR}.${maven_VERSION_PATCH})

# Append local module folder to module path
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# Construct platform code
include(MultiLib)
include(NativeLibraryUtils)
set_platform_code(PLATFORM_CODE)
if(PLATFORM_CODE STREQUAL "lin")
  set(PLATFORM_CODE "lnx")
endif()
if(PLATFORM_CODE STREQUAL "mac")
  set(PLATFORM_CODE "osx")
endif()

# JAR VARIABLES
set(JARDIR ${CMAKE_BINARY_DIR}/jars)

set(JAR_VERSION "${maven_VERSION}-SNAPSHOT")

set(JAR_NAME_NO_EXTN "og-maths-${PLATFORM_CODE}-${JAR_VERSION}" )
set(JAR_SOURCES_NAME_NO_EXTN "${JAR_NAME_NO_EXTN}-sources" )
set(JAR_TEST_NAME_NO_EXTN "${JAR_NAME_NO_EXTN}-tests" )
set(JAR_JAVADOC_NAME_NO_EXTN "${JAR_NAME_NO_EXTN}-javadoc" )

set(JAR_EXT ".jar")

set(JAR_NAME "${JAR_NAME_NO_EXTN}${JAR_EXT}" )
set(JAR_SOURCES_NAME "${JAR_SOURCES_NAME_NO_EXTN}${JAR_EXT}" )
set(JAR_TEST_NAME "${JAR_TEST_NAME_NO_EXTN}${JAR_EXT}" )
set(JAR_JAVADOC_NAME "${JAR_JAVADOC_NAME_NO_EXTN}${JAR_EXT}" )

set(PATH_AND_JAR_NAME "${JARDIR}/${JAR_NAME}" )
set(PATH_AND_JAR_SOURCES_NAME "${JARDIR}/${JAR_SOURCES_NAME}" )
set(PATH_AND_JAR_TEST_NAME "${JARDIR}/${JAR_TEST_NAME}" )
set(PATH_AND_JAR_JAVADOC_NAME "${JARDIR}/${JAR_JAVADOC_NAME}" )

# Use C99 and C++11, other flags for preventing common errors. Also add thread libs.
set(CMAKE_C_FLAGS "-std=c99 -Wall -Werror -Wextra -pedantic ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-sign-compare -Werror -Wextra -pedantic ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_CXX_FLAGS}")
# we want static libstdc++ everywhere!
set(CMAKE_SHARED_LINKER_FLAGS "-static-libstdc++")

# On Linux, add -rdynamic for stack traces
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -rdynamic")
endif()

# Use the native assembler on Apple in order to be able to generate AVX instructions.
if(APPLE)
  set(CMAKE_C_FLAGS "-Wa,-q ${CMAKE_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "-Wa,-q ${CMAKE_CXX_FLAGS}")
endif()

# Add Java
find_package(Java)
include(UseJava)
find_package(JNI)
include(JNIUtils)
include(NativeLibraryBuilder)

# Always include debug info in Java, since it does no harm and we don't have multiple
# build types for it in CMake as far as I am aware.
set(CMAKE_Java_COMPILE_FLAGS "${CMAKE_Java_COMPILE_FLAGS} -g")

# "Backwards compatibility" for javadoc jar creation
set(CMAKE_JAVA_TARGET_OUTPUT_DIR ${JARDIR})

set(CMAKE_JAVA_INCLUDE_PATH $ENV{CLASSPATH})

# Place executables and libraries in bin on Windows so that its linker can find them.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${og_maths_BINARY_DIR}/bin)

enable_testing()

# Set target types
set(TARGET_TYPES dbg std)
if(BITS64)
  set(TARGET_TYPES ${TARGET_TYPES} sse41 avx1)
endif()

# Tweak flags. These flags are similar to --ffast-math, except:
# -funsafe-math-optimizations is missing as it messes with the FPU control word
# which can cause segfaults when running through the JNI interface.
# -ffinite-math-only is missing as it messes with NaN and Inf in ways that
# break comparison behaviours.
set(FFAST_MATH_SAFE "-fno-math-errno  -fno-rounding-math -fno-signaling-nans -fcx-limited-range")

if (WIN32)
  # We need a larger than default stack to prevent the entrypt test
  # failing, and we need to force the export of all symbols since
  # they were all visible by default in the original implementation.
  set (PLATFORM_FLAGS "-Wl,--export-all-symbols -Wl,--stack,4194304")
  if (BITS32)
    # Store all float results in memory locations - casts them to double from
    # long double for consistency with 64-bit machines
    set(PLATFORM_FLAGS "${PLATFORM_FLAGS} -ffloat-store")
    # Export additional symbols without annotations
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--add-stdcall-alias")
  endif()
endif()

set(OPT_FLAGS_dbg "-DDEBUG ${CMAKE_C_FLAGS_DEBUG} ${PLATFORM_FLAGS}")
set(OPT_FLAGS_std "-g ${FFAST_MATH_SAFE} -O2 -funroll-loops -DNDEBUG ${PLATFORM_FLAGS}")
set(OPT_FLAGS_sse41 "-g ${OPT_FLAGS_std} -march=core2 -msse4.1")
set(OPT_FLAGS_avx1 "-g ${OPT_FLAGS_std}  -march=corei7-avx -mavx256-split-unaligned-load -mavx256-split-unaligned-store")

# Gtest
add_subdirectory(external/gtest-1.6.0)
include_directories(${gtest_SOURCE_DIR}/include)
include(OpenGammaGTest)

# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doc
                    ${DOXYGEN_EXECUTABLE}
                    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Generating API documentation with Doxygen"
                    VERBATIM
  )
endif(DOXYGEN_FOUND)

# Set the name of the java tests so we can exclude then from C++ coverage
set(TESTNG_TEST TestNG)
set(NATIVE_SELF_TEST NativeLibrariesSelfTest)
set(JAVA_TESTS_REGEX "\"(${TESTNG_TEST})|(${NATIVE_SELF_TEST})\"")
set(DEBUG_TESTS_REGEX "\".*dbg$$\"")
# testng annotation listeners
set(TESTNG_LISTENERS "com.opengamma.maths.fuzzer.TransformAnnotationFuzzOnly")

# Coverage - must build underneath the source dir for this to work.
# This should be run before make test to avoid gathering coverage info
# for code that was run as a result of calling it through the Java interface.
if (NOT WIN32)
  set(OPT_FLAGS_dbg "--coverage ${OPT_FLAGS_dbg}")
  add_custom_target(gencovinfo
                    COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process -R ${DEBUG_TESTS_REGEX} -E ${JAVA_TESTS_REGEX}
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    COMMENT "Running tests to gather C++ coverage information")
  add_custom_target(coverageinfo
                    COMMAND lcov --capture -i -d . --output-file ${CMAKE_BINARY_DIR}/coverage-initial.info --no-external
                    COMMAND lcov --capture -d . --output-file ${CMAKE_BINARY_DIR}/coverage-run.info --no-external
                    COMMAND lcov -a ${CMAKE_BINARY_DIR}/coverage-initial.info -a ${CMAKE_BINARY_DIR}/coverage-run.info --output-file ${CMAKE_BINARY_DIR}/coverage.info --no-external
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    DEPENDS gencovinfo
                    COMMENT "Capturing coverage info")

  add_custom_target(coverageextract
                     DEPENDS coverageinfo
                     COMMAND lcov -o coverage-extract.info --extract coverage.info "\"${CMAKE_SOURCE_DIR}/include/*\"" "\"${CMAKE_SOURCE_DIR}/src/*\"" "\"${CMAKE_BINARY_DIR}/src/*\"" "\"${CMAKE_BINARY_DIR}/include/*\""
                     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                     COMMENT "Removing external from coverage info")

  add_custom_target(coverage
                    COMMAND genhtml -o lcov.out coverage-extract.info
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    DEPENDS coverageextract
                    COMMENT "Generating coverage report")
endif()

# Whether we want noisy debug messages
if (DEBUGMESSAGES)
  add_definitions(-DDEBUGMESSAGES)
endif()

# Where the generated include files live
set(BIN_INCLUDE_DIR ${CMAKE_BINARY_DIR}/include)

# Add version info file
if(NOT DEFINED BUILDNUMBER)
  set(BUILDNUMBER local)
endif()
add_custom_target(verinfo
                  ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/cmake/verinfo.py ${CMAKE_BINARY_DIR}/verinfo.yaml ${JAR_VERSION} ${LAPACK_BUILDDIR}/verinfo.yaml ${IZY_BUILDDIR}/verinfo.yaml ${BUILDNUMBER}
                  COMMENT "Generating version info")

add_subdirectory(src)

add_gtest_report()

if (WIN32)
  string(REPLACE ";" "\\;" CLASSPATH "${CMAKE_JAVA_INCLUDE_PATH};${PATH_AND_JAR_NAME};${PATH_AND_JAR_TEST_NAME}")
  # Test each target
  foreach(TARGET ${TARGET_TYPES})
    if(SUPPORT_${TARGET})
      add_test(${TESTNG_TEST}_${TARGET} ${Java_JAVA_EXECUTABLE} -Xcheck:jni -DinstructionSet=${TARGET} -verbose org.testng.TestNG -listener ${TESTNG_LISTENERS} -testjar ${PATH_AND_JAR_TEST_NAME})
      set_tests_properties(${TESTNG_TEST}_${TARGET} PROPERTIES ENVIRONMENT "CLASSPATH=${CLASSPATH}")
    endif()
  endforeach()
  # Test with no target specified
  add_test(${TESTNG_TEST} ${Java_JAVA_EXECUTABLE} -Xcheck:jni -verbose org.testng.TestNG -listener ${TESTNG_LISTENERS} -testjar ${PATH_AND_JAR_TEST_NAME})
  set_tests_properties(${TESTNG_TEST} PROPERTIES ENVIRONMENT "CLASSPATH=${CLASSPATH}")
else()
  set(JAVA_CP ${CMAKE_JAVA_INCLUDE_PATH}:${PATH_AND_JAR_NAME}:${PATH_AND_JAR_TEST_NAME})
  set(JAVA_TESTNG_ARGS -Xcheck:jni -verbose -cp ${JAVA_CP} org.testng.TestNG -listener ${TESTNG_LISTENERS} -testjar ${PATH_AND_JAR_TEST_NAME})
  # Test each target
  foreach(TARGET ${TARGET_TYPES})
    if(SUPPORT_${TARGET})
      add_test(${TESTNG_TEST}_${TARGET} ${Java_JAVA_EXECUTABLE} -DinstructionSet=${TARGET} ${JAVA_TESTNG_ARGS})
    endif()
  endforeach()
  # Test with no target specified
  add_test(${TESTNG_TEST} ${Java_JAVA_EXECUTABLE} ${JAVA_TESTNG_ARGS})
  set(JACOCOJAR $ENV{JACOCOJAR})
  add_custom_target(jacocorun
                    COMMAND ${Java_JAVA_EXECUTABLE} -javaagent:${JACOCOJAR} ${JAVA_TESTNG_ARGS}
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    COMMENT "Gathering Java test coverage information")
  add_custom_target(jacoco
                    COMMAND ${Java_JAVA_EXECUTABLE} com.opengamma.jacocoreporter.ReportGenerator jacoco.exec
                            ${JARDIR}/CMakeFiles/${JAR_NAME_NO_EXTN}.dir ${CMAKE_SOURCE_DIR}/src/java/src/main/java
                            coveragereport
                    DEPENDS jacocorun
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    COMMENT "Generating Java test coverage report")
endif()

# Add testing of blob combiner
add_test(NAME CombineJarvester
         WORKING_DIRECTORY ${og_maths_SOURCE_DIR}/combine
         COMMAND ${PYTHON_EXECUTABLE} test_combine.py)

# Installation to local maven repository
add_custom_target(maven_install
                  COMMAND mvn install:install-file
                          -Dfile=${JAR_NAME}
                          -DgroupId=com.opengamma.maths
                          -DartifactId=og-maths
                          -Dversion=${JAR_VERSION}
                          -Dpackaging=jar
                          -Djavadoc=${JAR_JAVADOC_NAME}
                          -Dsources=${JAR_SOURCES_NAME}
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/jars)

message("Build type: " ${CMAKE_BUILD_TYPE})

message("
  Package " ${CMAKE_PROJECT_NAME} " version " ${OGMW_VERSION} "
  Prefix.....................: " ${CMAKE_INSTALL_PREFIX} "
  C Compiler.................: " ${CMAKE_C_COMPILER} "
  C Flags....................: " ${CMAKE_C_FLAGS} "
  C Flags (release)..........: " ${CMAKE_C_FLAGS_RELEASE} "
  C Flags (debug)............: " ${CMAKE_C_FLAGS_DEBUG} "
  C Flags (relwithdebinfo)...: " ${CMAKE_C_FLAGS_RELWITHDEBINFO} "
  C++ Compiler...............: " ${CMAKE_CXX_COMPILER} "
  C++ Flags..................: " ${CMAKE_CXX_FLAGS} "
  C++ Flags (release)........: " ${CMAKE_CXX_FLAGS_RELEASE} "
  C++ Flags (debug)..........: " ${CMAKE_CXX_FLAGS_DEBUG} "
  C++ Flags (relwithdebinfo).: " ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} "
  Java Compiler..............: " ${Java_JAVAC_EXECUTABLE} "
  Java Compiler flags........: " ${CMAKE_Java_COMPILE_FLAGS} "
  JavaH Tool.................: " ${Java_JAVAH_EXECUTABLE} "
  Jar Tool...................: " ${Java_JAR_EXECUTABLE} "
  Java Library...............: " ${JAVA_JVM_LIBRARY} "
  Python executable..........: " ${PYTHON_EXECUTABLE} "
  Threads flag..............:: " ${CMAKE_THREAD_LIBS_INIT} "
  ")
