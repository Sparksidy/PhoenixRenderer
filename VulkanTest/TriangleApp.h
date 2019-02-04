#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <iostream>

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

	bool checkValidationSupport();


#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif



	GLFWwindow* window = nullptr;

	VkInstance instance;

	const std::vector<const char* > validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

};
