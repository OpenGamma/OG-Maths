#
# Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

include_directories (${NATIVE_HEADER_DIR} ${CMAKE_SOURCE_DIR}/include ${JNIDIR} ${JNI_INCLUDE_DIRS} ${BIN_INCLUDE_DIR})
SET(CMAKE_FC_FLAGS  "${CMAKE_FC_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -cpp" )

link_directories(${og_maths_BINARY_DIR}/src/librdag)

set(TEST_HELPER_SOURCES
    testnodes
    )

add_multitarget_library(testhelper
                        VERSION ${og_maths_VERSION}
                        SOVERSION ${og_maths_VERSION_MAJOR}
                        SOURCES ${TEST_HELPER_SOURCES}
                        LINK_MULTILIBRARIES rdag
                        LINK_LIBRARIES gtest gtest_main pthread
                        TARGETS ${TARGET_TYPES})

set(TESTS
    check_ctranspose
    check_inv
    check_lu
    check_mldivide
    check_mtimes
    check_norm2
    check_pinv
    check_selectresult
    check_svd
    check_transpose
    # auto generated
    check_plus
    check_minus
    check_rdivide
    check_times
   )

# Compile and link each test and add to the list of tests
foreach(TEST ${TESTS})
  add_multitarget_gtest(${TEST}
                        SOURCES ${TEST}.cc
                        LINK_LIBRARIES testhelper rdag
                        TARGETS ${TARGET_TYPES}
                        SUPPRESSIONS ${CMAKE_CURRENT_SOURCE_DIR}/../valgrind.supp)
endforeach()
