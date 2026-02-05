#include <gtest/gtest.h>

#include "app/bcifs/coefpool.h"
#include "app/bcifs/constraintsolver.h"
#include "app/bcifs/formalcoef.h"
#include "app/bcifs/formalmatrix.h"

TEST(Constraint, ConstraintsOnMatrices) {
    BCIFS::CoefPool pool;
    BCIFS::FormalCoef a(pool.makeVar(0.1f));
    BCIFS::FormalCoef b(pool.makeVar(0.2f));
    BCIFS::FormalCoef c(pool.makeVar(0.3f));
    BCIFS::FormalCoef d(pool.makeVar(0.4f));
    BCIFS::FormalCoef e(pool.makeVar(0.5f));
    BCIFS::FormalCoef f(pool.makeVar(0.6f));
    BCIFS::FormalCoef g(pool.makeVar(0.7f));
    BCIFS::FormalCoef h(pool.makeVar(0.8f));
    BCIFS::FormalCoef i(pool.makeVar(0.9f));

    BCIFS::FormalCoef j(pool.makeVar(1.1f));
    BCIFS::FormalCoef k(pool.makeVar(1.2f));
    BCIFS::FormalCoef l(pool.makeVar(1.3f));
    BCIFS::FormalCoef m(pool.makeVar(1.4f));
    BCIFS::FormalCoef n(pool.makeVar(1.5f));
    BCIFS::FormalCoef o(pool.makeVar(1.6f));
    BCIFS::FormalCoef p(pool.makeVar(1.7f));
    BCIFS::FormalCoef q(pool.makeVar(1.8f));
    BCIFS::FormalCoef r(pool.makeVar(1.9f));

    BCIFS::FormalMatrix T0(3, 3, &pool);
    BCIFS::FormalMatrix T1(3, 3, &pool);
    BCIFS::FormalMatrix BL(3, 1, &pool);
    BCIFS::FormalMatrix BR(3, 1, &pool);
    BCIFS::FormalMatrix TS(1, 1, &pool);
    BCIFS::FormalMatrix P0(3, 3, &pool);

    T0.set(0, 0, a);
    T0.set(1, 0, b);
    T0.set(2, 0, c);
    T0.set(0, 1, d);
    T0.set(1, 1, e);
    T0.set(2, 1, f);
    T0.set(0, 2, g);
    T0.set(1, 2, h);
    T0.set(2, 2, i);

    T1.set(0, 0, j);
    T1.set(1, 0, k);
    T1.set(2, 0, l);
    T1.set(0, 1, m);
    T1.set(1, 1, n);
    T1.set(2, 1, o);
    T1.set(0, 2, p);
    T1.set(1, 2, q);
    T1.set(2, 2, r);

    BL.set(0, 0, BCIFS::FormalCoef::one());
    BR.set(2, 0, BCIFS::FormalCoef::one());

    TS.set(0, 0, BCIFS::FormalCoef::one());

    P0.set(0, 2, BCIFS::FormalCoef::one());
    P0.set(1, 1, BCIFS::FormalCoef::one());
    P0.set(2, 0, BCIFS::FormalCoef::one());

    BCIFS::FormalMatrix leftIncLeft = BL * TS;
    BCIFS::FormalMatrix rightIncLeft = T0 * BL;

    BCIFS::FormalMatrix leftIncRight = BR * TS;
    BCIFS::FormalMatrix rightIncRight = T1 * BR;

    BCIFS::FormalMatrix leftAdj = T0 * BR;
    BCIFS::FormalMatrix rightAdj = T1 * BL;

    BCIFS::FormalMatrix leftPermutLeft = T0 * P0;
    BCIFS::FormalMatrix rightPermutLeft = P0 * T1;

    BCIFS::FormalMatrix leftPermutRight = T1 * P0;
    BCIFS::FormalMatrix rightPermutRight = P0 * T0;

    BCIFS::ConstraintSolver::solve(leftIncLeft, rightIncLeft, pool);

    EXPECT_EQ(T0.value(0, 0), 1.0f);
    EXPECT_EQ(T0.value(1, 0), 0.0f);
    EXPECT_EQ(T0.value(2, 0), 0.0f);

    BCIFS::ConstraintSolver::solve(leftIncRight, rightIncRight, pool);

    EXPECT_EQ(T1.value(0, 2), 0.0f);
    EXPECT_EQ(T1.value(1, 2), 0.0f);
    EXPECT_EQ(T1.value(2, 2), 1.0f);

    BCIFS::ConstraintSolver::solve(leftAdj, rightAdj, pool);

    EXPECT_EQ(T0.value(0, 2), T1.value(0, 0));
    EXPECT_EQ(T0.value(1, 2), T1.value(1, 0));
    EXPECT_EQ(T0.value(2, 2), T1.value(2, 0));

    BCIFS::ConstraintSolver::solve(leftPermutLeft, rightPermutLeft, pool);

    EXPECT_EQ(T0.value(0, 1), T1.value(2, 1));
    EXPECT_EQ(T0.value(1, 1), T1.value(1, 1));
    EXPECT_EQ(T0.value(2, 1), T1.value(0, 1));

    BCIFS::ConstraintSolver::solve(leftPermutRight, rightPermutRight, pool);

    EXPECT_EQ(T0.value(0, 1), T1.value(2, 1));
    EXPECT_EQ(T0.value(1, 1), T1.value(1, 1));
    EXPECT_EQ(T0.value(2, 1), T1.value(0, 1));

    pool.setValue(f.index(), 0.1f);
    pool.setValue(g.index(), 0.1f);

    EXPECT_EQ(T0.value(0, 0), 1.0f);
    EXPECT_EQ(T0.value(1, 0), 0.0f);
    EXPECT_EQ(T0.value(2, 0), 0.0f);
    EXPECT_EQ(T0.value(0, 1), 0.4f);
    EXPECT_EQ(T0.value(1, 1), 0.5f);
    EXPECT_EQ(T0.value(2, 1), 0.1f);
    EXPECT_EQ(T0.value(0, 2), 0.1f);
    EXPECT_EQ(T0.value(1, 2), 0.8f);
    EXPECT_EQ(T0.value(2, 2), 0.1f);

    EXPECT_EQ(T1.value(0, 0), 0.1f);
    EXPECT_EQ(T1.value(1, 0), 0.8f);
    EXPECT_EQ(T1.value(2, 0), 0.1f);
    EXPECT_EQ(T1.value(0, 1), 0.1f);
    EXPECT_EQ(T1.value(1, 1), 0.5f);
    EXPECT_EQ(T1.value(2, 1), 0.4f);
    EXPECT_EQ(T1.value(0, 2), 0.0f);
    EXPECT_EQ(T1.value(1, 2), 0.0f);
    EXPECT_EQ(T1.value(2, 2), 1.0f);
}

TEST(Constraint, ConstraintMatrixDifferentSize) {
    BCIFS::CoefPool pool;
    BCIFS::FormalMatrix T0(3, 3, &pool);
    BCIFS::FormalMatrix T1(3, 4, &pool);
    EXPECT_ANY_THROW(BCIFS::ConstraintSolver::solve(T0, T1, pool));
    BCIFS::FormalMatrix T2(3, 3, &pool);
    BCIFS::FormalMatrix T3(4, 3, &pool);
    EXPECT_ANY_THROW(BCIFS::ConstraintSolver::solve(T2, T3, pool));
    BCIFS::FormalMatrix T4(3, 3, &pool);
    BCIFS::FormalMatrix T5(4, 4, &pool);
    EXPECT_ANY_THROW(BCIFS::ConstraintSolver::solve(T4, T5, pool));
}