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
    m_vao.bind();
    m_vbo.bind();

    m_layout.pushFloats(3); // position
    m_layout.pushFloats(3); // normal

    m_vao.addBuffer(m_vbo, m_layout);

    m_program.addShaderFromFile(Core::ShaderType::Vertex, "../res/shaders/bcifs/vertexShader.glsl");
    m_program.addShaderFromFile(Core::ShaderType::Fragment, "../res/shaders/bcifs/fragmentShader.glsl");
    m_program.link();

    // unbind the vao *before* the vbo
    m_vao.unbind();
    m_vbo.unbind();
    m_program.unbind();
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
    m_bcifs.addGrid(edge, {{{ b0edge }, { internalEdge[0] }, { b1edge }}});
    m_bcifs.addGrid(face, {
            {{ b0face, b0edge }, { b0face, internalEdge[0] }, { b0face, b1edge }},
            {{ b1face, b0edge }, { b1face, internalEdge[0] }, { b1face, b1edge }},
            {{ b2face, b0edge }, { b2face, internalEdge[0] }, { b2face, b1edge }},
            {{ b3face, b0edge }, { b3face, internalEdge[0] }, { b3face, b1edge }},
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
    BCIFS::TransitionID s1init = m_bcifs.addSubdivision("1", init, face);
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
    // on init state
    m_bcifs.addConstraint({ s0init, b0face, permut }, { s1init, b0face });

    // define all matrices
    m_bcifs.validate();
}

void LayerBcifs::onUpdate(float /*deltaTime*/) {
    if (m_bcifsChanged) {
        // update data from BCIFS
        m_count = 0;
        m_data.clear();
        // get all faces for the current iteration level
        // and add them to the buffer
        std::vector<std::vector<glm::vec3>> faces = m_bcifs.faces(m_iterationLevel);

        std::size_t nbTriangles = LayerBcifs::findNbTriangles(faces);
        std::size_t nbAdds = 3 * nbTriangles;
        m_data.resize(nbAdds * m_floatsPerVertex);

        for (const std::vector<glm::vec3>& face: faces) {
            this->addFace(face);
        }

        m_vbo.bind();
        m_vbo.bufferData(m_data);
        m_vbo.unbind();

        m_bcifsChanged = false;
    }
}

void LayerBcifs::onRender() {
    m_program.bind();
    if (m_uniformsDirty) {
        glm::mat4 view = m_camera.getViewMatrix();
        glm::mat4 mvp = m_proj * view;
        m_program.setUniformMat4f("u_mvp", mvp);
        m_program.setUniform3f("lightPos", m_camera.getEye());
        m_program.setUniform3f("cameraPos", m_camera.getEye());
        m_uniformsDirty = false;
    }
    Core::Renderer::draw(m_vao, m_count / m_floatsPerVertex, m_program);
}

void LayerBcifs::onImGuiRender() {
    ImGui::Begin("BC-IFS");
    ImGui::Text("The BC-IFS Window");
    if (ImGui::Button("Test constraints")) {
        this->testConstraints();
    }
    if (ImGui::Button("Create BC-IFS")) {
        this->testBCIFSAutomaton();
        m_bcifsChanged = true;
    }
    if (ImGui::Button("Print BC-IFS")) {
        m_bcifs.print();
    }

    if (ImGui::InputInt("Iteration level", &m_iterationLevel)) {
        if (m_iterationLevel < 0)
            m_iterationLevel = 0;
        m_bcifsChanged = true;
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

std::size_t LayerBcifs::findNbTriangles(const std::vector<std::vector<glm::vec3>>& faces) {
    std::size_t res = 0;
    for (const std::vector<glm::vec3>& face: faces) {
        res += face.size();
    }
    return res;
}

void LayerBcifs::addFace(const std::vector<glm::vec3>& vertices) {
    glm::vec3 barycenter { 0, 0, 0 };
    for (const glm::vec3& vertex: vertices) {
        barycenter += vertex;
    }
    barycenter /= static_cast<float>(vertices.size());
    for (std::size_t i = 0; i < vertices.size(); i++) {
        this->addTriangle(barycenter, vertices[i], vertices[(i + 1) % vertices.size()]);
    }
}

void LayerBcifs::addTriangle(const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec3& pos3) {
    //compute the normal of the triangleSphere
    glm::vec3 n = glm::normalize(glm::cross(pos2 - pos1, pos3 - pos2));

    //add the vertices to the data
    this->addVertexFace(pos1, n);
    this->addVertexFace(pos2, n);
    this->addVertexFace(pos3, n);
}

void LayerBcifs::addVertexFace(const glm::vec3& v, const glm::vec3& n) {
    // add to the end of the data already added
    float* p = m_data.data() + m_count;
    // the coordinates of the vertex
    *p++ = v.x;
    *p++ = v.y;
    *p++ = v.z;
    // the normal of the vertex
    *p++ = n.x;
    *p++ = n.y;
    *p++ = n.z;
    // we update the amount of data
    m_count += m_floatsPerVertex;
}
