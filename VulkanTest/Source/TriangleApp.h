#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <array>
#include <set>
#include <fstream>
#include <iostream>
#include <optional>
#include <algorithm>

#include <glm/glm.hpp>

const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;


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

		struct QueueFamilyIndices;														
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		struct SwapChainSupportDetails;
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice);

		VkSurfaceFormatKHR  chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		void createSwapChain();

		void createImageViews();

		void createGraphicsPipeline();

		std::vector<char> readFile(const std::string& fileName);

		VkShaderModule createShaderModule(std::vector<char> shaderCode);

		void createRenderPass();

		void createFramebuffers();

		void createCommandPool();

		void createCommandBuffers();

		void drawFrame();

		void createSyncObjects();

		void recreateSwapChain();

		void cleanupSwapChain();

		static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
		{
			auto app = reinterpret_cast<TriangleApp*>(glfwGetWindowUserPointer(window));
			app->framebufferResized = true;
		}

		void createVertexBuffer();

		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

		void copyBuffer(VkBuffer srcbuffer, VkBuffer destbuffer, VkDeviceSize size);

		void createIndexBuffer();

/*		MEMBER DATA */

		GLFWwindow* window = nullptr;

		VkInstance instance;

		VkDebugUtilsMessengerEXT debugMessenger;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		VkDevice logicalDevice;

		VkQueue graphicsQueue;

		VkSurfaceKHR surface;

		VkQueue presentQueue;

		VkSwapchainKHR swapChain;

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

		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;

		std::vector<VkImageView> swapChainImageViews;

		VkRenderPass renderPass;

		VkPipelineLayout pipelineLayout;

		VkPipeline graphicsPipeline;

		std::vector<VkFramebuffer> swapChainFramebuffers;

		VkCommandPool commandPool;

		std::vector<VkCommandBuffer> commandBuffers;

		std::vector<VkSemaphore> imageAvailableSemaphore;

		std::vector<VkSemaphore> renderFinishedSemaphore;

		std::vector<VkFence> inFlightFences;

		size_t currentFrame = 0;

		bool framebufferResized = false;

		struct Vertex {
			glm::vec2 pos;
			glm::vec3 color;

			static VkVertexInputBindingDescription getBindingDescription()
			{
				VkVertexInputBindingDescription description = {};

				description.binding = 0;
				description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
				description.stride = sizeof(Vertex);

				return description;
			}

			static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescription()
			{
				std::array<VkVertexInputAttributeDescription, 2> description = {};

				description[0].binding = 0;
				description[0].format = VK_FORMAT_R32G32_SFLOAT;
				description[0].location = 0;
				description[0].offset = offsetof(Vertex, pos);


				description[1].binding = 0;
				description[1].location = 1;
				description[1].format = VK_FORMAT_R32G32B32_SFLOAT;
				description[1].offset = offsetof(Vertex, color);

				return description;
			}
		};

		const std::vector<Vertex> vertices{
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};

		const std::vector<uint16_t> indices{
			0, 1, 2, 2, 3, 0
		};

		VkBuffer vertexBuffer;

		VkDeviceMemory vertexBufferMemory;

		VkBuffer indexBuffer;

		VkDeviceMemory indexBufferMemory;
};
