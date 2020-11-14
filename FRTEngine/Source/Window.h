#pragma once
#include "WindowsMinimal.h"
#include "Exception.h"
#include "Keyboard.h"
#include "Mouse.h"
//#include "Graphics.h"
#include <optional>
#include <memory>

namespace frt
{
	class Window
	{
	public:
		class WinException : public frt::Exception
		{
		public:
			static std::string TranslateErrorCode(HRESULT hr) noexcept;
			WinException(int line, const char* file, HRESULT hr) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			HRESULT GetErrorCode() const noexcept;
			std::string GetErrorDescription() const noexcept;
		private:
			HRESULT hr;
		};
		//class HrException : public Exception
		//{
		//public:
		//	HrException(int line, const char* file, HRESULT hr) noexcept;
		//	const char* what() const noexcept override;
		//	const char* GetType() const noexcept override;
		//	HRESULT GetErrorCode() const noexcept;
		//	std::string GetErrorDescription() const noexcept;
		//private:
		//	HRESULT hr;
		//};
		//class NoGfxException : public Exception
		//{
		//public:
		//	using Exception::Exception;
		//	const char* GetType() const noexcept override;
		//};

	public:
		Window(int width, int height, const char* name, HICON icon)
	#ifndef _DEBUG
			noexcept
	#endif // _DEBUG
			;
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		void SetTitle(const std::string& title);
		//void EnableCursor() noexcept;
		//void DisableCursor() noexcept;
		//bool CursorEnabled() const noexcept;
		//static std::optional<int> ProcessMessages() noexcept;
		//Graphics& Gfx();
	private:
		//void ConfineCursor() noexcept;
		//void FreeCursor() noexcept;
		//void ShowCursor() noexcept;
		//void HideCursor() noexcept;
		//void EnableImGuiMouse() noexcept;
		//void DisableImGuiMouse() noexcept;
		static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	public:
		Keyboard keyboard;
		Mouse mouse;
	private:
		//bool cursorEnabled = true;
		int width;
		int height;
		HWND hWnd;
		HINSTANCE hInst;

		const char* windowClassName = "FRT Window Base Class";

		//std::unique_ptr<Graphics> pGfx;
		//std::vector<BYTE> rawBuffer;
		//std::string commandLine;

		void RegisterWinAPIClass(HICON winIcon) noexcept;

	};

	#define WINDOW_EXCEPT( hr ) Window::WinException(__LINE__, __FILE__, hr);
	#define WINDOW_LAST_EXCEPT( ) Window::WinException(__LINE__, __FILE__, GetLastError());
}
