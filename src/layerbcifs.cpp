#include <armadillo>
#include "layerbcifs.h"
#include "bcifs/formalcoef.h"
#include "bcifs/formalmatrix.h"
#include "bcifs/constraintsolver.h"

LayerBcifs::LayerBcifs() {
    BCIFS::FormalCoefRef a = BCIFS::FormalCoef::var(0.1f);
    BCIFS::FormalCoefRef b = BCIFS::FormalCoef::var(0.2f);
    BCIFS::FormalCoefRef c = BCIFS::FormalCoef::var(0.3f);
    BCIFS::FormalCoefRef d = BCIFS::FormalCoef::var(0.4f);
    BCIFS::FormalCoefRef e = BCIFS::FormalCoef::var(0.5f);
    BCIFS::FormalCoefRef f = BCIFS::FormalCoef::var(0.6f);
    BCIFS::FormalCoefRef g = BCIFS::FormalCoef::var(0.7f);
    BCIFS::FormalCoefRef h = BCIFS::FormalCoef::var(0.8f);
    BCIFS::FormalCoefRef i = BCIFS::FormalCoef::var(0.9f);

    BCIFS::FormalCoefRef j = BCIFS::FormalCoef::var(1.1f);
    BCIFS::FormalCoefRef k = BCIFS::FormalCoef::var(1.2f);
    BCIFS::FormalCoefRef l = BCIFS::FormalCoef::var(1.3f);
    BCIFS::FormalCoefRef m = BCIFS::FormalCoef::var(1.4f);
    BCIFS::FormalCoefRef n = BCIFS::FormalCoef::var(1.5f);
    BCIFS::FormalCoefRef o = BCIFS::FormalCoef::var(1.6f);
    BCIFS::FormalCoefRef p = BCIFS::FormalCoef::var(1.7f);
    BCIFS::FormalCoefRef q = BCIFS::FormalCoef::var(1.8f);
    BCIFS::FormalCoefRef r = BCIFS::FormalCoef::var(1.9f);

    BCIFS::FormalMatrix T0(3, 3);
    BCIFS::FormalMatrix T1(3, 3);
    BCIFS::FormalMatrix BL(3, 1);
    BCIFS::FormalMatrix BR(3, 1);
    BCIFS::FormalMatrix TS(1, 1);
    BCIFS::FormalMatrix P0(3, 3);

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

    T0.print();
    T1.print();

    BCIFS::ConstraintSolver::solve(leftIncLeft, rightIncLeft);

    T0.print();
    T1.print();

    BCIFS::ConstraintSolver::solve(leftIncRight, rightIncRight);

    T0.print();
    T1.print();

    BCIFS::ConstraintSolver::solve(leftAdj, rightAdj);

    T0.print();
    T1.print();

    BCIFS::ConstraintSolver::solve(leftPermutLeft, rightPermutLeft);

    T0.print();
    T1.print();

    BCIFS::ConstraintSolver::solve(leftPermutRight, rightPermutRight);

    T0.print();
    T1.print();

    f->setValue(0.1f);
    g->setValue(0.1f);

    T0.print();
    T1.print();

    T0.print(true);
    T1.print(true);
}
