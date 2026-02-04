#ifndef AUTOFRAC_LAYERMENU_H
#define AUTOFRAC_LAYERMENU_H

#include "core/layer.h"

namespace Core {
class KeyReleasedEvent;
}

class LayerMenu : public Core::Layer {
public:
    LayerMenu() = default;
    void onImGuiRender() override;
    void onUpdate(float deltaTime) override;
    void onEvent(Core::Event& event) override;

private:
    bool onKeyReleasedEvent(const Core::KeyReleasedEvent& event);

private:
    float m_deltaTime = 0.0f;
    bool m_showDemo = false;
};

#endif //AUTOFRAC_LAYERMENU_H
