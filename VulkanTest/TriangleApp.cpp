#include "TriangleApp.h"

void TriangleApp::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(800, 600, "Phoenix Renderer", nullptr, nullptr);

}

void TriangleApp::initVulkan() {

	createInstance();
	setupDebugCallback(); 
	pickPhysicalDevice();
	createLogicalDevice();
}

void TriangleApp::mainLoop()
{
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void TriangleApp::cleanup()
{
	vkDestroyInstance(instance, nullptr);
	vkDestroyDevice(logicalDevice, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();
}

void TriangleApp::createInstance()
{

	if (enableValidationLayers && !checkValidationSupport())
	{
		throw std::runtime_error("Validation layers requested but not supported! ");
	}

	//Create application info
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	//Create Instance Information
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	//Get the glfw extensions
	uint32_t glfwExtensionsCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
	createInfo.enabledExtensionCount = glfwExtensionsCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	createInfo.enabledLayerCount = 0;

	//Create the vkInstance and verify if created successfully
	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}

	//Get the extension poperties 
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);

	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	std::cout << "available extensions:" << std::endl;

	for (const auto& extension : extensions) {
		std::cout << "\t" << extension.extensionName << std::endl;
	}
}

bool TriangleApp::checkValidationSupport()
{
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	//Check if the validation layers is available in the available layers
	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for (const auto& layers : availableLayers)
		{
			if (strcmp(layerName, layers.layerName)) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
			return false;
	}

	return true;
}

void TriangleApp::setupDebugCallback()
{
	
}

void TriangleApp::pickPhysicalDevice()
{
	uint32_t physicalDevicesCount = 0;
	vkEnumeratePhysicalDevices(instance, &physicalDevicesCount, &physicalDevice);

	if (physicalDevicesCount == 0)
	{
		std::runtime_error("Failed to find the GPU's with vulkan support");
	}

	std::vector<VkPhysicalDevice> physicalDevices;
	vkEnumeratePhysicalDevices(instance, &physicalDevicesCount, physicalDevices.data());

	for (auto device : physicalDevices)
	{
		if (isDeviceSuitable(device)) {
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE)
	{
		std::runtime_error("Unable to find a suitable GPU");
	}
}

bool TriangleApp::isDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = findQueueFamilies(device);

	return indices.isComplete();
}

void TriangleApp::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

	VkDeviceQueueCreateInfo queuecreateInfo = {};

	queuecreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queuecreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
	queuecreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queuecreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;


	createInfo.pQueueCreateInfos = &queuecreateInfo;
	createInfo.queueCreateInfoCount = 1;

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = 0;

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
}

