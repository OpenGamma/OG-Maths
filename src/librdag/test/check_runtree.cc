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
    OGRealScalar * s1 = new OGRealScalar(10);
    OGRealMatrix * m1 = new OGRealMatrix(new real16[2]{1,2},2,1,OWNER);
    OGRealMatrix * m2 = new OGRealMatrix(new real16[2]{1,2},2,1,OWNER);

    OGExpr * s1m1 = new MTIMES(s1,m1);
    OGExpr * s1m1m2 = new MTIMES(s1m1,m2);
    EXPECT_THROW(runtree(s1m1m2),rdag_error);

    delete s1m1m2;
}

TEST(RunTree, ExecOk)
{
    OGRealScalar * s1 = new OGRealScalar(10);
    OGRealMatrix * m1 = new OGRealMatrix(new real16[2]{1,2},1,2,OWNER);
    OGRealMatrix * m2 = new OGRealMatrix(new real16[2]{1,2},2,1,OWNER);

    OGExpr * s1m1 = new MTIMES(s1,m1);
    OGExpr * s1m1m2 = new MTIMES(s1m1,m2);

    runtree(s1m1m2);

    OGRealScalar * expected = new OGRealScalar(50);
    EXPECT_TRUE(s1m1m2->getRegs()[0]->asOGTerminal()->mathsequals(expected));

    delete s1m1m2;
    delete expected;
}