/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef RPATH_HACK_H
#define RPATH_HACK_H

#ifdef __MINGW32__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define WINSLASH '\\'

#ifdef __cplusplus
extern "C"
#endif
void rpath_hack();

#ifdef __cplusplus
extern "C"
#endif
HMODULE getThisLibHandle();

#endif //__MINGW32__

#endif // RPATH_HACK_H

