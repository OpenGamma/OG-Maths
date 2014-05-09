
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
pOGRealMatrix
ConvertTo::convertToOGRealMatrix(pOGRealScalar thing) const
{
  pOGRealMatrix ret = pOGRealMatrix{new OGRealMatrix(new real16[1](),1,1, OWNER)};
  ret->getData()[0] = thing->getValue();
  return ret;
}

pOGRealMatrix
ConvertTo::convertToOGRealMatrix(pOGIntegerScalar thing) const
{
  pOGRealMatrix ret = pOGRealMatrix{new OGRealMatrix(new real16[1](),1,1, OWNER)};
  ret->getData()[0]=thing->getValue();
  return ret;
}

pOGRealMatrix
ConvertTo::convertToOGRealMatrix(pOGRealDiagonalMatrix thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  pOGRealMatrix ret = pOGRealMatrix{new OGRealMatrix(new real16[rows*cols](),rows,cols, OWNER)};
  real16 * diagdata = thing->getData();
  real16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

pOGRealMatrix
ConvertTo::convertToOGRealMatrix(pOGLogicalMatrix thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  pOGRealMatrix ret = pOGRealMatrix{new OGRealMatrix(new real16[wlen](),rows,cols, OWNER)};
  real16 * thedata = thing->getData();
  real16 * data = ret->getData();
  memcpy(data,thedata,sizeof(real16)*wlen);
  return ret;
}


pOGRealMatrix
ConvertTo::convertToOGRealMatrix(pOGRealSparseMatrix thing) const
{

  int rows = thing->getRows();
  int cols = thing->getCols();
  int * colPtr = thing->getColPtr();
  int * rowIdx = thing->getRowIdx();
  real16 * sparsedata = thing->getData();

  pOGRealMatrix ret = pOGRealMatrix{new OGRealMatrix(new real16[rows*cols](),rows,cols, OWNER)};
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

pOGComplexMatrix
ConvertTo::convertToOGComplexMatrix(pOGRealScalar thing) const
{
  pOGComplexMatrix ret = pOGComplexMatrix{new OGComplexMatrix(new complex16[1](),1,1, OWNER)};
  ret->getData()[0]=thing->getValue();
  return ret;
}

pOGComplexMatrix
ConvertTo::convertToOGComplexMatrix(pOGIntegerScalar thing) const
{
  pOGComplexMatrix ret = pOGComplexMatrix{new OGComplexMatrix(new complex16[1](),1,1, OWNER)};
  ret->getData()[0]=thing->getValue();
  return ret;
}

pOGComplexMatrix
ConvertTo::convertToOGComplexMatrix(pOGComplexScalar thing) const
{
  pOGComplexMatrix ret = pOGComplexMatrix{new OGComplexMatrix(new complex16[1](),1,1, OWNER)};
  ret->getData()[0]=thing->getValue();
  return ret;
}

pOGComplexMatrix
ConvertTo::convertToOGComplexMatrix(pOGRealDiagonalMatrix thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  pOGComplexMatrix ret = pOGComplexMatrix{new OGComplexMatrix(new complex16[rows*cols](),rows,cols, OWNER)};
  real16 * diagdata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

pOGComplexMatrix
ConvertTo::convertToOGComplexMatrix(pOGComplexDiagonalMatrix thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  pOGComplexMatrix ret = pOGComplexMatrix{new OGComplexMatrix(new complex16[rows*cols](),rows,cols, OWNER)};
  complex16 * diagdata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

pOGComplexMatrix
ConvertTo::convertToOGComplexMatrix(pOGRealSparseMatrix thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int * colPtr = thing->getColPtr();
  int * rowIdx = thing->getRowIdx();
  real16 * sparsedata = thing->getData();
  pOGComplexMatrix ret = pOGComplexMatrix{new OGComplexMatrix(new complex16[rows*cols](),rows,cols, OWNER)};
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

pOGComplexMatrix
ConvertTo::convertToOGComplexMatrix(pOGComplexSparseMatrix thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int * colPtr = thing->getColPtr();
  int * rowIdx = thing->getRowIdx();
  complex16 * sparsedata = thing->getData();
  pOGComplexMatrix ret = pOGComplexMatrix{new OGComplexMatrix(new complex16[rows*cols](),rows,cols, OWNER)};
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

pOGComplexMatrix
ConvertTo::convertToOGComplexMatrix(pOGRealMatrix thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  pOGComplexMatrix ret = pOGComplexMatrix{new OGComplexMatrix(new complex16[wlen](),rows,cols, OWNER)};
  real16 * densedata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i]=densedata[i];
  }
  return ret;
}

pOGComplexMatrix
ConvertTo::convertToOGComplexMatrix(pOGLogicalMatrix thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  pOGComplexMatrix ret = pOGComplexMatrix{new OGComplexMatrix(new complex16[wlen](),rows,cols, OWNER)};
  real16 * densedata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i]=densedata[i];
  }
  return ret;
}

} // end namespace
