#include "window/window.h"
#include "window/input.h"

#include "engine/scene/scene.h"
#include "engine/runtime/framepacer.h"

#include  <vulkan/vulkan_raii.hpp>

int main() {

    GlfwContext glfw;
    Window window(800, 600, "kokaVK");
	Input input(window);
    Framepacer framepacer;
	Scene scene("scene.json");

    VkInstance instance;
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "kokaVK";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 5, 1);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.ppEnabledLayerNames = nullptr;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance");
    }

    VkSurfaceKHR rawsurface;
    if(glfwCreateWindowSurface(instance, window.GetWindowPtr(), nullptr, &rawsurface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface");
    }

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("no GPUs with vulkan support");
    }

    std::vector<VkPhysicalDevice> device(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, device.data());

    VkPhysicalDevice physicalDevice = device[0];

	while (!window.ShouldClose())
	{
		window.PollEvents();
		input.Update(*scene.GetCameraList()[0], framepacer.GetDeltaTime());
		framepacer.Start();

        // start here lol
        
		window.SwapBuffers();

		framepacer.End();
	}

    vkDestroySurfaceKHR(instance, rawsurface, nullptr);
    vkDestroyInstance(instance, nullptr);

    return 0;
}
