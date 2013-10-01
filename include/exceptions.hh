/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _EXCEPTIONS_HH
#define _EXCEPTIONS_HH

#include <exception>
#include <stdexcept>

using namespace std;

namespace librdag {

/*
 * General exception thing
 */
class rdag_error: public runtime_error
{
  using runtime_error::runtime_error;
};

} // namespace librdag

namespace convert {

/*
 * General exception thing
 */
class convert_error: public runtime_error
{
  using runtime_error::runtime_error;
};

} // namespace convert

#endif
