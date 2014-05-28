/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <vector>
#include "expressionbase.hh"
#include "jvmmanager.hh"

using librdag::OGNumeric;

namespace convert {

OGNumeric::Ptr createExpression(jobject obj);
OGNumeric::Ptr translateNode(JNIEnv* env, jobject obj, OGNumeric::Ptr arg0 = OGNumeric::Ptr{}, OGNumeric::Ptr arg1 = OGNumeric::Ptr{});

} // namespace convert
