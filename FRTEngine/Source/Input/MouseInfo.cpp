#include "Input/MouseInfo.h"


namespace frt
{
MouseState::MouseState() noexcept
    : buttonNumber(0), buttonType(MouseButtonType::None), pointerPosition(POINTS())
{}

MouseState::MouseState(uint8_t buttonNumber, POINTS pointerPosition) noexcept
    : buttonNumber(buttonNumber),
    buttonType(static_cast<MouseButtonType>(buttonNumber)),
    pointerPosition(pointerPosition)
{}
}