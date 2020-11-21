#include "KeyboardEvent.h"


namespace frt
{
	KeyboardEvent::KeyboardEvent() noexcept
		: type(KeyboardEventType::Invalid), keyCode(0u), keyCharacter('\0')
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

	void KeyboardEvent::Invoke(KeyboardEventType type, unsigned char keyCode) noexcept
	{
		this->type = type;
		this->keyCode = keyCode;
		__super::Invoke();
	}

	void KeyboardEvent::Invoke(KeyboardEventType type, char keyCharacter) noexcept
	{
		this->type = type;
		this->keyCharacter = keyCharacter;
		__super::Invoke();
	}

	inline bool KeyboardEvent::IsPress() const noexcept
	{
		return type == KeyboardEventType::Press;
	}

	inline bool KeyboardEvent::IsRelease() const noexcept
	{
		return type == KeyboardEventType::Release;
	}

	inline bool KeyboardEvent::IsValid() const noexcept
	{
		return type != KeyboardEventType::Invalid;
	}

	inline KeyboardEventType KeyboardEvent::GetType() const noexcept
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
