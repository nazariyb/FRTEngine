#pragma once
#include "Input/InputEvent.h"
#include "Input/MouseInfo.h"


namespace frt
{
	enum class MouseEventType
	{
		INPUT_EVENT_TYPE_INIT()
		WheelUp,
		WheelDown,
		Move,
		EnterWindow,
		LeaveWindow
	};

	class MouseEvent : public InputEvent
	{
		friend class Mouse;

	public:

		MouseEvent() noexcept;
		MouseEvent(MouseEventType type) noexcept;
		MouseEvent(MouseEventType type, const MouseState& mouseState) noexcept;

		void Invoke() noexcept override;
		void Invoke(const MouseState& mouseState) noexcept;
		void Invoke(MouseEventType type, const MouseState& mouseState) noexcept;

		bool IsPress() const noexcept override;
		bool IsRelease() const noexcept override;
		bool IsValid() const noexcept override;
		MouseEventType GetType() const noexcept;

		POINTS GetPosition() const noexcept;
		int GetPositionX() const noexcept;
		int GetPositionY() const noexcept;

		bool IsLeftPressed() const noexcept;
		bool IsRightPressed() const noexcept;

	private:

		MouseEventType type;
		MouseState mouseState;
	};
}
