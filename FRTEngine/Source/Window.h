#pragma once
#include "FRTEngine.h"

#include "WindowsMinimal.h"
#include "Exception.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Render/Graphics.h"
#include <optional>
#include <memory>

namespace frt
{
class FRTENGINE_API Window
{
public:
    Window(int width, int height, const char* name, HICON icon)
#ifndef _DEBUG
        noexcept
#endif // _DEBUG
        ;
    ~Window();

    void SetTitle(const std::string& title);
    //void EnableCursor() noexcept;
    //void DisableCursor() noexcept;
    //bool CursorEnabled() const noexcept;
    static std::optional<int> ProcessMessages() noexcept;
    Graphics& GetGraphics();
private:
    //void ConfineCursor() noexcept;
    //void FreeCursor() noexcept;
    //void ShowCursor() noexcept;
    //void HideCursor() noexcept;

    static LRESULT WINAPI HandleMsgSetup(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT WINAPI HandleMsgThunk(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMsg(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
    Keyboard keyboard;
    Mouse mouse;
    struct { const unsigned width; const unsigned height; } GetResolution() const { return { _width, _height }; }

private:
    //bool cursorEnabled = true;
    unsigned _width;
    unsigned _height;
    HWND _hWindow;
    HINSTANCE _hInstance;
    std::unique_ptr <Graphics> _graphics;

    const char* windowClassName = "FRT Window Base Class";

    void RegisterWinAPIClass(HICON winIcon) noexcept;
};
}
