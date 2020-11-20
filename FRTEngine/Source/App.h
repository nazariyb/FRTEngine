#pragma once
#include "Window.h"


using frt::Window;

namespace frt
{
	class App
	{
	public:
		App() = default;
		App(int width, int height, const char* name, HICON icon);
		virtual ~App() {};

		virtual int Start() { return 0; };

	protected:
		virtual void Update() {};

		Window window;
	};
}

