/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <vector>
#include "expressionbase.hh"
#include "jvmmanager.hh"

using librdag::pOGNumeric;

namespace convert {

pOGNumeric createExpression(jobject obj);
pOGNumeric translateNode(JNIEnv* env, jobject obj, pOGNumeric arg0 = pOGNumeric{}, pOGNumeric arg1 = pOGNumeric{});

} // namespace convert
