#include "app/layers/layereditfractal.h"

#include "core/layerevent.h"
#include "imgui/imgui.h"

void LayerEditFractal::onImGuiRender() {
    constexpr float width = 0.6f;
    ImGui::Begin("BC-IFS", nullptr, ImGuiWindowFlags_NoCollapse);
    ImGui::Text("Face:");
    ImGui::Text("%s", m_face.c_str());

    ImGui::Text("Face edges:");
    if (ImGui::BeginListBox("##listbox 1")) {
        for (std::size_t n = 0; n < m_edges.size(); n++) {
            const bool is_selected = (m_currentEdge == n);
            if (ImGui::Selectable((m_edges[n] + "##" + std::to_string(n)).c_str(), is_selected)) {
                m_currentEdge = n;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }

    ImGui::Text("Edge settings:");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    ImGui::Combo("Type", &m_currentEdgeType, "Cantor\0Bezier");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    ImGui::SliderInt("Subdivisions", &m_currentEdgeNbSubs, 2, 10);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    ImGui::SliderInt("Delay", &m_currentEdgeDelay, 0, 10);
    if (ImGui::Button("Add edge", ImVec2(-FLT_MIN,0))) {
        m_edges.emplace_back(this->buildEdge(m_currentEdgeType, m_currentEdgeNbSubs, m_currentEdgeDelay));
        m_currentEdge = m_edges.size() - 1;
        this->buildFace();
    }

    if (ImGui::Button("Default face", ImVec2(-FLT_MIN, 0))) {
        m_edges.clear();
        m_edges.reserve(6);
        for (int i = 0; i < 3; i++) {
            m_edges.emplace_back("C_2_0");
            m_edges.emplace_back("B_2_0");
        }
        m_currentEdge = m_edges.size() - 1;
        this->buildFace();
    }

    ImGui::BeginDisabled(m_edges.empty());
    if (ImGui::Button("Remove selected edge", ImVec2(-FLT_MIN, 0))) {
        m_edges.erase(m_edges.begin() + m_currentEdge);
        if (m_currentEdge == m_edges.size()) {
            m_currentEdge--;
        }
        this->buildFace();
    }
    ImGui::EndDisabled();

    ImGui::SeparatorText("E parameter");
    ImGui::Text("Ea edge:");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (ImGui::Combo("Type##Ea", &m_EaType, "Cantor\0Bezier")) {
        this->buildFace();
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (ImGui::SliderInt("Subdivisions##Ea", &m_EaNbSubs, 2, 10)) {
        this->buildFace();
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (ImGui::SliderInt("Delay##Ea", &m_EaDelay, 0, 10)) {
        this->buildFace();
    }

    ImGui::Text("El edge:");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (ImGui::Combo("Type##El", &m_ElType, "Cantor\0Bezier")) {
        this->buildFace();
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (ImGui::SliderInt("Subdivisions##El", &m_ElNbSubs, 2, 10)) {
        this->buildFace();
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (ImGui::SliderInt("Delay##El", &m_ElDelay, 0, 10)) {
        this->buildFace();
    }

    ImGui::Text("Ec edge:");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (ImGui::Combo("Type##Ec", &m_EcType, "Cantor\0Bezier")) {
        this->buildFace();
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (ImGui::SliderInt("Subdivisions##Ec", &m_EcNbSubs, 2, 10)) {
        this->buildFace();
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (ImGui::SliderInt("Delay##Ec", &m_EcDelay, 0, 10)) {
        this->buildFace();
    }

    ImGui::SeparatorText("Delay and processus");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (ImGui::SliderInt("Face delay", &m_faceDelay, 0, 10)) {
        this->buildFace();
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * width);
    if (ImGui::Combo("Face processus", &m_faceProc, "Surround Delay\0Surround delay and Bézier\0Corners")) {
        this->buildFace();
    }
    if (ImGui::Combo("Cantor type", &m_cantorType, "Linear\0Quadratic\0Cubic")) {
        this->buildFace();
    }
    if (ImGui::Combo("Bezier type", &m_bezierType, "Linear\0Quadratic\0Cubic")) {
        this->buildFace();
    }
    ImGui::End();
}

void LayerEditFractal::onEvent(Core::Event& event) {
    Core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<Core::LayerSwappedEvent>([this](const Core::LayerSwappedEvent& e) { return this->onLayerSwappedEvent(e); });
}

std::string LayerEditFractal::buildEdge(int type, int nbSubs, int delay) {
    std::string res = "";
    if (type == 0) {
        res += "C_";
    } else {
        res += "B_";
    }
    res += std::to_string(nbSubs);
    res += "_";
    res += std::to_string(delay);
    return res;
}

void LayerEditFractal::buildFace() {
    m_face = "";
    if (m_edges.empty()) { return; }
    bool firstEdge = true;
    for (const std::string& edge : m_edges) {
        if (firstEdge) {
            m_face += edge;
            firstEdge = false;
        } else {
            m_face += " - " + edge;
        }
    }
    m_face += " / ";
    m_face += this->buildEdge(m_EaType, m_EaNbSubs, m_EaDelay) + " - ";
    m_face += this->buildEdge(m_ElType, m_ElNbSubs, m_ElDelay) + " - ";
    m_face += this->buildEdge(m_EcType, m_EcNbSubs, m_EcDelay) + " / ";
    m_face += std::to_string(m_faceDelay) + " / ";
    m_face += std::to_string(m_faceProc);

    m_edited = m_face != m_lastFace || m_lastBezierType != m_bezierType || m_lastCantorType != m_cantorType;
}

bool LayerEditFractal::onLayerSwappedEvent(const Core::LayerSwappedEvent& event) {
    if (event.getLayer() == this) {
        m_edited = false;
        m_lastFace = m_face;
        m_lastCantorType = m_cantorType;
        m_lastBezierType = m_bezierType;
        return true;
    }
    return false;
}
