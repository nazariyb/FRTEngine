#include "Keyboard.h"

#include <queue>
#include <optional>


namespace frt
{
Keyboard::Keyboard() noexcept :
    onKeyPressedEvent(KeyboardEventType::Press),
    onKeyReleasedEvent(KeyboardEventType::Release),
    onCharEnteredEvent(KeyboardEventType::Character)
{}

inline bool Keyboard::IsKeyPressed(unsigned char keycode) const noexcept
{
    return keyStates[keycode];
}

inline void Keyboard::EnableAutorepeat() noexcept
{
    autorepeatEnabled = true;
}

inline void Keyboard::DisableAutorepeat() noexcept
{
    autorepeatEnabled = false;
}

bool Keyboard::IsAutorepeatEnabled() const noexcept
{
    return autorepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
    keyStates[keycode] = true;
    onKeyPressedEvent.Invoke(keycode);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
    keyStates[keycode] = false;
    onKeyReleasedEvent.Invoke(keycode);
}

void Keyboard::OnCharEntered(char character) noexcept
{
    onCharEnteredEvent.Invoke(character);
}

void Keyboard::ClearKeyStates() noexcept
{
    keyStates.reset();
}
}
