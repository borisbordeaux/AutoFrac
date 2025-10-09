#ifndef AUTOFRAC_LAYERCLEARCOLOR_H
#define AUTOFRAC_LAYERCLEARCOLOR_H

#include <glm/detail/type_vec4.hpp>
#include "layer.h"

class LayerClearColor : public Layer {
public:
    LayerClearColor();
    void onRender() override;
    void onImGuiRender() override;

private:
    glm::tvec4<float> m_clearColor;
};


#endif //AUTOFRAC_LAYERCLEARCOLOR_H
