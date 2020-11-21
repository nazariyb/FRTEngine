#include "WindowsMinimal.h"
#include "Mouse.h"


namespace frt
{
	std::pair<int, int> Mouse::GetPosition() const noexcept
	{
		return { mouseState.pointerPosition.x, mouseState.pointerPosition.y };
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

	MouseEvent Mouse::Read() noexcept
	{
		if (buffer.size() > 0u)
		{
			MouseEvent e = buffer.front();
			buffer.pop();
			return e;
		}
		else
		{
			return MouseEvent();
		}
	}

	void Mouse::Flush() noexcept
	{
		buffer = std::queue<MouseEvent>();
	}

	void Mouse::OnMouseMove(int newX, int newY) noexcept
	{
		mouseState.pointerPosition.x = newX;
		mouseState.pointerPosition.y = newY;

		buffer.push(MouseEvent(MouseEventType::Move, mouseState));
		TrimBuffer();
	}

	void Mouse::OnMouseEnter() noexcept
	{
		isInWindow = true;
		buffer.push(MouseEvent(MouseEventType::Enter, mouseState));
		TrimBuffer();
	}

	void Mouse::OnMouseLeave() noexcept
	{
		isInWindow = false;
		buffer.push(MouseEvent(MouseEventType::Leave, mouseState));
		TrimBuffer();
	}

	void Mouse::OnLeftPressed(int x, int y) noexcept
	{
		mouseState.buttonType = MouseButtonType::Left;

		buffer.push(MouseEvent(MouseEventType::Press, mouseState));
		TrimBuffer();
	}

	void Mouse::OnLeftReleased(int x, int y) noexcept
	{
		mouseState.buttonType = MouseButtonType::Left;

		buffer.push(MouseEvent(MouseEventType::Release, mouseState));
		TrimBuffer();
	}

	void Mouse::OnRightPressed(int x, int y) noexcept
	{
		mouseState.buttonType = MouseButtonType::Right;

		buffer.push(MouseEvent(MouseEventType::Press, mouseState));
		TrimBuffer();
	}

	void Mouse::OnRightReleased(int x, int y) noexcept
	{
		mouseState.buttonType = MouseButtonType::Right;

		buffer.push(MouseEvent(MouseEventType::Release, mouseState));
		TrimBuffer();
	}

	void Mouse::OnWheelUp(int x, int y) noexcept
	{
		buffer.push(MouseEvent(MouseEventType::WheelUp, mouseState));
		TrimBuffer();
	}

	void Mouse::OnWheelDown(int x, int y) noexcept
	{
		buffer.push(MouseEvent(MouseEventType::WheelDown, mouseState));
		TrimBuffer();
	}

	void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
	{
		wheelDeltaCarry += delta;
		while (wheelDeltaCarry >= WHEEL_DELTA)
		{
			wheelDeltaCarry -= WHEEL_DELTA;
			OnWheelUp(x, y);
		}
		while (wheelDeltaCarry <= -WHEEL_DELTA)
		{
			wheelDeltaCarry += WHEEL_DELTA;
			OnWheelDown(x, y);
		}
	}

	void Mouse::TrimBuffer() noexcept
	{
		while (buffer.size() > bufferSize)
		{
			buffer.pop();
		}
	}
}