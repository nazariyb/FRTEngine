#pragma once
#include "Input/InputEvent.h"


namespace frt
{
	enum class KeyboardEventType : uint8_t
	{
		INPUT_EVENT_TYPE_INIT()
	};

	class KeyboardEvent : public InputEvent
	{
	public:
		KeyboardEvent() noexcept;
		KeyboardEvent(KeyboardEventType type, unsigned char keyCode) noexcept;

		inline bool IsPress() const noexcept override;
		inline bool IsRelease() const noexcept override;
		inline bool IsValid() const noexcept override;
		inline KeyboardEventType GetType() const noexcept;

	private:
		KeyboardEventType type;
		unsigned char keyCode;
	};
}
