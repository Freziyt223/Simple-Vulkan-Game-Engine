#include "Window/Window.h"
//if exit, destroy glfw components
void ExitHandle() {
    glfwTerminate();
};
//Configurate and initialize window and it's parameters
void InitializeProgram(Program *ProgramState) {
    glfwInit();
    WindowCreate(ProgramState);
};
//Get updated data about program
void UpdateProgram(Program *ProgramState) {
    while(!glfwWindowShouldClose(ProgramState->window.Handle)) {
        glfwPollEvents();
    }
};
//Clean program and memory
void TerminateFunctions(Program *ProgramState) {
    for (int i = 0; i < ProgramState->window.swapchain.ImageNumber; ++i) {
        vkDestroyImageView(ProgramState->initialize.Device, ProgramState->window.swapchain.ImageViews[i], ProgramState->Allocator);
    }
    free(ProgramState->window.swapchain.ImageViews);
    free(ProgramState->window.swapchain.Images);

    vkDestroySwapchainKHR(ProgramState->initialize.Device, ProgramState->window.swapchain.Handle, ProgramState->Allocator);
    vkDestroySurfaceKHR(ProgramState->initialize.Instance, ProgramState->window.Surface, ProgramState->Allocator);
    glfwDestroyWindow(ProgramState->window.Handle);
    vkDestroyDevice(ProgramState->initialize.Device, ProgramState->Allocator);
    vkDestroyInstance(ProgramState->initialize.Instance, ProgramState->Allocator);
};
void Log() {
    uint32_t instanceApiVersion;
    VkResult Result = vkEnumerateInstanceVersion(&instanceApiVersion);
    if(Result != VK_SUCCESS)
    {Error("Failed to get Vulkan Version", Result);}
    uint32_t apiVersionVariant = VK_API_VERSION_VARIANT(instanceApiVersion);
    uint32_t apiVersionMajor = VK_API_VERSION_MAJOR(instanceApiVersion);
    uint32_t apiVersionMinor = VK_API_VERSION_MINOR(instanceApiVersion);
    uint32_t apiVersionPatch = VK_API_VERSION_PATCH(instanceApiVersion);
    printf("Vulkan API %i.%i.%i.%i\n", apiVersionVariant, apiVersionMajor, apiVersionMinor, apiVersionPatch);
    printf("GLFW %s\n", glfwGetVersionString());
}
int main() 
{
    //Initialize base window properties,
    Program program = {
        .window = {
            .Title = "Vulkan Engine",
            .Width = 1260,
            .Height = 720,
            .swapchain = {
                .Buffering = 3
            }
        },
        .ApiVersion = VK_API_VERSION_1_3
    };
    Log();
    //Configurate and make a window,
    InitializeProgram(&program);
    //Update window and it's parameters,
    UpdateProgram(&program);
    //When no updates needed, end the work with window and program,
    TerminateFunctions(&program);
    atexit(ExitHandle);
    //If program was ended without errors return success.
    return 0;
};