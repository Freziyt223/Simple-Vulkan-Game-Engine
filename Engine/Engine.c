#include "Window/Window.h"
void ExitHandle() {
    glfwTerminate();
};
void InitializeProgram(Program *ProgramState) {
    glfwInit();
    CreateWindow(ProgramState);
};
void UpdateProgram(Program *ProgramState) {
    while(!glfwWindowShouldClose(ProgramState->window.Handle)) {
        glfwPollEvents();
    }
};
void TerminateFunctions(Program *ProgramState) {
    glfwDestroyWindow(ProgramState->window.Handle);
    vkDestroyDevice(ProgramState->initialize.Device, ProgramState->Allocator);
    vkDestroyInstance(ProgramState->initialize.Instance, ProgramState->Allocator);
};
int main() 
{
    Program program = {
        .window = {
            .Title = "Vulkan Engine",
            .Width = 1260,
            .Height = 720
        },
        .ApiVersion = VK_API_VERSION_1_3
    };
    InitializeProgram(&program);
    UpdateProgram(&program);
    TerminateFunctions(&program);
    atexit(ExitHandle);
    return 0;
};