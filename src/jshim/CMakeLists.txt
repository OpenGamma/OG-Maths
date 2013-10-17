#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#
add_subdirectory(test)

include_directories (${NATIVE_HEADER_DIR} ${CMAKE_SOURCE_DIR}/include ${JNIDIR} ${JNI_INCLUDE_DIRS})
SET(CMAKE_FC_FLAGS  "${CMAKE_FC_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -cpp" )
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wl,--no-undefined")
SET(CMAKE_CC_FLAGS  "${CMAKE_CC_FLAGS} -Wl,--no-undefined -frepo")
 

add_library(jshim SHARED jshim.cc jterminals.cc jbindings.cc exprfactory.cc jdispatch.cc jvmmanager.cc)
add_dependencies(jshim rdag)
target_link_libraries(jshim rdag)
set_target_properties(jshim PROPERTIES SOVERSION ${longdog_VERSION_MAJOR} VERSION ${longdog_VERSION})
install(TARGETS jshim DESTINATION lib)

add_library(jshimtest SHARED jshim.cc jterminals.cc jbindings.cc exprfactory.cc jdispatch.cc jvmmanager.cc)
add_dependencies(jshimtest rdag)
target_link_libraries(jshimtest rdag)
set_target_properties(jshimtest PROPERTIES SOVERSION ${longdog_VERSION_MAJOR} VERSION ${longdog_VERSION} COMPILE_DEFINITIONS _FAKE_JNI_H=1)

jar_native_library(jshim)
