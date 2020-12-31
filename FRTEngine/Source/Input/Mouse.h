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

        MouseEvent onButtonPressedEvent;
        MouseEvent onButtonReleasedEvent;
        MouseEvent onWheelUpEvent;
        MouseEvent onWheelDownEvent;
        MouseEvent onEnterWindowEvent;
        MouseEvent onLeaveWindowEvent;
        MouseEvent onMouseMoveEvent;

        Mouse();

        POINTS GetPosition() const noexcept;
        int GetPositionX() const noexcept;
        int GetPositionY() const noexcept;
        bool IsInWindow() const noexcept;
        bool IsLeftPressed() const noexcept;
        bool IsRightPressed() const noexcept;

    private:

        void OnButtonPressed(POINTS currentPosition, MouseButtonType button) noexcept;
        void OnButtonReleased(POINTS currentPosition, MouseButtonType button) noexcept;
        void OnMouseMove(POINTS currentPosition) noexcept;
        void OnWheelUp(POINTS currentPosition) noexcept;
        void OnWheelDown(POINTS currentPosition) noexcept;
        void OnWheelDelta(POINTS currentPosition, int delta) noexcept;
        void OnEnterWindow() noexcept;
        void OnLeaveWindow() noexcept;

    private:

        MouseState mouseState;
        bool isInWindow = false;
        int wheelDeltaCarry = 0;
    };
}
