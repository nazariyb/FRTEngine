#pragma once
#include "FRTEngine.h"
#include "Input/InputEvent.h"


namespace frt
{
enum class FRTENGINE_API KeyboardEventType : uint8_t
{
    INPUT_EVENT_TYPE_INIT()
    Character
};

class FRTENGINE_API KeyboardEvent : public InputEvent
{
    friend class Keyboard;

public:

    KeyboardEvent() noexcept;
    KeyboardEvent(KeyboardEventType type) noexcept;
    KeyboardEvent(KeyboardEventType type, unsigned char keyCode) noexcept;
    KeyboardEvent(KeyboardEventType type, char keyCharacter) noexcept;

    void Invoke() noexcept override;
    void Invoke(unsigned char keyCode) noexcept;
    void Invoke(char keyCharacter) noexcept;
    void Invoke(KeyboardEventType type, unsigned char keyCode) noexcept;
    void Invoke(KeyboardEventType type, char keyCharacter) noexcept;

    bool IsPress() const noexcept override;
    bool IsRelease() const noexcept override;
    bool IsValid() const noexcept override;

    KeyboardEventType GetType() const noexcept;
    unsigned char GetKeyCode() const noexcept;
    char GetKeyCharacter() const noexcept;

private:

    KeyboardEventType type;
    unsigned char keyCode;
    char keyCharacter;
};
}
