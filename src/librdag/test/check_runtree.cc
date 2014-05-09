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
    pOGNumeric s1 = pOGNumeric{new OGRealScalar(10)};
    pOGNumeric m1 = pOGNumeric{new OGRealMatrix(new real16[2]{1,2},2,1,OWNER)};
    pOGNumeric m2 = pOGNumeric{new OGRealMatrix(new real16[2]{1,2},2,1,OWNER)};

    pOGExpr s1m1 = pOGExpr{new MTIMES(s1,m1)};
    pOGExpr s1m1m2 = pOGExpr{new MTIMES(s1m1,m2)};
    EXPECT_THROW(runtree(s1m1m2),rdag_error);
}

TEST(RunTree, ExecOk)
{
    pOGNumeric s1 = pOGNumeric{new OGRealScalar(10)};
    pOGNumeric m1 = pOGNumeric{new OGRealMatrix(new real16[2]{1,2},1,2,OWNER)};
    pOGNumeric m2 = pOGNumeric{new OGRealMatrix(new real16[2]{1,2},2,1,OWNER)};

    pOGExpr s1m1 = pOGExpr{new MTIMES(s1,m1)};
    pOGExpr s1m1m2 = pOGExpr{new MTIMES(s1m1,m2)};

    runtree(s1m1m2);

    pOGTerminal expected = pOGTerminal{new OGRealScalar(50)};
    EXPECT_TRUE(s1m1m2->getRegs()[0]->asOGTerminal()->mathsequals(expected));
}
