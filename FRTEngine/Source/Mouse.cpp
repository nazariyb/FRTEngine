#include "WindowsMinimal.h"
#include "Mouse.h"


namespace frt
{
	std::pair<int, int> Mouse::GetPosition() const noexcept
	{
		return { x, y };
	}

	int Mouse::GetPositionX() const noexcept
	{
		return x;
	}

	int Mouse::GetPositionY() const noexcept
	{
		return y;
	}

	bool Mouse::IsInWindow() const noexcept
	{
		return isInWindow;
	}

	bool Mouse::IsLeftPressed() const noexcept
	{
		return isLeftPressed;
	}

	bool Mouse::IsRightPressed() const noexcept
	{
		return isRightPressed;
	}

	Mouse::Event Mouse::Read() noexcept
	{
		if (buffer.size() > 0u)
		{
			Mouse::Event e = buffer.front();
			buffer.pop();
			return e;
		}
		else
		{
			return Mouse::Event();
		}
	}

	void Mouse::Flush() noexcept
	{
		buffer = std::queue<Event>();
	}

	void Mouse::OnMouseMove(int newX, int newY) noexcept
	{
		x = newX;
		y = newY;

		buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
		TrimBuffer();
	}

	void Mouse::OnMouseEnter() noexcept
	{
		isInWindow = true;
		buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
		TrimBuffer();
	}

	void Mouse::OnMouseLeave() noexcept
	{
		isInWindow = false;
		buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
		TrimBuffer();
	}

	void Mouse::OnLeftPressed(int x, int y) noexcept
	{
		isLeftPressed = true;

		buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
		TrimBuffer();
	}

	void Mouse::OnLeftReleased(int x, int y) noexcept
	{
		isLeftPressed = false;

		buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
		TrimBuffer();
	}

	void Mouse::OnRightPressed(int x, int y) noexcept
	{
		isRightPressed = true;

		buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
		TrimBuffer();
	}

	void Mouse::OnRightReleased(int x, int y) noexcept
	{
		isRightPressed = false;

		buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
		TrimBuffer();
	}

	void Mouse::OnWheelUp(int x, int y) noexcept
	{
		buffer.push(Mouse::Event(Mouse::Event::Type::WheeleUp, *this));
		TrimBuffer();
	}

	void Mouse::OnWheelDown(int x, int y) noexcept
	{
		buffer.push(Mouse::Event(Mouse::Event::Type::WheeleDown, *this));
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