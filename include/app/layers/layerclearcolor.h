#ifndef AUTOFRAC_LAYERCLEARCOLOR_H
#define AUTOFRAC_LAYERCLEARCOLOR_H

#include <glm/vec4.hpp>
#include "core/layer.h"

class LayerClearColor : public Core::Layer {
public:
    LayerClearColor();
    void onRender() override;
    void onImGuiRender() override;

private:
    glm::vec4 m_clearColor;
};


#endif //AUTOFRAC_LAYERCLEARCOLOR_H
