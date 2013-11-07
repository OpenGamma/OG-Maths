#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

include_directories (${CMAKE_SOURCE_DIR}/include ${BIN_INCLUDE_DIR})
SET(CMAKE_FC_FLAGS  "${CMAKE_FC_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -cpp" )

set(TESTS check_jvmmanager_fakejni)

# Compile and link each test and add to the list of tests
foreach(TEST ${TESTS})
  add_multitarget_gtest(${TEST}
                        SOURCES ${TEST}.cc
                        LINK_LIBRARIES jshimtest
                        TARGETS ${TARGET_TYPES}
                        COMPILE_DEFINITIONS _FAKE_JNI_H=1)
endforeach()

