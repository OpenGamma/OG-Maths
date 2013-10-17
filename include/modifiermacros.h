/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _MODIFIERMACROS_H
#define _MODIFIERMACROS_H


#ifdef __MINGW32__ // windows needs dll exporting

// dllexport a C/C++ function
#define DLLEXPORT_C __attribute__ ((dllexport))

// dllexport a fortran function
#define DLLEXPORT_FORTRAN !GCC$ ATTRIBUTES DLLEXPORT::

#else // other OSs don't need exports

#define DLLEXPORT_C
#define DLLEXPORT_FORTRAN

#endif

#endif