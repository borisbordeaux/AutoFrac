#include "core/layer.h"
#include "core/application.h"

namespace Core {

void Layer::swapLayer() {
    std::size_t thisLayer = 0;
    auto& layerStack = Application::get().m_layerStack;
    for (std::size_t i = 0; i< layerStack.size(); i++) {
        if (layerStack[i].get() == this) {
            thisLayer = i;
        }
    }
    Application::get().swapWithCacheLayer(thisLayer);
}

void Layer::queueTransition(std::unique_ptr<Layer> toLayer) {
    // TODO: needs to be queued
    auto& layerStack = Application::get().m_layerStack;
    for (auto& layer: layerStack) {
        if (layer.get() == this) {
            layer = std::move(toLayer);
        }
    }
}

} // Core