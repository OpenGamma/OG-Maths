/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
#include "numerictypes.hh"

using namespace std;

namespace librdag {

real8 getNaN()
{
  union {
    real8 d;
    unsigned long long int i;
  } nanval;
  nanval.i = 0x7FF1010101010101;
  return nanval.d;
}

complex16 getComplexNaN()
{
  union {
    real8 d;
    unsigned long long int i;
  } nanval1, nanval2;
  nanval1.i = 0x7FFDEAD101010101;
  nanval2.i = 0x7FFDEAD202020202;
  return complex16(nanval1.d, nanval2.d);
}

real8 getPosInf()
{
  union {
    real8 d;
    unsigned long long int i;
  } val;
  val.i = 0x7FF0000000000000;
  return val.d;
}

real8 getNegInf()
{
  union {
    real8 d;
    unsigned long long int i;
  } val;
  val.i = 0xFFF0000000000000;
  return val.d;
}

complex16 getPosPosComplexInf()
{
 complex16 val = {getPosInf(), getPosInf()};
 return val;
}

complex16 getPosNegComplexInf()
{
 complex16 val = {getPosInf(), getNegInf()};
 return val;
}

complex16 getNegPosComplexInf()
{
 complex16 val = {getNegInf(), getPosInf()};
 return val;
}

complex16 getNegNegComplexInf()
{
   complex16 val = {getNegInf(), getNegInf()};
 return val;
}

}