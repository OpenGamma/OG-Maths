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
class librdagException: public exception
{
  public:
    virtual const char* what() const throw();
};

} // namespace librdag

namespace convert {

/*
 * General exception thing
 */
class convertException: public runtime_error
{
  using runtime_error::runtime_error;
};

} // namespace convert

#endif
