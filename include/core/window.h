#ifndef AUTOFRAC_WINDOW_H
#define AUTOFRAC_WINDOW_H

#include <string>
#include <functional>
#include <glm/glm.hpp>

class GLFWwindow;

namespace Core {

class Event;

struct WindowSpecification {
    std::string title;
    uint32_t width = 1280;
    uint32_t height = 720;
    uint8_t samples = 0;
    bool isResizable = true;
    bool VSync = true;

    using EventCallbackFn = std::function<void(Event&)>;
    EventCallbackFn eventCallback;
};

class Window {
public:
    explicit Window(WindowSpecification specification = WindowSpecification());
    ~Window();
    void create();
    void destroy();
    void update() const;
    void raiseEvent(Event& event) const;
    glm::vec2 framebufferSize() const;
    glm::vec2 mousePos() const;
    bool shouldClose() const;
    void pollEvents() const;
    float time() const;
    GLFWwindow* handle() const { return m_handle; }

private:
    WindowSpecification m_specification;
    GLFWwindow* m_handle = nullptr;
};

} // Core

#endif //AUTOFRAC_WINDOW_H
