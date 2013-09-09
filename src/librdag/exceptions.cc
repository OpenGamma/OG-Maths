#include "exceptions.hh"
#include <iostream>

using namespace std;

/*
 * The namespace for the DAG library
 */
namespace librdag
{
  
/*
 * librdagException
 */

const char*
librdagException::what() const throw()
{
  return "Exception occurred.";
}

} // namespace librdag
