#include "app/layers/layereditfractal.h"

#include "app/fractal/structure.h"
#include "core/application.h"
#include "core/layerevent.h"
#include "core/mouseevents.h"
#include "core/renderer.h"
#include "core/windowevents.h"
#include "imgui/imgui.h"

#include <cmath>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/vec2.hpp>

LayerEditFractal::LayerEditFractal() :
    m_proj(glm::perspective(glm::pi<float>() / 4.0f, Core::Application::get().framebufferSize().x / Core::Application::get().framebufferSize().y, 0.005f, 250.0f))
{}

void LayerEditFractal::onUpdate(float /*deltaTime*/) {
    if (m_uniformsDirty) {
        m_batchFaceScheme.setMVP(*m_camera, m_proj);
        m_batchControlPointScheme.setMVP(*m_camera, m_proj);
        m_batchEdgesScheme.setMVP(*m_camera, m_proj);
        m_batchEdgesScheme.setStructure(m_structure);
        m_uniformsDirty = false;
    }
}

void LayerEditFractal::onImGuiRender() {
    constexpr float width = 0.6f;
    ImGui::Begin("BC-IFS", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav);
    ImGui::SeparatorText("Faces");
    if (ImGui::BeginListBox("##listbox faces", ImVec2(-FLT_MIN, 7 * ImGui::GetTextLineHeightWithSpacing()))) {
        for (std::size_t n = 0; n < m_faces.size(); n++) {
            const bool is_selected = (m_selectedFace == n);
            if (ImGui::Selectable((m_faces[n] + "##" + std::to_string(n)).c_str(), is_selected)) {
                m_selectedFace = n;
                m_selectedEdge = 0;
                m_batchFaceScheme.setSelectedFace(m_selectedFace);
                this->updateBatchFace();
            }
            ImGui::SetItemTooltip("%s", m_faces[n].c_str());
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }

    if (ImGui::BeginTable("table1Faces", 3)) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        if (ImGui::Button("+##face", ImVec2(-FLT_MIN, 0))) {
            if (m_faces.empty()) {
                m_faces.emplace_back("C_2_0 - B_2_0 - C_2_0 - B_2_0 - C_2_0 - B_2_0 / C_2_0 - B_2_0 - B_2_0 / 0 / 1");
            } else {
                m_selectedFace++;
                m_faces.emplace(m_faces.begin() + m_selectedFace, "C_2_0 - B_2_0 - C_2_0 - B_2_0 - C_2_0 - B_2_0 / C_2_0 - B_2_0 - B_2_0 / 0 / 1");
            }
            this->updateStructure();
        }
        ImGui::TableSetColumnIndex(1);
        ImGui::BeginDisabled(m_faces.empty());
        if (ImGui::Button("Duplicate##face", ImVec2(-FLT_MIN, 0))) {
            std::string face = m_faces[m_selectedFace];
            m_selectedFace++;
            m_selectedEdge = 0;
            m_faces.emplace(m_faces.begin() + m_selectedFace, face);
            this->updateStructure();
        }
        ImGui::TableSetColumnIndex(2);
        if (ImGui::Button("-##face", ImVec2(-FLT_MIN, 0))) {
            m_faces.erase(m_faces.begin() + m_selectedFace);
            if (m_selectedFace == m_faces.size()) {
                if (!m_faces.empty()) {
                    m_selectedFace--;
                    m_selectedEdge = 0;
                }
            }
            this->updateStructure();
        }
        ImGui::EndDisabled();
        ImGui::EndTable();
    }

    if (m_faces.empty()) {
        ImGui::End();
        this->updateEdited();
        return;
    }

    ImGui::SeparatorText("Selected face edges");
    if (ImGui::BeginListBox("##listbox edges", ImVec2(-FLT_MIN, 7 * ImGui::GetTextLineHeightWithSpacing()))) {
        const std::vector<frac::Edge>& edges = m_structure.faces()[m_selectedFace].constData();
        for (std::size_t n = 0; n < edges.size(); n++) {
            const bool is_selected = (m_selectedEdge == n);
            if (ImGui::Selectable((edges[n].toString() + "##" + std::to_string(n)).c_str(), is_selected)) {
                m_selectedEdge = n;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }

    if (ImGui::BeginTable("table1Edges", 3)) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        if (ImGui::Button("+##edge", ImVec2(-FLT_MIN, 0))) {
            m_selectedEdge++;
            m_structure.faces()[m_selectedFace].data().emplace(m_structure.faces()[m_selectedFace].data().begin() + m_selectedEdge, frac::Edge(frac::EdgeType::CANTOR, 2, 0));
            m_faces[m_selectedFace] = m_structure.faces()[m_selectedFace].toString();
            this->updateStructure();
        }
        ImGui::TableSetColumnIndex(1);
        if (ImGui::Button("Duplicate##edge", ImVec2(-FLT_MIN, 0))) {
            frac::Edge e = m_structure.faces()[m_selectedFace].data()[m_selectedEdge];
            m_selectedEdge++;
            m_structure.faces()[m_selectedFace].data().emplace(m_structure.faces()[m_selectedFace].data().begin() + m_selectedEdge, e);
            m_faces[m_selectedFace] = m_structure.faces()[m_selectedFace].toString();
            this->updateStructure();
        }
        ImGui::TableSetColumnIndex(2);
        ImGui::BeginDisabled(m_structure.faces()[m_selectedFace].constData().size() == 3);
        if (ImGui::Button("-##edge", ImVec2(-FLT_MIN, 0))) {
            m_structure.faces()[m_selectedFace].data().erase(m_structure.faces()[m_selectedFace].data().begin() + m_selectedEdge);
            if (m_selectedEdge == m_structure.faces()[m_selectedFace].data().size()) {
                m_selectedEdge--;
            }
            m_faces[m_selectedFace] = m_structure.faces()[m_selectedFace].toString();
            this->updateStructure();
        }
        ImGui::EndDisabled();
        ImGui::EndTable();
    }

    frac::Edge& edge = m_structure.faces()[m_selectedFace].data()[m_selectedEdge];
    m_selectedEdgeType = static_cast<int>(edge.edgeType());
    this->displayEdgeSettings("Selected edge:", edge, &m_selectedEdgeType);

    ImGui::SeparatorText("E parameter");

    frac::Edge& Ea = m_structure.faces()[m_selectedFace].adjEdge();
    m_EaType = static_cast<int>(Ea.edgeType());
    this->displayEdgeSettings("Ea edge:", Ea, &m_EaType);

    frac::Edge& El = m_structure.faces()[m_selectedFace].gapEdge();
    m_ElType = static_cast<int>(El.edgeType());
    this->displayEdgeSettings("El edge:", El, &m_ElType);

    frac::Edge& Ec = m_structure.faces()[m_selectedFace].reqEdge();
    m_ElType = static_cast<int>(Ec.edgeType());
    this->displayEdgeSettings("Ec edge:", Ec, &m_EcType);

    ImGui::SeparatorText("Delay and process");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (LayerEditFractal::mySliderUnsignedInt("Face delay", &m_structure.faces()[m_selectedFace].delay())) {
        m_faces[m_selectedFace] = m_structure.faces()[m_selectedFace].toString();
        this->updateStructure(false);
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    const char* procNames[3] = { "Surround Delay", "Surround delay and Bézier", "Corners" };
    if (ImGui::SliderInt("Face process", &m_faceProc, 0, 2, procNames[m_faceProc], ImGuiSliderFlags_NoInput)) {
        m_structure.faces()[m_selectedFace].setAlgo(static_cast<frac::AlgorithmSubdivision>(m_faceProc));
        m_faces[m_selectedFace] = m_structure.faces()[m_selectedFace].toString();
        this->updateStructure(false);
    }
    ImGui::SeparatorText("Geometry");
    const char* typeNames[3] = { "Linear", "Quadratic", "Cubic" };
    if (ImGui::SliderInt("Cantor type", &m_cantorType, 0, 2, typeNames[m_cantorType], ImGuiSliderFlags_NoInput)) {
        this->updateStructure();
    }
    if (ImGui::SliderInt("Bézier type", &m_bezierType, 0, 2, typeNames[m_bezierType], ImGuiSliderFlags_NoInput)) {
        this->updateStructure();
    }
    ImGui::SeparatorText("Colors");
    const char* colorTypeNames[3] = { "Uncolored", "Colored by subdivision", "Colored by states" };
    if (ImGui::SliderInt("Color type", &m_colorType, 0, 2, colorTypeNames[m_colorType], ImGuiSliderFlags_NoInput)) {
        this->updateStructure(false);
    }
    ImGui::End();
    this->updateEdited();
}

void LayerEditFractal::onEvent(Core::Event& event) {
    Core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<Core::LayerSwappedEvent>([this](const Core::LayerSwappedEvent& e) { return this->onLayerSwappedEvent(e); });
    dispatcher.dispatch<Core::MouseButtonPressedEvent>([this](const Core::MouseButtonPressedEvent& e) { return this->onMousePressedEvent(e); });
    dispatcher.dispatch<Core::MouseMovedEvent>([this](const Core::MouseMovedEvent& e) { return this->onMouseMovedEvent(e); });
    dispatcher.dispatch<Core::MouseButtonReleasedEvent>([this](const Core::MouseButtonReleasedEvent& e) { return this->onMouseReleasedEvent(e); });
    dispatcher.dispatch<Core::WindowResizedEvent>([this](const Core::WindowResizedEvent& e) { return this->onWindowResizedEvent(e); });
    dispatcher.dispatch<Core::MouseScrolledEvent>([this](const Core::MouseScrolledEvent& e) { return this->onMouseScrolledEvent(e); });
}

void LayerEditFractal::onRender() {
    Core::GLCall(glDisable(GL_DEPTH_TEST));
    m_batchFaceScheme.render();
    m_batchEdgesScheme.render();
    m_batchControlPointScheme.render();
    Core::GLCall(glEnable(GL_DEPTH_TEST));
}

void LayerEditFractal::setCamera(Core::Camera* camera) {
    m_camera = camera;
}

bool LayerEditFractal::onMousePressedEvent(const Core::MouseButtonPressedEvent& event) {
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
        m_camera->reset(glm::vec3(0.0f, 0.0f, 0.0f), 8.0f, glm::radians(90.0f), glm::radians(0.0f));
        m_uniformsDirty = true;
        return true;
    }
    return false;
}

bool LayerEditFractal::onMouseMovedEvent(const Core::MouseMovedEvent& event) {
    double dx = event.x() - m_mousePos.x;
    double dy = event.y() - m_mousePos.y;

    if (m_leftMousePressed) {
        m_mousePos.x = event.x();
        m_mousePos.y = event.y();
        if (m_currentControlPoint.has_value()) {
            this->handleMoveControlPoint();
        } else {
            // move the camera
            glm::vec2 size = Core::Application::get().framebufferSize();
            m_camera->rotateAzimuth(static_cast<float>(dx / size.x * 8.0));
            m_camera->rotatePolar(static_cast<float>(dy / size.y * 4.0));
            m_uniformsDirty = true;
        }
        return true;
    }

    if (m_rightMousePressed) {
        m_mousePos.x = event.x();
        m_mousePos.y = event.y();
        m_camera->moveHorizontal(static_cast<float>(-dx) / 20.0f);
        m_camera->moveVertical(static_cast<float>(dy) / 20.0f);
        m_uniformsDirty = true;
        return true;
    }

    return false;
}

bool LayerEditFractal::onMouseReleasedEvent(const Core::MouseButtonReleasedEvent& event) {
    if (event.getMouseButton() == GLFW_MOUSE_BUTTON_1) {
        m_leftMousePressed = false;
        m_currentControlPoint = std::nullopt;
        return true;
    }
    if (event.getMouseButton() == GLFW_MOUSE_BUTTON_2) {
        m_rightMousePressed = false;
        return true;
    }
    return false;
}

bool LayerEditFractal::onMouseScrolledEvent(const Core::MouseScrolledEvent& event) {
    float val = static_cast<float>(event.yOffset());

    if (val > 0.0f) {
        this->m_camera->zoom();
    } else {
        this->m_camera->dezoom();
    }

    m_uniformsDirty = true;
    return true;
}

bool LayerEditFractal::onWindowResizedEvent(const Core::WindowResizedEvent& event) {
    m_proj = glm::perspective(glm::pi<float>() / 4.0f, static_cast<float>(event.width()) / static_cast<float>(event.height()), 0.005f, 250.0f);
    m_uniformsDirty = true;
    return false;
}

void LayerEditFractal::handleSelection() {
    glm::vec2 size = Core::Application::get().framebufferSize();

    float x = static_cast<float>(2.0 * m_mousePos.x / size.x - 1.0);
    float y = static_cast<float>(1.0 - (2.0 * m_mousePos.y) / size.y);
    glm::vec4 rayClip(x, y, -1.0f, 1.0f);

    glm::vec4 rayEye = glm::inverse(m_proj) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    glm::vec3 rayDirection = glm::normalize(glm::vec3(glm::inverse(m_camera->getViewMatrix()) * rayEye));

    glm::vec3 rayOrigin = m_camera->getEye();

    float bestT = 1000000.0f;
    float tanHalfFovy = 1.0f / m_proj[1][1];

    // test control points
    for (std::size_t i = 0; i < m_structure.controlPoints().size(); i++) {
        for (std::size_t j = 0; j < m_structure.controlPoints()[i].size(); j++) {
            glm::vec2 point = m_structure.controlPoints()[i][j];
            glm::vec3 pointPos(point.x, point.y, 0.0f);
            glm::vec3 toV = pointPos - rayOrigin;
            float t = glm::dot(toV, rayDirection);
            if (t <= 0.0f) {
                continue;
            }

            glm::vec3 closest = rayOrigin + t * rayDirection;
            float dist2 = glm::dot(pointPos - closest, pointPos - closest);

            float worldPerPixel = (2.0f * t * tanHalfFovy) / size.y;
            float pixelRadius = 8.0f;
            float radius = pixelRadius * worldPerPixel;
            float radius2 = radius * radius;

            if (dist2 > radius2) {
                continue;
            }

            if (t < bestT) {
                bestT = t;
                m_currentControlPoint.emplace(i, j);
                m_selectedFace = i;
                m_selectedEdge = 0;
            }
        }
    }
}

void LayerEditFractal::handleMoveControlPoint() {
    glm::vec3 planeNormal{ 0, 0, 1 };

    glm::vec2 size = Core::Application::get().framebufferSize();
    float x = static_cast<float>(2.0 * m_mousePos.x / size.x - 1.0);
    float y = static_cast<float>(1.0 - (2.0 * m_mousePos.y) / size.y);
    glm::vec4 rayClip(x, y, -1.0f, 1.0f);

    glm::vec4 rayEye = glm::inverse(m_proj) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    glm::vec3 rayDirection = glm::normalize(glm::vec3(glm::inverse(m_camera->getViewMatrix()) * rayEye));

    glm::vec3 rayOrigin = m_camera->getEye();

    float t;
    if (LayerEditFractal::intersectRayPlane(rayOrigin, rayDirection, glm::vec3(0, 0, 0), planeNormal, t)) {
        glm::vec3 newPos = rayOrigin + t * rayDirection;
        m_structure.controlPoints()[m_currentControlPoint->first][m_currentControlPoint->second].x = newPos.x;
        m_structure.controlPoints()[m_currentControlPoint->first][m_currentControlPoint->second].y = newPos.y;
        m_movedControlPoints = true;
    }
    this->updateBatchFace();
    this->updateBatchControlPoints();
    this->updateBatchEdge();
}

bool LayerEditFractal::intersectRayPlane(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& planePoint, const glm::vec3& planeNormal, float& t) {
    float denom = dot(rayDirection, planeNormal);
    if (std::abs(denom) < 1e-6f)
        return false;

    t = dot(planePoint - rayOrigin, planeNormal) / denom;
    return t >= 0.0f;
}

void LayerEditFractal::updateEdited() {
    bool editedFaces = m_lastFaces.size() != m_faces.size();
    if (!editedFaces) {
        for (std::size_t i = 0; i < m_faces.size(); i++) {
            if (m_lastFaces[i] != m_faces[i]) {
                editedFaces = true;
            }
        }
    }
    bool editedBezierType = m_lastBezierType != m_bezierType;
    bool editedCantorType = m_lastCantorType != m_cantorType;
    bool editedColors = m_lastColorType != m_colorType;
    m_edited = editedFaces || editedBezierType || editedCantorType || m_movedControlPoints || editedColors;
}

bool LayerEditFractal::onLayerSwappedEvent(const Core::LayerSwappedEvent& event) {
    if (event.getLayer() == this) {
        m_edited = false;
        m_lastFaces = m_faces; // copy
        m_lastCantorType = m_cantorType;
        m_lastBezierType = m_bezierType;
        m_movedControlPoints = false;
        m_lastColorType = m_colorType;
        m_proj = glm::perspective(glm::pi<float>() / 4.0f, Core::Application::get().framebufferSize().x / Core::Application::get().framebufferSize().y, 0.005f, 250.0f);
        m_uniformsDirty = true;
        return true;
    }
    return false;
}

bool LayerEditFractal::mySliderUnsignedInt(const char* label, unsigned int* value, unsigned int min) {
    int max = 10;
    return ImGui::SliderScalar(label, ImGuiDataType_U32, value, &min, &max, "%lld");
}

void LayerEditFractal::displayEdgeSettings(const std::string& desc, frac::Edge& edge, int* type) {
    ImGui::Text("%s", desc.c_str());
    if (ImGui::BeginTable(("tableEdgeSetting" + desc).c_str(), 3)) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("Subdivisions");
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("Delay");
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::SetNextItemWidth(-FLT_MIN);
        bool needUpdate = false;
        bool needChangeControlPoints = false;
        const char* typeNames[2] = { "Cantor", "Bézier" };
        if (ImGui::SliderInt(("##type" + desc).c_str(), type, 0, 1, typeNames[*type], ImGuiSliderFlags_NoInput)) {
            edge.setEdgeType(static_cast<frac::EdgeType>(*type));
            needUpdate = true;
            needChangeControlPoints = m_bezierType != m_cantorType;
        }
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        if (LayerEditFractal::mySliderUnsignedInt(("##subdivision" + desc).c_str(), &edge.nbSubdivisions(), 2)) {
            needUpdate = true;
        }
        ImGui::TableSetColumnIndex(2);
        ImGui::SetNextItemWidth(-FLT_MIN);
        if (LayerEditFractal::mySliderUnsignedInt(("##delay" + desc).c_str(), &edge.delay())) {
            needUpdate = true;
        }
        if (needUpdate) {
            m_faces[m_selectedFace] = m_structure.faces()[m_selectedFace].toString();
            this->updateStructure(needChangeControlPoints);
        }
        ImGui::EndTable();
    }
}

void LayerEditFractal::updateStructure(bool changeControlPoints) {
    frac::Face::reset();
    std::vector<frac::Face> faces;
    for (const std::string& faceStr : m_faces) {
        faces.push_back(frac::Face::fromStr(faceStr));
    }

    frac::BezierType bezierType = static_cast<frac::BezierType>(m_bezierType);
    frac::CantorType cantorType = static_cast<frac::CantorType>(m_cantorType);
    frac::ColorType colorType = static_cast<frac::ColorType>(m_colorType);

    std::vector<std::vector<glm::vec2>> ctrlPts = m_structure.controlPoints();

    m_structure = frac::Structure{ faces, bezierType, cantorType, colorType };

    if (changeControlPoints) {
        m_structure.fillControlPoints();
    } else {
        m_structure.setControlPoints(std::move(ctrlPts));
    }

    this->updateBatchFace();
    this->updateBatchControlPoints();
    this->updateBatchEdge();
}

void LayerEditFractal::updateBatchFace() {
    m_batchFaceScheme.setSelectedFace(m_selectedFace);
    m_batchFaceScheme.setStructure(m_structure);
}

void LayerEditFractal::updateBatchControlPoints() {
    m_batchControlPointScheme.setControlPoints(m_structure.controlPoints());
}

void LayerEditFractal::updateBatchEdge() {
    m_batchEdgesScheme.setStructure(m_structure);
}
