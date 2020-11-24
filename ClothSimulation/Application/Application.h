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
	virtual void Draw(float deltaTime) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void FixedUpdate(float deltaTime) = 0;
	virtual void ShutDOwn() = 0;
	virtual void HandleMouseScroll(double xoffset, double yoffset) = 0;
	virtual void HandleMouse(double xPosition, double yPosition) = 0;
private:
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void WindowResizeCallback(GLFWwindow* window, int width, int height);
	static void ErrorCallback(int error, const char* description);
	static void ScrollCallBack(GLFWwindow* window, double xOffset, double yOffset);
protected:
	std::string _windowTitle;
	int _windowWith;
	int _windowHeight;
	std::string _glslVersion;
	GLFWwindow* _window;
	StopWatch _stopWatch;
};