#include "Precompiled.h"

//#define GLAD_VULKAN_IMPLEMENTATION
#include <glad/vulkan.h>

//#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "VulkanRenderer.h"

GLFWwindow* window;
VulkanRenderer renderer;

void InitWindow(std::string _name = "Vulkan Window", const int _width = 800, const int height = 600)
{
    //Initialise GLAD
    gladLoaderLoadVulkan(nullptr, nullptr, nullptr);

    //Initialise GLFW
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);
}

int main()
{
    //Create window
    InitWindow();

    //Create vulkan renderer
    renderer.Init(window);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    //Destroy vulkan renderer
    renderer.Exit();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}