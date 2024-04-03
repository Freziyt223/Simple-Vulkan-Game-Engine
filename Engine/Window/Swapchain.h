#include "../Program.h"
void swapchainImagesGet(Program *ProgramState) {
    VkResult EnumerateResult = vkGetSwapchainImagesKHR(ProgramState->initialize.Device, ProgramState->window.swapchain.Handle, &ProgramState->window.swapchain.ImageNumber, NULL);

    if(EnumerateResult != VK_SUCCESS)
    {Error("Failed to get swapchain images", EnumerateResult);}

    ProgramState->window.swapchain.Images = (VkImage *)malloc(ProgramState->window.swapchain.ImageNumber*sizeof(VkImage));

    if(!ProgramState->window.swapchain.Images) 
    {Error("Couldn't allocate memory for swapchain images", 1);}

    VkResult Result = vkGetSwapchainImagesKHR(ProgramState->initialize.Device, ProgramState->window.swapchain.Handle, &ProgramState->window.swapchain.ImageNumber, ProgramState->window.swapchain.Images);

    if(Result != VK_SUCCESS)
    {Error("Failed to use swapchain images", Result);}

};

void swapchainImageViewsCreate(Program *ProgramState) {
    ProgramState->window.swapchain.ImageViews = (VkImageView *)malloc(ProgramState->window.swapchain.ImageNumber*sizeof(VkImageView));

    if(!ProgramState->window.swapchain.ImageViews) 
    {Error("Couldn't allocate memory for swapchain image views", 1);}

    for (int i = 0; i < ProgramState->window.swapchain.ImageNumber; ++i) {
        VkResult Result = vkCreateImageView(
        ProgramState->initialize.Device, 
        &(VkImageViewCreateInfo) 
        {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = ProgramState->window.swapchain.Images[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = ProgramState->window.SurfaceFormat.format,
            .components = (VkComponentMapping){},
            .subresourceRange = (VkImageSubresourceRange) {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .levelCount = 1,
                .layerCount = 1,
            },
            
        }, 
        ProgramState->Allocator, 
        &ProgramState->window.swapchain.ImageViews[i]);
        if(Result != VK_SUCCESS)
        {Error("Failed to create image view", Result);}
    }
}

struct ErrorHandle CreateSwapChain(Program *ProgramState) {
    VkResult Result = vkCreateSwapchainKHR(
        ProgramState->initialize.Device, 
        &(VkSwapchainCreateInfoKHR) 
        {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface = ProgramState->window.Surface, 
            .minImageCount = clamp(ProgramState->window.swapchain.Buffering, ProgramState->window.SurfaceCapabilities.minImageCount, ProgramState->window.SurfaceCapabilities.maxImageCount ? ProgramState->window.SurfaceCapabilities.maxImageCount : UINT32_MAX),
            .imageFormat = ProgramState->window.SurfaceFormat.format,
            .imageColorSpace = ProgramState->window.SurfaceFormat.colorSpace,
            .imageExtent = ProgramState->window.SurfaceCapabilities.currentExtent,
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices = &ProgramState->initialize.QueueFamily_index,
            .preTransform = ProgramState->window.SurfaceCapabilities.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = VK_PRESENT_MODE_FIFO_KHR,
            .clipped = true,
           
        },
        ProgramState->Allocator,
        &ProgramState->window.swapchain.Handle);

        if(Result != VK_SUCCESS)
        {Error("Failed to create swapchain", Result);}

        swapchainImagesGet(ProgramState);
        swapchainImageViewsCreate(ProgramState);
        return (struct ErrorHandle){"Success"};
};