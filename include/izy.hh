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

/**
 * Vectorised abs() function, y:=abs(a).
 * Computes the absolute value of \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process..
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type <real8> of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<real8[]>
vx_abs(const int count, T * a);


/**
 * Vectorised acos() function, y:=acos(a).
 * Computes the inverse cosine of \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_acos(const int count, T * a);


/**
 * Vectorised acosh() function, y:=acosh(a).
 * Computes the inverse hyperbolic cosine of \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of\a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_acosh(const int count, T * a);


/**
 * Vectorised add() function, y:=a+b
 * Computes the arithmetic sum of \a a and \a b such that y:=a+b
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the first vector of data on which the operation shall be performed.
 * @param b the second vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a and \a b.
 */
template<typename T>
std::unique_ptr<T[]>
vx_add(const int count, T * a, T * b);


/**
 * Vectorised addx function, y:=a+(constant)b.
 * Computes the arithmetic sum of \a a and a constant \a b such that y:=a+(constant)b
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @param b a scalar constant on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of
 * \a a and the value \a b.
 */
template<typename T>
std::unique_ptr<T[]>
vx_addx(const int count, T * a, T b);


/**
 * Vectorised arg function, y:=arg(a).
 * Computes the argument (phase angle) of the complex number \a a.
 * @param T the data type, <complex16> accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<real8[]>
vx_arg(const int count, T * a);


/**
 * Vectorised arc sine function, y:=asin(a).
 * Computes the inverse sine of \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_asin(const int count, T * a);


/**
 * Vectorised asinh() function, y:=asinh(a).
 * Computes the inverse hyperbolic sine of \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_asinh(const int count, T * a);


/**
 * Vectorised atan() function, y:=atan(a).
 * Computes the inverse tangent of \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_atan(const int count, T * a);


/**
 * Vectorised atan2() function, y:=atan2(a,b).
 * Computes the principal values of the inverse tangent of a/b and further uses the signs 
 * given in the two arguments to determine the quandrant in which the result lies.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the first vector of data on which the operation shall be performed.
 * @param b the second vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_atan2(const int count, T * a, T * b);


/**
 * Vectorised atanh() function, y:=atanh(a).
 * Computes the inverse hyperbolic tangent of \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_atanh(const int count, T * a);


/**
 * Vectorised cbrt() function, y:=cbrt(a).
 * Computes the cube root of \a a.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_cbrt(const int count, T * a);


/**
 * Vectorised cdfnorm() function, y:=cdfnorm(a).
 * Computes the cumulative normal distribution of \a a.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.

 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of 
 * \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_cdfnorm(const int count, T * a);


/**
 * Vectorised cdfnorminv() function, y:=cdfnorminv(a).
 * Computes the inverse cumulative normal distribution of \a a.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_cdfnorminv(const int count, T * a);


/**
 * Vectorised ceil() function, y:=ceil(a).
 * Computes the ceiling of \a a.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_ceil(const int count, T * a);


/**
 * Vectorised conj() function, y:=conj(a).
 * Computes the complex conjugate of \a a.
 * @param T the data type <complex16> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_conj(const int count, T * a);


/**
 * Vectorised cos() function, y:=cos(a).
 * Computes the cosine of \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_cos(const int count, T * a);


/**
 * Vectorised cosh() function, y:=cosh(a).
 * Computes the hyperbolic cosine of \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_cosh(const int count, T * a);


/**
 * Vectorised div() function, y:=a/b.
 * Computes the arithmetic division of \a a by \a b such that y:=a/b
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the first vector of data on which the operation shall be performed.
 * @param b the second vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a and \a b.
 */
template<typename T>
std::unique_ptr<T[]>
vx_div(const int count, T * a, T * b);


/**
 * Vectorised divx() function, y:=a/(constant)b.
 * Computes the arithmetic division of \a a by a constant \a b such that y:=a/(constant)b.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @param b a scalar constant on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of
 * \a a and the value \a b.
 */
template<typename T>
std::unique_ptr<T[]>
vx_divx(const int count, T * a, T b);


/**
 * Vectorised erf() function, y:=erf(a).
 * Computes the value of the error function at \a a.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_erf(const int count, T * a);


/**
 * Vectorised erfc() function, y:=erfc(a).
 * Computes the value of the complimentary error function at \a a.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_erfc(const int count, T * a);


/**
 * Vectorised erfcinv() function, y:=erfcinv(a).
 * Computes the value of the inverse complimentary error function at \a a.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_erfcinv(const int count, T * a);


/**
 * Vectorised erfinv() function, y:=erfinv(a).
 * Computes the value of the inverse error function at \a a.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_erfinv(const int count, T * a);


/**
 * Vectorised exp() function, y:=exp(a).
 * Computes the value of the base 'e' exponential function at \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_exp(const int count, T * a);


/**
 * Vectorised expm1() function, y:=expm1(a).
 * Computes the equivalent of the value of, the base 'e' exponential function at \a a,
 * then subtracts one. i.e. exp(\a a) - 1.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_expm1(const int count, T * a);


/**
 * Vectorised floor() function, y:=floor(a).
 * Computes the floor of \a a.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_floor(const int count, T * a);


/**
 * Vectorised hypot function(), y:=hypot(a,b).
 * Computes the Euclidean distance between \a a and \a b, this is the equivalent to y:=sqrt(a*a+b*b)
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the first vector of data on which the operation shall be performed.
 * @param b the second vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_hypot(const int count, T * a, T * b);


/**
 * Vectorised inv() function, y:=inv(a).
 * Computes the reciprocal of \a a.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_inv(const int count, T * a);


/**
 * Vectorised invcbrt() function, y:=invcbrt(a).
 * Computes the inverse cube root of \a a.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_invcbrt(const int count, T * a);


/**
 * Vectorised invsqrt() function, y:=invsqrt(a).
 * Computes the inverse square root of \a a.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_invsqrt(const int count, T * a);


/**
 * Vectorised lgamma() function, y:=lgamma(a).
 * Computes the natural (base 'e') logarithm of the gamma function at \a a.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_lgamma(const int count, T * a);


/**
 * Vectorised ln() function, y:=ln(a).
 * Computes the natural (base 'e') logarithm at \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_ln(const int count, T * a);


/**
 * Vectorised log10() function, y:=log10(a).
 * Computes the base 10 logarithm at \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_log10(const int count, T * a);


/**
 * Vectorised log1p() function, y:=log1p(a).
 * Computes the equivalent of, the value of the natural (base 'e') logarithm at location \a a + 1.
 * i.e. log(a+1).
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_log1p(const int count, T * a);


/**
 * Vectorised modf() function, {x,y}:=modf(a).
 * Computes the signed integral and fractional values of \a a returning the integral part in \a x
 * and the fractional part in \a y.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return a pair of arrays of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a. The returned
 * Pair::first contains the signed integral part, the returned Pair::second contains
 * the fractional part.
 */
template<typename T>
std::pair<std::unique_ptr<T[]>,std::unique_ptr<T[]>>
vx_modf(const int count, T * a);


/**
 * Vectorised mul() function, y:=a*b.
 * Computes the arithmetic multiplication of \a a by \a b such that y:=a*b.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the first vector of data on which the operation shall be performed.
 * @param b the second vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a and \a b.
 */
template<typename T>
std::unique_ptr<T[]>
vx_mul(const int count, T * a, T * b);


/**
 * Vectorised mulbyconj() function, y:=a*conj(b).
 * Computes the arithmetic multiplication of \a a by the complex conjugate of \a b 
 * such that y:=a*conj(b).
 * @param T the data type <complex16> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the first vector of data on which the operation shall be performed.
 * @param b the second vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a and \a b.
 */
template<typename T>
std::unique_ptr<T[]>
vx_mulbyconj(const int count, T * a, T * b);


/**
 * Vectorised mulx() function, y:=a*(constant)b
 * Computes the arithmetic multiplication of \a a by a constant \a b such that y:=a+(constant)b
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @param b a scalar constant on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of
 * \a a and the value \a b.
 */
template<typename T>
std::unique_ptr<T[]>
vx_mulx(const int count, T * a, T b);


/**
 * Vectorised nearbyint() function, y:=nearbyint(a).
 * Computes the nearest integer to \a a.
 * @param T the data types <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_nearbyint(const int count, T * a);


/**
 * Vectorised negate() function, y:=-a.
 * Computes the negation of \a a. In the case of a complex number both the real
 * and the imaginary parts of \a a are negated.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of 
 * \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_negate(const int count, T * a);


/**
 * Vectorised negatereal() function, y:=-real(a)+imag(a).
 * Computes complex negation of the real part of \a a, the imaginary part is ignored.
 * @param T the data type <complex16> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_negatereal(const int count, T * a);


/**
 * Vectorised pow() function, y:=pow(a,b).
 * Computes \a a raised to the power \a b.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the first vector of data on which the operation shall be performed.
 * @param b the second vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_pow(const int count, T * a, T * b);


/**
 * Vectorised pow2o3() function, y:=pow2o3(a).
 * Computes \a a raised to the power of (2/3).
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of 
 * \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_pow2o3(const int count, T * a);


/**
 * Vectorised pow3o2() function, y:=pow3o2(a).
 * Computes \a a raised to the power of (3/2).
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_pow3o2(const int count, T * a);


/**
 * Vectorised powx() function, y:=pow(a,(constant)b).
 * Computes \a a raised to the power of a constant \a b.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @param b a scalar constant on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a
 * and the value \a b.
 */
template<typename T>
std::unique_ptr<T[]>
vx_powx(const int count, T * a, T b);


/**
 * Vectorised round() function, y:=round(a).
 * Computes the rounded integer value of \a a with tied cases rounding away from zero.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_round(const int count, T * a);


/**
 * Vectorised sin() function, y:=sin(a).
 * Computes the sine of \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_sin(const int count, T * a);


/**
 * Vectorised sincos() function, {x,y}:=sincos(a).
 * Computes the sine and cosine of \a a simultaneously, c.f.  {y,x}={sin(a),cos(a)}.
 * returning the sine of \a a in \a x and cosine in \a y.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return a pair of arrays of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a. The returned
 * Pair::first contains the sine of \a a, the returned Pair::second contains the cosine of \a a.
 */
template<typename T>
std::pair<std::unique_ptr<T[]>,std::unique_ptr<T[]>>
vx_sincos(const int count, T * a);


/**
 * Vectorised sinh() function, y:=sinh(a).
 * Computes the hyperbolic sine of \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_sinh(const int count, T * a);


/**
 * Vectorised sqr() function, y:=sqr(a).
 * Computes the square of \a a, this is the equivalent of y:=a*a.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_sqr(const int count, T * a);


/**
 * Vectorised sqrt() function, y:=sqrt(a).
 * Computes the square root of \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_sqrt(const int count, T * a);


/**
 * Vectorised sub() function, y:=sub(a,b)
 * Computes the arithmetic subtraction of \a b from \a a such that y:=a-b.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the first vector of data on which the operation shall be performed.
 * @param b the second vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a and \a b.
 */
template<typename T>
std::unique_ptr<T[]>
vx_sub(const int count, T * a, T * b);


/**
 * Vectorised subx() function, y:=a-(constant)b.
 * Computes the arithmetic subtraction of a constant \a b from \a a such that y:=a-(constant)b
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @param b a scalar constant on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of
 * \a a and the value \a b.
 */
template<typename T>
std::unique_ptr<T[]>
vx_subx(const int count, T * a, T b);


/**
 * Vectorised tan() function, y:=tan(a).
 * Computes the tangent of \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_tan(const int count, T * a);


/**
 * Vectorised tanh() function, y:=tanh(a).
 * Computes the hyperbolic tangent of \a a.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_tanh(const int count, T * a);


/**
 * Vectorised tgamma() function, y:=tgamma(a).
 * Computes value of the (true) gamma function at \a a.
 * @param T the data type <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_tgamma(const int count, T * a);


/**
 * Vectorised trunc() function, y:=trunc(a).
 * Computes the rounded integer value of \a a such that the rounded value is not larger in 
 * magnitude than \a a (essentially round towards zero).
 * @param T the data types <real8> is accepted.
 * @param count the number of elements of \a a to process.
 * @param a the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_trunc(const int count, T * a);


/**
 * Vectorised xdiv() function, y:=(constant)a/b.
 * Computes the arithmetic division of a constant \a a by \a a such that y:=(constant)a/b.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a a scalar constant on which the operation shall be performed.
 * @param b the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a b
 * and the value \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_xdiv(const int count, T a, T * b);


/**
 * Vectorised xsub() function, y:=(constant)a-b.
 * Computes the arithmetic subtraction of \a b from a constant \a a such that y:=(constant)a - b.
 * @param T the data types <real8> and <complex16> are accepted.
 * @param count the number of elements of \a a to process.
 * @param a a scalar constant on which the operation shall be performed.
 * @param b the vector of data on which the operation shall be performed.
 * @return an array of type T of length \a count wrapped in a unique_ptr<T>, the values of
 * which are obtained from applying the operation to the first \a count members of \a b
 * and the value \a a.
 */
template<typename T>
std::unique_ptr<T[]>
vx_xsub(const int count, T a, T * b);


}

#endif