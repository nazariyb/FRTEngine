#pragma once
#include <queue>

#include "Input/MouseInfo.h"
#include "Input/MouseEvent.h"


namespace frt
{
	class Mouse
	{
		friend class Window;

	public:
		Mouse() = default;
		Mouse(const Mouse&) = delete;
		Mouse& operator=(const Mouse&) = delete;

		std::pair<int, int> GetPosition() const noexcept;
		int GetPositionX() const noexcept;
		int GetPositionY() const noexcept;
		bool IsInWindow() const noexcept;
		bool IsLeftPressed() const noexcept;
		bool IsRightPressed() const noexcept;
		MouseEvent Read() noexcept;
		bool IsEmpty() const noexcept { return buffer.empty(); }
		void Flush() noexcept;

	private:
		void OnMouseMove(int x, int y) noexcept;
		void OnMouseEnter() noexcept;
		void OnMouseLeave() noexcept;
		void OnLeftPressed(int x, int y) noexcept;
		void OnLeftReleased(int x, int y) noexcept;
		void OnRightPressed(int x, int y) noexcept;
		void OnRightReleased(int x, int y) noexcept;
		void OnWheelUp(int x, int y) noexcept;
		void OnWheelDown(int x, int y) noexcept;
		void OnWheelDelta(int x, int y, int delta) noexcept;
		void TrimBuffer() noexcept;

	private:
		static constexpr unsigned int bufferSize = 16u;
		MouseState mouseState;
		bool isInWindow = false;
		int wheelDeltaCarry = 0;
		std::queue<MouseEvent> buffer;
	};
}
