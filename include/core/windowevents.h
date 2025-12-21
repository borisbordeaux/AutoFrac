#ifndef AUTOFRAC_WINDOWEVENTS_H
#define AUTOFRAC_WINDOWEVENTS_H

#include "core/event.h"

namespace Core {

class WindowResizedEvent : public Event {
public:
    WindowResizedEvent(int width, int height) : m_width(width), m_height(height) {}

    std::string toString() const override {
        return "WindowResizedEvent: " + std::to_string(m_width) + " x " + std::to_string(m_height);
    }

    int width() const { return m_width; }
    int height() const { return m_height; }

    EVENT_CLASS_TYPE(WindowResized)

private:
    int m_width;
    int m_height;
};

class WindowClosedEvent : public Event {
public:
    EVENT_CLASS_TYPE(WindowClosed)
};

} // Core

#endif //AUTOFRAC_WINDOWEVENTS_H
