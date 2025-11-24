#include "core/layer.h"
#include "core/application.h"

namespace Core {

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