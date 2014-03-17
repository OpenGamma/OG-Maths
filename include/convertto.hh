/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _CONVERTTO_HH
#define _CONVERTTO_HH
#include "terminal.hh"

// defines permissable type conversions

//                                            _TO_
// _FROM_        RealScal CmplxScal RDiag  CDiag   Logical  Idx  RSparse CSparse   RDense  CDense
//RealScalar    |  Yes   |   Yes   | Yes |  Yes   |  No   | No  |  Yes  |   Yes   | Yes  |  Yes  |
//ComplexScalar |   No   |   Yes   | No  |  Yes   |  No   | No  |  No   |   Yes   | No   |  Yes  |
//RealDiag      |   No   |   No    | Yes |  Yes   |  No   | No  |  Yes  |   Yes   | Yes  |  Yes  |
//CplxDiag      |   No   |   No    | No  |  Yes   |  No   | No  |  No   |   Yes   | No   |  Yes  |
//Logical       |   No   |   No    | No  |  No    |  Yes  | No  |  Yes  |   Yes   | Yes  |  Yes  |
//Idx           |   No   |   No    | No  |  No    |  No   | Yes |  Yes  |   Yes   | Yes  |  Yes  |
//RealSparse    |   No   |   No    | No  |  No    |  No   | No  |  Yes  |   Yes   | Yes  |  Yes  |
//CplxSparse    |   No   |   No    | No  |  No    |  No   | No  |  No   |   Yes   | No   |  Yes  |
//RealDense     |   No   |   No    | No  |  No    |  No   | No  |  No   |   No    | Yes  |  Yes  |
//CplxDense     |   No   |   No    | No  |  No    |  No   | No  |  No   |   No    | No   |  Yes  |


namespace librdag {

// conversion class, it need not be a class
class ConvertTo
{
  public:
    ConvertTo();
    OGRealMatrix * convertToOGRealMatrix(OGRealScalar const * thing) const;
    OGRealMatrix * convertToOGRealMatrix(OGIntegerScalar const * thing) const;
    OGRealMatrix * convertToOGRealMatrix(OGRealDiagonalMatrix const * thing) const;
    OGRealMatrix * convertToOGRealMatrix(OGLogicalMatrix const * thing) const;
    OGRealMatrix * convertToOGRealMatrix(OGRealSparseMatrix const * thing) const;

    OGComplexMatrix * convertToOGComplexMatrix(OGRealScalar const * thing) const;
    OGComplexMatrix * convertToOGComplexMatrix(OGIntegerScalar const * thing) const;
    OGComplexMatrix * convertToOGComplexMatrix(OGComplexScalar const * thing) const;
    OGComplexMatrix * convertToOGComplexMatrix(OGRealDiagonalMatrix const * thing) const;
    OGComplexMatrix * convertToOGComplexMatrix(OGComplexDiagonalMatrix const * thing) const;
    OGComplexMatrix * convertToOGComplexMatrix(OGRealSparseMatrix const * thing) const;
    OGComplexMatrix * convertToOGComplexMatrix(OGComplexSparseMatrix const * thing) const;
    OGComplexMatrix * convertToOGComplexMatrix(OGRealMatrix const * thing) const;
    OGComplexMatrix * convertToOGComplexMatrix(OGLogicalMatrix const * thing) const;
};

}

#endif //_CONVERTTO_HH
