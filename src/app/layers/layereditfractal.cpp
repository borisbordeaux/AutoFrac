#include "app/layers/layereditfractal.h"

#include "imgui/imgui.h"

void LayerEditFractal::onImGuiRender() {
    ImGui::Begin("BC-IFS");
    if (ImGui::Button("Render mode")) {
        this->swapLayer();
    }
    ImGui::Text("%s", m_currentFace.c_str());
    const char* items[] = { "Cantor", "Bezier" };
    ImGui::Combo("Edge Type", &m_currentEdgeType, items, IM_ARRAYSIZE(items));
    ImGui::SliderInt("Number of subdivisions", &m_currentEdgeNbSubs, 2, 10);
    ImGui::SliderInt("Number of delay", &m_currentEdgeDelay, 0, 10);
    if (ImGui::Button("Add edge")) {
        m_edges.emplace_back(this->buildEdge());
        this->buildFace();
    }
    ImGui::End();
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
