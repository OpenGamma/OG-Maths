#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

include_directories (${CMAKE_SOURCE_DIR}/include)
SET(CMAKE_FC_FLAGS  "${CMAKE_FC_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -cpp" )

set(TESTS check_jvmmanager_fakejni)

# Compile and link each test and add to the list of tests
foreach(TEST ${TESTS})
  add_executable(${TEST} ${TEST}.cc)
  set_target_properties(${TEST} PROPERTIES COMPILE_DEFINITIONS _FAKE_JNI_H=1)
  target_link_libraries(${TEST} jshimtest gtest gtest_main pthread)
  get_target_property(TEST_LOC ${TEST} LOCATION)
  add_gtest(${TEST})
endforeach()

