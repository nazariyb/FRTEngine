#include "KeyboardEvent.h"


namespace frt
{
    KeyboardEvent::KeyboardEvent() noexcept
        : type(KeyboardEventType::Invalid), keyCode(0u), keyCharacter('\0')
    {}

    KeyboardEvent::KeyboardEvent(KeyboardEventType type) noexcept
        : type(type), keyCode(0u), keyCharacter('\0')
    {}

    KeyboardEvent::KeyboardEvent(KeyboardEventType type, unsigned char keyCode) noexcept
        : type(type), keyCode(keyCode), keyCharacter('\0')
    {}

    KeyboardEvent::KeyboardEvent(KeyboardEventType type, char keyCharacter) noexcept
        : type(type), keyCode(0u), keyCharacter(keyCharacter)
    {}

    void KeyboardEvent::Invoke() noexcept
    {
        __super::Invoke();
    }

    void KeyboardEvent::Invoke(unsigned char keyCode) noexcept
    {
        this->keyCode = keyCode;
        Invoke();
    }

    void KeyboardEvent::Invoke(char keyCharacter) noexcept
    {
        this->keyCharacter = keyCharacter;
        Invoke();
    }

    void KeyboardEvent::Invoke(KeyboardEventType type, unsigned char keyCode) noexcept
    {
        this->type = type;
        Invoke(keyCode);
    }

    void KeyboardEvent::Invoke(KeyboardEventType type, char keyCharacter) noexcept
    {
        this->type = type;
        Invoke(keyCharacter);
    }

    bool KeyboardEvent::IsPress() const noexcept
    {
        return type == KeyboardEventType::Press;
    }

    bool KeyboardEvent::IsRelease() const noexcept
    {
        return type == KeyboardEventType::Release;
    }

    bool KeyboardEvent::IsValid() const noexcept
    {
        return type != KeyboardEventType::Invalid;
    }

    KeyboardEventType KeyboardEvent::GetType() const noexcept
    {
        return type;
    }

    unsigned char KeyboardEvent::GetKeyCode() const noexcept
    {
        return keyCode;
    }

    char KeyboardEvent::GetKeyCharacter() const noexcept
    {
        return keyCharacter;
    }
}
