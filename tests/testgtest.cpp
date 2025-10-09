#include <gtest/gtest.h>
#include "bcifs/formalcoef.h"
#include "bcifs/formalmatrix.h"
#include "bcifs/constraintsolver.h"

TEST(FormalCoef, ZeroAndOne) {
    auto zero = BCIFS::FormalCoef::zero();
    auto one = BCIFS::FormalCoef::one();
    EXPECT_EQ(zero->type(), BCIFS::CoefType::ZERO);
    EXPECT_EQ(one->type(), BCIFS::CoefType::ONE);
}

TEST(FormalCoef, Var) {
    auto var = BCIFS::FormalCoef::var(0.5f);
    EXPECT_EQ(0.5f, var->value());
}

TEST(ConstraintSolver, UnifyConstants) {
    auto v = BCIFS::FormalCoef::var(0.5f);
    auto one = BCIFS::FormalCoef::one();

    BCIFS::FormalMatrix L(1, 1);
    BCIFS::FormalMatrix R(1, 1);
    L.set(0, 0, v);
    R.set(0, 0, one);

    BCIFS::ConstraintSolver::solve(L, R);

    EXPECT_EQ(v->findRoot()->type(), BCIFS::CoefType::ONE);
}