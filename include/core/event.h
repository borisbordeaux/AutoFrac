#ifndef AUTOFRAC_EVENT_H
#define AUTOFRAC_EVENT_H

#include <string>
#include <functional>

namespace Core {

enum class EventType {
    NONE = 0,
    WindowClosed, WindowResized,
    KeyPressed, KeyReleased,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; }\
                               virtual EventType getEventType() const override { return this->getStaticType(); }\
                               virtual const char* getName() const override { return #type; }

class Event {
public:
    virtual ~Event() {}

    virtual EventType getEventType() const = 0;
    virtual const char* getName() const = 0;

    virtual std::string toString() const { return this->getName(); }

    void setHandled(bool handled) { m_handled = handled; }

    bool handled() const { return m_handled; }

private:
    bool m_handled = false;
};

class EventDispatcher {
    template<typename T>
    using EventFn = std::function<bool(T&)>;
public:
    EventDispatcher(Event& event) : m_event(event) {}

    template<typename T>
    bool dispatch(EventFn<T> func) {
        if (m_event.getEventType() == T::getStaticType() && !m_event.handled()) {
            m_event.setHandled(func(*reinterpret_cast<T*>(&m_event)));
            return true;
        }
        return false;
    }

private:
    Event& m_event;
};

} // Core

#endif //AUTOFRAC_EVENT_H
