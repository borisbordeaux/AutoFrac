#ifndef AUTOFRAC_LAYER_H
#define AUTOFRAC_LAYER_H

namespace Core {

class Event;

class Layer {
public:
    virtual ~Layer() = default;

    virtual void onUpdate(float /*deltaTime*/) {}

    virtual void onEvent(Event& /*event*/) {}

    virtual void onRender() {}

    virtual void onImGuiRender() {}
};

} // Core

#endif //AUTOFRAC_LAYER_H
