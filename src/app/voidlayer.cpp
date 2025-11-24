#include "app/voidlayer.h"
#include "core/renderer.h"

void VoidLayer::onRender() {
    Core::Renderer::setClearColor(0.59f, 0.73f, 1.0f, 1.0f);
    Core::Renderer::clear();
}
