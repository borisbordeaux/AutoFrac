#include "app/layers/layermenu.h"
#include "imgui/imgui.h"
#include "app/layers/layerclearcolor.h"
#include "app/layers/layertexture.h"
#include "app/layers/layerbcifs.h"
#include "core/application.h"
#include "app/layers/voidlayer.h"

void LayerMenu::onImGuiRender() {
    if (m_showDemo) {
        ImGui::ShowDemoWindow(&m_showDemo);
    }
    ImGui::Begin("Menu");
    ImGui::Text("Application average %.3f ms/frame (%.0f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Delta time is %.3f seconds, %.3f ms", m_deltaTime, m_deltaTime * 1000.0f);
    ImGui::Checkbox("Show Demo Window", &m_showDemo);
    if (ImGui::Button("Void Layer")) {
        Core::Application::get().getLayer(0)->transitionTo<VoidLayer>();
    }
    if (ImGui::Button("Clear Color")) {
        Core::Application::get().getLayer(0)->transitionTo<LayerClearColor>();
    }
    if (ImGui::Button("Texture")) {
        Core::Application::get().getLayer(0)->transitionTo<LayerTexture>();
    }
    if (ImGui::Button("BC-IFS")) {
        Core::Application::get().getLayer(0)->transitionTo<LayerBcifs>();
    }

    ImGui::End();
}

void LayerMenu::onUpdate(float deltaTime) {
    m_deltaTime = deltaTime;
}
