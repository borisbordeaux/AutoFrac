#ifndef AUTOFRAC_MOUSEEVENTS_H
#define AUTOFRAC_MOUSEEVENTS_H

#include "core/event.h"

namespace Core {

class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(double x, double y) : m_x(x), m_y(y) {}

    inline double x() const { return m_x; }
    inline double y() const { return m_y; }

    std::string toString() const override {
        return "MouseMovedEvent: " + std::to_string(m_x) + ", " + std::to_string(m_y);
    }

    EVENT_CLASS_TYPE(MouseMoved)

private:
    double m_x;
    double m_y;
};

class MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(double xOffset, double yOffset) :
            m_xOffset(xOffset), m_yOffset(yOffset) {}

    std::string toString() const override {
        return "MouseScrolledEvent: " + std::to_string(m_xOffset) + ", " + std::to_string(m_yOffset);
    }

    EVENT_CLASS_TYPE(MouseScrolled)

private:
    double m_xOffset;
    double m_yOffset;
};

class MouseButtonEvent : public Event {
public:
    inline int getMouseButton() const { return m_button; }

protected:
    MouseButtonEvent(int button) : m_button(button) {}

private:
    int m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
    explicit MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

    std::string toString() const override {
        return "MouseButtonPressedEvent: " + std::to_string(this->getMouseButton());
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
    explicit MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    std::string toString() const override {
        return "MouseButtonReleasedEvent: " + std::to_string(this->getMouseButton());
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};

} // Core

#endif //AUTOFRAC_MOUSEEVENTS_H
