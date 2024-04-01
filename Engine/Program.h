#pragma once
#include "Include.h"
typedef struct {
    GLFWwindow *Handle;
    const char *Title;
    int Width, Height;
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
struct ErrorHandle {
    const char *Reason;
    int Code;
};
struct ErrorHandle Error(const char *Reason, int Code) {
    printf("Error occur: %s, exited with code: %i \n", Reason, Code);
    struct ErrorHandle Return = {
        .Reason = Reason,
        .Code = Code,
    };
    return Return;
    exit(1);
};