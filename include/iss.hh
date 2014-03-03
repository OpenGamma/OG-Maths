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

bool isScalar(const OGTerminal *);
bool isMatrix(const OGTerminal *);
bool isVector(const OGTerminal *);
bool isReal(const OGTerminal *);
bool isComplex(const OGTerminal *);

}
#endif