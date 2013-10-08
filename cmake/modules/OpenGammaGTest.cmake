#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

set(gtests CACHE INTERNAL "GTest tests to run")

set(GTEST_XML_DIR ${CMAKE_BINARY_DIR}/gtest-output)
set(GTEST_REPORT ${GTEST_XML_DIR}/report.html)
set(GTEST_XSL ${CMAKE_SOURCE_DIR}/external/ant/junit-noframes.xsl)
file(MAKE_DIRECTORY ${GTEST_XML_DIR})

macro(add_gtest TESTNAME)
  get_target_property(TEST_LOC ${TESTNAME} LOCATION)
  add_test(${TESTNAME} ${TEST_LOC} --gtest_output=xml:${CMAKE_BINARY_DIR}/gtest-output/${TESTNAME}.xml)
  if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_test(${TESTNAME}_valgrind valgrind --leak-check=full --error-exitcode=1 ${TEST_LOC})
  endif()
  list(APPEND gtests ${TESTNAME})
  set(gtests ${gtests} CACHE INTERNAL "GTest tests to run")
endmacro()

# This must be run after you have added all your gtest tests
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
