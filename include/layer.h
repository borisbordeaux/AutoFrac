#ifndef AUTOFRAC_LAYER_H
#define AUTOFRAC_LAYER_H

class Layer {
public:
    Layer() {}

    virtual ~Layer() {}

    virtual void onUpdate(float /*deltaTime*/) {}

    virtual void onRender() {}

    virtual void onImGuiRender() {}
};

#endif //AUTOFRAC_LAYER_H
