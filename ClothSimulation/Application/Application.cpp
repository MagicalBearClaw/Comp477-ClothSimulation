#include "../stdafx.h"
#include "application.h"

Application::Application(const std::string& windowTitle, int windowWith, int windowHeight)
	: _windowTitle(windowTitle), _windowWith(windowWith), _windowHeight(windowHeight), 
	  _window(nullptr)
{
}

Application::~Application()
{
}

static void GLFWErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
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

	ShutDOwn();
	glfwDestroyWindow(_window);
	glfwTerminate();

	return 0;
}


bool Application::Initialize()
{
	glfwSetErrorCallback(GLFWErrorCallback);
	if (!glfwInit())
		return false;

	_glslVersion = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(_windowWith, _windowHeight, _windowTitle.c_str(), nullptr, nullptr);
	if (_window == nullptr)
		return false;

	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1); // Enable vsync

}
