#pragma once
#include "../Program.h"
struct ValidationLayer {
        uint32_t LayersNumber;
        const char *LayerNames[];
} validationLayer;

//Error Handle definition is in Program.h
struct ErrorHandle CheckValidationLayerSupport() {
    vkEnumerateInstanceLayerProperties(&validationLayer.LayersNumber, NULL);

    if(validationLayer.LayersNumber == 0)
    {Error("No valid layers available", -1);}

    VkLayerProperties *AvailableLayers = (VkLayerProperties *)malloc(validationLayer.LayersNumber * sizeof(VkLayerProperties));
    
    if(!AvailableLayers)
    {Error("Failed to allocate memory for validation layers", 1);}

    vkEnumerateInstanceLayerProperties(&validationLayer.LayersNumber, AvailableLayers);

    for(int i = 0; i < validationLayer.LayersNumber; ++i)
    {validationLayer.LayerNames[i] = AvailableLayers[i].layerName;}

    free(AvailableLayers);
    AvailableLayers = NULL;
    
    return (struct ErrorHandle){"Success"};
};
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
            .enabledLayerCount = validationLayer.LayersNumber,
            .ppEnabledLayerNames = validationLayer.LayerNames
        },
        ProgramState->Allocator, 
        &ProgramState->initialize.Instance);

    if(Result != VK_SUCCESS) 
    {Error("Failed to Create Instance", Result);}
    return (struct ErrorHandle){"Success"};
};