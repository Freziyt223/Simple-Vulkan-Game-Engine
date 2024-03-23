#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#define ERROROCCUR(ERROR, FORMAT, ...) {if(ERROR) { fprintf(stderr, "%s, at %s function, at %i line, with %i exit code:\n\t" FORMAT "\n", __FILE_NAME__, __FUNCTION__, __LINE__, ERROR, ##__VA_ARGS__); raise(SIGABRT);}}

void glfwErrorCallback(int error_code, const char* description) {
    ERROROCCUR(error_code, "GLFW: %s", description)
}

void exitCallback() {
    glfwTerminate();
}

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
} State;


void createWindow(State *WindowState) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, WindowState->Resizable);

    if(WindowState->Fullscreen) {
        WindowState->WindowMonitor = glfwGetPrimaryMonitor();
    }

    WindowState->window = glfwCreateWindow(WindowState->Width, WindowState->Height, WindowState->Title, WindowState->WindowMonitor, NULL);
}
void createInstance(State *WindowState) {
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
    ERROROCCUR(vkCreateInstance(&CreateInfo, WindowState -> allocator, &WindowState -> Instance), "Couldn't create an instance");
};
void init(State *WindowState) {
    createWindow(WindowState);
}

void loop(State *WindowState) {
    while(!glfwWindowShouldClose(WindowState->window)) {
        glfwPollEvents();
    }
}

void cleanup(State *WindowState) {
    glfwDestroyWindow(WindowState->window);
    vkDestroyInstance(WindowState->Instance, WindowState->allocator);
}

int main() {
    State Window = {
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