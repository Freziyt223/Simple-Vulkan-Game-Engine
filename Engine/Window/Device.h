#pragma once
#include "../Program.h"
//Error Handle definition is in Program.h


struct ErrorHandle SelectPhysicalDevice(Program *ProgramState) {
    uint32_t DeviceNumber;
    VkResult EnumerateResult = vkEnumeratePhysicalDevices(ProgramState->initialize.Instance, &DeviceNumber, NULL);

    if(EnumerateResult != VK_SUCCESS) 
    {Error("Couldn't enumerate physical devices", EnumerateResult);}

    if(DeviceNumber == 0) 
    {Error("Couldn't find a vulkan supporting device", 1); exit(1);}

    VkResult SelectingResult = vkEnumeratePhysicalDevices(ProgramState->initialize.Instance, &DeviceNumber, &ProgramState->initialize.PhysicalDevice);

    if(SelectingResult != VK_SUCCESS) 
    {Error("Couldn't select physical devices", SelectingResult);}

    return (struct ErrorHandle){"Success"};
};
struct ErrorHandle SelectQueueFamily(Program *ProgramState) {
    ProgramState->initialize.QueueFamily_index = UINT32_MAX;

    uint32_t FamilyNumber;
    vkGetPhysicalDeviceQueueFamilyProperties(ProgramState->initialize.PhysicalDevice, &FamilyNumber, NULL);

    VkQueueFamilyProperties *QueueFamilyProperties = malloc(FamilyNumber * sizeof(VkQueueFamilyProperties));

    if(QueueFamilyProperties == NULL) 
    {Error("Couldn't allocate memory to an queue families", 1);}

    vkGetPhysicalDeviceQueueFamilyProperties(ProgramState->initialize.PhysicalDevice, &FamilyNumber, QueueFamilyProperties);

    for(uint32_t FamilyIndex = 0; FamilyIndex < FamilyNumber; ++FamilyIndex) {
        VkQueueFamilyProperties Properties = QueueFamilyProperties[FamilyIndex];

        if((Properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) && glfwGetPhysicalDevicePresentationSupport(ProgramState->initialize.Instance, ProgramState->initialize.PhysicalDevice, FamilyIndex))
        {
            ProgramState->initialize.QueueFamily_index = FamilyIndex;
            break;
        }
    }

    if(ProgramState->initialize.QueueFamily_index == UINT32_MAX) 
    {Error("Couldn't find suitable queue family", 2);}

    free(QueueFamilyProperties);
    QueueFamilyProperties = NULL;
    return (struct ErrorHandle){"Success"};
};

struct ErrorHandle CreateDevice(Program *ProgramState) {
    VkResult Result = vkCreateDevice(
        ProgramState->initialize.PhysicalDevice, 
        &(VkDeviceCreateInfo)
        {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &(VkDeviceQueueCreateInfo)
            {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = ProgramState->initialize.QueueFamily_index,
                .queueCount = 1,
                .pQueuePriorities = &(float){1.0},
            },
            
            .enabledExtensionCount = 1,
            .ppEnabledExtensionNames = &(const char *){VK_KHR_SWAPCHAIN_EXTENSION_NAME},
        },
        ProgramState->Allocator, &ProgramState->initialize.Device
        );
    if(Result != VK_SUCCESS) 
    {Error("Couldn't create device", Result);}

    return (struct ErrorHandle){"Success"};
};

void GetQueue(Program *ProgramState) {
    vkGetDeviceQueue(ProgramState->initialize.Device, ProgramState->initialize.QueueFamily_index, 0, &ProgramState->initialize.Queue);
};
// Countainer for all Device functions
struct ErrorHandle DeviceConfigurate(Program *ProgramState) {
    SelectPhysicalDevice(ProgramState);
    SelectQueueFamily(ProgramState);
    CreateDevice(ProgramState);
    GetQueue(ProgramState);

    return (struct ErrorHandle){"Success"};
};