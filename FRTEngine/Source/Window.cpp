#include "Window.h"
#include <sstream>
//#include "WindowsThrowMacros.h"
//#include "imgui/imgui_impl_win32.h"

namespace frt
{
void Window::RegisterWinAPIClass(HICON winIcon) noexcept
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = _hInstance;
    wc.hIcon = winIcon;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = windowClassName;
    wc.hIconSm = nullptr;
    RegisterClassEx(&wc);
}

Window::~Window()
{
    UnregisterClass(windowClassName, _hInstance);
    //ImGui_ImplWin32_Shutdown();
    DestroyWindow(_hWindow);
}

Window::Window(int width, int height, const char* name, HICON icon)
#ifndef _DEBUG
noexcept
#endif // _DEBUG
    : _width(width), _height(height), _hInstance(GetModuleHandle(nullptr))
{
    RegisterWinAPIClass(icon);

    RECT wr;
    wr.left = 500;
    wr.right = _width + wr.left;
    wr.top = 500;
    wr.bottom = _height + wr.top;
    if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_SIZEBOX, FALSE) == 0)
    {
#ifdef _DEBUG
        throw LAST_EXCEPTION();
#endif // _DEBUG
    }

    _hWindow = CreateWindow(
        windowClassName, name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, _hInstance, this
    );

    if (_hWindow == nullptr)
    {
#ifdef _DEBUG
        throw LAST_EXCEPTION();
#endif // _DEBUG
    }

    ShowWindow(_hWindow, SW_SHOWDEFAULT);

    _graphics = std::make_unique<Graphics>(_hWindow);

    //// Init ImGui Win32 Impl
    //ImGui_ImplWin32_Init(_hWindow);
    //// register mouse raw input device
    //RAWINPUTDEVICE rid;
    //rid.usUsagePage = 0x01; // mouse page
    //rid.usUsage = 0x02; // mouse usage
    //rid.dwFlags = 0;
    //rid.hwndTarget = nullptr;
    //if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
    //{
    //	throw CHWND_LAST_EXCEPT();
    //}
}

void Window::SetTitle(const std::string& title)
{
    if (SetWindowText(_hWindow, title.c_str()) == 0)
    {
        throw LAST_EXCEPTION();
    }
}

//void Window::EnableCursor() noexcept
//{
//	cursorEnabled = true;
//	ShowCursor();
//	EnableImGuiMouse();
//	FreeCursor();
//}
//
//void Window::DisableCursor() noexcept
//{
//	cursorEnabled = false;
//	HideCursor();
//	DisableImGuiMouse();
//	ConfineCursor();
//}
//
//bool Window::CursorEnabled() const noexcept
//{
//	return cursorEnabled;
//}

std::optional<int> Window::ProcessMessages() noexcept
{
    MSG msg;
    // while queue has messages, remove and dispatch them (but do not block on empty queue)
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        // check for quit because peekmessage does not signal this via return val
        if (msg.message == WM_QUIT)
        {
            // return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
            return (int)msg.wParam;
        }

        // TranslateMessage will post auxilliary WM_CHAR messages from key msgs
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // return empty optional when not quitting app
    return {};
}

Graphics& Window::GetGraphics()
{
    return *_graphics;
}

//Graphics& Window::Gfx()
//{
//	if (!pGfx)
//	{
//		throw CHWND_NOGFX_EXCEPT();
//	}
//	return *pGfx;
//}
//
//void Window::ConfineCursor() noexcept
//{
//	RECT rect;
//	GetClientRect(_hWindow, &rect);
//	MapWindowPoints(_hWindow, nullptr, reinterpret_cast<POINT*>(&rect), 2);
//	ClipCursor(&rect);
//}
//
//void Window::FreeCursor() noexcept
//{
//	ClipCursor(nullptr);
//}
//
//void Window::HideCursor() noexcept
//{
//	while (::ShowCursor(FALSE) >= 0);
//}
//
//void Window::ShowCursor() noexcept
//{
//	while (::ShowCursor(TRUE) < 0);
//}
//
//void Window::EnableImGuiMouse() noexcept
//{
//	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
//}
//
//void Window::DisableImGuiMouse() noexcept
//{
//	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
//}

LRESULT CALLBACK Window::HandleMsgSetup(HWND _hWindow, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
    if (msg == WM_NCCREATE)
    {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window instance
        SetWindowLongPtr(_hWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(_hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        // forward message to window instance handler
        return pWnd->HandleMsg(_hWindow, msg, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(_hWindow, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND _hWindow, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // retrieve ptr to window instance
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(_hWindow, GWLP_USERDATA));
    // forward message to window instance handler
    return pWnd->HandleMsg(_hWindow, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND _hWindow, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    //if (ImGui_ImplWin32_WndProcHandler(_hWindow, msg, wParam, lParam))
    //{
    //	return true;
    //}
    //const auto& imio = ImGui::GetIO();

    switch (msg)
    {
        // we don't want the DefProc to handle this message because
        // we want our destructor to destroy the window, so return 0 instead of break
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
        //	// clear keystate when window loses focus to prevent input getting "stuck"
    case WM_KILLFOCUS:
        keyboard.ClearKeyStates();
        break;
        //case WM_ACTIVATE:
        //	// confine/free cursor on window to foreground/background if cursor disabled
        //	if (!cursorEnabled)
        //	{
        //		if (wParam & WA_ACTIVE)
        //		{
        //			ConfineCursor();
        //			HideCursor();
        //		}
        //		else
        //		{
        //			FreeCursor();
        //			ShowCursor();
        //		}
        //	}
        //	break;

            /*********** KEYBOARD MESSAGES ***********/
    case WM_KEYDOWN:
        // syskey commands need to be handled to track ALT key (VK_MENU) and F10
    case WM_SYSKEYDOWN:
        // stifle this keyboard message if imgui wants to capture
        //if (imio.WantCaptureKeyboard)
        //{
        //	break;
        //}
        if (!(lParam & 0x40000000) || keyboard.IsAutorepeatEnabled()) // filter autorepeat
        {
            keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        // stifle this keyboard message if imgui wants to capture
        //if (imio.WantCaptureKeyboard)
        //{
        //	break;
        //}
        keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;
    case WM_CHAR:
        // stifle this keyboard message if imgui wants to capture
        //if (imio.WantCaptureKeyboard)
        //{
        //	break;
        //}
        keyboard.OnCharEntered(static_cast<unsigned char>(wParam));
        break;
        /*********** END KEYBOARD MESSAGES ***********/

        /************* MOUSE MESSAGES ****************/
    case WM_MOUSEMOVE:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnMouseMove(pt);
        // cursorless exclusive gets first dibs
        //if (!cursorEnabled)
        //{
        //	if (!mouse.IsInWindow())
        //	{
        //		SetCapture(_hWindow);
        //		mouse.OnMouseEnter();
        //		HideCursor();
        //	}
        //	break;
        //}
        //// stifle this mouse message if imgui wants to capture
        //if (imio.WantCaptureMouse)
        //{
        //	break;
        //}
        //// in client region -> log move, and log enter + capture mouse (if not previously in window)
        if (pt.x >= 0 && pt.x < _width && pt.y >= 0 && pt.y < _height)
        {
            mouse.OnMouseMove(pt);
            if (!mouse.IsInWindow())
            {
                SetCapture(_hWindow);
                mouse.OnEnterWindow();
            }
        }
        // not in client -> log move / maintain capture if button down
        else
        {
            if (wParam & (MK_LBUTTON | MK_RBUTTON))
            {
                mouse.OnMouseMove(pt);
            }
            // button up -> release capture / log event for leaving
            else
            {
                ReleaseCapture();
                mouse.OnLeaveWindow();
            }
        }
        break;
    }
    case WM_LBUTTONDOWN:
    {
        //SetForegroundWindow(_hWindow);
        //if (!cursorEnabled)
        //{
        //	ConfineCursor();
        //	HideCursor();
        //}
        //// stifle this mouse message if imgui wants to capture
        //if (imio.WantCaptureMouse)
        //{
        //	break;
        //}
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnButtonPressed(pt, MouseButtonType::Left);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        // stifle this mouse message if imgui wants to capture
        //if (imio.WantCaptureMouse)
        //{
        //	break;
        //}
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnButtonPressed(pt, MouseButtonType::Right);
        break;
    }
    case WM_LBUTTONUP:
    {
        // stifle this mouse message if imgui wants to capture
        //if (imio.WantCaptureMouse)
        //{
        //	break;
        //}
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnButtonReleased(pt, MouseButtonType::Left);
        //// release mouse if outside of window
        //if (pt.x < 0 || pt.x >= _width || pt.y < 0 || pt.y >= _height)
        //{
        //	ReleaseCapture();
        //	mouse.OnMouseLeave();
        //}
        break;
    }
    case WM_RBUTTONUP:
    {
        // stifle this mouse message if imgui wants to capture
        //if (imio.WantCaptureMouse)
        //{
        //	break;
        //}
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnButtonReleased(pt, MouseButtonType::Right);
        // release mouse if outside of window
        //if (pt.x < 0 || pt.x >= _width || pt.y < 0 || pt.y >= _height)
        //{
        //	ReleaseCapture();
        //	mouse.OnMouseLeave();
        //}
        break;
    }
    case WM_MOUSEWHEEL:
    {
        // stifle this mouse message if imgui wants to capture
        //if (imio.WantCaptureMouse)
        //{
        //	break;
        //}
        const POINTS pt = MAKEPOINTS(lParam);
        const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        mouse.OnWheelDelta(pt, delta);
        break;
    }
    /************** END MOUSE MESSAGES **************/

    ///************** RAW MOUSE MESSAGES **************/
    //case WM_INPUT:
    //{
    //	if (!mouse.RawEnabled())
    //	{
    //		break;
    //	}
    //	UINT size;
    //	// first get the size of the input data
    //	if (GetRawInputData(
    //		reinterpret_cast<HRAWINPUT>(lParam),
    //		RID_INPUT,
    //		nullptr,
    //		&size,
    //		sizeof(RAWINPUTHEADER)) == -1)
    //	{
    //		// bail msg processing if error
    //		break;
    //	}
    //	rawBuffer.resize(size);
    //	// read in the input data
    //	if (GetRawInputData(
    //		reinterpret_cast<HRAWINPUT>(lParam),
    //		RID_INPUT,
    //		rawBuffer.data(),
    //		&size,
    //		sizeof(RAWINPUTHEADER)) != size)
    //	{
    //		// bail msg processing if error
    //		break;
    //	}
    //	// process the raw input data
    //	auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
    //	if (ri.header.dwType == RIM_TYPEMOUSE &&
    //		(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
    //	{
    //		mouse.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
    //	}
    //	break;
    //}
    /************** END RAW MOUSE MESSAGES **************/
    }

    return DefWindowProc(_hWindow, msg, wParam, lParam);
}
}