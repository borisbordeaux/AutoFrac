#include "app/layers/layermenu.h"

#include <GLFW/glfw3.h>

#include "core/application.h"
#include "core/keyevents.h"
#include "imgui/imgui.h"

void LayerMenu::onImGuiRender() {
    if (m_showDemo) {
        ImGui::ShowDemoWindow(&m_showDemo);
    }
    static int location = 1;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (location >= 0)
    {
        constexpr float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.5f); // Transparent background
    if (ImGui::Begin("Example: Simple overlay", nullptr, window_flags))
    {
        ImGui::Text("Application average %.3f ms/frame (%.0f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Delta time is %.3f seconds, %.3f ms", m_deltaTime, m_deltaTime * 1000.0f);
        ImGui::Checkbox("Show Demo Window", &m_showDemo);
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom", nullptr, location == -1)) location = -1;
            if (ImGui::MenuItem("Top-left", nullptr, location == 0)) location = 0;
            if (ImGui::MenuItem("Top-right", nullptr, location == 1)) location = 1;
            if (ImGui::MenuItem("Bottom-left", nullptr, location == 2)) location = 2;
            if (ImGui::MenuItem("Bottom-right", nullptr, location == 3)) location = 3;
            ImGui::EndPopup();
        }
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
