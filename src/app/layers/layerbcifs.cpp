#include "app/layers/layerbcifs.h"
#include "app/bcifs/formalcoef.h"
#include "app/bcifs/formalmatrix.h"
#include "app/bcifs/constraintsolver.h"
#include "app/bcifs/bcifs.h"

#include <glm/detail/type_mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "core/event.h"
#include "core/mouseevents.h"
#include "core/windowevents.h"
#include "core/application.h"
#include "core/renderer.h"

LayerBcifs::LayerBcifs() :
    m_mousePos(0.0f, 0.0f),
    m_camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 8.0f, 0.0051f, 250.0f, glm::radians(90.0f), glm::radians(0.0f)),
    m_proj(glm::perspective(glm::pi<float>() / 4.0f, Core::Application::get().framebufferSize().x / Core::Application::get().framebufferSize().y, 0.005f, 250.0f)) {
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

void LayerBcifs::testSubdQuad() {
    m_bcifs.reset();
    // states
    auto [vert, internalVert] = m_bcifs.addState("V", 1);
    auto [edge, internalEdge] = m_bcifs.addState("A", 1);
    auto [face, internalFace] = m_bcifs.addState("F", 0);
    BCIFS::StateID init = m_bcifs.addInitState();
    // permutations
    BCIFS::TransitionID permut = m_bcifs.addPermutation("0", edge, edge);
    // boundary of states
    BCIFS::TransitionID b0edge = m_bcifs.addBoundary("0", edge, vert);
    BCIFS::TransitionID b1edge = m_bcifs.addBoundary("1", edge, vert);
    BCIFS::TransitionID b0face = m_bcifs.addBoundary("0", face, edge);
    BCIFS::TransitionID b1face = m_bcifs.addBoundary("1", face, edge);
    BCIFS::TransitionID b2face = m_bcifs.addBoundary("2", face, edge);
    BCIFS::TransitionID b3face = m_bcifs.addBoundary("3", face, edge);
    // grid of states
    m_bcifs.addGrid(face, {
        { { b0face, b0edge }, { b0face, internalEdge[0] }, { b0face, b1edge } },
        { { b1face, b0edge }, { b1face, internalEdge[0] }, { b1face, b1edge } },
        { { b2face, b0edge }, { b2face, internalEdge[0] }, { b2face, b1edge } },
        { { b3face, b0edge }, { b3face, internalEdge[0] }, { b3face, b1edge } },
    });

    // space of states
    m_bcifs.setSpace(edge, { b0edge, internalEdge[0], b1edge });
    m_bcifs.setSpace(face, { b0face, b1face, b2face, b3face });
    // subdivision of states
    BCIFS::TransitionID s0vert = m_bcifs.addSubdivision("0", vert, vert);
    BCIFS::TransitionID s0edge = m_bcifs.addSubdivision("0", edge, edge);
    BCIFS::TransitionID s1edge = m_bcifs.addSubdivision("1", edge, edge);
    BCIFS::TransitionID s0face = m_bcifs.addSubdivision("0", face, face);
    BCIFS::TransitionID s1face = m_bcifs.addSubdivision("1", face, face);
    BCIFS::TransitionID s2face = m_bcifs.addSubdivision("2", face, face);
    BCIFS::TransitionID s3face = m_bcifs.addSubdivision("3", face, face);
    BCIFS::TransitionID s0init = m_bcifs.addSubdivision("0", init, face);
    // permutation constraints
    // to define permutation operators
    m_bcifs.addConstraint({ permut, b0edge }, { b1edge });
    m_bcifs.addConstraint({ permut, b1edge }, { b0edge });
    m_bcifs.addConstraint({ permut, internalEdge[0] }, { internalEdge[0] });
    // to constraint subdivision operators using permutation operators
    m_bcifs.addConstraint({ permut, s0edge }, { s1edge, permut });
    m_bcifs.addConstraint({ permut, s1edge }, { s0edge, permut });
    // incidence constraints
    // on edge
    m_bcifs.addConstraint({ b0edge, s0vert }, { s0edge, b0edge });
    m_bcifs.addConstraint({ b1edge, s0vert }, { s1edge, b1edge });
    // on face
    m_bcifs.addConstraint({ b0face, s0edge }, { s0face, b0face });
    m_bcifs.addConstraint({ b0face, s1edge }, { s1face, b0face });
    m_bcifs.addConstraint({ b1face, s0edge }, { s1face, b1face });
    m_bcifs.addConstraint({ b1face, s1edge }, { s2face, b1face });
    m_bcifs.addConstraint({ b2face, s0edge }, { s2face, b2face });
    m_bcifs.addConstraint({ b2face, s1edge }, { s3face, b2face });
    m_bcifs.addConstraint({ b3face, s0edge }, { s3face, b3face });
    m_bcifs.addConstraint({ b3face, s1edge }, { s0face, b3face });
    // adjacency constraints
    // on edge
    m_bcifs.addConstraint({ s0edge, b1edge }, { s1edge, b0edge });
    // on face
    m_bcifs.addConstraint({ s0face, b1face, permut }, { s1face, b3face });
    m_bcifs.addConstraint({ s1face, b2face, permut }, { s2face, b0face });
    m_bcifs.addConstraint({ s2face, b3face, permut }, { s3face, b1face });
    m_bcifs.addConstraint({ s3face, b0face, permut }, { s0face, b2face });
    // on incidence operators
    m_bcifs.addConstraint({ b0face, b1edge }, { b1face, b0edge });
    m_bcifs.addConstraint({ b1face, b1edge }, { b2face, b0edge });
    m_bcifs.addConstraint({ b2face, b1edge }, { b3face, b0edge });
    m_bcifs.addConstraint({ b3face, b1edge }, { b0face, b0edge });

    // init control points
    m_bcifs.setInitMat(s0init, BCIFS::FormalMatrix({
        { -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, -1.0f },
        { -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f },
        { 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f }
    }, BCIFS::CoefType::CONST));

    // define all matrices
    m_bcifs.validate();
}

void LayerBcifs::testSierpinski() {
    m_bcifs.reset();
    // states
    auto [vert, internalVert] = m_bcifs.addState("V", 1);
    auto [edge, internalEdge] = m_bcifs.addState("A", 0);
    auto [face, internalFace] = m_bcifs.addState("F", 0);
    BCIFS::StateID init = m_bcifs.addInitState();
    // boundary of states
    BCIFS::TransitionID b0edge = m_bcifs.addBoundary("0", edge, vert);
    BCIFS::TransitionID b1edge = m_bcifs.addBoundary("1", edge, vert);
    BCIFS::TransitionID b0face = m_bcifs.addBoundary("0", face, edge);
    BCIFS::TransitionID b1face = m_bcifs.addBoundary("1", face, edge);
    BCIFS::TransitionID b2face = m_bcifs.addBoundary("2", face, edge);
    // grid of states
    m_bcifs.addGrid(face, {
        { { b0face, b0edge }, { b0face, b1edge } },
        { { b1face, b0edge }, { b1face, b1edge } },
        { { b2face, b0edge }, { b2face, b1edge } },
    });
    // space of states
    m_bcifs.setSpace(edge, { b0edge, b1edge });
    m_bcifs.setSpace(face, { b0face, b1face, b2face });
    // subdivision of states
    BCIFS::TransitionID s0vert = m_bcifs.addSubdivision("0", vert, vert);
    BCIFS::TransitionID s0edge = m_bcifs.addSubdivision("0", edge, edge);
    BCIFS::TransitionID s1edge = m_bcifs.addSubdivision("1", edge, edge);
    BCIFS::TransitionID s0face = m_bcifs.addSubdivision("0", face, face);
    BCIFS::TransitionID s1face = m_bcifs.addSubdivision("1", face, face);
    BCIFS::TransitionID s2face = m_bcifs.addSubdivision("2", face, face);
    BCIFS::TransitionID s0init = m_bcifs.addSubdivision("0", init, face);
    BCIFS::TransitionID s1init = m_bcifs.addSubdivision("1", init, face);

    // incidence constraints
    // on edge
    m_bcifs.addConstraint({ b0edge, s0vert }, { s0edge, b0edge });
    m_bcifs.addConstraint({ b1edge, s0vert }, { s1edge, b1edge });
    // on face
    m_bcifs.addConstraint({ b0face, s0edge }, { s0face, b0face });
    m_bcifs.addConstraint({ b0face, s1edge }, { s1face, b0face });
    m_bcifs.addConstraint({ b1face, s0edge }, { s1face, b1face });
    m_bcifs.addConstraint({ b1face, s1edge }, { s2face, b1face });
    m_bcifs.addConstraint({ b2face, s0edge }, { s2face, b2face });
    m_bcifs.addConstraint({ b2face, s1edge }, { s0face, b2face });
    // adjacency constraints
    // on edge
    m_bcifs.addConstraint({ s0edge, b1edge }, { s1edge, b0edge });
    // on incidence operators
    m_bcifs.addConstraint({ b0face, b1edge }, { b1face, b0edge });
    m_bcifs.addConstraint({ b1face, b1edge }, { b2face, b0edge });
    m_bcifs.addConstraint({ b2face, b1edge }, { b0face, b0edge });
    // on init state
    m_bcifs.addConstraint({ s0init, b0face }, { s1init, b0face });

    // init control points
    m_bcifs.setInitMat(s0init, BCIFS::FormalMatrix({
        { -1.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.7f },
        { 1.0f, 0.0f, 0.0f }
    }, BCIFS::CoefType::CONST));

    m_bcifs.setInitMat(s1init, BCIFS::FormalMatrix({
        { 1.0f, -1.0f, 0.0f },
        { 0.0f, 0.0f, -1.0f },
        { 0.0f, 1.0f, 2.0f }
    }, BCIFS::CoefType::CONST));

    // define all matrices
    m_bcifs.validate();
}

void LayerBcifs::testG2() {
    m_bcifs.reset();
    // states
    auto [vert, internalVert] = m_bcifs.addState("V", 1);
    auto [bezier, internalBezier] = m_bcifs.addState("B", 1);
    auto [cantor, internalCantor] = m_bcifs.addState("C", 0);
    auto [face, internalFace] = m_bcifs.addState("F", 0);
    BCIFS::StateID init = m_bcifs.addInitState();
    // permutations
    BCIFS::TransitionID permut = m_bcifs.addPermutation("0", cantor, cantor);
    // boundary of states
    BCIFS::TransitionID b0bezier = m_bcifs.addBoundary("0", bezier, vert);
    BCIFS::TransitionID b1bezier = m_bcifs.addBoundary("1", bezier, vert);
    BCIFS::TransitionID b0cantor = m_bcifs.addBoundary("0", cantor, vert);
    BCIFS::TransitionID b1cantor = m_bcifs.addBoundary("1", cantor, vert);
    BCIFS::TransitionID b0face = m_bcifs.addBoundary("0", face, cantor);
    BCIFS::TransitionID b1face = m_bcifs.addBoundary("1", face, bezier);
    BCIFS::TransitionID b2face = m_bcifs.addBoundary("2", face, cantor);
    BCIFS::TransitionID b3face = m_bcifs.addBoundary("3", face, bezier);
    BCIFS::TransitionID b4face = m_bcifs.addBoundary("4", face, cantor);
    BCIFS::TransitionID b5face = m_bcifs.addBoundary("5", face, bezier);
    // grid of states
    m_bcifs.addGrid(face, {
        { { b0face, b0cantor }, { b0face, b1cantor } },
        { { b1face, b0bezier }, { b1face, internalBezier[0] }, { b1face, b1bezier } },
        { { b2face, b0cantor }, { b2face, b1cantor } },
        { { b3face, b0bezier }, { b3face, internalBezier[0] }, { b3face, b1bezier } },
        { { b4face, b0cantor }, { b4face, b1cantor } },
        { { b5face, b0bezier }, { b5face, internalBezier[0] }, { b5face, b1bezier } }
    });

    // space of states
    m_bcifs.setSpace(bezier, { b0bezier, internalBezier[0], b1bezier });
    m_bcifs.setSpace(cantor, { b0cantor, b1cantor });
    m_bcifs.setSpace(face, { b0face, b1face, b2face, b3face, b4face, b5face });
    // subdivision of states
    BCIFS::TransitionID s0vert = m_bcifs.addSubdivision("0", vert, vert);
    BCIFS::TransitionID s0bezier = m_bcifs.addSubdivision("0", bezier, bezier);
    BCIFS::TransitionID s1bezier = m_bcifs.addSubdivision("1", bezier, bezier);
    BCIFS::TransitionID s0cantor = m_bcifs.addSubdivision("0", cantor, cantor);
    BCIFS::TransitionID s1cantor = m_bcifs.addSubdivision("1", cantor, cantor);
    BCIFS::TransitionID s0face = m_bcifs.addSubdivision("0", face, face);
    BCIFS::TransitionID s1face = m_bcifs.addSubdivision("1", face, face);
    BCIFS::TransitionID s2face = m_bcifs.addSubdivision("2", face, face);
    BCIFS::TransitionID s3face = m_bcifs.addSubdivision("3", face, face);
    BCIFS::TransitionID s4face = m_bcifs.addSubdivision("4", face, face);
    BCIFS::TransitionID s5face = m_bcifs.addSubdivision("5", face, face);
    BCIFS::TransitionID s0init = m_bcifs.addSubdivision("0", init, face);
    // permutation constraints
    // to define permutation operators
    m_bcifs.addConstraint({ permut, b0cantor }, { b1cantor });
    m_bcifs.addConstraint({ permut, b1cantor }, { b0cantor });
    // to constrain subdivision operators using permutation operators
    m_bcifs.addConstraint({ permut, s0cantor }, { s1cantor, permut });
    m_bcifs.addConstraint({ permut, s1cantor }, { s0cantor, permut });
    // incidence constraints
    // on edge
    m_bcifs.addConstraint({ b0cantor, s0vert }, { s0cantor, b0cantor });
    m_bcifs.addConstraint({ b1cantor, s0vert }, { s1cantor, b1cantor });
    m_bcifs.addConstraint({ b0bezier, s0vert }, { s0bezier, b0bezier });
    m_bcifs.addConstraint({ b1bezier, s0vert }, { s1bezier, b1bezier });
    // on face
    m_bcifs.addConstraint({ b0face, s0cantor }, { s5face, b0face });
    m_bcifs.addConstraint({ b0face, s1cantor }, { s0face, b0face });
    m_bcifs.addConstraint({ b1face, s0bezier }, { s0face, b1face });
    m_bcifs.addConstraint({ b1face, s1bezier }, { s1face, b1face });
    m_bcifs.addConstraint({ b2face, s0cantor }, { s1face, b2face });
    m_bcifs.addConstraint({ b2face, s1cantor }, { s2face, b2face });
    m_bcifs.addConstraint({ b3face, s0bezier }, { s2face, b3face });
    m_bcifs.addConstraint({ b3face, s1bezier }, { s3face, b3face });
    m_bcifs.addConstraint({ b4face, s0cantor }, { s3face, b4face });
    m_bcifs.addConstraint({ b4face, s1cantor }, { s4face, b4face });
    m_bcifs.addConstraint({ b5face, s0bezier }, { s4face, b5face });
    m_bcifs.addConstraint({ b5face, s1bezier }, { s5face, b5face });
    // adjacency constraints
    // on edge
    m_bcifs.addConstraint({ s0bezier, b1bezier }, { s1bezier, b0bezier });
    // on face
    m_bcifs.addConstraint({ s0face, b2face, permut }, { s1face, b0face });
    m_bcifs.addConstraint({ s1face, b4face, permut }, { s2face, b0face });
    m_bcifs.addConstraint({ s2face, b4face, permut }, { s3face, b2face });
    m_bcifs.addConstraint({ s3face, b0face, permut }, { s4face, b2face });
    m_bcifs.addConstraint({ s4face, b0face, permut }, { s5face, b4face });
    m_bcifs.addConstraint({ s5face, b2face, permut }, { s0face, b4face });
    // on incidence operators
    m_bcifs.addConstraint({ b0face, b1cantor }, { b1face, b0bezier });
    m_bcifs.addConstraint({ b1face, b1bezier }, { b2face, b0cantor });
    m_bcifs.addConstraint({ b2face, b1cantor }, { b3face, b0bezier });
    m_bcifs.addConstraint({ b3face, b1bezier }, { b4face, b0cantor });
    m_bcifs.addConstraint({ b4face, b1cantor }, { b5face, b0bezier });
    m_bcifs.addConstraint({ b5face, b1bezier }, { b0face, b0cantor });

    // init matrices for edges
    m_bcifs.setInitMat(s0cantor, BCIFS::FormalMatrix({
        { 1.0f, 2.0f / 3.0f },
        { 0.0f, 1.0f / 3.0f }
    }, BCIFS::CoefType::CONST));
    m_bcifs.setInitMat(s1cantor, BCIFS::FormalMatrix({
        { 1.0f / 3.0f, 0.0f },
        { 2.0f / 3.0f, 1.0f }
    }, BCIFS::CoefType::CONST));

    m_bcifs.setInitMat(s0bezier, BCIFS::FormalMatrix({
        { 1.0f, 0.5f, 0.25f },
        { 0.0f, 0.5f, 0.50f },
        { 0.0f, 0.0f, 0.25f }
    }, BCIFS::CoefType::CONST));

    m_bcifs.setInitMat(s1bezier, BCIFS::FormalMatrix({
        { 0.25f, 0.0f, 0.0f },
        { 0.50f, 0.5f, 0.0f },
        { 0.25f, 0.5f, 1.0f }
    }, BCIFS::CoefType::CONST));

    // init control points
    m_bcifs.setInitMat(s0init, BCIFS::FormalMatrix({
        {
            std::cos(0.0f * M_PIf * 2.0f / 6.0f),
            std::cos(1.0f * M_PIf * 2.0f / 6.0f),
            0.7f * std::cos(3.0f * M_PIf * 2.0f / 12.0f),
            std::cos(2.0f * M_PIf * 2.0f / 6.0f),
            std::cos(3.0f * M_PIf * 2.0f / 6.0f),
            0.7f * std::cos(7.0f * M_PIf * 2.0f / 12.0f),
            std::cos(4.0f * M_PIf * 2.0f / 6.0f),
            std::cos(5.0f * M_PIf * 2.0f / 6.0f),
            0.7f * std::cos(11.0f * M_PIf * 2.0f / 12.0f)
        },
        {
            std::sin(0.0f * M_PIf * 2.0f / 6.0f),
            std::sin(1.0f * M_PIf * 2.0f / 6.0f),
            0.7f * std::sin(3.0f * M_PIf * 2.0f / 12.0f),
            std::sin(2.0f * M_PIf * 2.0f / 6.0f),
            std::sin(3.0f * M_PIf * 2.0f / 6.0f),
            0.7f * std::sin(7.0f * M_PIf * 2.0f / 12.0f),
            std::sin(4.0f * M_PIf * 2.0f / 6.0f),
            std::sin(5.0f * M_PIf * 2.0f / 6.0f),
            0.7f * std::sin(11.0f * M_PIf * 2.0f / 12.0f)
        },
        { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
    }, BCIFS::CoefType::CONST));

    // define all matrices
    m_bcifs.validate();
}

void LayerBcifs::testSquareSierpinski() {
    m_bcifs.reset();
    // states
    auto [vert, internalVert] = m_bcifs.addState("V", 1);
    auto [edge, internalEdge] = m_bcifs.addState("A", 1);
    auto [face, internalFace] = m_bcifs.addState("F", 0);
    auto [triangle, internalTriangle] = m_bcifs.addState("T", 0);
    BCIFS::StateID init = m_bcifs.addInitState();
    // permutations
    BCIFS::TransitionID permut = m_bcifs.addPermutation("0", edge, edge);
    // boundary of states
    BCIFS::TransitionID b0edge = m_bcifs.addBoundary("0", edge, vert);
    BCIFS::TransitionID b1edge = m_bcifs.addBoundary("1", edge, vert);
    BCIFS::TransitionID b0face = m_bcifs.addBoundary("0", face, edge);
    BCIFS::TransitionID b1face = m_bcifs.addBoundary("1", face, edge);
    BCIFS::TransitionID b2face = m_bcifs.addBoundary("2", face, edge);
    BCIFS::TransitionID b3face = m_bcifs.addBoundary("3", face, edge);
    BCIFS::TransitionID b0triangle = m_bcifs.addBoundary("0", triangle, edge);
    BCIFS::TransitionID b1triangle = m_bcifs.addBoundary("1", triangle, edge);
    BCIFS::TransitionID b2triangle = m_bcifs.addBoundary("2", triangle, edge);
    // grid of states
    m_bcifs.addGrid(face, {
        { { b0face, b0edge }, { b0face, internalEdge[0] }, { b0face, b1edge } },
        { { b1face, b0edge }, { b1face, internalEdge[0] }, { b1face, b1edge } },
        { { b2face, b0edge }, { b2face, internalEdge[0] }, { b2face, b1edge } },
        { { b3face, b0edge }, { b3face, internalEdge[0] }, { b3face, b1edge } },
    });

    m_bcifs.addGrid(triangle, {
        { { b0triangle, b0edge }, { b0triangle, internalEdge[0] }, { b0triangle, b1edge } },
        { { b1triangle, b0edge }, { b1triangle, internalEdge[0] }, { b1triangle, b1edge } },
        { { b2triangle, b0edge }, { b2triangle, internalEdge[0] }, { b2triangle, b1edge } },
    });

    // space of states
    m_bcifs.setSpace(edge, { b0edge, internalEdge[0], b1edge });
    m_bcifs.setSpace(face, { b0face, b1face, b2face, b3face });
    m_bcifs.setSpace(triangle, { b0triangle, b1triangle, b2triangle });
    // subdivision of states
    BCIFS::TransitionID s0vert = m_bcifs.addSubdivision("0", vert, vert);
    BCIFS::TransitionID s0edge = m_bcifs.addSubdivision("0", edge, edge);
    BCIFS::TransitionID s1edge = m_bcifs.addSubdivision("1", edge, edge);
    BCIFS::TransitionID s0face = m_bcifs.addSubdivision("0", face, face);
    BCIFS::TransitionID s1face = m_bcifs.addSubdivision("1", face, face);
    BCIFS::TransitionID s2face = m_bcifs.addSubdivision("2", face, face);
    BCIFS::TransitionID s3face = m_bcifs.addSubdivision("3", face, triangle);
    BCIFS::TransitionID s0triangle = m_bcifs.addSubdivision("0", triangle, triangle);
    BCIFS::TransitionID s1triangle = m_bcifs.addSubdivision("1", triangle, triangle);
    BCIFS::TransitionID s2triangle = m_bcifs.addSubdivision("2", triangle, triangle);
    [[maybe_unused]] BCIFS::TransitionID s0init = m_bcifs.addSubdivision("0", init, face);
    // permutation constraints
    // to define permutation operators
    m_bcifs.addConstraint({ permut, b0edge }, { b1edge });
    m_bcifs.addConstraint({ permut, b1edge }, { b0edge });
    m_bcifs.addConstraint({ permut, internalEdge[0] }, { internalEdge[0] });
    // to constraint subdivision operators using permutation operators
    m_bcifs.addConstraint({ permut, s0edge }, { s1edge, permut });
    m_bcifs.addConstraint({ permut, s1edge }, { s0edge, permut });
    // incidence constraints
    // on edge
    m_bcifs.addConstraint({ b0edge, s0vert }, { s0edge, b0edge });
    m_bcifs.addConstraint({ b1edge, s0vert }, { s1edge, b1edge });
    // on face
    m_bcifs.addConstraint({ b0face, s0edge }, { s0face, b0face });
    m_bcifs.addConstraint({ b0face, s1edge }, { s1face, b0face });
    m_bcifs.addConstraint({ b1face, s0edge }, { s1face, b1face });
    m_bcifs.addConstraint({ b1face, s1edge }, { s2face, b1face });
    m_bcifs.addConstraint({ b2face, s0edge }, { s2face, b2face });
    m_bcifs.addConstraint({ b2face, s1edge }, { s3face, b1triangle });
    m_bcifs.addConstraint({ b3face, s0edge }, { s3face, b2triangle });
    m_bcifs.addConstraint({ b3face, s1edge }, { s0face, b3face });
    // on triangle
    m_bcifs.addConstraint({ b0triangle, s0edge }, { s0triangle, b0triangle });
    m_bcifs.addConstraint({ b0triangle, s1edge }, { s1triangle, b0triangle });
    m_bcifs.addConstraint({ b1triangle, s0edge }, { s1triangle, b1triangle });
    m_bcifs.addConstraint({ b1triangle, s1edge }, { s2triangle, b1triangle });
    m_bcifs.addConstraint({ b2triangle, s0edge }, { s2triangle, b2triangle });
    m_bcifs.addConstraint({ b2triangle, s1edge }, { s0triangle, b2triangle });
    // adjacency constraints
    // on edge
    m_bcifs.addConstraint({ s0edge, b1edge }, { s1edge, b0edge });
    // on face
    m_bcifs.addConstraint({ s0face, b1face, permut }, { s1face, b3face });
    m_bcifs.addConstraint({ s1face, b2face, permut }, { s2face, b0face });
    // on incidence operators
    m_bcifs.addConstraint({ b0face, b1edge }, { b1face, b0edge });
    m_bcifs.addConstraint({ b1face, b1edge }, { b2face, b0edge });
    m_bcifs.addConstraint({ b2face, b1edge }, { b3face, b0edge });
    m_bcifs.addConstraint({ b3face, b1edge }, { b0face, b0edge });

    m_bcifs.addConstraint({ b0triangle, b1edge }, { b1triangle, b0edge });
    m_bcifs.addConstraint({ b1triangle, b1edge }, { b2triangle, b0edge });
    m_bcifs.addConstraint({ b2triangle, b1edge }, { b0triangle, b0edge });

    m_bcifs.setInitMat(s0edge, BCIFS::FormalMatrix({
        { 1.0f, 0.5f, 0.25f },
        { 0.0f, 0.5f, 0.50f },
        { 0.0f, 0.0f, 0.25f }
    }, BCIFS::CoefType::CONST));

    m_bcifs.setInitMat(s1edge, BCIFS::FormalMatrix({
        { 0.25f, 0.0f, 0.0f },
        { 0.50f, 0.5f, 0.0f },
        { 0.25f, 0.5f, 1.0f }
    }, BCIFS::CoefType::CONST));

    // // init control points
    m_bcifs.setInitMat(s0init, BCIFS::FormalMatrix({
        { -1.0f, 0.0f, 1.0f, 0.6f, 1.0f, 0.0f, -1.0f, -1.6f },
        { -1.0f, -0.6f, -1.0f, 0.0f, 1.0f, 1.6f, 1.0f, 0.0f },
        { 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f }
    }, BCIFS::CoefType::CONST));

    // define all matrices
    m_bcifs.validate();
}

void LayerBcifs::onUpdate(float /*deltaTime*/) {
    if (m_updateMSS) {
        if (m_currentIterationMSS <= 0) {
            m_updateMSS = false;
            m_currentIterationMSS = m_nbIterationsMSS;
        } else {
            for (int i = 0; i < 10; i++) {
                m_bcifs.updateMSS();
            }
            m_bcifsChanged = true;
            m_currentIterationMSS -= 10;
        }
    }
    if (m_bcifsChanged) {
        // update data from BCIFS
        m_batchFace.setBcifs(m_bcifs, m_iterationLevel);

        if (m_displayGrid) {
            m_batchGrid.setBcifs(m_bcifs);
            m_batchControlPoint.setBcifs(m_bcifs);
        }

        m_bcifsChanged = false;
    }
}

void LayerBcifs::onRender() {
    if (m_uniformsDirty) {
        m_batchFace.setMVP(m_camera, m_proj);
        m_batchGrid.setMVP(m_camera, m_proj);
        m_batchControlPoint.setMVP(m_camera, m_proj);

        m_uniformsDirty = false;
    }
    m_batchFace.render();

    if (m_displayGrid) {
        m_batchGrid.render();
        m_batchControlPoint.render();
    }
}

void LayerBcifs::onImGuiRender() {
    ImGui::Begin("BC-IFS");
    ImGui::Text("The BC-IFS Window");
    if (ImGui::Button("Test constraints")) {
        this->testConstraints();
    }
    if (ImGui::Button("Create BC-IFS subd quad")) {
        this->testSubdQuad();
        m_bcifsChanged = true;
    }
    if (ImGui::Button("Create BC-IFS Sierpinski")) {
        this->testSierpinski();
        m_bcifsChanged = true;
    }
    if (ImGui::Button("Create BC-IFS G2")) {
        this->testG2();
        m_bcifsChanged = true;
    }
    if (ImGui::Button("Create BC-IFS Square Sierpinski")) {
        this->testSquareSierpinski();
        m_bcifsChanged = true;
    }
    if (ImGui::Button("Print BC-IFS")) {
        m_bcifs.print();
    }
    if (ImGui::Checkbox("Display Grid", &m_displayGrid)) {
        m_bcifsChanged = true;
    }
    ImGui::InputInt("Iteration level", &m_iterationLevel);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
        if (m_iterationLevel < 0)
            m_iterationLevel = 0;
        m_bcifsChanged = true;
        m_bcifs.invalidate();
    }

    ImGui::DragFloat("K", m_bcifs.k(), 0.001f);
    ImGui::DragFloat("Damping", m_bcifs.damping(), 0.01f);

    if (ImGui::InputInt("MSS Max Iterations", &m_nbIterationsMSS)) {
        m_currentIterationMSS = m_nbIterationsMSS;
    }
    ImGui::InputInt("MSS Iterations", &m_currentIterationMSS);
    if (ImGui::Button("Update mss")) {
        m_updateMSS = true;
    }

    if (ImGui::Button("Print mss")) {
        m_bcifs.printMSS();
    }

    if (ImGui::Button("Print shortest paths")) {
        std::unordered_map<BCIFS::StateID, BCIFS::Path> paths = m_bcifs.automaton().shortestPaths(m_bcifs.initState());
        for (const std::pair<const BCIFS::StateID, BCIFS::Path>& path : paths) {
            std::cout << "For state " << path.first << ": ";
            for (BCIFS::TransitionID transitionId : path.second) {
                BCIFS::Transition transition = m_bcifs.automaton().findTransitionByID(transitionId);
                std::cout << transition.toString() << ", ";
            }
        }
        std::cout << std::endl;
    }

    ImGui::Text("Faces: %zu", m_batchFace.nbFaces());
    ImGui::Text("Triangles: %zu", m_batchFace.nbTriangles());

    ImGui::Text("Control points");
    float speed = 0.01f;
    std::vector<BCIFS::FormalMatrix> controlPoints = m_bcifs.controlPoints();
    for (std::size_t j = 0; j < controlPoints.size(); j++) {
        for (std::size_t i = 0; i < controlPoints[j].cols(); i++) {
            ImGui::Text("Control point %zu", i);
            if (ImGui::DragFloat((std::string("x##") + std::to_string(i) + std::to_string(j)).c_str(), controlPoints[j].get(0, i)->valueRef(), speed)) {
                m_bcifsChanged = true;
            }
            if (ImGui::DragFloat((std::string("y##") + std::to_string(i) + std::to_string(j)).c_str(), controlPoints[j].get(1, i)->valueRef(), speed)) {
                m_bcifsChanged = true;
            }
            if (ImGui::DragFloat((std::string("z##") + std::to_string(i) + std::to_string(j)).c_str(), controlPoints[j].get(2, i)->valueRef(), speed)) {
                m_bcifsChanged = true;
            }
        }
    }

    ImGui::End();
}

void LayerBcifs::onEvent(Core::Event& event) {
    Core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e) { return this->onMouseMovedEvent(e); });
    dispatcher.dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) { return this->onMousePressedEvent(e); });
    dispatcher.dispatch<Core::MouseButtonReleasedEvent>([this](Core::MouseButtonReleasedEvent& e) { return this->onMouseReleasedEvent(e); });
    dispatcher.dispatch<Core::MouseScrolledEvent>([this](Core::MouseScrolledEvent& e) { return this->onMouseScrolledEvent(e); });
    dispatcher.dispatch<Core::WindowResizedEvent>([this](Core::WindowResizedEvent& e) { return this->onWindowResizedEvent(e); });
}

bool LayerBcifs::onMousePressedEvent(Core::MouseButtonPressedEvent& event) {
    if (event.getMouseButton() == GLFW_MOUSE_BUTTON_1) {
        m_leftMousePressed = true;
        m_mousePos = Core::Application::get().window()->mousePos();
        return true;
    }
    if (event.getMouseButton() == GLFW_MOUSE_BUTTON_2) {
        m_rightMousePressed = true;
        m_mousePos = Core::Application::get().window()->mousePos();
        return true;
    }
    if (event.getMouseButton() == GLFW_MOUSE_BUTTON_3) {
        m_camera.reset(glm::vec3(0.0f, 0.0f, 0.0f), 8.0f, glm::radians(90.0f), glm::radians(0.0f));
        m_uniformsDirty = true;
        return true;
    }
    return false;
}

bool LayerBcifs::onMouseMovedEvent(Core::MouseMovedEvent& event) {
    //compute rotations
    double dx = event.x() - m_mousePos.x;
    double dy = event.y() - m_mousePos.y;

    m_mousePos.x = event.x();
    m_mousePos.y = event.y();

    if (m_leftMousePressed) {
        glm::vec2 size = Core::Application::get().framebufferSize();
        m_camera.rotateAzimuth(static_cast<float>(dx / size.x * 8.0));
        m_camera.rotatePolar(static_cast<float>(dy / size.y * 4.0));
        m_uniformsDirty = true;
    }

    if (m_rightMousePressed) {
        m_camera.moveHorizontal(static_cast<float>(-dx) / 20.0f);
        m_camera.moveVertical(static_cast<float>(dy) / 20.0f);
        m_uniformsDirty = true;
    }
    return false;
}

bool LayerBcifs::onMouseReleasedEvent(Core::MouseButtonReleasedEvent& event) {
    if (event.getMouseButton() == GLFW_MOUSE_BUTTON_1) {
        m_leftMousePressed = false;
        return true;
    }
    if (event.getMouseButton() == GLFW_MOUSE_BUTTON_2) {
        m_rightMousePressed = false;
        return true;
    }
    return false;
}

bool LayerBcifs::onWindowResizedEvent(Core::WindowResizedEvent& event) {
    m_proj = glm::perspective(glm::pi<float>() / 4.0f, static_cast<float>(event.width()) / static_cast<float>(event.height()), 0.005f, 250.0f);
    m_uniformsDirty = true;
    return false;
}

bool LayerBcifs::onMouseScrolledEvent(Core::MouseScrolledEvent& event) {
    // compute new distance of camera from object
    float val = static_cast<float>(event.yOffset()) / 500.0f;

    if (val > 0.0f) {
        this->m_camera.zoom();
    } else {
        this->m_camera.dezoom();
    }

    m_uniformsDirty = true;
    return true;
}
