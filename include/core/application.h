#ifndef AUTOFRAC_APPLICATION_H
#define AUTOFRAC_APPLICATION_H

#include <memory>
#include <string>

#include "core/layer.h"
#include "core/window.h"

namespace Core {

struct ApplicationSpecification {
    std::string name = "Application";
    std::string initLayout = "imgui.ini";
    WindowSpecification windowSpec;
};

class Application {
public:
    explicit Application(ApplicationSpecification specification = ApplicationSpecification());
    ~Application();
    void run();
    void stop();
    void raiseEvent(Event& event);

    template <typename TLayer, typename... Args>
    void pushLayer(Args... args) {
        m_layerStack.push_back(std::make_unique<TLayer>(std::forward<Args>(args)...));
    }

    template <typename TLayer, typename... Args>
    void pushCacheLayer(Args... args) {
        m_cacheLayerStack.push_back(std::make_unique<TLayer>(std::forward<Args>(args)...));
    }

    void swapWithCacheLayer(std::size_t indexLayer, std::size_t indexCacheLayer);
    Layer* getLayer(std::size_t index) const;
    Layer* getCacheLayer(std::size_t index) const;
    glm::vec2 framebufferSize() const;
    glm::vec2 inverseFramebufferSize() const;
    std::shared_ptr<Window> window() const { return m_window; }
    static Application& get();
    static float time();

private:
    ApplicationSpecification m_specification;
    std::shared_ptr<Window> m_window;
    bool m_running = true;
    std::vector<std::unique_ptr<Layer>> m_layerStack;
    std::vector<std::unique_ptr<Layer>> m_cacheLayerStack;
};

} // Core

#endif //AUTOFRAC_APPLICATION_H
