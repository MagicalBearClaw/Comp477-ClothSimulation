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
		FixedUpdate(1.0f / 60.0f);
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
	{
		std::cout << "Failed to initialize GLDFW" << std::endl;
		return false;

	}

	std::cout << "Initializing GLDFW" << std::endl;
	_glslVersion = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // Important in Mac
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	_window = glfwCreateWindow(_windowWith, _windowHeight, _windowTitle.c_str(), nullptr, nullptr);
	if (_window == nullptr)
		return false;
	glfwMakeContextCurrent(_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	std::cout << "Initialized GLAD" << std::endl;
	// tell GLFW to capture our mouse
	//glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetFramebufferSizeCallback(_window, WindowResizeCallback);
	std::cout << "Set window resize callback" << std::endl;
	glfwSetCursorPosCallback(_window, MouseCallback);
	glfwSetScrollCallback(_window, ScrollCallBack);
	std::cout << "Set window mouse input callback" << std::endl;
	glfwSetWindowUserPointer(_window, this);
	std::cout << "Set user pointer to application framework" << std::endl;
}
