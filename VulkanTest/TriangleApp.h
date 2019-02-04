#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class TriangleApp {

public:
	void run()
	{
		initWindow();

		initVulkan();

		mainLoop();

		cleanup();
	}


private:
	void initWindow();

	void initVulkan();

	void mainLoop();

	void cleanup();


	void createInstance();


	GLFWwindow* window = nullptr;

	VkInstance instance;
};
