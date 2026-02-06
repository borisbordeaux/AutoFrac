#ifndef AUTOFRAC_KEYEVENTS_H
#define AUTOFRAC_KEYEVENTS_H

#include "event.h"

namespace Core {

class KeyEvent : public Event {
public:
    int getKey() const { return m_key; }
    int getScanCode() const { return m_scanCode; }
    const char& getKeyName() const { return m_keyName; }

protected:
    KeyEvent(int key, int scanCode, const char* keyName) : m_key(key), m_scanCode(scanCode), m_keyName(keyName == nullptr ? '\0' : *keyName) {}

private:
    int m_key;
    int m_scanCode;
    char m_keyName;
};

class KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(int key, int scanCode, const char* keyName, bool isRepeat) : KeyEvent(key, scanCode, keyName), m_isRepeat(isRepeat) {}
    bool isRepeat() const { return m_isRepeat; }
    std::string toString() const override {
        return "KeyPressedEvent: " + std::to_string(this->getKey()) + ", " + std::to_string(this->getScanCode()) + ", " + this->getKeyName() + " (repeat=" + std::to_string(m_isRepeat) + ")";
    }
    EVENT_CLASS_TYPE(KeyPressed)

private:
    bool m_isRepeat;
};

class KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(int key, int scanCode, const char* keyName) : KeyEvent(key, scanCode, keyName) {}
    std::string toString() const override {
        return "KeyReleasedEvent: " + std::to_string(this->getKey()) + ", " + std::to_string(this->getScanCode()) + ", " + this->getKeyName();
    }
    EVENT_CLASS_TYPE(KeyReleased)
};

} // Core

#endif //AUTOFRAC_KEYEVENTS_H
