#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

createexpr_cc = """\
/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "expression.hh"
#include "exprtypeenum.h"
#include "exprfactory.hh"
#include "jvmmanager.hh"
#include "jterminals.hh"
#include "debug.h"

namespace convert {

using namespace librdag;

OGNumeric* createExprWithID(jlong ID, jobject obj)
{
  librdag::OGNumeric * _expr = nullptr;

  switch(ID)
  {
%(switch_cases)s
  default:
  {
    throw convert_error("Unknown node type");
  }
  break;
  }

  if(_expr == nullptr)
  {
    throw convert_error("_expr is NULL, and probably hasn't been set by the factory");
  }

  return _expr;
}

} // namespace convert
"""

terminal_case = """\
    case %(enumname)s:
      DEBUG_PRINT("Binding a J%(typename)s\\n");
      _expr = new J%(typename)s(obj);
      break;
"""

expr_case = """\
    case %(enumname)s:
      DEBUG_PRINT("%(typename)s function\\n");
      _expr = new %(typename)s(generateArgs(obj));
      break;
"""
