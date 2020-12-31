#include "WindowsMinimal.h"
#include "MouseEvent.h"


namespace frt
{
    MouseEvent::MouseEvent() noexcept
        : type(MouseEventType::Invalid), mouseState()
    {}

    MouseEvent::MouseEvent(MouseEventType type) noexcept
        : type(type), mouseState()
    {}

    MouseEvent::MouseEvent(MouseEventType type, const MouseState& mouseState) noexcept
        : type(type), mouseState(mouseState)
    {}

    void MouseEvent::Invoke() noexcept
    {
        __super::Invoke();
    }

    void MouseEvent::Invoke(const MouseState& mouseState) noexcept
    {
        this->mouseState = mouseState;
        Invoke();
    }

    void MouseEvent::Invoke(MouseEventType type, const MouseState& mouseState) noexcept
    {
        this->type = type;
        Invoke(mouseState);
    }

    bool MouseEvent::IsPress() const noexcept
    {
        return type == MouseEventType::Press;
    }

    bool MouseEvent::IsRelease() const noexcept
    {
        return type == MouseEventType::Release;
    }

    bool MouseEvent::IsValid() const noexcept
    {
        return type != MouseEventType::Invalid;
    }

    MouseEventType MouseEvent::GetType() const noexcept
    {
        return type;
    }

    POINTS MouseEvent::GetPosition() const noexcept
    {
        return mouseState.pointerPosition;
    }

    int MouseEvent::GetPositionX() const noexcept
    {
        return mouseState.pointerPosition.x;
    }

    int MouseEvent::GetPositionY() const noexcept
    {
        return mouseState.pointerPosition.y;
    }

    bool MouseEvent::IsLeftPressed() const noexcept
    {
        return IsPress() && (mouseState.buttonType == MouseButtonType::Left);
    }

    bool MouseEvent::IsRightPressed() const noexcept
    {
        return IsPress() && (mouseState.buttonType == MouseButtonType::Right);
    }
}
