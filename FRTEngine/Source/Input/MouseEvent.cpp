#include "WindowsMinimal.h"
#include "MouseEvent.h"


namespace frt
{
	MouseEvent::MouseEvent() noexcept
		: type(MouseEventType::Invalid), mouseState()
	{}

	MouseEvent::MouseEvent(MouseEventType type, const MouseState& mouseState) noexcept
		: type(type), mouseState(mouseState)
	{}

	inline bool MouseEvent::IsPress() const noexcept
	{
		return type == MouseEventType::Press;
	}

	inline bool MouseEvent::IsRelease() const noexcept
	{
		return type == MouseEventType::Release;
	}

	inline bool MouseEvent::IsValid() const noexcept
	{
		return type != MouseEventType::Invalid;
	}

	inline MouseEventType MouseEvent::GetType() const noexcept
	{
		return type;
	}

	inline POINT MouseEvent::GetPosition() const noexcept
	{
		return mouseState.pointerPosition;
	}

	inline int MouseEvent::GetPositionX() const noexcept
	{
		return mouseState.pointerPosition.x;
	}

	inline int MouseEvent::GetPositionY() const noexcept
	{
		return mouseState.pointerPosition.y;
	}

	inline bool MouseEvent::IsLeftPressed() const noexcept
	{
		return IsPress() && mouseState.buttonType == MouseButtonType::Left;
	}

	inline bool MouseEvent::IsRightPressed() const noexcept
	{
		return IsPress() && mouseState.buttonType == MouseButtonType::Right;
	}
}
