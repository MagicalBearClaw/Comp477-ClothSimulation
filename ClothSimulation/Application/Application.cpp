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

void Application::ErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void Application::WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Application::MouseCallback(GLFWwindow* window, double xPosition, double yPosition)
{
	Application* application = (Application*)glfwGetWindowUserPointer(window);
	application->HandleMouse(xPosition, yPosition);
}

void Application::ScrollCallBack(GLFWwindow* window, double xOffset, double yOffset)
{
	Application* application = (Application*)glfwGetWindowUserPointer(window);
	application->HandleMouseScroll(xOffset, yOffset);
}

int Application::Run()
{
	while (!glfwWindowShouldClose(_window))
	{
		glfwPollEvents();
		FixedUpdate(1 / 60);
		Update(_stopWatch.ElapsedTime());
		Draw(_stopWatch.ElapsedTime());
		glfwSwapBuffers(_window);

	}

	ShutDOwn();
	glfwDestroyWindow(_window);
	glfwTerminate();

	return 0;
}


bool Application::Initialize()
{
	glfwSetErrorCallback(ErrorCallback);
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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}


	glfwSetFramebufferSizeCallback(_window, WindowResizeCallback);

	glfwSetCursorPosCallback(_window, MouseCallback);
	glfwSetScrollCallback(_window, ScrollCallBack);

	glfwSetWindowUserPointer(_window, this);
}
