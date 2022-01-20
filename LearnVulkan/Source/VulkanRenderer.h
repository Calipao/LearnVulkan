#pragma once

#define GLAD_VULKAN_IMPLEMENTATION
#include <glad/vulkan.h>

#include <GLFW/glfw3.h>

#include "Utils.h"

class VulkanRenderer
{
public:
    VulkanRenderer();
    ~VulkanRenderer();

    int Init(GLFWwindow* _window);
    void Exit();

private:
    GLFWwindow* m_window;

    //Vulkan variables
    VkInstance m_instance;
    VkPhysicalDevice m_physical_device;
    VkDevice m_device;
    VkQueue m_graphics_queue;

    //Vulkan functions
    void CreateInstance();
    bool CheckInstanceExtensionSupport(const std::vector<const char*>* _checkExtensions);

    void GetPhysicalDevice();
    bool CheckDeviceSuitable(VkPhysicalDevice _device);

    void CreateDevice();

    QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice _device);
};
