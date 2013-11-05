#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#
include_directories(${CMAKE_SOURCE_DIR}/include ${JNI_INCLUDE_DIRS})
add_library(jinitialise SHARED rpath_hack.c)
set_target_properties(jinitialise PROPERTIES SOVERSION ${longdog_VERSION_MAJOR} VERSION ${longdog_VERSION})
jar_native_library(jinitialise)
