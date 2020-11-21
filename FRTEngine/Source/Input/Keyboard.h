#pragma once
#include <queue>
#include <bitset>
#include <optional>

#include "Input/KeyboardEvent.h"


namespace frt
{
	class Keyboard
	{
		friend class Window;

	public:
		KeyboardEvent onKeyPressedEvent;
		KeyboardEvent onKeyReleasedEvent;
		KeyboardEvent onCharEnteredEvent;

		Keyboard() noexcept;

		inline bool IsKeyPressed(unsigned char keycode) const noexcept;

		inline void EnableAutorepeat() noexcept;
		inline void DisableAutorepeat() noexcept;
		bool IsAutorepeatEnabled() const noexcept;

	private:
		void OnKeyPressed(unsigned char keycode) noexcept;
		void OnKeyReleased(unsigned char keycode) noexcept;
		void OnCharEntered(char character) noexcept;

		void ClearKeyStates() noexcept;

	private:
		bool autorepeatEnabled = false;

		static constexpr unsigned int maxKeysNumber = 256u;
		std::bitset<maxKeysNumber> keyStates;
	};
}

