#pragma once
#include "../Program.h"
//Error Handle definition is in Program.h

struct ErrorHandle CreateInstance(Program *ProgramState) {
    uint32_t ExtensionsNumber;
    const char **RequiredExtensions = glfwGetRequiredInstanceExtensions(&ExtensionsNumber);
    VkResult Result = vkCreateInstance(
        &(VkInstanceCreateInfo)
        {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &(VkApplicationInfo) 
            {
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pApplicationName = ProgramState->ApplicationName,
                .pEngineName = ProgramState->EngineName,
                .apiVersion = ProgramState->ApiVersion,
            },
            .enabledExtensionCount = ExtensionsNumber,
            .ppEnabledExtensionNames = RequiredExtensions,
            .enabledLayerCount = 1,
            .ppEnabledLayerNames = ValidationLayers,
        },
        ProgramState->Allocator, 
        &ProgramState->initialize.Instance);
    if(Result != VK_SUCCESS) 
    {Error("Failed to Create Instance", Result);}
    return (struct ErrorHandle){"Success"};
};