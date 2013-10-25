/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _ISS_HH
#define _ISS_HH
#include "terminal.hh"
#include "modifiermacros.h"

namespace librdag {

DLLEXPORT_C bool isScalar(const OGTerminal *);
DLLEXPORT_C bool isMatrix(const OGTerminal *);
DLLEXPORT_C bool isReal(const OGTerminal *);
DLLEXPORT_C bool isComplex(const OGTerminal *);

}
#endif