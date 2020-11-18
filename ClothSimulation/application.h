#pragma once
#include "stop_watch.h"
#include "render_context.h"


class Application
{
public:
	Application(const std::string& windowTitle, int windowWith, int windowHeight);
	int Run();
protected:
	virtual bool Initialize();
	virtual bool HandleWindowEvent(SDL_Event& event) = 0;
	virtual void Draw(float deltaTime) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void FixedUpdate(float deltaTime) = 0;

protected:
	std::string _windowTitle;
	int _windowWith;
	int _windowHeight;

	SDL_Window* _window;
	HWND _windowHandle;
	RenderContext* _renderContext;
	StopWatch _stopWatch;
};