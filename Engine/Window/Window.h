#pragma once
#include "Instance.h"
#include "Device.h"
#include "Swapchain.h"
#include "Surface.h"
void WindowCreate(Program *ProgramState) {
    CreateInstance(ProgramState);

    DeviceConfigurate(ProgramState);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    ProgramState->window.Handle = glfwCreateWindow(ProgramState->window.Width, ProgramState->window.Height, ProgramState->window.Title, NULL, NULL);
    
    CreateWindowSurface(ProgramState);
    CreateSwapChain(ProgramState);

};