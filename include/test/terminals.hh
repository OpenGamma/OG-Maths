/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

/**
 * Some terminal setup that can be re-used across tests.
 *
 * Don't include this twice in a program - this shouldn't be an issue since
 * tests are compiled from a single source anyway
 */

#ifndef _TEST_TERMINALS_HH
#define _TEST_TERMINALS_HH

#include "terminal.hh"

using namespace librdag;

namespace testinternal {


const OGTerminal* ogreal = new OGRealScalar(1.0);
const OGTerminal* ogcomplex = new OGComplexScalar(complex16(1.0, 2.0));
const OGTerminal* ogint = new OGIntegerScalar(1);

double realData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
const OGTerminal* ogrealmatrix = new OGRealMatrix(realData, 2, 3);


complex16 complexData[6] = { {1.0, 2.0}, {3.0, 4.0},  {5.0, 6.0},
                             {7.0, 8.0}, {9.0, 10.0}, {11.0, 12.0} };
const OGComplexMatrix* ogcomplexmatrix = new OGComplexMatrix(complexData, 2, 3);

double rdiagData[3] = { 1.0, 2.0, 3.0 };
const OGTerminal* ogrealdiagonalmatrix = new OGRealDiagonalMatrix(rdiagData, 3, 4);

complex16 cdiagData[3] = { {1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0} };
const OGComplexDiagonalMatrix* ogcomplexdiagonalmatrix = new OGComplexDiagonalMatrix(cdiagData, 3, 4);

int colPtr[3] = { 0, 2, 2 };
int rowIdx[2] = { 0, 1 };
double rsparseData[2] = { 1.0, 2.0 };
const OGRealSparseMatrix *ogrealsparsematrix = new OGRealSparseMatrix(colPtr, rowIdx, rsparseData, 2, 2);

complex16 csparseData[2] = { {1.0, 2.0}, {3.0, 4.0} };
const OGComplexSparseMatrix *ogcomplexsparsematrix = new OGComplexSparseMatrix(colPtr, rowIdx, csparseData, 2, 2);

} // namespace internal

/**
 * List of terminals that can be used for instantiating value-parameterised test
 * cases over terminals
 */
pOGTerminal terminals[] = { pOGTerminal{testinternal::ogreal},
                            pOGTerminal{testinternal::ogcomplex},
                            pOGTerminal{testinternal::ogint},
                            pOGTerminal{testinternal::ogrealmatrix},
                            pOGTerminal{testinternal::ogcomplexmatrix},
                            pOGTerminal{testinternal::ogrealdiagonalmatrix},
                            pOGTerminal{testinternal::ogcomplexdiagonalmatrix},
                            pOGTerminal{testinternal::ogrealsparsematrix},
                            pOGTerminal{testinternal::ogcomplexsparsematrix} };


#endif
