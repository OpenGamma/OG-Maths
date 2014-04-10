/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <vector>
#include "expressionbase.hh"
#include "containers.hh"
#include "jvmmanager.hh"

using librdag::OGNumeric;

namespace convert {

OGNumeric* createExpression(jobject obj);
OGNumeric* translateNode(JNIEnv* env, jobject obj, const OGNumeric* arg0 = nullptr, const OGNumeric* arg1 = nullptr);

} // namespace convert
