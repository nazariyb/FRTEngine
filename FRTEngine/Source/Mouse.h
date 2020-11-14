#pragma once
#include <queue>


namespace frt
{
	class Mouse
	{
		friend class Window;
	public:
		class Event
		{
		public:
			enum  class Type
			{
				LPress,
				LRelease,
				RPress,
				RRelease,
				WheeleUp,
				WheeleDown,
				Move,
				Enter,
				Leave,
				Invalid
			};
		private:
			Type type;
			// TODO make MouseState class
			bool isLeftPressed;
			bool isRightPressed;
			int x;
			int y;
		public:
			Event() noexcept : type(Type::Invalid), isLeftPressed(false), isRightPressed(false), x(0), y(0) {}
			Event(Type type, const Mouse& mouse) noexcept
				: type(type), isLeftPressed(mouse.isLeftPressed), isRightPressed(mouse.isRightPressed), x(mouse.x), y(mouse.y) {}
			bool IsValid() const noexcept { return type != Type::Invalid;  }
			Type GetType() const noexcept { return type; }
			std::pair<int, int> GetPosition() const noexcept { return { x, y }; }
			int GetPositionX() const noexcept { return x; }
			int GetPositionY() const noexcept { return y; }
			bool IsLeftPressed() const noexcept { return isLeftPressed; }
			bool IsRightPressed() const noexcept { return isRightPressed; }
		};

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
		Mouse::Event Read() noexcept;
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
		int x, y;
		bool isLeftPressed = false;
		bool isRightPressed = false;
		bool isInWindow = false;
		int wheelDeltaCarry = 0;
		std::queue<Event> buffer;
	};
}
