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


OGTerminal::Ptr ogreal = OGRealScalar::create(1.0);
OGTerminal::Ptr ogcomplex = OGComplexScalar::create(complex16(1.0, 2.0));
OGTerminal::Ptr ogint = OGIntegerScalar::create(1);

double realData[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
OGTerminal::Ptr ogrealmatrix = OGRealDenseMatrix::create(realData, 2, 3);


complex16 complexData[6] = { {1.0, 2.0}, {3.0, 4.0},  {5.0, 6.0},
                             {7.0, 8.0}, {9.0, 10.0}, {11.0, 12.0} };
OGTerminal::Ptr ogcomplexmatrix = OGComplexDenseMatrix::create(complexData, 2, 3);

double rdiagData[3] = { 1.0, 2.0, 3.0 };
OGTerminal::Ptr ogrealdiagonalmatrix = OGRealDiagonalMatrix::create(rdiagData, 3, 4);

complex16 cdiagData[3] = { {1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0} };
OGComplexDiagonalMatrix::Ptr ogcomplexdiagonalmatrix = OGComplexDiagonalMatrix::create(cdiagData, 3, 4);

int4 colPtr[3] = { 0, 2, 2 };
int4 rowIdx[2] = { 0, 1 };
double rsparseData[2] = { 1.0, 2.0 };
OGRealSparseMatrix::Ptr ogrealsparsematrix = OGRealSparseMatrix::create(colPtr, rowIdx, rsparseData, 2, 2);

complex16 csparseData[2] = { {1.0, 2.0}, {3.0, 4.0} };
OGComplexSparseMatrix::Ptr ogcomplexsparsematrix = OGComplexSparseMatrix::create(colPtr, rowIdx, csparseData, 2, 2);

} // namespace internal

/**
 * List of terminals that can be used for instantiating value-parameterised test
 * cases over terminals
 */
OGTerminal::Ptr terminals[] = { testinternal::ogreal,
                                testinternal::ogcomplex,
                                testinternal::ogint,
                                testinternal::ogrealmatrix,
                                testinternal::ogcomplexmatrix,
                                testinternal::ogrealdiagonalmatrix,
                                testinternal::ogcomplexdiagonalmatrix,
                                testinternal::ogrealsparsematrix,
                                testinternal::ogcomplexsparsematrix };


#endif
