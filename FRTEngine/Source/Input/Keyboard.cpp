#include "Keyboard.h"


namespace frt
{
	Keyboard::Keyboard() noexcept
		: onKeyPressedEvent(), onKeyReleasedEvent(), onCharEnteredEvent()
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
		onKeyPressedEvent.Invoke(KeyboardEventType::Press, keycode);
	}

	void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
	{
		keyStates[keycode] = false;
		onKeyReleasedEvent.Invoke(KeyboardEventType::Release, keycode);
	}

	void Keyboard::OnCharEntered(char character) noexcept
	{
		onCharEnteredEvent.Invoke(KeyboardEventType::Character, character);
	}

	void Keyboard::ClearKeyStates() noexcept
	{
		keyStates.reset();
	}
}
