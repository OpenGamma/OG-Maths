#
# Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

include_directories (${NATIVE_HEADER_DIR} ${CMAKE_SOURCE_DIR}/include ${JNIDIR} ${JNI_INCLUDE_DIRS} ${BIN_INCLUDE_DIR})
SET(CMAKE_FC_FLAGS  "${CMAKE_FC_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -cpp" )

link_directories(${longdog_BINARY_DIR}/src/librdag)

set(TEST_HELPER_SOURCES
    testnodes
    )

add_multitarget_library(testhelper
                        VERSION ${longdog_VERSION}
                        SOVERSION ${longdog_VERSION_MAJOR}
                        SOURCES ${TEST_HELPER_SOURCES}
                        TARGETS ${TARGET_TYPES})

set(TESTS
    check_norm2
   )

# Compile and link each test and add to the list of tests
foreach(TEST ${TESTS})
  add_multitarget_gtest(${TEST}
                        SOURCES ${TEST}.cc
                        LINK_LIBRARIES rdag testhelper
                        TARGETS ${TARGET_TYPES})
endforeach()
