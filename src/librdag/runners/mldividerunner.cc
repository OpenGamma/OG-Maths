/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for licence.
 *
 */

#include "dispatch.hh"
#include "runners.hh"
#include "expression.hh"
#include "iss.hh"
#include "terminal.hh"
#include "uncopyable.hh"
#include "lapack.hh"
#include "equals.hh"

#include <stdio.h>
#include <complex>
#include <sstream>

using namespace std;


/*
 * Unit contains code for MLDIVIDE node runners
 */

namespace librdag
{

/**
 * Local implementation details.
 */
namespace detail
{

/**
  * row_start_invalid is used as a flag
  */
static constexpr std::size_t row_start_invalid = std::numeric_limits<std::size_t>::max();

/**
 * Casts a value to a char
 * @param the value to cast
 * @return the char representation of the value
 */
template<typename T> char asChar(T value)
{
  return static_cast<char>(value);
}

// Auxiliary structs for holding intermediary guiding data


// NOTE: The following enums are used in some cases to pass flags to lapack.
// Ideally lapack:: members holding `char *` would be used, but these are
// declared extern which conflicts with the constexpr needed in the following.
// At some point, for completeness, this could be fixed.

/**
 * UPLO flags upper or lower triangular matrix.
 */
enum class UPLO: char
{
  /**
   * Flags neither upper or lower.
   */
  NEITHER = 'N',
  /**
   * Flags an upper triangular matrix.
   */
  UPPER = 'U',
  /**
   * Flags a lower triangular matrix.
   */
  LOWER = 'L'
};

/**
 * UNITDIAG flags if a triangular matrix has a unit diagonal.
 */
enum class UNITDIAG: char
{
  /**
   * Flags a unit diagonal.
   */
  UNIT    = 'U',
  /**
   * Flags a non-unit diagonal.
   */
  NONUNIT = 'N'
};

/**
 * PERMUTATION flags the possible permutation type identifiable in a triangular matrix.
 */
enum class PERMUTATION: char
{
  /**
   * Flags as standard, no permutation present.
   */
  STANDARD = 'S',
  /**
   * Flags as a row permutation.
   */
  ROW      = 'R',
  /**
   * Flags as a column permutation.
   */
  COLUMN   = 'C',
};

/**
 * This struct holds the state from deciding if a matrix is in some way triangular.
 *
 * Flags are based on LAPACK chars.
 */
struct TriangularStruct
{
  /**
   * @param flagUPLO is it upper or lower
   */
  UPLO flagUPLO = UPLO::NEITHER;
  /**
   * @param flagDiag is it diagonal
   */
  UNITDIAG flagDiag = UNITDIAG::NONUNIT;
  /**
   * @param flagPerm is it permuted
   */
  PERMUTATION flagPerm = PERMUTATION::STANDARD;
  /**
   * @param perm the permutation applied, can be null if \a flagPerm is \a PERMUTATION::STANDARD
   */
  unique_ptr<size_t[]> perm = nullptr;
};

// These are helper routines

/**
 * Checks if a matrix is Hermitian.
 * @param data the data from the matrix, column major
 * @param rows the number of rows
 * @param cols the number of columns
 * @return true if Hermitian false else.
 */
template<typename T>
bool isHermitian(T * data, size_t rows, size_t cols)
{
  size_t ir;
  for (size_t i = 0; i < cols; i++)
  {
    ir = i * rows;
    for (size_t j = 0; j < rows; j++)
    {
      if (data[ir + j] != std::conj(data[j * rows + i])) // fp comparison, strict equality
      {
        return false;
      }
    }
  }
  return true;
}


/**
 * Checks if a matrix is lower triangular.
 * @param data the column major data
 * @param rows the number of row in the matrix
 * @param cols the number of cols in the matrix
 * @param tri enum pointer of type UPLO to indicate, upper, lower, neither.
 * 
 */
template<typename T>
void checkIfLowerTriangular(T * data, size_t rows, size_t cols, UPLO* tri, UNITDIAG* diag)
{
  size_t ir;
  *tri = UPLO::LOWER;
  *diag = UNITDIAG::UNIT;
  bool isUnit = true;
  if (!SingleValueFuzzyEquals(data[0],1.e0,std::numeric_limits<real8>::epsilon(),std::numeric_limits<real8>::epsilon()))
  {
    *diag = UNITDIAG::NONUNIT;
  }
  for (size_t  i = 1; i < cols; i++)
  {
    ir = i * rows;
    // check if diag == 1
    if (isUnit)
    {
      if(!SingleValueFuzzyEquals(data[ir +i],1.e0,std::numeric_limits<real8>::epsilon(),std::numeric_limits<real8>::epsilon()))
      {
        cerr << "13. checking lower:: not unit diag at element [" << i << "," << i << "]" << "found value = "<< data[ir + i] << std::endl;
        *diag = UNITDIAG::NONUNIT;
        isUnit = false;
      }
    }
    for (size_t j = 0; j < i; j++)  // check if upper triangle is empty
    {
      if (!SingleValueFuzzyEquals(data[ir + j],0.e0,std::numeric_limits<real8>::epsilon(),std::numeric_limits<real8>::epsilon()))
      {
        cerr << "14. checking lower:: returning as nonzero in upper triangle at [" << j << "," << i << "]" << "found value = "<< data[ir + j] << std::endl;
        SingleValueFuzzyEquals(data[ir +j],0.e0,std::numeric_limits<real8>::epsilon(),std::numeric_limits<real8>::epsilon());
        *tri = UPLO::NEITHER;
        return;
      }
    }
  }
}

/**
 * Checks if a square matrix contains an upper triangular matrix or row permutation of.
 * @param data the column major matrix data.
 * @param rows the number of rows in the matrix.
 * @param cols the number of cols in the matrix.
 * @param ret the struct holding the state of triangular matrix probing.
 */
template<typename T>
void checkIfUpperTriangularPermuteRows(T * data, size_t rows, size_t cols,  TriangularStruct * ret)
{
  // This holds the location of the "start" of rows, i.e. first non-zero value.
  unique_ptr<size_t[]> rowStarts (new size_t[rows]);
  std::fill(rowStarts.get(), rowStarts.get() + rows, row_start_invalid); // row_start_invalid is our flag

  // This holds the logic for whether we've seen a valid row of a triangular matrix before or not.
  std::unique_ptr<bool[]> rowTagPtr (new bool[rows]);
  std::fill(rowTagPtr.get(),rowTagPtr.get()+rows,false);

  // Assume we have a unit diagonal matrix and flip it later if not
  ret->flagDiag = UNITDIAG::UNIT;

  // Assume we have upper triangular matrix and flip it later if not
  ret->flagUPLO = UPLO::UPPER;

  // flag to indicate if a triangular permutation is valid upper permutation
  bool upperTriangleIfValidIspermuted = false;

  // flag to indicate if a triangular permutation is valid
  bool validPermutation = true;

  // flag to cause fast exist on branches testing for unit diagonal if we've already noted it isn't
  bool isUDswitch = true; // is unit diag

  // Check if the lower part is all 0 walk rows to look for when number appears in [0...0, Number....]
  // we assume that there is a valid permutation possible within the data and attempt to find it
  for (size_t i = 0; i < cols; i++)
  {
    for (size_t j = 0; j < rows; j++)
    {
      if (
        rowStarts[j] == row_start_invalid // we've not seen this row before
        && // AND
        // it's not got a zero at location j.
        !SingleValueFuzzyEquals(data[i * rows + j],0.e0,std::numeric_limits<real8>::epsilon(),std::numeric_limits<real8>::epsilon())
      )
      { // So set our rowstarts for this location to point to this col.
        rowStarts[j] = i;
        // Check and see if the element (which will end up on the diag) is 1 and therefore unit diag 
        // is a possible optimisation.
        if (isUDswitch && !SingleValueFuzzyEquals(data[i * rows + j],1.e0,std::numeric_limits<real8>::epsilon(),std::numeric_limits<real8>::epsilon()))
        {
          cerr << "16. checking upper:: not unit diag at element [" << i << "," << j << "]" << std::endl;

          // set as not unit diagonal
          ret->flagDiag = UNITDIAG::NONUNIT;
          // flip the switch so we get fast exists in future avoiding calling fuzzy equals
          isUDswitch = false;
        }
      }
    }
  }

  // Fix for [MAT-443]
  // Check to see if the row starts are valid before we work out if the permutation is valid.
  // i.e. if there's a row which is all zeros, then rowStarts will still contain a
  // row_start_invalid, if you go and look up a size_t containing row_start_invalid
  // that's a monstrously large number to look up and will be technically wrong and also
  // likely cause a load of invalid access related problems.
  for (size_t i = 0; i < rows; i++)
  {
    if (rowStarts[i]==row_start_invalid)
    {
      validPermutation = false;
      break;
    }
  }

  // if it's still possibly valid...
  if (validPermutation)
  {
    // Check to see if the matrix is a permutation of an upper triangle
    // Do this by checking that the claimed start of rows cover all columns
    // first set flags, then check flags, complete set needed to assess whether the perm is present
    // We basically set flags in rowTag based on start locations, then check that all rowTags are set.
    // If the permutation is valid *but* its not declared as permuted then we just have a standard
    // upper triangular matrix.
    for (size_t i = 0; i < rows; i++)
    {
      if (!rowTagPtr[rowStarts[i]])
      {
        rowTagPtr[rowStarts[i]] = true;
      }
      if (!upperTriangleIfValidIspermuted && rowStarts[i] != i)
      {
        upperTriangleIfValidIspermuted = true;
      }
    }
    for (size_t i = 0; i < rows; i++)
    {
      if (!rowTagPtr[i])
      {
        validPermutation = false;
        break;
      }
    }
  }

  if (!validPermutation)
  {
    // we don't have a valid upper permutation, return declaring so
    cerr << "17. checking upper:: not upper triangular, permuted or otherwise" << std::endl;
    ret->flagUPLO = UPLO::NEITHER;
  }
  else
  {
    if(upperTriangleIfValidIspermuted)
    {
      // permutation is valid row, update struct. foo(rowStarts) = 1: length(rowStarts) gives direct perm
      cerr << "18. checking upper:: Row permutation spotted" << std::endl;
      ret->flagPerm = PERMUTATION::ROW;
      ret->perm = std::move(rowStarts);
    }
    else
    {
      // permutation is in fact the 1:1 map, we have a triangle already
      cerr << "19. checking upper:: standard upper triangle" << std::endl;
      ret->flagPerm = PERMUTATION::STANDARD;
    }
  }
  return;
}

/**
 * This function probes a matrix to see if it is triangular and has related exploitable properties.
 * @param T data the type, real8 or complex16 are valid.
 * @param data the column major matrix data.
 * @param rows the number of rows in the matrix.
 * @param cols the number of columns in the matrix.
 * @return a unique_ptr to a \a TriangularStruct type containing the properties found
 */
template<typename T>
std::unique_ptr<TriangularStruct> isTriangular(T * data, size_t rows, size_t cols)
{
  // This struct has mutated members updated as properties are found!
  std::unique_ptr<TriangularStruct> tptr (new TriangularStruct());

  TriangularStruct * ref = tptr.get();

  // See if it's lower triangular
  checkIfLowerTriangular(data, rows, cols, &(ref->flagUPLO), &(ref->flagDiag));
  if (ref->flagUPLO != UPLO::NEITHER)
  {
    return tptr;
  }

  // See if it's Upper or permuted upper
  checkIfUpperTriangularPermuteRows(data, rows, cols, ref);

  // return anyway, tptr contains state
  return tptr;

}


} // end namespace detail


// This is a rough translation of OG mldivide from OG-Maths_Legacy ~2012,
//  which in turn is based on my libllsq from ~2011

/**
 * mldivide_dense_runner is the "super solver" routine. Will attempt to optimally (in terms of speed and accuracy) solve any valid linear system of the form AX=B, where A, X and B are all matrices.
 * @param T the data type, real8 or complex16 are valid.
 * @param reg0 the register for the return value, X.
 * @param arg0 the matrix A.
 * @param arg0 the matrix B.
 */
template<typename T>
void*
mldivide_dense_runner(RegContainer& reg0, shared_ptr<const OGMatrix<T>> arg0, shared_ptr<const OGMatrix<T>> arg1)
{

  // debug flag, true for debug statements, useful when developing
  constexpr bool debug_ = true;

  // data from array 1 (A)
  T * ptrdata1 = arg0->getData();
  std::size_t rows1 = arg0->getRows();
  std::size_t cols1 = arg0->getCols();
  int4 int4rows1 = rows1;
  int4 int4cols1 = cols1;
  std::size_t len1 = rows1 * cols1;
  std::unique_ptr<T[]> data1Ptr (new T[len1]);
  T * data1 = data1Ptr.get();
  std::copy(ptrdata1,ptrdata1+len1,data1);

  // data from array 2 (B)
  T * ptrdata2 = arg1->getData();
  std::size_t rows2 = arg1->getRows();
  std::size_t cols2 = arg1->getCols();
  int4 int4rows2 = rows2;
  int4 int4cols2 = cols2;
  std::size_t len2 = rows2 * cols2;
  std::unique_ptr<T[]> data2Ptr (new T[len2]);
  T * data2 = data2Ptr.get();
  std::copy(ptrdata2,ptrdata2+len2,data2);

  // Internal variables signifcant in flow control.
  real8 rcond = 0; // estimate of reciprocal condition number
  real8 anorm = 0; // the 1 norm of a matrix
  bool singular = false; // is the matrix identified as singular
  bool attemptQR = true; // should QR decomposition be attempted for singular/over determined systems prior to using SVD?

  // Auxiallary variable

  // for pointer switching if a perm is needed but system is bad and needs resetting
  unique_ptr<T[]> triPtr1Ptr = nullptr;
  unique_ptr<T[]> triPtr2Ptr = nullptr;
  T * triPtr1 = nullptr;
  T * triPtr2 = nullptr;

  // pointer switching for case when xgels creates a bigger system than for which there's space
  unique_ptr<T[]> bdata2Ptr = nullptr;

  // return item
  OGNumeric::Ptr ret;

  // Approx alg: This is based loosely on my llsq library, see internal OG C code.
  // 1) check if system commutes
  // 2) check if square ? goto 3) : goto 6)
  // 3) check if triangular (or perm of), if so compute rcond, if rcond ok solve else goto 6), if solve fails goto 6)
  // 4) check if symmetric, if so, try cholesky, if decomp fails it's not s.p.d. If decomp succeeds, try solve, if fails, compute rcond and goto 6)
  // 5) Try LUP. If decomp succeeds, try solve, if fails, compute rcond and goto 6)
  // 6) rcond+1!=1 ? try QR based llsq : goto 8)
  // 7) if QR decomp succeeds return, else goto 8)
  // 8) run svd based llsq
  // 9) if svd llsq fails warn always return.

  // First...

  // LAPACK info variable, normally seen as kind(int):: INFO in Fortran.
  int4 info = 0;

  // check if the system is sane
  if(rows1!=rows2)
  {
    stringstream msg;
    msg << "System does not commute. Rows in arg0: " << rows1 << ". Rows in arg1 " << rows2 << std::endl;
    throw rdag_error(msg.str());
  }

  // check for all zeros system matrix, quick return if so
  bool allzero=true;
  for(size_t i = 0; i < len1; i++)
  {
    if(data1[i]!=0.e0)
    {
      allzero = false;
      break;
    }
  }
  if(allzero)
  {
    if (debug_)
    {
      cerr << "5. Matrix is all zeros, returning Inf"  << std::endl;
    }
    unique_ptr<T[]> retData(new T[len2]);
    for(size_t i = 0; i < len2; i++)
    {
      retData.get()[i]=std::numeric_limits<real8>::infinity();
    }
    ret = makeConcreteDenseMatrix(retData.release(), rows2, cols2, OWNER);
    reg0.push_back(ret);
  }


  // Do the alg above:
  // is it square?
  if (rows1 == cols1)
  {
    if (debug_)
    {
      cerr << "10. Matrix is square"  << std::endl;
    }

    // Is the array1 triangular or permuted triangular, send probe and get back struct containing result
    unique_ptr<detail::TriangularStruct> tptr = detail::isTriangular(data1, rows1, cols1);

    // Set flags based on triangular probe
    detail::UPLO UPLO = tptr->flagUPLO;
    detail::UNITDIAG DIAG = tptr->flagDiag;

    // Is this matrix layout some breed of triangular?
    if (UPLO != detail::UPLO::NEITHER)
    {
      if (debug_)
      {
        cerr << "20. Matrix is " << (UPLO == detail::UPLO::UPPER ? "upper" : "lower") << " triangular, " << (DIAG == detail::UNITDIAG::NONUNIT ? "non-unit" : "unit") << " diagonal." << std::endl;
      }

      detail::PERMUTATION DATA_PERMUTATION = tptr->flagPerm;
      // Is the triangular matrix permuted?
      if (DATA_PERMUTATION != detail::PERMUTATION::STANDARD)
      {
        // we have a permutation of the data, so we'll rewire the system
        if (debug_)
        {
          cerr << "25. Matrix is " << (DATA_PERMUTATION == detail::PERMUTATION::ROW ? "row" : "column") << " permuted triangular." << std::endl;
        }
        // We have a row permutation... this gets a bit hairy pointer twiddling wise.
        // The idea is that it'd be nice when it comes to calling lapack to use the same set of
        // calls and logic as with the general "it's triangular" branch.
        // A data rewrite is done based on the permutation to make the system triangular, 
        // however, we need to keep hold of the original data in case the triangular solve 
        // fails and we need to try something else. So the new permuted system swaps references 
        // with the original system so the "original" system logic can be used. Later on, 
        // should the triangular solve fail, the reverse reference swap occurs so the alg
        // can proceed with the original data and the smart pointer to the permutation will
        // delete[] the permuted data allocated here.
        if (DATA_PERMUTATION == detail::PERMUTATION::ROW)
        {
          triPtr1Ptr = unique_ptr<T[]>(new T[len1]());
          triPtr1Ptr.swap(data1Ptr); // triPtr1Ptr now points to original data
          // assign pointers for read/write ops
          triPtr1  = triPtr1Ptr.get();
          data1 = data1Ptr.get();
          std::fill(data1,data1+len1,0e0);
          for (size_t i = 0; i < cols1; i++)
          {
            for (size_t j = 0; j < rows1; j++)
            {
              data1[i * rows1 + tptr->perm.get()[j]] = triPtr1[i * rows1 + j];
            }
          }
          triPtr2Ptr = unique_ptr<T[]>(new T[len2]());
          triPtr2Ptr.swap(data2Ptr); // triPtr2Ptr now points to original data
          // assign pointers for read/write ops
          triPtr2  = triPtr2Ptr.get();
          data2 = data2Ptr.get();
          std::fill(data2,data2+len2,0e0);
          for (size_t i = 0; i < cols2; i++)
          {
            for (size_t j = 0; j < rows2; j++)
            {
              data2[i * rows2 + tptr->perm.get()[j]] = triPtr2[i * rows2 + j];
            }
          }
        }

        // TODO:: spot and mutate based on column permutations

      } // end permutation branch


      cerr << "28. Hitting lapack tri routines with UPLO = " << (UPLO == detail::UPLO::UPPER ? "upper" : "lower") << " DIAG = " << (DIAG == detail::UNITDIAG::NONUNIT ? "non-unit" : "unit") << std::endl;

      // Compute reciprocal condition number, if it's bad say so and least squares solve

      // Get enumerated values as chars for guiding the lapack call
      auto lapack_uplo = asChar(UPLO);
      auto lapack_diag = asChar(DIAG);
      lapack::xtrcon(lapack::ONE, &lapack_uplo, &lapack_diag, &int4rows1, data1, &int4rows1, &rcond, &info);

      // Test impact of reciprocal condition number on floating point arithmetic at scale of rcond itself.
      if (rcond + 1.e0 != 1.e0)
      {
        // branch taken as reciprocal condition number is acceptable to attempt triangular back solve
        info = 0;
        try
        {
          // try lapack triangular system solve
          lapack::xtrtrs(&lapack_uplo, lapack::N, &lapack_diag, &int4rows1, &int4cols2, data1, &int4rows1, data2, &int4rows2, &info);
          // Solve was successful so we create a new matrix to return, release the data from unique_ptr
          // ownership as the matrix shared_ptr container will handle and own it from now.
          ret = makeConcreteDenseMatrix(data2Ptr.release(), rows2, cols2, OWNER);
          reg0.push_back(ret);
          if (debug_)
          {
            cerr << "30. Triangular solve success, returning" << std::endl;
          }
          return nullptr;
        }
        catch (rdag_error& e)
        {
          if(info<0) // caught a xerbla error, rethrow
          {
            throw;
          }
          // In theory, the rest of the code in scope is dead...
          // Triangular solve should never fail because the condition estimators should pick up
          // blatently ill-conditioned systems and just move on to a more appropriate method
          // of solution, however, this branch is left in as part of a catch-all just in case
          // some system makes it through the ill-conditioned test and the solver then fails.
          // If this is the case, we mark as singular and if there's a permutation applied 
          // it is reversed.
          if (debug_)
          {
            cerr<< "40. Triangular solve fail. Mark as singular." << std::endl;
          }
          // solve failed, system is singular so set the flag to skip doing anything else
          // in the "exact" solution regime
          singular = true;

          // If we are here, we've failed to do a triangular solve, but the data might still be the permuted
          // variant, given we are in the realm of least squares solutions, we need to reset the permutation
          // back to the original data, it's just a pointer switch, not doing so might influence results from
          // iterative llsq solvers.
          if (DATA_PERMUTATION != detail::PERMUTATION::STANDARD)
          {
            if(debug_)
            {
              cerr << "45. Resetting permutation." << std::endl;
            }

            // swap back pointers
            data1Ptr.swap(triPtr1Ptr);
            data2Ptr.swap(triPtr2Ptr);
            data1 = data1Ptr.get();
            data2 = data2Ptr.get();
          }
        }
      }
      else
      {
        // NOTE: this branch isn't needed, singular can be moved to outer branch, *but* is left in
        // for debug logic.
        // Reciprocal condition number was near so near zero solution was not attempted, set singular flag
        // and abandon any further attemps at exact solution.
        if (debug_)
        {
          cerr << "43. Triangular condition was computed as too bad to attempt solve." << std::endl;
        }
        singular = true;
      }
    } // end "this matrix is triangular"

    // if it's triangular and it failed, then it's singular and a condition number is already computed and this can be jumped!
    if (!singular)
    {
      if (debug_)
      {
        cerr << "50. Not triangular" << std::endl;
      }
      bool cholesky_mangled_data = false;
      // See if it's Hermitian (symmetric in the real case)
      if (detail::isHermitian(data1, rows1, cols1))
      {
        if (debug_)
        {
          cerr << "60. Is Hermitian" << std::endl;
        }
        info = 0;
        // Attempt to compute a Cholesky factorisation
        try
        {
          cholesky_mangled_data = true;
          lapack::xpotrf(lapack::L, &int4rows1, data1, &int4rows1, &info);
        }
        catch(rdag_error& e)
        {
          if(info < 0)
          {
            throw;
          }
        }
        // If the Cholesky factorisation was ok, matrix is s.p.d.
        // Factorisation is in the lower triangle, back solve based on this.
        if(info == 0)
        {
          if (debug_)
          {
            cerr << "70. Cholesky success.  Computing condition based on cholesky factorisation" << std::endl;
          }
          // technically this should be zlanhe() in the complex case, but the 1 norm estimate is the same
          // as sum(abs(A)) == sum(abs(conj(A)))
          anorm = lapack::xlansy(lapack::ONE, lapack::L, &int4rows1, data1, &int4rows1);

          // compute a Cholesky condition estimate
          lapack::xpocon(lapack::L, &int4rows1, data1, &int4rows1, &anorm, &rcond, &info);
          if (debug_)
          {
            cerr << "80. Cholesky condition estimate. " << rcond << std::endl;
          }
          // again test if reciprocal condition is acceptable
          if (1.e0 + rcond != 1.e0)
          {
            if (debug_)
            {
              cerr << "90. Cholesky condition acceptable. Backsolve and return." << std::endl;
            }
            lapack::xpotrs(lapack::L, &int4rows1, &int4cols2, data1, &int4rows1, data2, &int4rows2, &info);
            // there is no possible numerical exception here, just input
            ret = makeConcreteDenseMatrix(data2Ptr.release(), rows2, cols2, OWNER);
            reg0.push_back(ret);
            return nullptr;
          }
          else
          {
            // Cholesky decomposition of matrix indicates condition is bad
            if (debug_)
            {
              cerr << "100. Cholesky condition bad. Mark as singular." << std::endl;
            }
            singular = true;
          }
        }
        else
        {
            if (debug_)
            {
              cerr << "110. Cholesky decomposition failed." << std::endl;
            }
        }
      } // end if Hermitian branch

      if (!singular)
      {
        // We are here if the matrix is not singular, not triangular and not Hermitian.
        if (debug_)
        {
          cerr << "120. Non-Hermitian OR not s.p.d." << std::endl;
        }
        //  we're here as matrix isn't s.p.d. as Cholesky failed. 
        // Get new copy of the matrix data if cholesky ran
        if(cholesky_mangled_data)
        {
          std::copy(arg0->getData(),arg0->getData()+len1,data1);
        }

        // try solving with generalised LUP solver, will need a pivot store first
        unique_ptr<int[]> ipivPtr (new int4[rows1]);

        if (debug_)
        {
          cerr << "130. LUP attempted" << std::endl;
        }

        // Try a LUP decomposition
        try
        {
          lapack::xgetrf<T, lapack::OnInputCheck::isfinite>(&int4rows1, &int4cols1, data1, &int4rows1, ipivPtr.get(), &info);
        }
        catch (rdag_recoverable_error& e)
        {
          // we're ok, this just means it's singular
        }
        // Else, exception propagates, stack unwinds

        if (info == 0)
        {
          // LUP decomposition was successful, check 
          if (debug_)
          {
            cerr << "140. LUP success. Computing condition based on LUP factorisation." << std::endl;
          }

          // compute a reciprocal condition estimate based on decomposition
          anorm = lapack::xlange(lapack::ONE, &int4rows1, &int4cols1, arg0->getData(), &int4rows1);
          lapack::xgecon(lapack::ONE, &int4rows1, data1, &int4rows1, &anorm, &rcond, &info);

          // if condition estimate isn't too bad then back solve
          if (1.e0 + rcond != 1.e0)
          {
            // back solving using LUP system solver
            lapack::xgetrs(lapack::N, &int4rows1, &int4cols2, data1, &int4rows1, ipivPtr.get(), data2, &int4rows2, &info);
            if (debug_)
            {
              cerr << "150. LUP returning" << std::endl;
            }
            // no throw possible unless on bad input, create return matrix 
            // freeing unique_ptr ref so shared_ptr in matrix type can have ownership on the way.
            ret = makeConcreteDenseMatrix(data2Ptr.release(), rows2, cols2, OWNER);
            reg0.push_back(ret);
            return nullptr;
          }
          else // reciprocal condition was computed as bad, mark as singular
          {
            if (debug_)
            {
              cerr << "160. LUP failed, condition too high" << std::endl;
            }
            singular = true;
          }
        }
        else // decomposition failed due to singularity in matrix, mark as singular
        {
          if (debug_)
          {
            cerr << "170. LUP factorisation failed. Mark as singular." << std::endl;
          }
          singular = true;
        }
      }
    }
  } // end if square branch

  // branch on rcond if computed, if cond is sky high then rcond is near zero
  // we use 1.e0 here againso cutoff is near machine precision
  if (singular)
  {
    cerr << "Square matrix is singular to machine precision. RCOND estimate = " << rcond << std::endl;
    if (debug_)
    {
      cerr << "190. Square matrix is singular." << std::endl;
    }
    if ((rcond + 1.e0) == 1.e0)
    {
      if (debug_)
      {
        cerr << "200. Condition of square matrix is too bad for QR least squares." << std::endl;
      }
      // Condition is really bad so don't use QR
      attemptQR = false;
    }
    // take a copy of the original data as it will have been destroyed above in the decomp trials
    std::copy(arg0->getData(),arg0->getData()+len1,data1Ptr.get());
  }

  // needed for QR and SVD, the leading dimension of matrix "B"
  int4 ldb = std::max(rows1, cols1);
  // allocated enough space for the solution as it will be rows1 * cols2 in size 
  // and data2 is rows2*cols2 which may not be big enough.
  // again this involves pointer swapping as with the permutation branch in the triangular solve
  // more details on the process are available at its call site.
  if (rows1 < cols1)
  {
    bdata2Ptr = unique_ptr<T[]>(new T[ldb * cols2]);
    T * b = bdata2Ptr.get();
    //copy in data strips
    for (size_t i = 0; i < cols2; i++)
    {
      std::copy(data2 + (i * rows2), data2 + ((i + 1)* rows2), b + i * ldb);
    }
    // switch pointers so data2 now points at a correct sized alloc
    data2Ptr.swap(bdata2Ptr);
    // reassign underlying data2 pointer
    data2 = data2Ptr.get();
  }

  // first, attempt QR if we haven't already decided it's a bad idea
  if (attemptQR)
  {
    if (debug_)
    {
      cerr << "210. Attempting QR solve." << std::endl;
    }

    // Attempt a least squares solve
    try
    {
      lapack::xgels(lapack::N, &int4rows1, &int4cols1, &int4cols2, data1, &int4rows1, data2, &ldb, &info);
    }
    catch(rdag_error& e)
    {
      if (info < 0)
      {
        throw;
      }
    }

    if (info > 0)
    {
      // It failed with a rank deficiency problem, clean up before moving on to SVD
      if (debug_)
      {
        cerr <<  "220. QR solve failed" << std::endl;
      }
      cerr <<  " WARN: Matrix of coefficients does not have full rank. Rank is " << info << "." << std::endl;

      // copy back in data1 and strips of data2, both will be needed for svd
      // no point in freeing here
      if (rows1 < cols1)
      {
        //copy in data2 strips, remember the pointers are still swapped!
        for (size_t i = 0; i < cols2; i++)
        {
          std::copy(bdata2Ptr.get() + (i * rows2), bdata2Ptr.get() + ((i + 1)* rows2), data2Ptr.get() + i * ldb);
        }
      }
      // take a copy of the original data as it will have been destroyed above
      std::copy(arg0->getData(),arg0->getData()+len1,data1);
    }
    else
    {
      // the QR solve was successful, pull out the solution data into a new allocation and return
      if (debug_)
      {
        cerr << "230. QR solve success, returning" << std::endl;
      }
      // new alloc
      T * ref = data2;
      unique_ptr<T[]> newdata2Ptr (new T[cols1 * cols2]);
      data2 = newdata2Ptr.get();
      // and copy 1:cols1 from each column of data2 needs to be returned
      for (size_t i = 0; i < cols2; i++)
      {
        std::copy(ref + (i * ldb), ref + ((i * ldb)+cols1), data2 + (i * cols1));
      }
      // wire in return
      ret = makeConcreteDenseMatrix(newdata2Ptr.release(), cols1, cols2, OWNER);
      reg0.push_back(ret);
      return nullptr;
    }
  }

  // If all else fails... we attempt a general least squares solve with SVD
  if (debug_)
  {
    cerr <<  "240. Attempting SVD" << std::endl;
  }

  // allocate space for the singular vectors
  unique_ptr<real8[]> sPtr (new real8[ldb]());
  real8 moorePenroseRcond = -1.e0; // this is the definition of singular in the Moore-Penrose sense, if set to -1 machine prec is used

  int4 rank=0; // needed for call but not used info
  try
  {
    // NOTE: Using the divide and conquer based SVD, uses twisted factorisation, is much quicker
    // at the cost of occasional loss of accuracy
    lapack::xgelsd(&int4rows1, &int4cols1, &int4cols2, data1, &int4rows1, data2, &ldb, sPtr.get(), &moorePenroseRcond, &rank, &info);
  }
  catch (rdag_error& e)
  {
    if(info<0)
    {
      throw;
    }
  }

  // handle fails
  if (info != 0)
  {
    // something went wrong in the solver, nothing we can do so throw...
    if (debug_)
    {
      cerr << "250. SVD failed" << std::endl;
    }
    stringstream msg;
    msg << "SVD Failed to converege. " << info << " out of " << std::max(rows1, cols1) << " off diagonals failed to converge to zero. RCOND = " << rcond << std::endl;
    throw rdag_error(msg.str());
  }
  else
  { // dead branch, used for debug
    if (debug_)
    {
      cerr << "260. SVD success" << std::endl;
    }
  }
  if (debug_)
  {
    cerr << "270. SVD returning" << std::endl;
  }

  // new alloc
  T * ref = data2;
  unique_ptr<T[]> newdata2Ptr (new T[cols1 * cols2]);
  data2 = newdata2Ptr.get();
  // and copy 1:cols1 from each column of data2 needs to be returned
  for (size_t i = 0; i < cols2; i++)
  {
    std::copy(ref + (i * ldb), ref + ((i * ldb)+cols1), data2 + (i * cols1));
  }
  // wire in return
  ret = makeConcreteDenseMatrix(newdata2Ptr.release(), cols1, cols2, OWNER);
  reg0.push_back(ret);
  return nullptr;
}


// MLDIVIDE runner:
void * MLDIVIDERunner::run(RegContainer& reg0, OGComplexDenseMatrix::Ptr arg0, OGComplexDenseMatrix::Ptr arg1) const
{
  mldivide_dense_runner<complex16>(reg0, arg0, arg1);
  return nullptr;
}


void * MLDIVIDERunner::run(RegContainer& reg0, OGRealDenseMatrix::Ptr arg0, OGRealDenseMatrix::Ptr arg1) const
{
  mldivide_dense_runner<real8>(reg0, arg0, arg1);
  return nullptr;
}

void *
MLDIVIDERunner::run(RegContainer& reg0, OGRealScalar::Ptr arg0, OGRealScalar::Ptr arg1) const
{
  real8 data1 = arg0->getValue();
  real8 data2 = arg1->getValue();
  OGNumeric::Ptr ret = OGRealScalar::create(data2/data1);
  reg0.push_back(ret);
  return nullptr;
}

}

