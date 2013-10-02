/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "entrypt.hh"
#include "expression.hh"
#include "terminal.hh"

using namespace std;
using namespace librdag;

int main(void ) {
  ArgContainer *args = new ArgContainer();
  args->push_back(new OGRealScalar(2));
  args->push_back(new OGRealScalar(3));
  OGExpr *plus = new PLUS(args);
  entrypt(plus);
  delete plus;
}
