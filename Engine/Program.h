#pragma once
#include "Include.h"
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

const char *ValidationLayers[] = {"VK_LAYER_KHRONOS_validation"};

typedef struct {
    VkSwapchainKHR Handle;

    uint32_t ImageNumber;
    VkImage *Images;
    VkImageView *ImageViews;
    uint32_t Buffering;

    VkFramebuffer FrameBuffers;
} Swapchain;
typedef struct {

    Swapchain swapchain;

    GLFWwindow *Handle;
    const char *Title;
    int Width, Height;
    bool Resizeable;
    uint8_t ScreenMode;
    VkSurfaceKHR Surface;
    VkSurfaceCapabilitiesKHR SurfaceCapabilities;
    VkSurfaceFormatKHR SurfaceFormat;
    VkPresentModeKHR PresentMode;

    int FrameBufferWidth, FrameBufferHeight;
} Window;
typedef struct {
    VkInstance Instance;
    VkPhysicalDevice PhysicalDevice;

    uint32_t QueueFamily_index;
    VkDevice Device;
    VkQueue Queue;
} Init;
typedef struct {
    Window window;
    Init initialize;
    uint32_t ApiVersion;
    const char *ApplicationName;
    const char *EngineName;
    VkAllocationCallbacks *Allocator;
} Program;
//Gets parameters of error Reason(const char *) and Error Code(int)
struct ErrorHandle {
    const char *Reason;
    int Code;
};
//Function, that prints error details, returns error data and exits the program. Usage: if there is error possibility, do the state check(using if function) and then "Error("Description of possible error", code(must be an int value))"
struct ErrorHandle Error(const char *Reason, int Code) {
    printf("Error occur: %s, exited with code: %i \n", Reason, Code);
    struct ErrorHandle Return = {
        .Reason = Reason,
        .Code = Code,
    };
    return Return;
    exit(1);
};
void ErrorCheck(int Code, const char *Reason) {
    fprintf(stderr, "GLFW Error: %d, %s\n", Code, Reason);
};
uint32_t clamp(uint32_t value, uint32_t min, uint32_t max) {
    if(value < min) {
        return min;
    } else if(value > max) {
        return max;
    }
    return value;
}
struct Pipeline {
    VkPipelineLayout PipelineLayout;
    VkRenderPass RenderPass;
    VkPipeline GraphicsPipeline;
} GraphicsPipeline;