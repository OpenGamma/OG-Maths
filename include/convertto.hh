/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _CONVERTTO_HH
#define _CONVERTTO_HH
#include "terminal.hh"
#include "register.hh"

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
    OGRealMatrixRegister * convertToOGRealMatrix(OGRealScalar const * thing) const;
    OGRealMatrixRegister * convertToOGRealMatrix(OGRealDiagonalMatrix const * thing) const;
    OGRealMatrixRegister * convertToOGRealMatrix(OGLogicalMatrix const * thing) const;
    OGRealMatrixRegister * convertToOGRealMatrix(OGRealSparseMatrix const * thing) const;

    OGComplexMatrixRegister * convertToOGComplexMatrix(OGRealScalar const * thing) const;
    OGComplexMatrixRegister * convertToOGComplexMatrix(OGComplexScalar const * thing) const;
    OGComplexMatrixRegister * convertToOGComplexMatrix(OGRealDiagonalMatrix const * thing) const;
    OGComplexMatrixRegister * convertToOGComplexMatrix(OGComplexDiagonalMatrix const * thing) const;
    OGComplexMatrixRegister * convertToOGComplexMatrix(OGRealSparseMatrix const * thing) const;
    OGComplexMatrixRegister * convertToOGComplexMatrix(OGComplexSparseMatrix const * thing) const;
    OGComplexMatrixRegister * convertToOGComplexMatrix(OGRealMatrix const * thing) const;
};

}

#endif //_CONVERTTO_HH
