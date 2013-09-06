#include "entrypt.h"
#include "expression.hh"

using namespace std;
using namespace librdag;

int main(void)
{
  OGExpr *plus = new PLUS(new OGRealScalar(2), new OGRealScalar(3));
  entrypt((struct c_OGNumeric*) plus);
}
