
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
OGOwningRealMatrix *
ConvertTo::convertToOGRealMatrix(OGRealScalar const * thing) const
{
  OGOwningRealMatrix * ret = new OGOwningRealMatrix(1,1);
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGOwningRealMatrix *
ConvertTo::convertToOGRealMatrix(OGIntegerScalar const * thing) const
{
  OGOwningRealMatrix * ret = new OGOwningRealMatrix(1,1);
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGOwningRealMatrix *
ConvertTo::convertToOGRealMatrix(OGRealDiagonalMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGOwningRealMatrix * ret = new OGOwningRealMatrix(rows,cols);
  real16 * diagdata = thing->getData();
  real16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

OGOwningRealMatrix *
ConvertTo::convertToOGRealMatrix(OGLogicalMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGOwningRealMatrix * ret = new OGOwningRealMatrix(rows,cols);
  real16 * thedata = thing->getData();
  real16 * data = ret->getData();
  memcpy(data,thedata,sizeof(real16)*wlen);
  return ret;
}


OGOwningRealMatrix *
ConvertTo::convertToOGRealMatrix(OGRealSparseMatrix const * thing) const
{

  int rows = thing->getRows();
  int cols = thing->getCols();
  int * colPtr = thing->getColPtr();
  int * rowIdx = thing->getRowIdx();
  real16 * sparsedata = thing->getData();

  OGOwningRealMatrix * ret = new OGOwningRealMatrix(rows,cols);
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

OGOwningComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGRealScalar const * thing) const
{
  OGOwningComplexMatrix * ret = new OGOwningComplexMatrix(1,1);
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGOwningComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGIntegerScalar const * thing) const
{
  OGOwningComplexMatrix * ret = new OGOwningComplexMatrix(1,1);
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGOwningComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGComplexScalar const * thing) const
{
  OGOwningComplexMatrix * ret = new OGOwningComplexMatrix(1,1);
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGOwningComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGRealDiagonalMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGOwningComplexMatrix * ret = new OGOwningComplexMatrix(rows,cols);
  real16 * diagdata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

OGOwningComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGComplexDiagonalMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGOwningComplexMatrix * ret = new OGOwningComplexMatrix(rows,cols);
  complex16 * diagdata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

OGOwningComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGRealSparseMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int * colPtr = thing->getColPtr();
  int * rowIdx = thing->getRowIdx();
  real16 * sparsedata = thing->getData();
  OGOwningComplexMatrix * ret = new OGOwningComplexMatrix(rows,cols);
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

OGOwningComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGComplexSparseMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int * colPtr = thing->getColPtr();
  int * rowIdx = thing->getRowIdx();
  complex16 * sparsedata = thing->getData();
  OGOwningComplexMatrix * ret = new OGOwningComplexMatrix(rows,cols);
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

OGOwningComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGRealMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGOwningComplexMatrix * ret = new OGOwningComplexMatrix(rows,cols);
  real16 * densedata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i]=densedata[i];
  }
  return ret;
}

OGOwningComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGLogicalMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGOwningComplexMatrix * ret = new OGOwningComplexMatrix(rows,cols);
  real16 * densedata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i]=densedata[i];
  }
  return ret;
}

} // end namespace