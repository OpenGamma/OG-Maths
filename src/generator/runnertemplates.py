#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

# Runners header file

runners_header = """\
/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 *
 * This file is autogenerated during the DOGMA2 build process - src/generator/generator.py
 */

#ifndef _RUNNERS_HH
#define _RUNNERS_HH

#include "dispatch.hh"
#include "uncopyable.hh"

namespace librdag {

%(class_definitions)s
} // end namespace librdag

#endif // _RUNNERS_HH
"""

# Runners cc file

runners_cc = """\
/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for licence.
 *
 * This file is autogenerated during the DOGMA2 build process - src/generator/generator.py
 */
#include "dispatch.hh"
#include "runners.hh"
#include "expression.hh"
#include "terminal.hh"
#include "uncopyable.hh"

namespace librdag {
%(function_definitions)s
} // end namespace librdag
"""

# Binary runner

binary_runner_class_definition = """\
class %(nodename)sRunner: public DispatchVoidBinaryOp, private Uncopyable
{
  public:
    using DispatchBinaryOp<void *>::run;
    virtual void * run(RegContainer* reg0, const OGComplexMatrix* arg0, const OGComplexMatrix* arg1) const override;
    virtual void * run(RegContainer* reg0, const OGRealMatrix*    arg0, const OGRealMatrix*    arg1) const override;
    virtual void * run(RegContainer* reg0, const OGRealScalar*    arg0, const OGRealScalar*    arg1) const override;
};

"""

binary_runner_function =  """\
void *
%(nodename)sRunner::run(RegContainer* reg0, const %(arg0type)s* arg0, const %(arg1type)s* arg1) const
{
  const %(returntype)s* ret;
%(implementation)s
  reg0->push_back(ret);
  return nullptr;
}

"""

# Integer parameter runner

integer_parameter_runner_class_definition = """\
class %(nodename)sRunner: public DispatchVoidOp, private Uncopyable
{
  public:
    virtual void* eval(RegContainer* reg, RegContainer const *arg0, OGIntegerScalar const *arg1) const;
    virtual void* run(RegContainer* reg0, const RegContainer* arg0, const OGIntegerScalar* arg1) const;
};

"""

# Infix runner

infix_scalar_runner_implementation = """\
  ret = new %(returntype)s(arg0->getValue() %(symbol)s arg1->getValue());\
"""

# Unary runner

unary_runner_class_definition = """\
class %(nodename)sRunner: public DispatchVoidUnaryOp, private Uncopyable
{
  public:
    virtual void * run(RegContainer* reg, const OGRealScalar*    arg) const override;
    virtual void * run(RegContainer* reg, const OGRealMatrix*    arg) const override;
    virtual void * run(RegContainer* reg, const OGComplexMatrix* arg) const override;
};
"""

unary_runner_function = """\
void *
%(nodename)sRunner::run(RegContainer* reg, const %(argtype)s* arg) const
{
  const %(returntype)s* ret;
%(implementation)s
  reg->push_back(ret);
  return nullptr;
}

"""

# Prefix runner

prefix_scalar_runner_implementation = """\
  ret = new %(returntype)s(%(symbol)s(arg->getValue()));\
"""

prefix_matrix_runner_implementation = """\
  %(datatype)s* data = arg->getData();
  int datalen = arg->getDatalen();
  %(datatype)s* newData = new %(datatype)s[datalen];
  for (int i = 0; i < datalen; ++i)
  {
    newData[i] = %(symbol)sdata[i];
  }
  ret = new %(returntype)s(newData, arg->getRows(), arg->getCols(), OWNER);
"""

# UnaryFunction runner

unaryfunction_scalar_runner_implementation = """\
  ret = new %(returntype)s(%(function)s(arg->getValue()));\
"""

unaryfunction_matrix_runner_implementation = """\
  %(datatype)s* data = arg->getData();
  int datalen = arg->getDatalen();
  %(datatype)s* newData = new %(datatype)s[datalen];
  for (int i = 0; i < datalen; ++i)
  {
    newData[i] = %(function)s(data[i]);
  }
  ret = new %(returntype)s(newData, arg->getRows(), arg->getCols(), OWNER);
"""

# Unimplemented runners

unimplementedunary_runner_function = """\
void *
%(nodename)sRunner::run(RegContainer SUPPRESS_UNUSED * reg, %(argtype)s const SUPPRESS_UNUSED * arg) const
{
  throw rdag_error("Unimplemented unary expression node");
  return nullptr;
}
"""

unimplementedbinary_runner_function =  """\
void *
%(nodename)sRunner::run(RegContainer SUPPRESS_UNUSED * reg0,
                        %(arg0type)s const SUPPRESS_UNUSED * arg0,
                        %(arg1type)s const SUPPRESS_UNUSED * arg1) const
{
  throw rdag_error("Unimplemented unary expression node");
  return nullptr;
}
"""
