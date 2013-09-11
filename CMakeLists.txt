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

# Gtest
add_subdirectory(external/gtest-1.6.0)
include_directories(${gtest_SOURCE_DIR}/include)
include(GTestTests)

enable_testing()

add_subdirectory(src)

set(LONGDOG ${PROJECT_NAME}-${longdog_VERSION})
set(LONGDOGJAR ${JARDIR}/${LONGDOG}.jar)
set(LONGDOGTESTJAR ${JARDIR}/${LONGDOG}-tests.jar)

if (WIN32)
  add_test(Test ${Java_JAVA_EXECUTABLE} -verbose org.testng.TestNG -testjar jars/${LONGDOG}-tests.jar)
  string(REPLACE ";" "\\;" CLASSPATH "${CMAKE_JAVA_INCLUDE_PATH};${LONGDOGJAR};${LONGDOGTESTJAR}")
  set_tests_properties(Test PROPERTIES ENVIRONMENT "CLASSPATH=${CLASSPATH}")
else()
  set(JAVA_CP ${CMAKE_JAVA_INCLUDE_PATH}:${LONGDOGJAR}:${LONGDOGTESTJAR})
  add_test(Test ${Java_JAVA_EXECUTABLE} -verbose -cp ${JAVA_CP} org.testng.TestNG -testjar jars/${LONGDOG}-tests.jar)
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

