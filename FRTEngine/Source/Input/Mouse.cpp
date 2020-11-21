#include "WindowsMinimal.h"
#include "Mouse.h"


namespace frt
{
	Mouse::Mouse() :
		onButtonPressedEvent(MouseEventType::Press),
		onButtonReleasedEvent(MouseEventType::Release),
		onWheelUpEvent(MouseEventType::WheelUp),
		onWheelDownEvent(MouseEventType::WheelDown),
		onEnterWindowEvent(MouseEventType::EnterWindow),
		onLeaveWindowEvent(MouseEventType::LeaveWindow),
		onMouseMoveEvent(MouseEventType::Move)
	{}

	POINTS Mouse::GetPosition() const noexcept
	{
		return mouseState.pointerPosition;
	}

	int Mouse::GetPositionX() const noexcept
	{
		return mouseState.pointerPosition.x;
	}

	int Mouse::GetPositionY() const noexcept
	{
		return mouseState.pointerPosition.y;
	}

	bool Mouse::IsInWindow() const noexcept
	{
		return isInWindow;
	}

	bool Mouse::IsLeftPressed() const noexcept
	{
		return mouseState.buttonType == MouseButtonType::Left;
	}

	bool Mouse::IsRightPressed() const noexcept
	{
		return mouseState.buttonType == MouseButtonType::Right;
	}

	void Mouse::OnMouseMove(POINTS currentPosition) noexcept
	{
		mouseState.pointerPosition = currentPosition;
		onMouseMoveEvent.Invoke(mouseState);
	}

	void Mouse::OnEnterWindow() noexcept
	{
		isInWindow = true;
		onEnterWindowEvent.Invoke(mouseState);
	}

	void Mouse::OnLeaveWindow() noexcept
	{
		isInWindow = false;
		onLeaveWindowEvent.Invoke();
	}

	void Mouse::OnButtonPressed(POINTS currentPosition, MouseButtonType button) noexcept
	{
		mouseState.buttonType = button;
		onButtonPressedEvent.Invoke(mouseState);
	}

	void Mouse::OnButtonReleased(POINTS currentPosition, MouseButtonType button) noexcept
	{
		mouseState.buttonType = MouseButtonType::None;
		onButtonReleasedEvent.Invoke(mouseState);
	}

	void Mouse::OnWheelUp(POINTS currentPosition) noexcept
	{
		onWheelUpEvent.Invoke(mouseState);
	}

	void Mouse::OnWheelDown(POINTS currentPosition) noexcept
	{
		onWheelDownEvent.Invoke(mouseState);
	}

	void Mouse::OnWheelDelta(POINTS currentPosition, int delta) noexcept
	{
		wheelDeltaCarry += delta;
		while (wheelDeltaCarry >= WHEEL_DELTA)
		{
			wheelDeltaCarry -= WHEEL_DELTA;
			OnWheelUp(currentPosition);
		}
		while (wheelDeltaCarry <= -WHEEL_DELTA)
		{
			wheelDeltaCarry += WHEEL_DELTA;
			OnWheelDown(currentPosition);
		}
	}
}
