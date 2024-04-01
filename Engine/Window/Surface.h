#pragma once
#include "../Program.h"

struct ErrorHandle CreateWindowSurface(Program *ProgramState) {
    VkResult SurfaceResult = glfwCreateWindowSurface(ProgramState->initialize.Instance, ProgramState->window.Handle, ProgramState->Allocator, &ProgramState->window.Surface);
    
    if(SurfaceResult != VK_SUCCESS)
    {Error("Failed to create a window surface", SurfaceResult);}
    return (struct ErrorHandle){"Success"};
};

struct ErrorHandle GetSurfaceCapabilities(Program *ProgramState) {
    VkResult CapabilitiesResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ProgramState->initialize.PhysicalDevice, ProgramState->window.Surface, &ProgramState->window.SurfaceCapabilities);
    
    if(CapabilitiesResult != VK_SUCCESS) 
    {Error("Failed to get window surface capabilities", CapabilitiesResult);}
    
    return (struct ErrorHandle){"Success"};
};
struct ErrorHandle surfaceFormatsSelect(Program *ProgramState) {
    uint32_t FormatNumber;
    VkResult NumberResult = vkGetPhysicalDeviceSurfaceFormatsKHR(ProgramState->initialize.PhysicalDevice, ProgramState->window.Surface, &FormatNumber, NULL);

    if(NumberResult != VK_SUCCESS)
    {Error("Couldn't get surface formats", NumberResult);}

    VkSurfaceFormatKHR *Formats = (VkSurfaceFormatKHR *)malloc(FormatNumber * sizeof(VkSurfaceFormatKHR));

    if(!Formats)
    {Error("Couldn't allocate formats memory for surface formats", 1);}

    VkResult FormatResult = vkGetPhysicalDeviceSurfaceFormatsKHR(ProgramState->initialize.PhysicalDevice, ProgramState->window.Surface, &FormatNumber, Formats);

    if(FormatResult != VK_SUCCESS)
    {Error("Couldn't get surface format", FormatResult);}

    for (int i = 0; i < FormatNumber; ++i) {
        ProgramState->window.SurfaceFormat = Formats[i];
        if(ProgramState->window.SurfaceFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR && ProgramState->window.SurfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB) {
            break;
        }
    }

    
    free(Formats);
    return (struct ErrorHandle){"Success"};
};

struct ErrorHandle surfacePresentModesSelect(Program *ProgramState) {
    ProgramState->window.PresentMode = VK_PRESENT_MODE_FIFO_KHR;

    uint32_t presentModeNumber;
    VkResult NumberResult = vkGetPhysicalDeviceSurfacePresentModesKHR(ProgramState->initialize.PhysicalDevice, ProgramState->window.Surface, &presentModeNumber, NULL);
    if(NumberResult != VK_SUCCESS)
    {Error("Failed to get present modes", NumberResult);}
    VkPresentModeKHR *presentModes = (VkPresentModeKHR *)malloc(presentModeNumber*sizeof(VkPresentModeKHR));
    
    if(!presentModes) 
    {Error("Couldn't allocate present modes memory", 1);}

    VkResult ModeResult = vkGetPhysicalDeviceSurfacePresentModesKHR(ProgramState->initialize.PhysicalDevice, ProgramState->window.Surface, &presentModeNumber, presentModes);
    if(ModeResult != VK_SUCCESS)
    {Error("Failed to initialize present modes", ModeResult);}
    uint32_t presentModeIndex = UINT32_MAX;

    for (int i = 0; i < presentModeNumber; ++i) {
        VkPresentModeKHR Mode = presentModes[i];
        if(Mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            presentModeIndex = i;
            break;
        }
    }

    if(presentModeIndex != UINT32_MAX) {
        ProgramState->window.PresentMode = presentModes[presentModeIndex];
    }

    free(presentModes);
    return (struct ErrorHandle){"Success"};
};