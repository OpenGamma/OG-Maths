#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#
include_directories(${CMAKE_SOURCE_DIR}/include ${JNI_INCLUDE_DIRS})
add_library(jinitialise SHARED jnionloadimpl.c rpath_hack.c probeinstructionset.c)
set_target_properties(jinitialise PROPERTIES SOVERSION ${og_maths_VERSION_MAJOR} VERSION ${og_maths_VERSION})
install(TARGETS jinitialise DESTINATION lib)
jar_native_library(jinitialise)
