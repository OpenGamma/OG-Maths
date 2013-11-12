/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "expressionbase.hh"
#include "containers.hh"
#include "jvmmanager.hh"

using librdag::OGNumeric;
using librdag::ArgContainer;

namespace convert {

ArgContainer* generateArgs(jobject obj);
OGNumeric* createExpression(jobject obj);
OGNumeric* createExprWithID(jlong ID, jobject obj);

} // namespace convert
