#pragma once
#include <queue>
#include <bitset>

#include "Input/KeyboardEvent.h"


namespace frt
{
	class Keyboard
	{
		friend class Window;

	public:

		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;

		bool KeyIsPressed(unsigned char keycode) const noexcept;
		KeyboardEvent ReadKey() noexcept;
		bool KeyIsEmpty() const noexcept;
		void FlushKey() noexcept;

		char ReadChar() noexcept;
		bool CharIsEmpty() const noexcept;
		void FlushChar() noexcept;
		void Flush() noexcept;

		inline void EnableAutorepeat() noexcept;
		inline void DisableAutorepeat() noexcept;
		bool IsAutorepeatEnabled() const noexcept;

	private:
		void OnKeyPressed(unsigned char keycode) noexcept;
		void OnKeyReleased(unsigned char keycode) noexcept;
		void OnChar(char character) noexcept;
		void ClearState() noexcept;
		template<typename T>
		static void TrimBuffer(std::queue<T>& buffer) noexcept;
	private:
		static constexpr unsigned int nKeys = 256u;
		static constexpr unsigned int bufferSize = 16u;
		bool autorepeatEnabled = false;
		std::bitset<nKeys> keyStates;
		std::queue<KeyboardEvent> keyBuffer;
		std::queue<char> charBuffer;
	};
}

