#include "app/layers/layereditfractal.h"

#include "core/layerevent.h"
#include "imgui/imgui.h"

void LayerEditFractal::onImGuiRender() {
    constexpr float width = 0.6f;
    ImGui::Begin("BC-IFS", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav);
    ImGui::SeparatorText("Faces");
    if (ImGui::BeginListBox("##listbox faces", ImVec2(-FLT_MIN, 7 * ImGui::GetTextLineHeightWithSpacing()))) {
        for (std::size_t n = 0; n < m_faces.size(); n++) {
            const bool is_selected = (m_selectedFace == n);
            if (ImGui::Selectable((m_faces[n] + "##" + std::to_string(n)).c_str(), is_selected)) {
                m_selectedFace = n;
                m_currentFace = frac::Face::fromStr(m_faces[m_selectedFace]);
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
            m_currentFace = frac::Face::fromStr(m_faces[m_selectedFace]);
        }
        ImGui::TableSetColumnIndex(1);
        ImGui::BeginDisabled(m_faces.empty());
        if (ImGui::Button("Duplicate##face", ImVec2(-FLT_MIN, 0))) {
            std::string face = m_faces[m_selectedFace];
            m_selectedFace++;
            m_faces.emplace(m_faces.begin() + m_selectedFace, face);
            m_currentFace = frac::Face::fromStr(m_faces[m_selectedFace]);
        }
        ImGui::TableSetColumnIndex(2);
        if (ImGui::Button("-##face", ImVec2(-FLT_MIN, 0))) {
            m_faces.erase(m_faces.begin() + m_selectedFace);
            if (m_selectedFace == m_faces.size()) {
                if (!m_faces.empty()) {
                    m_selectedFace--;
                    m_currentFace = frac::Face::fromStr(m_faces[m_selectedFace]);
                }
            }
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
        const std::vector<frac::Edge>& edges = m_currentFace.constData();
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
            m_currentFace.data().emplace(m_currentFace.data().begin() + m_selectedEdge, frac::Edge(frac::EdgeType::CANTOR, 2, 0));
            m_faces[m_selectedFace] = m_currentFace.toString();
        }
        ImGui::TableSetColumnIndex(1);
        if (ImGui::Button("Duplicate##edge", ImVec2(-FLT_MIN, 0))) {
            frac::Edge e = m_currentFace.data()[m_selectedEdge];
            m_selectedEdge++;
            m_currentFace.data().emplace(m_currentFace.data().begin() + m_selectedEdge, e);
            m_faces[m_selectedFace] = m_currentFace.toString();
        }
        ImGui::TableSetColumnIndex(2);
        ImGui::BeginDisabled(m_currentFace.constData().size() == 3);
        if (ImGui::Button("-##edge", ImVec2(-FLT_MIN, 0))) {
            m_currentFace.data().erase(m_currentFace.data().begin() + m_selectedEdge);
            if (m_selectedEdge == m_currentFace.data().size()) {
                m_selectedEdge--;
            }
            m_faces[m_selectedFace] = m_currentFace.toString();
        }
        ImGui::EndDisabled();
        ImGui::EndTable();
    }

    frac::Edge& edge = m_currentFace.data()[m_selectedEdge];
    m_selectedEdgeType = static_cast<int>(edge.edgeType());
    this->displayEdgeSettings("Selected edge:", edge, &m_selectedEdgeType);

    ImGui::SeparatorText("E parameter");

    frac::Edge& Ea = m_currentFace.adjEdge();
    m_EaType = static_cast<int>(Ea.edgeType());
    this->displayEdgeSettings("Ea edge:", Ea, &m_EaType);

    frac::Edge& El = m_currentFace.gapEdge();
    m_ElType = static_cast<int>(El.edgeType());
    this->displayEdgeSettings("El edge:", El, &m_ElType);

    frac::Edge& Ec = m_currentFace.reqEdge();
    m_ElType = static_cast<int>(Ec.edgeType());
    this->displayEdgeSettings("Ec edge:", Ec, &m_EcType);

    ImGui::SeparatorText("Delay and process");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (LayerEditFractal::mySliderUnsignedInt("Face delay", &m_currentFace.delay())) {
        m_faces[m_selectedFace] = m_currentFace.toString();
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    const char* procNames[3] = { "Surround Delay", "Surround delay and Bézier", "Corners" };
    if (ImGui::SliderInt("Face process", &m_faceProc, 0, 2, procNames[m_faceProc], ImGuiSliderFlags_NoInput)) {
        m_currentFace.setAlgo(static_cast<frac::AlgorithmSubdivision>(m_faceProc));
        m_faces[m_selectedFace] = m_currentFace.toString();
    }
    ImGui::SeparatorText("Geometry");
    const char* typeNames[3] = { "Linear", "Quadratic", "Cubic" };
    ImGui::SliderInt("Cantor type", &m_cantorType, 0, 2, typeNames[m_cantorType], ImGuiSliderFlags_NoInput);
    ImGui::SliderInt("Bézier type", &m_bezierType, 0, 2, typeNames[m_bezierType], ImGuiSliderFlags_NoInput);
    ImGui::End();
    this->updateEdited();
}

void LayerEditFractal::onEvent(Core::Event& event) {
    Core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<Core::LayerSwappedEvent>([this](const Core::LayerSwappedEvent& e) { return this->onLayerSwappedEvent(e); });
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
    m_edited = editedFaces || editedBezierType || editedCantorType;
}

bool LayerEditFractal::onLayerSwappedEvent(const Core::LayerSwappedEvent& event) {
    if (event.getLayer() == this) {
        m_edited = false;
        m_lastFaces = m_faces; // copy
        m_lastCantorType = m_cantorType;
        m_lastBezierType = m_bezierType;
        frac::Face::reset();
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
        const char* typeNames[2] = { "Cantor", "Bézier" };
        if (ImGui::SliderInt(("##type" + desc).c_str(), type, 0, 1, typeNames[*type], ImGuiSliderFlags_NoInput)) {
            edge.setEdgeType(static_cast<frac::EdgeType>(*type));
            m_faces[m_selectedFace] = m_currentFace.toString();
        }
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        if (LayerEditFractal::mySliderUnsignedInt(("##subdivision" + desc).c_str(), &edge.nbSubdivisions(), 2)) {
            m_faces[m_selectedFace] = m_currentFace.toString();
        }
        ImGui::TableSetColumnIndex(2);
        ImGui::SetNextItemWidth(-FLT_MIN);
        if (LayerEditFractal::mySliderUnsignedInt(("##delay" + desc).c_str(), &edge.delay())) {
            m_faces[m_selectedFace] = m_currentFace.toString();
        }
        ImGui::EndTable();
    }
}
