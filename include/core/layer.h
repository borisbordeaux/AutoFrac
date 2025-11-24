#ifndef AUTOFRAC_LAYER_H
#define AUTOFRAC_LAYER_H

#include <memory>

namespace Core {

class Event;

class Layer {
public:
    virtual ~Layer() = default;

    virtual void onUpdate(float /*deltaTime*/) {}

    virtual void onEvent(Event& /*event*/) {}

    virtual void onRender() {}

    virtual void onImGuiRender() {}

    template<typename TLayer, typename... Args>
    void transitionTo(Args&& ... args) {
        this->queueTransition(std::move(std::make_unique<TLayer>(std::forward<Args>(args)...)));
    }

private:
    void queueTransition(std::unique_ptr<Layer> toLayer);
};

} // Core

#endif //AUTOFRAC_LAYER_H
