#ifndef AUTOFRAC_APPLICATION_H
#define AUTOFRAC_APPLICATION_H

#include <string>
#include <memory>
#include "core/window.h"
#include "core/layer.h"

namespace Core {

struct ApplicationSpecification {
    std::string name = "Application";
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

    template <typename TLayer>
    void pushCacheLayer() {
        m_cacheLayer = std::make_unique<TLayer>();
    }

    Layer* cacheLayer() const { return m_cacheLayer.get(); }

    template <typename TLayer>
    TLayer* getLayer() {
        for (const auto& layer : m_layerStack) {
            if (auto casted = dynamic_cast<TLayer*>(layer.get()))
                return casted;
        }
        return nullptr;
    }

    Layer* getLayer(std::size_t index) {
        if (index < m_layerStack.size())
            return m_layerStack[index].get();
        return nullptr;
    }

    void swapWithCacheLayer(std::size_t index) {
        if (index < m_layerStack.size() && m_cacheLayer) {
            std::swap(m_layerStack[index], m_cacheLayer);
        }
    }

    glm::vec2 framebufferSize() const;

    std::shared_ptr<Window> window() const { return m_window; }

    static Application& get();
    static float time();

private:
    ApplicationSpecification m_specification;
    std::shared_ptr<Window> m_window;
    bool m_running = true;
    std::vector<std::unique_ptr<Layer>> m_layerStack;
    std::unique_ptr<Layer> m_cacheLayer;

    friend class Layer;
};

} // Core

#endif //AUTOFRAC_APPLICATION_H
