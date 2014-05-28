
/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "convertto.hh"
#include "terminal.hh"
#include "warningmacros.h"
#include "exceptions.hh"
#include <cstring>

using namespace std;
namespace librdag {


ConvertTo::ConvertTo()
{}

// things that convert to OGRealMatrix
OGRealMatrix::Ptr
ConvertTo::convertToOGRealMatrix(OGRealScalar::Ptr thing) const
{
  OGRealMatrix::Ptr ret = OGRealMatrix::Ptr{new OGRealMatrix(new real16[1](),1,1, OWNER)};
  ret->getData()[0] = thing->getValue();
  return ret;
}

OGRealMatrix::Ptr
ConvertTo::convertToOGRealMatrix(OGIntegerScalar::Ptr thing) const
{
  OGRealMatrix::Ptr ret = OGRealMatrix::Ptr{new OGRealMatrix(new real16[1](),1,1, OWNER)};
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGRealMatrix::Ptr
ConvertTo::convertToOGRealMatrix(OGRealDiagonalMatrix::Ptr thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGRealMatrix::Ptr ret = OGRealMatrix::Ptr{new OGRealMatrix(new real16[rows*cols](),rows,cols, OWNER)};
  real16 * diagdata = thing->getData();
  real16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

OGRealMatrix::Ptr
ConvertTo::convertToOGRealMatrix(OGLogicalMatrix::Ptr thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGRealMatrix::Ptr ret = OGRealMatrix::Ptr{new OGRealMatrix(new real16[wlen](),rows,cols, OWNER)};
  real16 * thedata = thing->getData();
  real16 * data = ret->getData();
  memcpy(data,thedata,sizeof(real16)*wlen);
  return ret;
}


OGRealMatrix::Ptr
ConvertTo::convertToOGRealMatrix(OGRealSparseMatrix::Ptr thing) const
{

  int rows = thing->getRows();
  int cols = thing->getCols();
  int * colPtr = thing->getColPtr();
  int * rowIdx = thing->getRowIdx();
  real16 * sparsedata = thing->getData();

  OGRealMatrix::Ptr ret = OGRealMatrix::Ptr{new OGRealMatrix(new real16[rows*cols](),rows,cols, OWNER)};
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

OGComplexMatrix::Ptr
ConvertTo::convertToOGComplexMatrix(OGRealScalar::Ptr thing) const
{
  OGComplexMatrix::Ptr ret = OGComplexMatrix::Ptr{new OGComplexMatrix(new complex16[1](),1,1, OWNER)};
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGComplexMatrix::Ptr
ConvertTo::convertToOGComplexMatrix(OGIntegerScalar::Ptr thing) const
{
  OGComplexMatrix::Ptr ret = OGComplexMatrix::Ptr{new OGComplexMatrix(new complex16[1](),1,1, OWNER)};
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGComplexMatrix::Ptr
ConvertTo::convertToOGComplexMatrix(OGComplexScalar::Ptr thing) const
{
  OGComplexMatrix::Ptr ret = OGComplexMatrix::Ptr{new OGComplexMatrix(new complex16[1](),1,1, OWNER)};
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGComplexMatrix::Ptr
ConvertTo::convertToOGComplexMatrix(OGRealDiagonalMatrix::Ptr thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGComplexMatrix::Ptr ret = OGComplexMatrix::Ptr{new OGComplexMatrix(new complex16[rows*cols](),rows,cols, OWNER)};
  real16 * diagdata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

OGComplexMatrix::Ptr
ConvertTo::convertToOGComplexMatrix(OGComplexDiagonalMatrix::Ptr thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGComplexMatrix::Ptr ret = OGComplexMatrix::Ptr{new OGComplexMatrix(new complex16[rows*cols](),rows,cols, OWNER)};
  complex16 * diagdata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

OGComplexMatrix::Ptr
ConvertTo::convertToOGComplexMatrix(OGRealSparseMatrix::Ptr thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int * colPtr = thing->getColPtr();
  int * rowIdx = thing->getRowIdx();
  real16 * sparsedata = thing->getData();
  OGComplexMatrix::Ptr ret = OGComplexMatrix::Ptr{new OGComplexMatrix(new complex16[rows*cols](),rows,cols, OWNER)};
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

OGComplexMatrix::Ptr
ConvertTo::convertToOGComplexMatrix(OGComplexSparseMatrix::Ptr thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int * colPtr = thing->getColPtr();
  int * rowIdx = thing->getRowIdx();
  complex16 * sparsedata = thing->getData();
  OGComplexMatrix::Ptr ret = OGComplexMatrix::Ptr{new OGComplexMatrix(new complex16[rows*cols](),rows,cols, OWNER)};
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

OGComplexMatrix::Ptr
ConvertTo::convertToOGComplexMatrix(OGRealMatrix::Ptr thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGComplexMatrix::Ptr ret = OGComplexMatrix::Ptr{new OGComplexMatrix(new complex16[wlen](),rows,cols, OWNER)};
  real16 * densedata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i]=densedata[i];
  }
  return ret;
}

OGComplexMatrix::Ptr
ConvertTo::convertToOGComplexMatrix(OGLogicalMatrix::Ptr thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGComplexMatrix::Ptr ret = OGComplexMatrix::Ptr{new OGComplexMatrix(new complex16[wlen](),rows,cols, OWNER)};
  real16 * densedata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i]=densedata[i];
  }
  return ret;
}

} // end namespace
