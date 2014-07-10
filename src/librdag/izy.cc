/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "izy.hh"
#include "exceptions.hh"

#include <memory>

using namespace librdag;

namespace izy {

namespace detail
{
  const int zero = 0;
}

template<>
std::unique_ptr<real8[]>
vx_cos(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_cos(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_cos(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_cos(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

}
