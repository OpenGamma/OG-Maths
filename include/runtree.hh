/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _RUNTREE_HH
#define _RUNTREE_HH

#include "numeric.hh"

namespace librdag {

/**
 * Runs a tree
 * @param root the root of the tree to run, on return the root's registers contain
 * the computed values
 */
void runtree(OGNumeric * root);

}

#endif // _RUNTREE_HH
