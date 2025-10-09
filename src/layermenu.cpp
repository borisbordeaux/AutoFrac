#include "layermenu.h"
#include "imgui/imgui.h"
#include "renderer.h"

LayerMenu::LayerMenu() : m_currentLayer(this) {

}

LayerMenu::~LayerMenu() {
    if (m_currentLayer != this) {
        delete m_currentLayer;
    }
}

void LayerMenu::onUpdate(float deltaTime) {
    if (m_currentLayer != this)
        m_currentLayer->onUpdate(deltaTime);
}

void LayerMenu::onRender() {
    if (m_currentLayer != this) {
        m_currentLayer->onRender();
    } else {
        GLCall(glClearColor(0.59f, 0.73f, 1.0f, 1.0f));
    }
}

void LayerMenu::onImGuiRender() {
    ImGui::Begin("Layer");
    ImGui::Text("Application average %.3f ms/frame (%.0f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    if (m_currentLayer != this) {
        if (ImGui::Button("<-")) {
            delete m_currentLayer;
            m_currentLayer = this;
        }
        m_currentLayer->onImGuiRender();
    } else {
        for (auto& layer: m_layers) {
            if (ImGui::Button(layer.first.c_str())) {
                if (m_currentLayer != this) {
                    delete m_currentLayer;
                }
                m_currentLayer = layer.second();
            }
        }
    }
    ImGui::End();
}
