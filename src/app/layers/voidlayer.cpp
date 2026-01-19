#include "app/layers/voidlayer.h"
#include "core/renderer.h"

void VoidLayer::onRender() {
    Core::Renderer::setClearColor(180.0f/255.0f, 180.0f/255.0f, 1.0f, 1.0f);
    Core::Renderer::clear();
}
