#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <set>
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

/*		MEMBER FUNCTIONS   */
		void initWindow();

		void initVulkan();

		void mainLoop();

		void cleanup();


		void createInstance();

		void createSurface();

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;

#define VK_EXT_DEBUG_UTILS_EXTENSION_NAME "VK_EXT_debug_utils"

#endif

		bool checkValidationSupport();

		std::vector<const char* > getRequiredExtensions();

		void setupDebugCallback();  

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData
		) {
			std::cerr << "Validation Layer: " << pCallbackData->pMessage << '\n';

			return VK_FALSE;
		}

		void pickPhysicalDevice();

		void createLogicalDevice();

		bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
		
		bool isDeviceSuitable(VkPhysicalDevice device);

		struct QueueFamilyIndices;														//Forward Declaration
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		struct SwapChainSupportDetails;
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice);



/*		MEMBER DATA */

		GLFWwindow* window = nullptr;

		VkInstance instance;

		VkDebugUtilsMessengerEXT debugMessenger;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		VkDevice logicalDevice;

		VkQueue graphicsQueue;

		VkSurfaceKHR surface;

		VkQueue presentQueue;

		const std::vector<const char* > validationLayers = {
			"VK_LAYER_LUNARG_standard_validation"
		};

		const std::vector<const char* > deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		struct QueueFamilyIndices {

			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentationFamily;

			bool isComplete() {
				return  graphicsFamily.has_value() && presentationFamily.has_value();
			}
		};

		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

};
