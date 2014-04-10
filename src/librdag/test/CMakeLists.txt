#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

include_directories (${NATIVE_HEADER_DIR} ${CMAKE_SOURCE_DIR}/include ${JNIDIR} ${JNI_INCLUDE_DIRS} ${BIN_INCLUDE_DIR})
SET(CMAKE_FC_FLAGS  "${CMAKE_FC_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -cpp" )

link_directories(${og_maths_BINARY_DIR}/src/librdag)

set(TESTS
  check_containers
  check_convertto
  check_dispatch
  check_entrypt
  check_equals
  check_execution
  check_expressions
  check_iss
  check_lapack
  check_numerictypes
  check_rtti
  check_terminals
  check_terminals_abstract_regression
  )

if(NOT WIN32)
  set(TESTS ${TESTS} check_exceptions)
endif()

# Compile and link each test and add to the list of tests
foreach(TEST ${TESTS})
  add_multitarget_gtest(${TEST}
                        SOURCES ${TEST}.cc
                        LINK_LIBRARIES rdag
                        TARGETS ${TARGET_TYPES}
                        SUPPRESSIONS ${CMAKE_CURRENT_SOURCE_DIR}/valgrind.supp)
endforeach()

add_subdirectory(nodes)
