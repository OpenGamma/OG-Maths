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


project (longdog)

enable_language(C)
enable_language(Fortran)

find_package(PythonInterp)
message(STATUS "Looking for Python found - ${PYTHONINTERP_FOUND}")
if (PYTHONINTERP_FOUND)
  message(STATUS "Using Python version ${PYTHON_VERSION_STRING}")
endif()


set(longdog_VERSION_MAJOR 0)
set(longdog_VERSION_MINOR 0)
set(longdog_VERSION_PATCH 1)
set(longdog_VERSION
  ${longdog_VERSION_MAJOR}.${longdog_VERSION_MINOR}.${longdog_VERSION_PATCH})

# Append local module folder to module path
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# tweak flags
set(CMAKE_C_FLAGS "-std=c99 -Wall -Werror -Wextra -pedantic ${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "-DDEBUG ${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_RELEASE "-ffast-math -O2 -march=core2 -funroll-loops -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-ffast-math -march=core2 -funroll-loops ${CMAKE_C_FLAGS_RELWITHDEBINFO}")

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Werror -Wextra -pedantic ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG ${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "-ffast-math -O2 -march=core2 -funroll-loops -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-ffast-math -march=core2 -funroll-loops ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")


# Add Java
find_package(Java)
include(UseJava)
find_package(JNI)
include(JNIUtils)
include(NativeLibraryBuilder)

# Always include debug info in Java, since it does no harm and we don't have multiple
# build types for it in CMake as far as I am aware.
set(CMAKE_Java_COMPILE_FLAGS "${CMAKE_Java_COMPILE_FLAGS} -g")

set(JARDIR ${CMAKE_BINARY_DIR}/jars)
# "Backwards compatibility" for javadoc jar creation
set(CMAKE_JAVA_TARGET_OUTPUT_DIR ${JARDIR})

set(CMAKE_JAVA_INCLUDE_PATH $ENV{CLASSPATH})

# Place executables and libraries in bin on Windows so that its linker can find them.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${longdog_BINARY_DIR}/bin)

enable_testing()

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

# Coverage - must build underneath the source dir for this to work.
# This should be run before make test to avoid gathering coverage info
# for code that was run as a result of calling it through the Java interface.
if (CMAKE_BUILD_TYPE MATCHES "Debug" AND NOT WIN32)
  set(CMAKE_C_FLAGS_DEBUG "--coverage ${CMAKE_C_FLAGS_DEBUG}")
  set(CMAKE_CXX_FLAGS_DEBUG "--coverage ${CMAKE_CXX_FLAGS_DEBUG}")
  add_custom_target(gencovinfo
                    COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process -E ${JAVA_TESTS_REGEX}
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
                     COMMAND lcov -o coverage-extract.info --extract coverage.info "\"${CMAKE_SOURCE_DIR}/include/*\"" "\"${CMAKE_SOURCE_DIR}/src/*\""
                     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                     COMMENT "Removing external from coverage info")

  add_custom_target(coverage
                    COMMAND genhtml -o lcov.out coverage-extract.info
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    DEPENDS coverageextract
                    COMMENT "Generating coverage report")
endif()


add_subdirectory(src)

add_gtest_report()

set(LONGDOG ${PROJECT_NAME}-${longdog_VERSION})
set(LONGDOGJAR ${JARDIR}/${LONGDOG}.jar)
set(LONGDOGTESTJAR ${JARDIR}/${LONGDOG}-tests.jar)

if (WIN32)
  add_test(${TESTNG_TEST} ${Java_JAVA_EXECUTABLE} -verbose org.testng.TestNG -testjar jars/${LONGDOG}-tests.jar)
  string(REPLACE ";" "\\;" CLASSPATH "${CMAKE_JAVA_INCLUDE_PATH};${LONGDOGJAR};${LONGDOGTESTJAR}")
  set_tests_properties(${TESTNG_TEST} PROPERTIES ENVIRONMENT "CLASSPATH=${CLASSPATH}")
else()
  set(JAVA_CP ${CMAKE_JAVA_INCLUDE_PATH}:${LONGDOGJAR}:${LONGDOGTESTJAR})
  add_test(${TESTNG_TEST} ${Java_JAVA_EXECUTABLE} -verbose -cp ${JAVA_CP} org.testng.TestNG -testjar jars/${LONGDOG}-tests.jar)
endif()


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
  Python executable..........: " ${PYTHON_EXECUTABLE} "
  ")
