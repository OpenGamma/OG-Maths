/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "entrypt.hh"
#include "expression.hh"

using namespace std;
using namespace librdag;

int main(void ) {
  OGExpr *plus = new PLUS(new OGRealScalar(2), new OGRealScalar(3));
  entrypt((struct c_OGNumeric*) plus);
}
