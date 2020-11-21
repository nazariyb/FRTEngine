#pragma once
#include "Input/InputEvent.h"


namespace frt
{
	enum class KeyboardEventType : uint8_t
	{
		INPUT_EVENT_TYPE_INIT()
		Character
	};

	class KeyboardEvent : public InputEvent
	{
		friend class Keyboard;

	public:
		KeyboardEvent() noexcept;
		KeyboardEvent(KeyboardEventType type, unsigned char keyCode) noexcept;
		KeyboardEvent(KeyboardEventType type, char keyCharacter) noexcept;

		void Invoke() noexcept override;
		void Invoke(KeyboardEventType type, unsigned char keyCode) noexcept;
		void Invoke(KeyboardEventType type, char keyCharacter) noexcept;

		inline bool IsPress() const noexcept override;
		inline bool IsRelease() const noexcept override;
		inline bool IsValid() const noexcept override;
		
		inline KeyboardEventType GetType() const noexcept;
		unsigned char GetKeyCode() const noexcept;
		char GetKeyCharacter() const noexcept;

	private:
		KeyboardEventType type;
		unsigned char keyCode;
		char keyCharacter;
	};
}
