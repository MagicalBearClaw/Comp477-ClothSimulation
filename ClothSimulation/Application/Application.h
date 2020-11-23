#pragma once
#include "../stdafx.h"
#include "../System/StopWatch.h"

class Application
{
public:
	Application(const std::string& windowTitle, int windowWith, int windowHeight);
	~Application();
	int Run();
protected:
	virtual bool Initialize();
	virtual bool HandleWindowEvent(SDL_Event& event) = 0;
	virtual void Draw(float deltaTime) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void FixedUpdate(float deltaTime) = 0;
	virtual void ShutDOwn() = 0;
protected:
	std::string _windowTitle;
	int _windowWith;
	int _windowHeight;
	std::string _glslVersion;
	GLFWwindow* _window;
	HWND _windowHandle;
	StopWatch _stopWatch;
};