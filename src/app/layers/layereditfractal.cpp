#include "app/layers/layereditfractal.h"

#include "core/layerevent.h"
#include "imgui/imgui.h"

void LayerEditFractal::onImGuiRender() {
    ImGui::Begin("BC-IFS");
    ImGui::Text("%s", m_currentFace.c_str());
    const char* items[] = { "Cantor", "Bezier" };
    ImGui::Combo("Edge Type", &m_currentEdgeType, items, IM_ARRAYSIZE(items));
    ImGui::SliderInt("Number of subdivisions", &m_currentEdgeNbSubs, 2, 10);
    ImGui::SliderInt("Number of delay", &m_currentEdgeDelay, 0, 10);
    if (ImGui::Button("Add edge")) {
        m_edited = true;
        m_edges.emplace_back(this->buildEdge());
        this->buildFace();
    }
    ImGui::End();
}

void LayerEditFractal::onEvent(Core::Event& event) {
    Core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<Core::LayerSwappedEvent>([this](const Core::LayerSwappedEvent& e) { return this->onLayerSwappedEvent(e); });
}

std::string LayerEditFractal::buildEdge() const {
    std::string res = "";
    if (m_currentEdgeType == 0) {
        res += "C_";
    } else {
        res += "B_";
    }
    res += std::to_string(m_currentEdgeNbSubs);
    res += "_";
    res += std::to_string(m_currentEdgeDelay);
    return res;
}

void LayerEditFractal::buildFace() {
    m_currentFace = "";
    if (m_edges.empty()) { return; }
    bool firstEdge = true;
    for (const std::string& edge : m_edges) {
        if (firstEdge) {
            m_currentFace += edge;
            firstEdge = false;
        } else {
            m_currentFace += " - " + edge;
        }
    }
    m_currentFace += " / C_2_0 - C_2_0 - B_2_0 / ";
    m_currentFace += std::to_string(m_currentFaceDelay);
    m_currentFace += " / ";
    m_currentFace += std::to_string(m_currentFaceAlgo);
}

bool LayerEditFractal::onLayerSwappedEvent(const Core::LayerSwappedEvent& event) {
    if (event.getLayer() == this) {
        m_edited = false;
        return true;
    }
    return false;
}
