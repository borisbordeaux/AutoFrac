#include "app/layers/layerbcifs.h"

#include <glm/detail/type_mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <GLFW/glfw3.h>

#include "app/bcifs/bcifs.h"
#include "app/bcifs/bcifsbuilder.h"
#include "app/bcifs/formalmatrix.h"
#include "app/fractal/face.h"
#include "app/fractal/structure.h"
#include "app/fractal/structureprinter.h"
#include "app/layers/layereditfractal.h"
#include "core/application.h"
#include "core/event.h"
#include "core/keyevents.h"
#include "core/layerevent.h"
#include "core/log.h"
#include "core/mouseevents.h"
#include "core/renderer.h"
#include "core/windowevents.h"
#include "imgui/imgui.h"
#include "imguifiledialog/ImGuiFileDialog.h"

LayerBcifs::LayerBcifs(const LayerEditFractal* layerEditFractal) :
    m_mousePos(0.0f, 0.0f),
    m_camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 8.0f, 0.0051f, 250.0f, glm::radians(90.0f), glm::radians(0.0f)),
    m_proj(glm::perspective(glm::pi<float>() / 4.0f, Core::Application::get().framebufferSize().x / Core::Application::get().framebufferSize().y, 0.005f, 250.0f)),
    m_layerEditFractal(layerEditFractal) {}

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
    }, BCIFS::CoefKind::VAR, m_bcifs.pool()));

    // define all matrices
    m_bcifs.finalize();
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

    // primitive of states
    m_bcifs.setPrimitive(face, {
        { { b0face, b1edge }, { b1face, b1edge }, { b2face, b1edge } }
    });

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
    }, BCIFS::CoefKind::VAR, m_bcifs.pool()));

    m_bcifs.setInitMat(s1init, BCIFS::FormalMatrix({
        { 1.0f, -1.0f, 0.0f },
        { 0.0f, 0.0f, -1.0f },
        { 0.0f, 1.0f, 2.0f }
    }, BCIFS::CoefKind::VAR, m_bcifs.pool()));

    // define all matrices
    m_bcifs.finalize();
}

void LayerBcifs::testG2(int rows, int cols) {
    m_bcifs.reset();
    // states
    auto [vert, internalVert] = m_bcifs.addState("V", 1);
    auto [bezier, internalBezier] = m_bcifs.addState("B", 1);
    auto [cantor, internalCantor] = m_bcifs.addState("C", 0);
    auto [face, internalFace] = m_bcifs.addState("F", 0);
    BCIFS::StateID init = m_bcifs.addInitState();
    // permutations
    BCIFS::TransitionID permut = m_bcifs.addPermutation("0", cantor, cantor);
    BCIFS::TransitionID permutB = m_bcifs.addPermutation("0", bezier, bezier);
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

    std::vector<BCIFS::TransitionID> initSubs;
    for (int subs = 0; subs < rows * cols; subs++) {
        initSubs.emplace_back(m_bcifs.addSubdivision(std::to_string(subs), init, face));
    }

    // primitive of states
    m_bcifs.setPrimitive(face, {
        {
            { b0face, b0cantor },
            { b1face, b0bezier },
            { b1face, s0bezier, s1bezier, b0bezier },
            { b1face, s1bezier, b0bezier },
            { b1face, s1bezier, s1bezier, b0bezier },
            { b2face, b0cantor },
            { b3face, b0bezier },
            { b3face, s0bezier, s1bezier, b0bezier },
            { b3face, s1bezier, b0bezier },
            { b3face, s1bezier, s1bezier, b0bezier },
            { b4face, b0cantor },
            { b5face, b0bezier },
            { b5face, s0bezier, s1bezier, b0bezier },
            { b5face, s1bezier, b0bezier },
            { b5face, s1bezier, s1bezier, b0bezier },
        }
    });

    // permutation constraints
    // to define permutation operators
    m_bcifs.addConstraint({ permut, b0cantor }, { b1cantor });
    m_bcifs.addConstraint({ permut, b1cantor }, { b0cantor });
    m_bcifs.addConstraint({ permutB, b0bezier }, { b1bezier });
    m_bcifs.addConstraint({ permutB, internalBezier[0] }, { internalBezier[0] });
    m_bcifs.addConstraint({ permutB, b1bezier }, { b0bezier });
    // to constrain subdivision operators using permutation operators
    m_bcifs.addConstraint({ permut, s0cantor }, { s1cantor, permut });
    m_bcifs.addConstraint({ permut, s1cantor }, { s0cantor, permut });
    m_bcifs.addConstraint({ permutB, s0bezier }, { s1bezier, permutB });
    m_bcifs.addConstraint({ permutB, s1bezier }, { s0bezier, permutB });
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
    }, BCIFS::CoefKind::CONST, m_bcifs.pool()));
    m_bcifs.setInitMat(s1cantor, BCIFS::FormalMatrix({
        { 1.0f / 3.0f, 0.0f },
        { 2.0f / 3.0f, 1.0f }
    }, BCIFS::CoefKind::CONST, m_bcifs.pool()));

    m_bcifs.setInitMat(s0bezier, BCIFS::FormalMatrix({
        { 1.0f, 0.5f, 0.25f },
        { 0.0f, 0.5f, 0.50f },
        { 0.0f, 0.0f, 0.25f }
    }, BCIFS::CoefKind::CONST, m_bcifs.pool()));

    m_bcifs.setInitMat(s1bezier, BCIFS::FormalMatrix({
        { 0.25f, 0.0f, 0.0f },
        { 0.50f, 0.5f, 0.0f },
        { 0.25f, 0.5f, 1.0f }
    }, BCIFS::CoefKind::CONST, m_bcifs.pool()));

    // init control points
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            m_bcifs.setInitMat(initSubs[row * cols + col], BCIFS::FormalMatrix({
                {
                    7.0f * col + 3.0f * std::cos(0.0f * M_PIf * 2.0f / 6.0f),
                    7.0f * col + 3.0f * std::cos(1.0f * M_PIf * 2.0f / 6.0f),
                    7.0f * col + 3.0f * 0.7f * std::cos(3.0f * M_PIf * 2.0f / 12.0f),
                    7.0f * col + 3.0f * std::cos(2.0f * M_PIf * 2.0f / 6.0f),
                    7.0f * col + 3.0f * std::cos(3.0f * M_PIf * 2.0f / 6.0f),
                    7.0f * col + 3.0f * 0.7f * std::cos(7.0f * M_PIf * 2.0f / 12.0f),
                    7.0f * col + 3.0f * std::cos(4.0f * M_PIf * 2.0f / 6.0f),
                    7.0f * col + 3.0f * std::cos(5.0f * M_PIf * 2.0f / 6.0f),
                    7.0f * col + 3.0f * 0.7f * std::cos(11.0f * M_PIf * 2.0f / 12.0f)
                },
                {
                    7.0f * row + 3.0f * std::sin(0.0f * M_PIf * 2.0f / 6.0f),
                    7.0f * row + 3.0f * std::sin(1.0f * M_PIf * 2.0f / 6.0f),
                    7.0f * row + 3.0f * 0.7f * std::sin(3.0f * M_PIf * 2.0f / 12.0f),
                    7.0f * row + 3.0f * std::sin(2.0f * M_PIf * 2.0f / 6.0f),
                    7.0f * row + 3.0f * std::sin(3.0f * M_PIf * 2.0f / 6.0f),
                    7.0f * row + 3.0f * 0.7f * std::sin(7.0f * M_PIf * 2.0f / 12.0f),
                    7.0f * row + 3.0f * std::sin(4.0f * M_PIf * 2.0f / 6.0f),
                    7.0f * row + 3.0f * std::sin(5.0f * M_PIf * 2.0f / 6.0f),
                    7.0f * row + 3.0f * 0.7f * std::sin(11.0f * M_PIf * 2.0f / 12.0f)
                },
                { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
            }, BCIFS::CoefKind::VAR, m_bcifs.pool()));
        }
    }

    // define all matrices
    m_bcifs.finalize();
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
    }, BCIFS::CoefKind::CONST, m_bcifs.pool()));

    m_bcifs.setInitMat(s1edge, BCIFS::FormalMatrix({
        { 0.25f, 0.0f, 0.0f },
        { 0.50f, 0.5f, 0.0f },
        { 0.25f, 0.5f, 1.0f }
    }, BCIFS::CoefKind::CONST, m_bcifs.pool()));

    // // init control points
    m_bcifs.setInitMat(s0init, BCIFS::FormalMatrix({
        { -1.0f, 0.0f, 1.0f, 0.6f, 1.0f, 0.0f, -1.0f, -1.6f },
        { -1.0f, -0.6f, -1.0f, 0.0f, 1.0f, 1.6f, 1.0f, 0.0f },
        { 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f }
    }, BCIFS::CoefKind::VAR, m_bcifs.pool()));

    // define all matrices
    m_bcifs.finalize();
}

void LayerBcifs::onUpdate(float /*deltaTime*/) {
    if (m_updateMSSSubdivisionPoints || m_updateMSSControlPoints) {
        if (m_currentIterationMSS <= 0) {
            m_updateMSSSubdivisionPoints = false;
            m_updateMSSControlPoints = false;
            m_currentIterationMSS = m_nbIterationsMSS;
        } else {
            for (int i = 0; i < 10; i++) {
                if (m_updateMSSSubdivisionPoints) {
                    m_bcifs.updateMSS();
                }
                if (m_updateMSSControlPoints) {
                    m_bcifs.updateMSSControlPoints();
                }
            }
            m_bcifsChanged = true;
            m_currentIterationMSS -= 10;
        }
    }
    if (m_bcifsChanged) {
        // update data from BCIFS
        m_bcifs.setColorDepth(static_cast<std::size_t>(m_colorDepth));
        m_batchFace.setBcifs(m_bcifs, m_iterationLevel);
        if (m_displayGrid) {
            m_gridChanged = true;
        }
        m_bcifsChanged = false;
    }
    if (m_gridChanged) {
        m_batchGrid.setBcifs(m_bcifs, static_cast<std::size_t>(m_gridLevel));
        m_batchControlPoint.setBcifs(m_bcifs, static_cast<std::size_t>(m_gridLevel));
        m_batchSubdivisionPoint.setBcifs(m_bcifs, static_cast<std::size_t>(m_gridLevel));
        m_gridChanged = false;
    }
    if (m_clearColorChanged) {
        m_clearColorChanged = false;
        Core::Renderer::setClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    }
}

void LayerBcifs::onRender() {
    if (m_uniformsDirty) {
        m_batchFace.setMVP(m_camera, m_proj);
        m_batchFace.setIlluminationMode(m_illuminationMode);
        m_batchGrid.setMVP(m_camera, m_proj);
        m_batchControlPoint.setMVP(m_camera, m_proj);
        m_batchSubdivisionPoint.setMVP(m_camera, m_proj);

        m_uniformsDirty = false;
    }
    m_batchFace.render();

    if (m_displayGrid) {
        if (m_displayHidden) {
            Core::GLCall(glDisable(GL_DEPTH_TEST));
            m_batchGrid.render();
            m_batchSubdivisionPoint.render();
            m_batchControlPoint.render();
            Core::GLCall(glEnable(GL_DEPTH_TEST));
        } else {
            m_batchGrid.render();
            m_batchControlPoint.render();
            m_batchSubdivisionPoint.render();
        }
    }
}

void LayerBcifs::onImGuiRender() {
    constexpr float width = 0.6f;
    ImGuiViewport* vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowSize(ImVec2(407, 962), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(vp->Pos.x + 2, vp->Pos.y + 116), ImGuiCond_FirstUseEver);
    ImGui::Begin("BC-IFS", nullptr, ImGuiWindowFlags_NoCollapse);
    ImGui::SeparatorText("Default fractals");
    if (ImGui::Button("Quad", ImVec2(-FLT_MIN,0))) {
        this->testSubdQuad();
        m_bcifsChanged = true;
    }
    if (ImGui::Button("Sierpinski", ImVec2(-FLT_MIN,0))) {
        this->testSierpinski();
        m_bcifsChanged = true;
    }
    static int dim[2] = { 1, 1 };
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    ImGui::SliderInt2("Grid size", dim, 1, 20);
    if (ImGui::Button("Hexagon in a grid", ImVec2(-FLT_MIN,0))) {
        this->testG2(dim[0], dim[1]);
        m_bcifsChanged = true;
    }
    if (ImGui::Button("Square Sierpinski", ImVec2(-FLT_MIN,0))) {
        this->testSquareSierpinski();
        m_bcifsChanged = true;
    }
    ImGui::SeparatorText("Interface BC-IFS");
    if (ImGui::Button("Load Lua File...", ImVec2(-FLT_MIN,0))) {
        IGFD::FileDialogConfig config;
        config.path = "../res/scripts";
        config.flags |= ImGuiFileDialogFlags_Modal;
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose a Lua script file", ".lua", config);
    }
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, ImVec2(700, 350))) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            // action if OK
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            this->loadLuaFile(filePathName);
        }
        ImGuiFileDialog::Instance()->Close();
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    ImGui::InputInt("Iteration level", &m_iterationLevel);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
        if (m_iterationLevel < 0)
            m_iterationLevel = 0;
        m_bcifsChanged = true;
        m_bcifs.invalidate();
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    ImGui::InputInt("Color level", &m_colorDepth);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
        if (m_colorDepth < 0)
            m_colorDepth = 0;
        m_bcifsChanged = true;
    }
    if (ImGui::Checkbox("Display Grid", &m_displayGrid)) {
        m_gridChanged = true;
    }
    ImGui::SameLine();
    ImGui::Checkbox("Display hidden", &m_displayHidden);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    ImGui::InputInt("Grid level", &m_gridLevel);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
        if (m_gridLevel < 0)
            m_gridLevel = 0;
        m_gridChanged = true;
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    ImGui::DragFloat("K", m_bcifs.k(), 0.001f);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    ImGui::DragFloat("Damping", m_bcifs.damping(), 0.01f);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (ImGui::InputInt("MSS Max Iterations", &m_nbIterationsMSS)) {
        m_currentIterationMSS = m_nbIterationsMSS;
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    ImGui::InputInt("MSS Iterations", &m_currentIterationMSS);

    if (ImGui::BeginTable("table1", 2))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        if (ImGui::Button("Random control points", ImVec2(-FLT_MIN,0))) {
            for (BCIFS::FormalMatrix& matrix : m_bcifs.controlPoints(0)) {
                matrix.setRandomValues();
            }
            m_bcifs.invalidate(true);
            m_bcifsChanged = true;
        }
        ImGui::TableSetColumnIndex(1);
        if (ImGui::Button("Auto control points", ImVec2(-FLT_MIN,0))) {
            m_updateMSSControlPoints = true;
        }
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        if (ImGui::Button("Auto subdivision points", ImVec2(-FLT_MIN,0))) {
            m_updateMSSSubdivisionPoints = true;
        }
        ImGui::TableSetColumnIndex(1);
        if (ImGui::Button("Auto all points", ImVec2(-FLT_MIN,0))) {
            m_updateMSSSubdivisionPoints = true;
            m_updateMSSControlPoints = true;
        }
        ImGui::EndTable();
    }
    static bool editControlPoints = false;
    ImGui::Checkbox("Edit control points", &editControlPoints);
    if (editControlPoints) {
        std::vector<BCIFS::FormalMatrix> controlPoints = m_bcifs.controlPoints(m_gridLevel);
        for (std::size_t j = 0; j < controlPoints.size(); j++) {
            for (std::size_t i = 0; i < controlPoints[j].cols(); i++) {
                ImGui::Text("Control point %zu", i);
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
                if (ImGui::DragFloat((std::string("x##") + std::to_string(i) + std::to_string(j)).c_str(), controlPoints[j].valueRef(0, i), 0.01f)) {
                    m_bcifsChanged = true;
                    m_bcifs.invalidate(true);
                }
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
                if (ImGui::DragFloat((std::string("y##") + std::to_string(i) + std::to_string(j)).c_str(), controlPoints[j].valueRef(1, i), 0.01f)) {
                    m_bcifsChanged = true;
                    m_bcifs.invalidate(true);
                }
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
                if (ImGui::DragFloat((std::string("z##") + std::to_string(i) + std::to_string(j)).c_str(), controlPoints[j].valueRef(2, i), 0.01f)) {
                    m_bcifsChanged = true;
                    m_bcifs.invalidate(true);
                }
            }
        }
    }
    ImGui::SeparatorText("Settings");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (ImGui::ColorEdit4("Clear Color", &m_clearColor[0])) {
        m_clearColorChanged = true;
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (ImGui::ColorEdit3("Front color", m_bcifs.defaultFrontColor())) {
        m_bcifsChanged = true;
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (ImGui::ColorEdit3("Back color", m_bcifs.defaultBackColor())) {
        m_bcifsChanged = true;
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (ImGui::Combo("Illumination mode", &m_currentIlluminationItem, "PHONG\0FLAT")) {
        m_illuminationMode = static_cast<IlluminationMode>(m_currentIlluminationItem);
        m_uniformsDirty = true;
    }
    ImGui::Checkbox("Cache transforms", m_bcifs.cacheTransforms());
    static bool cullFaces = false;
    ImGui::SameLine();
    if (ImGui::Checkbox("Cull faces", &cullFaces)) {
        if (cullFaces) {
            Core::GLCall(glEnable(GL_CULL_FACE));
        } else {
            Core::GLCall(glDisable(GL_CULL_FACE));
        }
    }
    ImGui::SeparatorText("Debug / Info");
    if (ImGui::Button("Print BC-IFS")) {
        m_bcifs.print();
    }
    ImGui::SameLine();
    if (ImGui::Button("Print Mass Spring Systems")) {
        m_bcifs.printMSS();
    }
    ImGui::Text("Faces: %zu", m_batchFace.nbFaces());
    ImGui::Text("Triangles: %zu", m_batchFace.nbTriangles());
    ImGui::Text("Vertices: %zu", m_batchFace.nbVertices());
    ImGui::Text("Indices: %zu", m_batchFace.nbIndices());
    ImGui::Text("Floats: %zu", m_batchFace.nbFloats());
    ImGui::Text("RAM used: %zu o, %zu ko, %zu Mo, %zu Go", m_batchFace.nbData(), m_batchFace.nbData() / 1000, m_batchFace.nbData() / 1000000, m_batchFace.nbData() / 1000000000);

    ImGui::End();
}

void LayerBcifs::onEvent(Core::Event& event) {
    Core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<Core::MouseMovedEvent>([this](const Core::MouseMovedEvent& e) { return this->onMouseMovedEvent(e); });
    dispatcher.dispatch<Core::MouseButtonPressedEvent>([this](const Core::MouseButtonPressedEvent& e) { return this->onMousePressedEvent(e); });
    dispatcher.dispatch<Core::MouseButtonReleasedEvent>([this](const Core::MouseButtonReleasedEvent& e) { return this->onMouseReleasedEvent(e); });
    dispatcher.dispatch<Core::MouseScrolledEvent>([this](const Core::MouseScrolledEvent& e) { return this->onMouseScrolledEvent(e); });
    dispatcher.dispatch<Core::KeyPressedEvent>([this](const Core::KeyPressedEvent& e) { return this->onKeyPressedEvent(e); });
    dispatcher.dispatch<Core::KeyReleasedEvent>([this](const Core::KeyReleasedEvent& e) { return this->onKeyReleasedEvent(e); });
    dispatcher.dispatch<Core::WindowResizedEvent>([this](const Core::WindowResizedEvent& e) { return this->onWindowResizedEvent(e); });
    dispatcher.dispatch<Core::LayerSwappedEvent>([this](const Core::LayerSwappedEvent& e) { return this->onLayerSwappedEvent(e); });
}

bool LayerBcifs::onMousePressedEvent(const Core::MouseButtonPressedEvent& event) {
    if (event.getMouseButton() == GLFW_MOUSE_BUTTON_1) {
        m_leftMousePressed = true;
        m_mousePos = Core::Application::get().window()->mousePos();
        this->handleSelection();
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

bool LayerBcifs::onMouseMovedEvent(const Core::MouseMovedEvent& event) {
    //compute rotations
    double dx = event.x() - m_mousePos.x;
    double dy = event.y() - m_mousePos.y;

    m_mousePos.x = event.x();
    m_mousePos.y = event.y();

    if (m_leftMousePressed) {
        if (m_currentControlPoint.has_value()) {
            this->handleMoveControlPoint();
        } else if (m_currentSubdivisionPoint.has_value()) {
            this->handleMoveSubdivisionPoint();
        } else {
            // move the camera
            glm::vec2 size = Core::Application::get().framebufferSize();
            m_camera.rotateAzimuth(static_cast<float>(dx / size.x * 8.0));
            m_camera.rotatePolar(static_cast<float>(dy / size.y * 4.0));
            m_uniformsDirty = true;
        }
    }

    if (m_rightMousePressed) {
        m_camera.moveHorizontal(static_cast<float>(-dx) / 20.0f);
        m_camera.moveVertical(static_cast<float>(dy) / 20.0f);
        m_uniformsDirty = true;
    }
    return false;
}

bool LayerBcifs::onMouseReleasedEvent(const Core::MouseButtonReleasedEvent& event) {
    if (event.getMouseButton() == GLFW_MOUSE_BUTTON_1) {
        m_leftMousePressed = false;
        m_currentControlPoint = std::nullopt;
        m_currentSubdivisionPoint = std::nullopt;
        return true;
    }
    if (event.getMouseButton() == GLFW_MOUSE_BUTTON_2) {
        m_rightMousePressed = false;
        return true;
    }
    return false;
}

bool LayerBcifs::onWindowResizedEvent(const Core::WindowResizedEvent& event) {
    m_proj = glm::perspective(glm::pi<float>() / 4.0f, static_cast<float>(event.width()) / static_cast<float>(event.height()), 0.005f, 250.0f);
    m_uniformsDirty = true;
    return false;
}

bool LayerBcifs::onMouseScrolledEvent(const Core::MouseScrolledEvent& event) {
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

bool LayerBcifs::onKeyPressedEvent(const Core::KeyPressedEvent& event) {
    if (event.getKeyName() == 'x') {
        m_xKeyPressed = true;
        return true;
    }
    if (event.getKeyName() == 'y') {
        m_yKeyPressed = true;
        return true;
    }
    if (event.getKeyName() == 'z') {
        m_zKeyPressed = true;
        return true;
    }
    return false;
}

bool LayerBcifs::onKeyReleasedEvent(const Core::KeyReleasedEvent& event) {
    if (event.getKeyName() == 'x') {
        m_xKeyPressed = false;
        return true;
    }
    if (event.getKeyName() == 'y') {
        m_yKeyPressed = false;
        return true;
    }
    if (event.getKeyName() == 'z') {
        m_zKeyPressed = false;
        return true;
    }
    return false;
}

bool LayerBcifs::onLayerSwappedEvent(const Core::LayerSwappedEvent& event) {
    if (event.getLayer() == this && m_layerEditFractal->face() != "" && m_layerEditFractal->edited()) {
        frac::Face::reset();

        std::vector<frac::Face> faces;
        faces.push_back(frac::Face::fromStr(m_layerEditFractal->face()));

        frac::Structure s{ faces, frac::BezierType::Quadratic_Bezier, frac::CantorType::Quadratic_Cantor };

        try {
            frac::StructurePrinter printer(s, false, "");
            this->loadLuaScript(printer.exportStruct());
        } catch (const std::runtime_error& error) {
            Core::LOG_ERROR(error.what());
        }
        return true;
    }
    return false;
}

void LayerBcifs::handleSelection() {
    if (!m_displayGrid) { return; }
    glm::vec2 size = Core::Application::get().framebufferSize();

    float depth;
    glReadPixels(static_cast<int>(m_mousePos.x), static_cast<int>(size.y - m_mousePos.y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

    float x = static_cast<float>(2.0 * m_mousePos.x / size.x - 1.0);
    float y = static_cast<float>(1.0 - (2.0 * m_mousePos.y) / size.y);
    glm::vec4 rayClip(x, y, -1.0f, 1.0f);

    glm::vec4 rayEye = glm::inverse(m_proj) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    glm::vec3 rayDirection = glm::normalize(glm::vec3(glm::inverse(m_camera.getViewMatrix()) * rayEye));

    glm::vec3 rayOrigin = m_camera.getEye();

    float bestT = 1000000.0f;
    float tanHalfFovy = 1.0f / m_proj[1][1];
    float pixelRadius = 8.0f;

    // test control points
    for (const BCIFS::FormalMatrix& points : m_bcifs.controlPoints(m_gridLevel)) {
        for (std::size_t col = 0; col < points.cols(); col++) {
            // be sure it is a selectable control points
            bool selectable = false;
            for (std::size_t row = 0; row < points.rows(); row++) {
                if (points.isVar(row, col)) {
                    selectable = true;
                }
            }
            if (!selectable) {
                continue;
            }
            BCIFS::FormalMatrix point = points.getCol(col);
            glm::vec3 pointPos(point.value(0, 0), point.value(1, 0), point.value(2, 0));
            glm::vec3 toV = pointPos - rayOrigin;
            float t = glm::dot(toV, rayDirection);
            if (t <= 0.0f) {
                continue;
            }

            glm::vec3 closest = rayOrigin + t * rayDirection;
            float dist2 = glm::dot(pointPos - closest, pointPos - closest);

            float worldPerPixel = (2.0f * t * tanHalfFovy) / size.y;
            float radius = pixelRadius * worldPerPixel;
            float radius2 = radius * radius;

            if (dist2 > radius2) {
                continue;
            }

            if (!m_displayHidden) {
                glm::vec4 clip = m_proj * m_camera.getViewMatrix() * glm::vec4(pointPos, 1.0f);
                clip /= clip.w;
                float pointDepth = clip.z * 0.5f + 0.5f;
                if (pointDepth > depth + 1e-4f) {
                    continue;
                }
            }

            if (t < bestT) {
                bestT = t;
                m_currentControlPoint = point;
                m_initialControlPointPosition.x = m_currentControlPoint->value(0, 0);
                m_initialControlPointPosition.y = m_currentControlPoint->value(1, 0);
                m_initialControlPointPosition.z = m_currentControlPoint->value(2, 0);
            }
        }
    }

    // also test subdivision points
    for (const BCIFS::SubdivisionPoint& point : m_bcifs.subdivisionPoints(m_gridLevel).first) {
        glm::vec3 pointPos = point.posR3();
        glm::vec3 toV = pointPos - rayOrigin;
        float t = glm::dot(toV, rayDirection);
        if (t <= 0.0f) {
            continue;
        }

        glm::vec3 closest = rayOrigin + t * rayDirection;
        float dist2 = glm::dot(pointPos - closest, pointPos - closest);

        float worldPerPixel = (2.0f * t * tanHalfFovy) / size.y;
        float radius = pixelRadius * worldPerPixel;
        float radius2 = radius * radius;

        if (dist2 > radius2) {
            continue;
        }

        if (!m_displayHidden) {
            glm::vec4 clip = m_proj * m_camera.getViewMatrix() * glm::vec4(pointPos, 1.0f);
            clip /= clip.w;
            float pointDepth = clip.z * 0.5f + 0.5f;
            if (pointDepth > depth) {
                continue;
            }
        }

        if (t < bestT) {
            bestT = t;
            m_currentSubdivisionPoint = point;
        }
    }
}

void LayerBcifs::handleMoveControlPoint() {
    // move the control point
    float t;
    glm::vec3 planePoint(m_currentControlPoint->value(0, 0), m_currentControlPoint->value(1, 0), m_currentControlPoint->value(2, 0));
    glm::vec3 planeNormal = m_camera.center() - m_camera.getEye();
    if (m_xKeyPressed) {
        planeNormal = glm::vec3(1, 0, 0);
        planePoint = m_initialControlPointPosition;
    } else if (m_yKeyPressed) {
        planeNormal = glm::vec3(0, 1, 0);
        planePoint = m_initialControlPointPosition;
    } else if (m_zKeyPressed) {
        planeNormal = glm::vec3(0, 0, 1);
        planePoint = m_initialControlPointPosition;
    }

    glm::vec2 size = Core::Application::get().framebufferSize();
    float x = static_cast<float>(2.0 * m_mousePos.x / size.x - 1.0);
    float y = static_cast<float>(1.0 - (2.0 * m_mousePos.y) / size.y);
    glm::vec4 rayClip(x, y, -1.0f, 1.0f);

    glm::vec4 rayEye = glm::inverse(m_proj) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    glm::vec3 rayDirection = glm::normalize(glm::vec3(glm::inverse(m_camera.getViewMatrix()) * rayEye));

    glm::vec3 rayOrigin = m_camera.getEye();

    if (intersectRayPlane(rayOrigin, rayDirection, planePoint, planeNormal, t)) {
        glm::vec3 newPos = rayOrigin + t * rayDirection;
        m_currentControlPoint->setValue(0, 0, newPos.x);
        m_currentControlPoint->setValue(1, 0, newPos.y);
        m_currentControlPoint->setValue(2, 0, newPos.z);
    }
    m_bcifs.invalidate(true);
    m_bcifsChanged = true;
}

void LayerBcifs::handleMoveSubdivisionPoint() {
    float t;
    glm::vec3 planePoint = m_currentSubdivisionPoint->posR3();
    glm::vec3 planeNormal = m_camera.center() - m_camera.getEye();

    glm::vec2 size = Core::Application::get().framebufferSize();
    float x = static_cast<float>(2.0 * m_mousePos.x / size.x - 1.0);
    float y = static_cast<float>(1.0 - (2.0 * m_mousePos.y) / size.y);
    glm::vec4 rayClip(x, y, -1.0f, 1.0f);

    glm::vec4 rayEye = glm::inverse(m_proj) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    glm::vec3 rayDirection = glm::normalize(glm::vec3(glm::inverse(m_camera.getViewMatrix()) * rayEye));

    glm::vec3 rayOrigin = m_camera.getEye();

    if (intersectRayPlane(rayOrigin, rayDirection, planePoint, planeNormal, t)) {
        glm::vec3 newPos = rayOrigin + t * rayDirection;
        BCIFS::FormalMatrix Uv = m_currentSubdivisionPoint->posBary().variableEmbeddingMatrix();
        arma::mat q = m_currentSubdivisionPoint->posBary().toMat();
        std::cout << "q  : " << std::endl << q << std::endl;
        arma::mat pv = Uv.toMat();
        std::cout << "pv  : " << std::endl << pv << std::endl;
        arma::mat matTrue = m_currentSubdivisionPoint->T();
        std::cout << "mat True  : " << std::endl << matTrue << std::endl;
        arma::mat mat(matTrue.n_rows + 1, matTrue.n_cols);
        mat.ones();
        for (std::size_t row = 0; row < matTrue.n_rows; row++) {
            for (std::size_t col = 0; col < matTrue.n_cols; col++) {
                mat.at(row, col) = matTrue.at(row, col);
            }
        }
        std::cout << "mat  : " << std::endl << mat << std::endl;
        BCIFS::FormalMatrix mv = m_currentSubdivisionPoint->posBary().variableMatrix();
        std::cout << "mv  : " << std::endl << mv.toString() << std::endl;

        glm::vec3 deltaR3glm = newPos;
        arma::mat p_(4, 1);
        p_.at(0, 0) = deltaR3glm.x;
        p_.at(1, 0) = deltaR3glm.y;
        p_.at(2, 0) = deltaR3glm.z;
        p_.at(3, 0) = 1.0f;
        arma::mat dp = p_;
        std::cout << "dp : " << std::endl << dp << std::endl;
        arma::mat m2 = dp;
        std::cout << "m2 : " << std::endl << m2 << std::endl;
        arma::mat m3 = mat * pv;
        std::cout << "m3 : " << std::endl << m3 << std::endl;
        arma::mat m4 = arma::pinv(m3, 0.01);
        std::cout << "m4 : " << std::endl << m4 << std::endl;
        arma::mat m6 = m4 * m2;
        std::cout << "m6 : " << std::endl << m6 << std::endl;
        for (std::size_t row = 0; row < mv.rows(); row++) {
            mv.setValue(row, 0, m6.at(row, 0));
        }
        arma::mat coord = mat * m_currentSubdivisionPoint->posBary().toMat();
        float r = 1.0f / coord.at(3, 0);
        arma::mat m7 = m6 * r;
        std::cout << "m7 : " << std::endl << m7 << std::endl;
        for (std::size_t row = 0; row < mv.rows(); row++) {
            mv.setValue(row, 0, m7.at(row, 0));
        }
    }

    m_bcifs.invalidate();
    m_bcifsChanged = true;
}

bool LayerBcifs::intersectRayPlane(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& planePoint, const glm::vec3& planeNormal, float& t) {
    float denom = dot(rayDirection, planeNormal);
    if (std::abs(denom) < 1e-6f)
        return false;

    t = dot(planePoint - rayOrigin, planeNormal) / denom;
    return t >= 0.0f;
}

void LayerBcifs::loadLuaFile(const std::string& filename) {
    bool ok = true;
    try {
        Core::LOG_DEBUG("Begin loading script...");
        BCIFS::BcifsBuilder bcifsBuilder(m_bcifs);
        bcifsBuilder.loadFile(filename);
        Core::LOG_DEBUG("Script loaded.");
    } catch (const std::exception& e) {
        Core::LOG_ERROR(e.what());
        m_bcifs.reset();
        ok = false;
    }
    if (ok) {
        Core::LOG_DEBUG("Checking description...");
        try {
            m_bcifs.check();
            m_bcifs.finalize();
            Core::LOG_DEBUG("Description is valid.");
        } catch (const std::exception& e) {
            Core::LOG_ERROR(e.what());
            m_bcifs.reset();
        }
    }
    m_bcifsChanged = true;
}

void LayerBcifs::loadLuaScript(const std::string& luaScript) {
    bool ok = true;
    try {
        Core::LOG_DEBUG("Begin loading script...");
        BCIFS::BcifsBuilder bcifsBuilder(m_bcifs);
        bcifsBuilder.loadScript(luaScript);
        Core::LOG_DEBUG("Script loaded.");
    } catch (const std::exception& e) {
        Core::LOG_ERROR(e.what());
        m_bcifs.reset();
        ok = false;
    }
    if (ok) {
        Core::LOG_DEBUG("Checking description...");
        try {
            m_bcifs.check();
            m_bcifs.finalize();
            Core::LOG_DEBUG("Description is valid.");
        } catch (const std::exception& e) {
            Core::LOG_ERROR(e.what());
            m_bcifs.reset();
        }
    }
    m_bcifsChanged = true;
}
