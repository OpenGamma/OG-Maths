#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

# Expressions header

expression_hh = """\
/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _EXPRESSION_HH
#define _EXPRESSION_HH

// bindings
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <memory>

#include "expressionbase.hh"
#include "numeric.hh"
#include "visitor.hh"
#include "exceptions.hh"
#include "containers.hh"

using namespace std;


/**
 * The namespace for the DAG library
 */
namespace librdag
{

%(expression_classes)s

} // namespace librdag

#endif
"""

expr_class = """\
class %(classname)s: public %(parentclass)s
{
  public:
    %(classname)s(ArgContainer *args);
    virtual OGNumeric* copy() const override;
    virtual const %(classname)s* as%(classname)s() const override;
    virtual void debug_print() const override;
    virtual ExprType_t getType() const override;
};
"""

# Expressions cc

expression_cc = """\
/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <iostream>
#include "expression.hh"
#include "terminal.hh"
#include "exceptions.hh"
#include "exprtypeenum.h"

using namespace std;

namespace librdag
{

%(expression_methods)s

} // namespace librdag
"""

expr_methods = """\
/**
 * %(classname)s node
 */

%(classname)s::%(classname)s(ArgContainer* args): %(parentclass)s(args) {}

OGNumeric*
%(classname)s::copy() const
{
  return new %(classname)s(this->getArgs()->copy());
}

const %(classname)s*
%(classname)s::as%(classname)s() const
{
  return this;
}

void
%(classname)s::debug_print() const
{
        cout << "%(classname)s class" << endl;
}

ExprType_t
%(classname)s::getType() const
{
  return %(classname)s_ENUM;
}

"""
