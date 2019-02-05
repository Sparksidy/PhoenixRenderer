#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <iostream>
#include <optional>

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

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		bool checkValidationSupport();

		void setupDebugCallback();  //TODO

		void pickPhysicalDevice();

		bool isDeviceSuitable(VkPhysicalDevice device);

		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;
			
			bool isComplete() {
				return graphicsFamily.has_value();
			}
		};

		

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
			QueueFamilyIndices indices;

			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

			int i = 0;
			for (const auto& queueFamily : queueFamilies) {
				if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					indices.graphicsFamily = i;
				}

				if (indices.isComplete()) {
					break;
				}

				i++;
			}

			return indices;

		}

		void createLogicalDevice();


		GLFWwindow* window = nullptr;

		VkInstance instance;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		VkDevice logicalDevice;

		VkQueue graphicsQueue;

		const std::vector<const char* > validationLayers = {
			"VK_LAYER_LUNARG_standard_validation"
		};

};
