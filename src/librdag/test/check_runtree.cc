/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "execution.hh"
#include "expression.hh"
#include "terminal.hh"
#include "test/terminals.hh"
#include "gtest/gtest.h"
#include "runtree.hh"

using namespace std;
using namespace librdag;


TEST(RunTree, ThrowDueToBadOp)
{
    OGNumeric::Ptr s1 = OGRealScalar::create(10);
    OGNumeric::Ptr m1 = OGRealMatrix::create(new real16[2]{1,2},2,1,OWNER);
    OGNumeric::Ptr m2 = OGRealMatrix::create(new real16[2]{1,2},2,1,OWNER);

    OGExpr::Ptr s1m1 = OGExpr::Ptr{new MTIMES(s1,m1)};
    OGExpr::Ptr s1m1m2 = OGExpr::Ptr{new MTIMES(s1m1,m2)};
    EXPECT_THROW(runtree(s1m1m2),rdag_error);
}

TEST(RunTree, ExecOk)
{
    OGNumeric::Ptr s1 = OGRealScalar::create(10);
    OGNumeric::Ptr m1 = OGRealMatrix::create(new real16[2]{1,2},1,2,OWNER);
    OGNumeric::Ptr m2 = OGRealMatrix::create(new real16[2]{1,2},2,1,OWNER);

    OGExpr::Ptr s1m1 = OGExpr::Ptr{new MTIMES(s1,m1)};
    OGExpr::Ptr s1m1m2 = OGExpr::Ptr{new MTIMES(s1m1,m2)};

    runtree(s1m1m2);

    OGTerminal::Ptr expected = OGRealScalar::create(50);
    EXPECT_TRUE(s1m1m2->getRegs()[0]->asOGTerminal()->mathsequals(expected));
}
