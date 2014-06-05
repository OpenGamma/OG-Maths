/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _CONVERTTO_HH
#define _CONVERTTO_HH

#include <memory>

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

class OGRealScalar;
class OGComplexScalar;
class OGIntegerScalar;
class OGRealMatrix;
class OGComplexMatrix;
class OGLogicalMatrix;
class OGRealDiagonalMatrix;
class OGComplexDiagonalMatrix;
class OGRealSparseMatrix;
class OGComplexSparseMatrix;

// conversion class, it need not be a class
class ConvertTo
{
  public:
    ConvertTo();
    std::shared_ptr<const OGRealMatrix> convertToOGRealMatrix(std::shared_ptr<const OGRealScalar> thing) const;
    std::shared_ptr<const OGRealMatrix> convertToOGRealMatrix(std::shared_ptr<const OGIntegerScalar> thing) const;
    std::shared_ptr<const OGRealMatrix> convertToOGRealMatrix(std::shared_ptr<const OGRealDiagonalMatrix> thing) const;
    std::shared_ptr<const OGRealMatrix> convertToOGRealMatrix(std::shared_ptr<const OGLogicalMatrix> thing) const;
    std::shared_ptr<const OGRealMatrix> convertToOGRealMatrix(std::shared_ptr<const OGRealSparseMatrix> thing) const;

    std::shared_ptr<const OGComplexMatrix> convertToOGComplexMatrix(std::shared_ptr<const OGRealScalar> thing) const;
    std::shared_ptr<const OGComplexMatrix> convertToOGComplexMatrix(std::shared_ptr<const OGIntegerScalar> thing) const;
    std::shared_ptr<const OGComplexMatrix> convertToOGComplexMatrix(std::shared_ptr<const OGComplexScalar> thing) const;
    std::shared_ptr<const OGComplexMatrix> convertToOGComplexMatrix(std::shared_ptr<const OGRealDiagonalMatrix>  thing) const;
    std::shared_ptr<const OGComplexMatrix> convertToOGComplexMatrix(std::shared_ptr<const OGComplexDiagonalMatrix> thing) const;
    std::shared_ptr<const OGComplexMatrix> convertToOGComplexMatrix(std::shared_ptr<const OGRealSparseMatrix> thing) const;
    std::shared_ptr<const OGComplexMatrix> convertToOGComplexMatrix(std::shared_ptr<const OGComplexSparseMatrix> thing) const;
    std::shared_ptr<const OGComplexMatrix> convertToOGComplexMatrix(std::shared_ptr<const OGRealMatrix> thing) const;
    std::shared_ptr<const OGComplexMatrix> convertToOGComplexMatrix(std::shared_ptr<const OGLogicalMatrix> thing) const;
};

}

#endif //_CONVERTTO_HH
