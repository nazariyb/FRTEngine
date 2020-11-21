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
		Enter,
		Leave
	};

	class MouseEvent : public InputEvent
	{

	public:

		MouseEvent() noexcept;
		MouseEvent(MouseEventType type, const MouseState& mouseState) noexcept;

		inline bool IsPress() const noexcept override;
		inline bool IsRelease() const noexcept override;
		inline bool IsValid() const noexcept override;
		inline MouseEventType GetType() const noexcept;

		inline POINT GetPosition() const noexcept;
		inline int GetPositionX() const noexcept;
		inline int GetPositionY() const noexcept;

		inline bool IsLeftPressed() const noexcept;
		inline bool IsRightPressed() const noexcept;

	private:

		MouseEventType type;
		MouseState mouseState;
	};
}
