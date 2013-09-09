#include <iostream>
using namespace std;


/*
 * The namespace for the DAG library
 */
namespace librdag
{
  
/*
 * General execption thing
 */
class librdagException: public exception
{
    public:
    virtual const char* what() const throw()
    {
      return "Exception occurred.";
    }
};

}