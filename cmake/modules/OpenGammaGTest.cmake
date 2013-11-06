#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

include(CMakeParseArguments)

set(gtests CACHE INTERNAL "GTest tests to run")

set(GTEST_XML_DIR ${CMAKE_BINARY_DIR}/gtest-output)
set(GTEST_REPORT ${GTEST_XML_DIR}/report.html)
set(GTEST_XSL ${CMAKE_SOURCE_DIR}/external/ant/junit-noframes.xsl)
file(MAKE_DIRECTORY ${GTEST_XML_DIR})

# Executes the target as a test once, and then once again with valgrind
# Appropriate flags are added so that GTest outputs JUnit XML for later
# processing. The target is appended to the list of tests that is used in the
# add_gtest_report macro.
macro(add_gtest TESTNAME)
  get_target_property(TEST_LOC ${TESTNAME} LOCATION)
  add_test(${TESTNAME} ${TEST_LOC} --gtest_output=xml:${CMAKE_BINARY_DIR}/gtest-output/${TESTNAME}.xml)
  if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_test(${TESTNAME}_valgrind valgrind --leak-check=full --error-exitcode=1 ${TEST_LOC})
  endif()
  list(APPEND gtests ${TESTNAME})
  set(gtests ${gtests} CACHE INTERNAL "GTest tests to run")
endmacro()

# Builds a test executable that uses Gtest from the specified SOURCES and links
# it to any additional LINK_LIBRARIES. LINK_LIBRARIES does not need to include
# the GTest libraries - these are added automatically. The sources are also
# compiled with the specified COMPILE_DEFINITIONS.
macro(add_multitarget_gtest TESTNAME)
  cmake_parse_arguments(MTGTEST "" ""
                        "SOURCES;LINK_LIBRARIES;COMPILE_DEFINITIONS;TARGETS"
                        ${ARGN})
  foreach(TARGET ${MTGTEST_TARGETS})
    if(SUPPORT_${TARGET})
      set(TEST ${TESTNAME}_${TARGET})
      add_executable(${TEST} ${MTGTEST_SOURCES})
      set_target_properties(${TEST} PROPERTIES COMPILE_FLAGS ${OPT_FLAGS_${TARGET}})
      target_link_libraries(${TEST} gtest gtest_main pthread)
      foreach(LINK_LIBRARY ${MTGTEST_LINK_LIBRARIES})
        target_link_libraries(${TEST} ${LINK_LIBRARY}_${TARGET})
      endforeach()
      foreach(COMPILE_DEFINITION ${MTGTEST_COMPILE_DEFINITIONS})
        set_target_properties(${TEST} PROPERTIES COMPILE_DEFINITIONS ${COMPILE_DEFINITION})
      endforeach()
      add_gtest(${TEST})
    endif()
  endforeach()
endmacro()

# This must be run after you have added all your gtest tests. The output XMLs
# are aggregated and turned into a report when the testresport target is run,
# but only gtests that were already known about can be added.
macro(add_gtest_report)
  foreach(gtest ${gtests})
    list(APPEND GTEST_XMLS ${GTEST_XML_DIR}/${gtest}.xml)
  endforeach()
  add_custom_target(testaggregate
                    ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/external/ant/aggregate-xml.py
                    -o ${GTEST_XML_DIR}/testaggregate.xml
                    ${GTEST_XMLS}
                    COMMENT "Aggregating test results")
  add_custom_target(testreport
                    xsltproc -o ${GTEST_REPORT} ${GTEST_XSL} ${GTEST_XML_DIR}/testaggregate.xml
                    DEPENDS testaggregate
                    COMMENT "Generating GTest report.")
endmacro()
