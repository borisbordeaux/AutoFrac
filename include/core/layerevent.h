#ifndef AUTOFRAC_LAYEREVENT_H
#define AUTOFRAC_LAYEREVENT_H

#include "core/event.h"

namespace Core {

class LayerSwappedEvent : public Event {
public:
    explicit LayerSwappedEvent(Layer* layer): m_layer(layer) {}
    Layer* getLayer() const { return m_layer; }
    EVENT_CLASS_TYPE(LayerSwapped)

private:
    Layer* m_layer;
};

} // Core

#endif //AUTOFRAC_LAYEREVENT_H