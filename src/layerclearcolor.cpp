#include "layerclearcolor.h"
#include "renderer.h"
#include "imgui/imgui.h"

LayerClearColor::LayerClearColor() :
        m_clearColor(0.59f, 0.73f, 1.0f, 1.0f) {}

void LayerClearColor::onRender() {
    GLCall(glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void LayerClearColor::onImGuiRender() {
    ImGui::ColorEdit4("Clear Color", &m_clearColor[0]);
}
