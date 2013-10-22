
/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "convertto.hh"
#include "terminal.hh"
#include "warningmacros.h"
#include "exceptions.hh"

using namespace std;
namespace librdag {


ConvertTo::ConvertTo()
{}

// things that convert to OGRealMatrix
OGRealMatrixRegister *
ConvertTo::convertToOGRealMatrix(OGRealScalar const * thing) const
{
  OGRealMatrixRegister * ret = new OGRealMatrixRegister(1,1);
  ret->incRef();
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGRealMatrixRegister *
ConvertTo::convertToOGRealMatrix(OGRealDiagonalMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGRealMatrixRegister * ret = new OGRealMatrixRegister(rows,cols);
  ret->incRef();
  real16 * diagdata = thing->getData();
  real16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

OGRealMatrixRegister *
ConvertTo::convertToOGRealMatrix(OGRealSparseMatrix const * thing) const
{

  int rows = thing->getRows();
  int cols = thing->getCols();
  int * colPtr = thing->getColPtr();
  int * rowIdx = thing->getRowIdx();
  real16 * sparsedata = thing->getData();

  OGRealMatrixRegister * ret = new OGRealMatrixRegister(rows,cols);
  ret->incRef();
  real16 * data = ret->getData();
  for (int ir = 0; ir < cols; ir++)
  {
    for (int i = colPtr[ir]; i < colPtr[ir + 1]; i++)
    {
      data[rowIdx[i] + ir * rows] = sparsedata[i];
    }
  }
  return ret;
}


// things that convert to OGComplexMatrix

OGComplexMatrixRegister *
ConvertTo::convertToOGComplexMatrix(OGRealScalar const * thing) const
{
  OGComplexMatrixRegister * ret = new OGComplexMatrixRegister(1,1);
  ret->incRef();
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGComplexMatrixRegister *
ConvertTo::convertToOGComplexMatrix(OGComplexScalar const * thing) const
{
  OGComplexMatrixRegister * ret = new OGComplexMatrixRegister(1,1);
  ret->incRef();
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGComplexMatrixRegister *
ConvertTo::convertToOGComplexMatrix(OGRealDiagonalMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGComplexMatrixRegister * ret = new OGComplexMatrixRegister(rows,cols);
  ret->incRef();
  real16 * diagdata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

OGComplexMatrixRegister *
ConvertTo::convertToOGComplexMatrix(OGComplexDiagonalMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGComplexMatrixRegister * ret = new OGComplexMatrixRegister(rows,cols);
  ret->incRef();
  complex16 * diagdata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

OGComplexMatrixRegister *
ConvertTo::convertToOGComplexMatrix(OGRealSparseMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int * colPtr = thing->getColPtr();
  int * rowIdx = thing->getRowIdx();
  real16 * sparsedata = thing->getData();

  OGComplexMatrixRegister * ret = new OGComplexMatrixRegister(rows,cols);
  ret->incRef();
  complex16 * data = ret->getData();
  for (int ir = 0; ir < cols; ir++)
  {
    for (int i = colPtr[ir]; i < colPtr[ir + 1]; i++)
    {
      data[rowIdx[i] + ir * rows] = sparsedata[i];
    }
  }
  return ret;
}

OGComplexMatrixRegister *
ConvertTo::convertToOGComplexMatrix(OGComplexSparseMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int * colPtr = thing->getColPtr();
  int * rowIdx = thing->getRowIdx();
  complex16 * sparsedata = thing->getData();

  OGComplexMatrixRegister * ret = new OGComplexMatrixRegister(rows,cols);
  ret->incRef();
  complex16 * data = ret->getData();
  for (int ir = 0; ir < cols; ir++)
  {
    for (int i = colPtr[ir]; i < colPtr[ir + 1]; i++)
    {
      data[rowIdx[i] + ir * rows] = sparsedata[i];
    }
  }
  return ret;
}

OGComplexMatrixRegister *
ConvertTo::convertToOGComplexMatrix(OGRealMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGComplexMatrixRegister * ret = new OGComplexMatrixRegister(rows,cols);
  ret->incRef();
  real16 * densedata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i]=densedata[i];
  }
  return ret;
}

} // end namespace