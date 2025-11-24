#ifndef AUTOFRAC_KEYEVENTS_H
#define AUTOFRAC_KEYEVENTS_H

#include "event.h"

namespace Core {

class KeyEvent : public Event {
public:
    inline int getKeyCode() const { return m_keyCode; }

protected:
    KeyEvent(int keyCode) : m_keyCode(keyCode) {}

private:
    int m_keyCode;
};

class KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(int keyCode, bool isRepeat) : KeyEvent(keyCode), m_isRepeat(isRepeat) {}

    inline bool isRepeat() const { return m_isRepeat; }

    std::string toString() const override {
        return "KeyPressedEvent: " + std::to_string(this->getKeyCode()) + " (repeat=" + std::to_string(m_isRepeat) + ")";
    }

    EVENT_CLASS_TYPE(KeyPressed)

private:
    bool m_isRepeat;
};

class KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

    std::string toString() const override {
        return "KeyReleasedEvent: " + std::to_string(this->getKeyCode());
    }

    EVENT_CLASS_TYPE(KeyPressed)
};

} // Core

#endif //AUTOFRAC_KEYEVENTS_H
