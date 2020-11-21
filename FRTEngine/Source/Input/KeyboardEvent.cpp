#include "KeyboardEvent.h"


namespace frt
{
	KeyboardEvent::KeyboardEvent() noexcept
		: type(KeyboardEventType::Invalid), keyCode(0u)
	{}

	KeyboardEvent::KeyboardEvent(KeyboardEventType type, unsigned char keyCode) noexcept
		: type(type), keyCode(keyCode)
	{}

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

	inline auto KeyboardEvent::GetType() const noexcept -> KeyboardEventType
	{
		return type;
	}
}
