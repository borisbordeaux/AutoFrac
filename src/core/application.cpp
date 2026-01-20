#include "core/application.h"
#include "core/log.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "core/renderer.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <utility>

namespace Core {

static Application* s_Application = nullptr;

static void GLFWErrorCallback(int error, const char* description) {
    LOG_ERROR("[GLFW Error {}]: {}", error, description);
}

Application::Application(ApplicationSpecification specification)
        : m_specification(std::move(specification)) {
    s_Application = this;

    Log::init();

    glfwSetErrorCallback(GLFWErrorCallback);
    glfwInit();

    // Set window title to app name if empty
    if (m_specification.windowSpec.title.empty())
        m_specification.windowSpec.title = m_specification.name;

    m_specification.windowSpec.eventCallback = [this](Event& event) { this->raiseEvent(event); };

    m_window = std::make_shared<Window>(m_specification.windowSpec);
    m_window->create();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigDockingTransparentPayload = true;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window->handle(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Renderer::initOpenGL();
}

Application::~Application() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    m_window->destroy();
    glfwTerminate();
    s_Application = nullptr;
}

void Application::run() {
    float lastTime = time();
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

    while (m_running) {
        m_window->pollEvents();

        if (m_window->shouldClose()) {
            stop();
            break;
        }

        float currentTime = time();
        float timestep = glm::clamp(currentTime - lastTime, 0.001f, 0.1f);
        lastTime = currentTime;

        for (const std::unique_ptr<Layer>& layer: m_layerStack)
            layer->onUpdate(timestep);

        Renderer::clear();
        for (const std::unique_ptr<Layer>& layer: m_layerStack)
            layer->onRender();

        // ImGui rendering
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar);
        for (const std::unique_ptr<Layer>& layer: m_layerStack)
            layer->onImGuiRender();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_window->update();
    }
}

void Application::stop() {
    m_running = false;
}

void Application::raiseEvent(Event& event) {
    for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it) {
        auto& layer = *it;
        layer->onEvent(event);
        if (event.handled())
            break;
    }
}

glm::vec2 Application::framebufferSize() const {
    return m_window->framebufferSize();
}

Application& Application::get() {
    assert(s_Application);
    return *s_Application;
}

float Application::time() {
    return (float) glfwGetTime();
}

} // Core