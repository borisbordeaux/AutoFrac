#include "core/window.h"
#include "core/keyevents.h"
#include "core/mouseevents.h"
#include "core/log.h"
#include "core/windowevents.h"
#include "imgui/imgui.h"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <utility>

namespace Core {

Window::Window(WindowSpecification specification) : m_specification(std::move(specification)) {}

Window::~Window() {
    this->destroy();
}

void Window::pollEvents() const {
    glfwPollEvents();
}

void Window::create() {
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, m_specification.samples);

    glfwWindowHint(GLFW_RESIZABLE, m_specification.isResizable ? GLFW_TRUE : GLFW_FALSE);

    m_handle = glfwCreateWindow(static_cast<int>(m_specification.width),
                                static_cast<int>(m_specification.height),
                                m_specification.title.c_str(), nullptr, nullptr);

    if (!m_handle) {
        LOG_ERROR("Failed to create GLFW window!");
        assert(false);
    }

    glfwMakeContextCurrent(m_handle);
    glfwSwapInterval(m_specification.VSync ? 1 : 0);

    glfwSetWindowUserPointer(m_handle, this);

    glfwSetWindowCloseCallback(m_handle, [](GLFWwindow* handle) {
        Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(handle));
        WindowClosedEvent event;
        window.raiseEvent(event);
    });

    glfwSetWindowSizeCallback(m_handle, [](GLFWwindow* handle, int width, int height) {
        Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(handle));
        glViewport(0, 0, width, height);
        WindowResizedEvent event(width, height);
        window.raiseEvent(event);
    });

    glfwSetKeyCallback(m_handle, [](GLFWwindow* handle, int key, int scancode, int action, int /*mods*/) {
        if (ImGui::GetIO().WantCaptureKeyboard) return;
        Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(handle));
        switch (action) {
            case GLFW_PRESS:
            case GLFW_REPEAT: {
                KeyPressedEvent event(key, scancode, glfwGetKeyName(key, scancode), action == GLFW_REPEAT);
                window.raiseEvent(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent event(key, scancode, glfwGetKeyName(key, scancode));
                window.raiseEvent(event);
                break;
            }
            default:
                break;
        }
    });

    glfwSetMouseButtonCallback(m_handle, [](GLFWwindow* handle, int button, int action, int /*mods*/) {
        if (ImGui::GetIO().WantCaptureMouse) return;
        Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(handle));
        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(button);
                window.raiseEvent(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(button);
                window.raiseEvent(event);
                break;
            }
            default:
                break;
        }
    });

    glfwSetScrollCallback(m_handle, [](GLFWwindow* handle, double xOffset, double yOffset) {
        if (ImGui::GetIO().WantCaptureMouse) return;
        Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(handle));
        MouseScrolledEvent event(xOffset, yOffset);
        window.raiseEvent(event);
    });

    glfwSetCursorPosCallback(m_handle, [](GLFWwindow* handle, double x, double y) {
        if (ImGui::GetIO().WantCaptureMouse) return;
        Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(handle));
        MouseMovedEvent event(x, y);
        window.raiseEvent(event);
    });
}

void Window::destroy() {
    if (m_handle)
        glfwDestroyWindow(m_handle);

    m_handle = nullptr;
}

void Window::update() const {
    glfwSwapBuffers(m_handle);
}

void Window::raiseEvent(Event& event) const {
    if (m_specification.eventCallback)
        m_specification.eventCallback(event);
}

glm::vec2 Window::framebufferSize() const {
    int width, height;
    glfwGetFramebufferSize(m_handle, &width, &height);
    return { width, height };
}

glm::vec2 Window::mousePos() const {
    double x, y;
    glfwGetCursorPos(m_handle, &x, &y);
    return { static_cast<float>(x), static_cast<float>(y) };
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_handle) != 0;
}

float Window::time() const {
    return static_cast<float>(glfwGetTime());
}

} // Core