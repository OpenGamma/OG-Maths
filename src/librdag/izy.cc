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
vx_acosh(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_acosh(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_acosh(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_acosh(&count, vector, &detail::zero, ret.get(), &detail::zero);
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
vx_arg(const int count, complex16 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vz_arg(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_asin(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_asin(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_asin(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_asin(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_asinh(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_asinh(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_asinh(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_asinh(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_atan(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_atan(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_atan(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_atan(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_atan2(const int count, real8 * vector0, real8 * vector1)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_atan2(&count, vector0, &detail::zero, vector1, &detail::zero, ret.get(), &detail::zero);
  return ret;
}


template<>
std::unique_ptr<real8[]>
vx_atanh(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_atanh(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_atanh(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_atanh(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_cbrt(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_cbrt(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_cdfnorm(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_cdfnorm(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_ceil(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_ceil(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_conj(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_conj(&count, vector, &detail::zero, ret.get(), &detail::zero);
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
vx_cosh(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_cosh(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_cosh(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_cosh(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_div(const int count, real8 * vector0, real8 * vector1)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_div(&count, vector0, &detail::zero, vector1, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_div(const int count, complex16 * vector0, complex16 * vector1)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_div(&count, vector0, &detail::zero, vector1, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_divx(const int count, real8 * vector0, real8 scalar0)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_divx(&count, vector0, &detail::zero, &scalar0, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_divx(const int count, complex16 * vector0, complex16 scalar0)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_divx(&count, vector0, &detail::zero, &scalar0, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_erf(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_erf(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_erfc(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_erfc(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_exp(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_exp(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_exp(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_exp(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_expm1(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_expm1(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_floor(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_floor(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_hypot(const int count, real8 * vector0, real8 * vector1)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_hypot(&count, vector0, &detail::zero, vector1, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_inv(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_inv(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_invcbrt(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_invcbrt(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_invsqrt(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_invsqrt(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_lgamma(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_lgamma(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_ln(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_ln(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_ln(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_ln(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_log10(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_log10(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_log10(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_log10(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_log1p(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_log1p(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::pair<std::unique_ptr<real8[]>,std::unique_ptr<real8[]>>
vx_modf(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> integral (new real8[count]);
  std::unique_ptr<real8[]> fractional (new real8[count]);
  vd_modf(&count, vector, &detail::zero, integral.get(), &detail::zero, fractional.get(), &detail::zero);
  return std::pair<std::unique_ptr<real8[]>,std::unique_ptr<real8[]>>(std::move(integral),std::move(fractional));
}

template<>
std::unique_ptr<complex16[]>
vx_mulbyconj(const int count, complex16 * vector0, complex16 * vector1)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_mulbyconj(&count, vector0, &detail::zero, vector1, &detail::zero, ret.get(), &detail::zero);
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


template<>
std::unique_ptr<real8[]>
vx_nearbyint(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_nearbyint(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_negate(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_negate(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_negate(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_negate(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_negatereal(const int count, complex16 * vector0)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_negatereal(&count, vector0, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_pow(const int count, real8 * vector0, real8 * vector1)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_pow(&count, vector0, &detail::zero, vector1, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_pow(const int count, complex16 * vector0, complex16 * vector1)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_pow(&count, vector0, &detail::zero, vector1, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_pow2o3(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_pow2o3(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_pow3o2(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_pow3o2(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_powx(const int count, real8 * vector0, real8 scalar0)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_powx(&count, vector0, &detail::zero, &scalar0, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_powx(const int count, complex16 * vector0, complex16 scalar0)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_powx(&count, vector0, &detail::zero, &scalar0, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_round(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_round(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_sin(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_sin(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_sin(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_sin(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::pair<std::unique_ptr<real8[]>,std::unique_ptr<real8[]>>
vx_sincos(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> sinpart (new real8[count]);
  std::unique_ptr<real8[]> cospart (new real8[count]);
  vd_sincos(&count, vector, &detail::zero, sinpart.get(), &detail::zero, cospart.get(), &detail::zero);
  return std::pair<std::unique_ptr<real8[]>,std::unique_ptr<real8[]>>(std::move(sinpart),std::move(cospart));
}

template<>
std::unique_ptr<real8[]>
vx_sinh(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_sinh(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_sinh(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_sinh(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_sqr(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_sqr(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_sqrt(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_sqrt(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_sqrt(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_sqrt(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_sub(const int count, real8 * vector0, real8 * vector1)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_sub(&count, vector0, &detail::zero, vector1, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_sub(const int count, complex16 * vector0, complex16 * vector1)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_sub(&count, vector0, &detail::zero, vector1, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_subx(const int count, real8 * vector0, real8 scalar0)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_subx(&count, vector0, &detail::zero, &scalar0, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_subx(const int count, complex16 * vector0, complex16 scalar0)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_subx(&count, vector0, &detail::zero, &scalar0, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_tan(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_tan(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_tan(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_tan(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_tanh(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_tanh(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_tanh(const int count, complex16 * vector)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_tanh(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_tgamma(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_tgamma(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_trunc(const int count, real8 * vector)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_trunc(&count, vector, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_xdiv(const int count, real8 scalar0, real8 * vector0)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_xdiv(&count, &scalar0, &detail::zero, vector0, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_xdiv(const int count, complex16 scalar0, complex16 * vector0)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_xdiv(&count, &scalar0, &detail::zero, vector0, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<real8[]>
vx_xsub(const int count, real8 scalar0, real8 * vector0)
{
  std::unique_ptr<real8[]> ret (new real8[count]);
  vd_xsub(&count, &scalar0, &detail::zero, vector0, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

template<>
std::unique_ptr<complex16[]>
vx_xsub(const int count, complex16 scalar0, complex16 * vector0)
{
  std::unique_ptr<complex16[]> ret (new complex16[count]);
  vz_xsub(&count, &scalar0, &detail::zero, vector0, &detail::zero, ret.get(), &detail::zero);
  return ret;
}

}
