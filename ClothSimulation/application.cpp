#include "stdafx.h"
#include "application.h"

Application::Application(const std::string& windowTitle, int windowWith, int windowHeight)
	: _windowTitle(windowTitle), _windowWith(windowWith), _windowHeight(windowHeight), _renderContext(nullptr), 
	  _window(nullptr)
{
}

Application::~Application()
{
	if (_renderContext)
	{
		delete _renderContext;
	}
}

int Application::Run()
{
	bool isDOne = false;
	while (!isDOne)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			isDOne = HandleWindowEvent(event);
		}

		FixedUpdate(1/60);
		Update(_stopWatch.ElapsedTime());
		Draw(_stopWatch.ElapsedTime());
	}

	_renderContext->Shutdown();
	SDL_DestroyWindow(_window);
	SDL_Quit();

	return 0;
}


bool Application::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		OutputDebugString(SDL_GetError());
		return false;
	}

	unsigned int windowFlags = (SDL_WindowFlags::SDL_WINDOW_RESIZABLE | SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI);
	_window = SDL_CreateWindow(_windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowWith, _windowHeight, windowFlags);
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(_window, &wmInfo);
	_windowHandle = (HWND)wmInfo.info.win.window;
	_renderContext = RenderContext::CreateInstance(_windowHandle, _windowWith, _windowHeight, false);
	if (!_renderContext)
	{
		return false;
	}

	return true;
}
