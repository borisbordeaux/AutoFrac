#ifndef AUTOFRAC_LAYERMENU_H
#define AUTOFRAC_LAYERMENU_H

#include "core/layer.h"

class LayerMenu : public Core::Layer {
public:
    LayerMenu() = default;
    void onImGuiRender() override;
    void onUpdate(float deltaTime) override;
private:
    float m_deltaTime = 0.0f;
    bool m_showDemo = false;
};

#endif //AUTOFRAC_LAYERMENU_H
