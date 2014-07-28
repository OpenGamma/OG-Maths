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
vx_abs(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_abs(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_abs(const int count, complex16 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vz_abs(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_acos(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_acos(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_acos(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_acos(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_add(const int count, real8 * vector0, real8 * vector1)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_add(&count, vector0, &detail::zero, vector1, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_add(const int count, complex16 * vector0, complex16 * vector1)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_add(&count, vector0, &detail::zero, vector1, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_addx(const int count, real8 * vector0, real8 scalar0)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_addx(&count, vector0, &detail::zero, &scalar0, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_addx(const int count, complex16 * vector0, complex16 scalar0)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_addx(&count, vector0, &detail::zero, &scalar0, &detail::zero, ret.get(), &detail::zero);
  return ret;
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

template<>
std::unique_ptr<real8[]>
vx_mul(const int count, real8 * vector0, real8 * vector1)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_mul(&count, vector0, &detail::zero, vector1, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_mul(const int count, complex16 * vector0, complex16 * vector1)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_mul(&count, vector0, &detail::zero, vector1, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_mulx(const int count, real8 * vector0, real8 scalar0)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_mulx(&count, vector0, &detail::zero, &scalar0, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_mulx(const int count, complex16 * vector0, complex16 scalar0)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_mulx(&count, vector0, &detail::zero, &scalar0, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

}
