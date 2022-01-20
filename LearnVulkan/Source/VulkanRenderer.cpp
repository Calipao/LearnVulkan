#include "Precompiled.h"
#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer()
{
    m_instance = nullptr;
    m_window = nullptr;

    m_physical_device = nullptr;
    m_device = nullptr;
}

VulkanRenderer::~VulkanRenderer()
{
    m_window = nullptr;
}

int VulkanRenderer::Init(GLFWwindow* _window)
{
    m_window = _window;

    CreateInstance();
    GetPhysicalDevice();
    CreateDevice();

    return 0;
}

void VulkanRenderer::Exit()
{
    vkDestroyDevice(m_device, nullptr);
    vkDestroyInstance(m_instance, nullptr);
}

void VulkanRenderer::CreateInstance()
{
    //Application info
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "NONE lol";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    //Prepare data for creating vulkan instance
    auto instanceExtensions = std::vector<const char*>();
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    for (size_t i = 0; i < glfwExtensionCount; i++)
    {
        instanceExtensions.push_back(glfwExtensions[i]);
    }
    if(!CheckInstanceExtensionSupport(&instanceExtensions))
    {
        std::cout << "ERROR: VkInstance does not support required extensions\n";
    }

    //Creation info for a vulkan instance
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
    createInfo.ppEnabledExtensionNames = instanceExtensions.data();
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;

    //Create the actual instance
    VkResult res = vkCreateInstance(&createInfo, nullptr, &m_instance);
    if (res != VK_SUCCESS)
    {
        std::cout << "ERROR: VkInstance failed to create\n";
    }

    //Reload GLAD after creating instance and device to make sure all extensions are loaded
    gladLoaderLoadVulkan(m_instance, nullptr, nullptr);
}

bool VulkanRenderer::CheckInstanceExtensionSupport(const std::vector<const char*>* _checkExtensions)
{
    //Get count first
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    //Get the extensions with the count
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    //Check if extensions are available
    for(const auto &exts : *_checkExtensions)
    {
        bool hasExt = false;
        for(const auto &ext : extensions)
        {
            if(strcmp(exts, ext.extensionName))
            {
                hasExt = true;
                break;
            }
        }

        if (!hasExt)
            return false;
    }

    return true;
}

void VulkanRenderer::GetPhysicalDevice()
{
    //Get count first
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

    //if no device available, means no devices have vulkan support
    if(deviceCount == 0)
    {
        std::cout << "ERROR: No available devices that support vulkan\n";
    }

    //Get all available devices
    std::vector<VkPhysicalDevice> deviceList(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, deviceList.data());

    //Get the physical device
    for(const auto &device : deviceList)
    {
        if(CheckDeviceSuitable(device))
        {
            m_physical_device = device;
            break;
        }
    }
}

bool VulkanRenderer::CheckDeviceSuitable(VkPhysicalDevice _device)
{
    //Get info about device itself
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(_device, &device_properties);

    //Get info about device capabilities and limits
    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(_device, &device_features);

    QueueFamilyIndices indices = GetQueueFamilies(_device);

    return indices.IsValid();
}

void VulkanRenderer::CreateDevice()
{
    QueueFamilyIndices indices = GetQueueFamilies(m_physical_device);

    float priority = 1.0f;
    VkDeviceQueueCreateInfo queue_info = {};
    queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info.queueFamilyIndex = indices.graphicsFamily;
    queue_info.queueCount = 1;
    queue_info.pQueuePriorities = &priority;

    VkPhysicalDeviceFeatures device_features = {};
    VkDeviceCreateInfo device_info = {};
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.queueCreateInfoCount = 1;
    device_info.pQueueCreateInfos = &queue_info;
    device_info.enabledExtensionCount = 0;
    device_info.ppEnabledExtensionNames = nullptr;
    device_info.pEnabledFeatures = &device_features;

    VkResult res = vkCreateDevice(m_physical_device, &device_info, nullptr, &m_device);
    if(res != VK_SUCCESS)
    {
        std::cout << "ERROR: Failed to create device\n";
    }

    //Queues are created same time as device
    vkGetDeviceQueue(m_device, indices.graphicsFamily, 0, &m_graphics_queue);
}

QueueFamilyIndices VulkanRenderer::GetQueueFamilies(VkPhysicalDevice _device)
{
    QueueFamilyIndices queue_family_indices;

    //Get count first
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, nullptr);

    //Get all queue family properties
    std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, queueFamilyList.data());

    //Find at least 1 of the required queue family
    int i = 0;
    for (const auto& qFamily : queueFamilyList)
    {
        if (qFamily.queueCount > 0 && qFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queue_family_indices.graphicsFamily = i;
        }

        if(queue_family_indices.IsValid())
        {
            break;
        }

        i++;
    }

    return queue_family_indices;
}
