/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _IZY_HH
#define _IZY_HH

#include <iostream>
#include <memory>
#include "libizy/izy.h"
#include "numerictypes.hh"

/**
 * The izy namespace contains templated variants of functions that vary
 * <real8,complex16> in their signature.
 */
namespace izy
{

template<typename T>
std::unique_ptr<T[]>
vx_cos(const int count, T * vector);

}

#endif