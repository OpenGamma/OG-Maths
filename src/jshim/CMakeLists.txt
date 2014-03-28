#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

include_directories (${NATIVE_HEADER_DIR} ${CMAKE_SOURCE_DIR}/include ${JNIDIR} ${JNI_INCLUDE_DIRS} ${BIN_INCLUDE_DIR})
SET(CMAKE_FC_FLAGS  "${CMAKE_FC_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -cpp" )
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wl,--no-undefined")
SET(CMAKE_CC_FLAGS  "${CMAKE_CC_FLAGS} -Wl,--no-undefined -frepo")

set_source_files_properties(${JSHIM_GENERATED} PROPERTIES GENERATED TRUE)

set(JSHIM_SOURCES jshim.cc jterminals.cc jbindings.cc exprfactory.cc jdispatch.cc jvmmanager.cc
                  ${JSHIM_GENERATED})

add_multitarget_library(jshim
                        VERSION ${og_maths_VERSION}
                        SOVERSION ${og_maths_VERSION_MAJOR}
                        SOURCES ${JSHIM_SOURCES}
                        TARGETS ${TARGET_TYPES}
                        DEPENDS createexpr_cc
                        LINK_MULTILIBRARIES rdag
                        )


add_multitarget_library(jshimtest
                        VERSION ${og_maths_VERSION}
                        SOVERSION ${og_maths_VERSION_MAJOR}
                        SOURCES ${JSHIM_SOURCES}
                        TARGETS ${TARGET_TYPES}
                        DEPENDS createexpr_cc
                        LINK_MULTILIBRARIES rdag
                        COMPILE_DEFINITIONS _FAKE_JNI_H=1)

jar_native_multitarget_library(jshim TARGETS ${TARGET_TYPES})

add_subdirectory(test)
