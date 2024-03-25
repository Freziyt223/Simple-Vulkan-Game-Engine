#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
typedef struct {
    const char *Title;
    int Width, Height;
    bool Resizable;
    bool Fullscreen;

    uint32_t api_version;

    GLFWmonitor *WindowMonitor;
    GLFWwindow *window;

    VkAllocationCallbacks *allocator;
    VkInstance Instance;
    VkPhysicalDevice SelectPhysicalDevice;
    VkSurfaceKHR Surface;
    uint32_t QueueFamily;
    VkDevice Device;
    VkQueue Queue;
} Program;
#define ERRORCHECK(ERROR, FORMAT, ...) {if(ERROR) { fprintf(stderr, "%s, at %s function, at %i line, with %i exit code:\n\t" FORMAT "\n", __FILE_NAME__, __FUNCTION__, __LINE__, ERROR, ##__VA_ARGS__); raise(SIGABRT);}}

void glfwErrorCallback(int error_code, const char* description) {
    ERRORCHECK(error_code, "GLFW: %s", description)
}

void exitCallback() {
    glfwTerminate();
}

void CreateWindow(Program *WindowState) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, WindowState->Resizable);

    if(WindowState->Fullscreen) {
        WindowState->WindowMonitor = glfwGetPrimaryMonitor();
    }

    WindowState->window = glfwCreateWindow(WindowState->Width, WindowState->Height, WindowState->Title, WindowState->WindowMonitor, NULL);
};
void CreateInstance(Program *WindowState) {
    uint32_t extension_count;
    const char **RequiredExtensions = glfwGetRequiredInstanceExtensions(&extension_count);
    VkApplicationInfo ApplicationInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .apiVersion = WindowState -> api_version,
    };
    VkInstanceCreateInfo CreateInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &ApplicationInfo,
        .enabledExtensionCount = extension_count,
        .ppEnabledExtensionNames = RequiredExtensions,
    };
    ERRORCHECK(vkCreateInstance(&CreateInfo, WindowState -> allocator, &WindowState -> Instance), "Couldn't create an instance");
};
void SelectPhysicalDevice(Program *WindowState) {
    uint32_t count;
    ERRORCHECK(vkEnumeratePhysicalDevices(WindowState->Instance, &count, NULL), "Couldn't enumerate physical devices")
    ERRORCHECK(count == 0, "Couldn't find any device that supports Vulkan");
    ERRORCHECK(vkEnumeratePhysicalDevices(WindowState->Instance, &count, &WindowState->SelectPhysicalDevice), "Couldn't enumerate physical device");
};
void CreateSurface(Program *WindowState) {
    ERRORCHECK(glfwCreateWindowSurface(WindowState->Instance, WindowState->window, WindowState->allocator, &WindowState->Surface), "Couldn't create a window surface");
};
void SelectQueueFamily(Program *WindowState) {
    WindowState->QueueFamily = UINT32_MAX;
    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(WindowState->SelectPhysicalDevice, &count, NULL);
    VkQueueFamilyProperties *QueueFamilies = malloc(count*sizeof(VkQueueFamilyProperties));
    ERRORCHECK(QueueFamilies == NULL, "Couldn't allocate memory for device");
    vkGetPhysicalDeviceQueueFamilyProperties(WindowState->SelectPhysicalDevice, &count, QueueFamilies);
    for(int QueueFamilyIndex = 0; QueueFamilyIndex < count; ++QueueFamilyIndex) {
        VkQueueFamilyProperties Properties = QueueFamilies[QueueFamilyIndex];
        if(Properties.queueFlags & VK_QUEUE_GRAPHICS_BIT && glfwGetPhysicalDevicePresentationSupport(WindowState->Instance, WindowState->SelectPhysicalDevice, QueueFamilyIndex)) {
            WindowState->QueueFamily = QueueFamilyIndex;
            break;
        }
    };
    ERRORCHECK(WindowState->QueueFamily == UINT32_MAX, "Couldn't find usable queue family");
    free(QueueFamilies);
};
void CreateDevice(Program *WindowState) {
    ERRORCHECK(vkCreateDevice(WindowState->SelectPhysicalDevice, &(VkDeviceCreateInfo) {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pQueueCreateInfos = & (VkDeviceQueueCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = WindowState->QueueFamily,
            .queueCount = 1,
            .pQueuePriorities = &(float){1.0}
        },
        .queueCreateInfoCount = 1,
        .enabledExtensionCount = 1,
        .ppEnabledExtensionNames = &(const char *){VK_KHR_SWAPCHAIN_EXTENSION_NAME}
    }, WindowState->allocator, &WindowState->Device), "Couldn't create device with queues");
};
void GetQueue(Program *WindowState) {
    vkGetDeviceQueue(WindowState->Device, WindowState->QueueFamily, 0, &WindowState->Queue);
};
void init(Program *WindowState) {
    CreateWindow(WindowState);
    CreateInstance(WindowState);
    SelectPhysicalDevice(WindowState);
    CreateSurface(WindowState);
    SelectQueueFamily(WindowState);
    CreateDevice(WindowState);
    GetQueue(WindowState);
}

void loop(Program *WindowState) {
    while(!glfwWindowShouldClose(WindowState->window)) {
        glfwPollEvents();
    }
}

void cleanup(Program *WindowState) {
    vkDestroyDevice(WindowState->Device, WindowState->allocator);   
    vkDestroySurfaceKHR(WindowState->Instance, WindowState->Surface, WindowState->allocator);
    vkDestroyInstance(WindowState->Instance, WindowState->allocator);
};
int main() {
    Program Window = {
        .Title = "Привіт Світ",
        .Width = 720,
        .Height = 480,
        .Resizable = false,
        .Fullscreen = false,
        .api_version = VK_API_VERSION_1_3,
    };
    glfwSetErrorCallback(glfwErrorCallback);
    atexit(exitCallback);
    init(&Window);
    loop(&Window);
    cleanup(&Window);
    return EXIT_SUCCESS;
};