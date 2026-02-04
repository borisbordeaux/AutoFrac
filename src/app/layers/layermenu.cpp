#include "app/layers/layermenu.h"

#include <GLFW/glfw3.h>

#include "core/application.h"
#include "core/keyevents.h"
#include "imgui/imgui.h"

void LayerMenu::onImGuiRender() {
    if (m_showDemo) {
        ImGui::ShowDemoWindow(&m_showDemo);
    }
    ImGui::Begin("Menu");
    ImGui::Text("Application average %.3f ms/frame (%.0f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Delta time is %.3f seconds, %.3f ms", m_deltaTime, m_deltaTime * 1000.0f);
    ImGui::Checkbox("Show Demo Window", &m_showDemo);
    if (ImGui::Button("Render/Edit mode")) {
        Core::Application::get().swapWithCacheLayer(0, 0);
    }
    ImGui::End();
}

void LayerMenu::onUpdate(float deltaTime) {
    m_deltaTime = deltaTime;
}

void LayerMenu::onEvent(Core::Event& event) {
    Core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<Core::KeyReleasedEvent>([this](const Core::KeyReleasedEvent& e) { return this->onKeyReleasedEvent(e); });
}

bool LayerMenu::onKeyReleasedEvent(const Core::KeyReleasedEvent& event) {
    if (event.getKey() == GLFW_KEY_TAB) {
        Core::Application::get().swapWithCacheLayer(0, 0);
        return true;
    }
    return false;
}
