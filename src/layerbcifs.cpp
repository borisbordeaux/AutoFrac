#include <armadillo>
#include "layerbcifs.h"
#include "bcifs/formalcoef.h"
#include "bcifs/formalmatrix.h"
#include "bcifs/constraintsolver.h"
#include "bcifs/bcifs.h"

LayerBcifs::LayerBcifs() {
    LayerBcifs::testConstraints();
    LayerBcifs::testBCIFSAutomaton();
}

void LayerBcifs::testConstraints() {
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

void LayerBcifs::testBCIFSAutomaton() {
    BCIFS::Bcifs bcifs;
    // states
    auto [vert, internalVert] = bcifs.addState("V", 1);
    auto [edge, internalEdge] = bcifs.addState("A", 1);
    auto [face, internalFace] = bcifs.addState("F", 1);
    // permutations
    BCIFS::TransitionID permut = bcifs.addPermutation("0", edge, edge);
    // boundary of states
    BCIFS::TransitionID b0edge = bcifs.addBoundary("0", edge, vert);
    BCIFS::TransitionID b1edge = bcifs.addBoundary("1", edge, vert);
    BCIFS::TransitionID b0face = bcifs.addBoundary("0", face, edge);
    BCIFS::TransitionID b1face = bcifs.addBoundary("1", face, edge);
    BCIFS::TransitionID b2face = bcifs.addBoundary("2", face, edge);
    BCIFS::TransitionID b3face = bcifs.addBoundary("3", face, edge);
    // space of states
    bcifs.setSpace(edge, { b0edge, internalEdge[0], b1edge });
    bcifs.setSpace(face, { b0face, internalFace[0], b1face, b2face, b3face });
    // subdivision of states
    BCIFS::TransitionID s0vert = bcifs.addSubdivision("0", vert, vert);
    BCIFS::TransitionID s0edge = bcifs.addSubdivision("0", edge, edge);
    BCIFS::TransitionID s1edge = bcifs.addSubdivision("1", edge, edge);
    [[maybe_unused]] BCIFS::TransitionID s0face = bcifs.addSubdivision("0", face, face);
    [[maybe_unused]] BCIFS::TransitionID s1face = bcifs.addSubdivision("1", face, face);
    [[maybe_unused]] BCIFS::TransitionID s2face = bcifs.addSubdivision("2", face, face);
    [[maybe_unused]] BCIFS::TransitionID s3face = bcifs.addSubdivision("3", face, face);
    // permutation constraints
    // define permutation operators
    bcifs.addConstraint({ permut, b0edge }, { b1edge });
    bcifs.addConstraint({ permut, b1edge }, { b0edge });
    bcifs.addConstraint({ permut, internalEdge[0] }, { internalEdge[0] });
    // constraint subdivision operators using permutation operators
    bcifs.addConstraint({ permut, s0edge }, { s1edge, permut });
    bcifs.addConstraint({ permut, s1edge }, { s0edge, permut });
    // incidence constraints
    // on edge
    bcifs.addConstraint({ b0edge, s0vert }, { s0edge, b0edge });
    bcifs.addConstraint({ b1edge, s0vert }, { s1edge, b1edge });
    // on face
    bcifs.addConstraint({ b0face, s0edge }, { s0face, b0face });
    bcifs.addConstraint({ b0face, s1edge }, { s1face, b0face });
    bcifs.addConstraint({ b1face, s0edge }, { s1face, b1face });
    bcifs.addConstraint({ b1face, s1edge }, { s2face, b1face });
    bcifs.addConstraint({ b2face, s0edge }, { s2face, b2face });
    bcifs.addConstraint({ b2face, s1edge }, { s3face, b2face });
    bcifs.addConstraint({ b3face, s0edge }, { s3face, b3face });
    bcifs.addConstraint({ b3face, s1edge }, { s0face, b3face });
    // adjacency constraints
    // on edge
    bcifs.addConstraint({ s0edge, b1edge }, { s1edge, b0edge });
    // on face
    bcifs.addConstraint({ s0face, b1face, permut }, { s1face, b3face });
    bcifs.addConstraint({ s1face, b2face, permut }, { s2face, b0face });
    bcifs.addConstraint({ s2face, b3face, permut }, { s3face, b1face });
    bcifs.addConstraint({ s3face, b0face, permut }, { s0face, b2face });
    // on incidence operators
    bcifs.addConstraint({ b0face, b1edge }, { b1face, b0edge });
    bcifs.addConstraint({ b1face, b1edge }, { b2face, b0edge });
    bcifs.addConstraint({ b2face, b1edge }, { b3face, b0edge });
    bcifs.addConstraint({ b3face, b1edge }, { b0face, b0edge });

    bcifs.validate();
    bcifs.print();
}
