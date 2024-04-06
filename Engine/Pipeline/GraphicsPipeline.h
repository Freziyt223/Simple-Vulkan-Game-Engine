#pragma once
#include "../Program.h"
struct Module {
    VkShaderModule VertexModule;
    VkShaderModule FragmentModule;
};
struct ErrorHandle CreateShaderModule(Program *ProgramState, uint32_t VertexCode[], size_t VertexSize, uint32_t FragmentCode[], size_t FragmentSize, struct Module *Modules) {

    VkResult VertexModuleResult = vkCreateShaderModule(ProgramState->initialize.Device, &(VkShaderModuleCreateInfo) {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = VertexSize,
        .pCode = VertexCode,
    }, ProgramState->Allocator, &Modules->VertexModule);

    VkResult FragmentModuleResult = vkCreateShaderModule(ProgramState->initialize.Device, &(VkShaderModuleCreateInfo) {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = FragmentSize,
        .pCode = FragmentCode,
    }, ProgramState->Allocator,&Modules->FragmentModule);

    if(VertexModuleResult != VK_SUCCESS)
    {Error("Failed to create vertex shader module", VertexModuleResult);}
    if(FragmentModuleResult != VK_SUCCESS)
    {Error("Failed to create fragment shader module", FragmentModuleResult);}

    return (struct ErrorHandle){"Success"};
};

struct ErrorHandle CreatePipelineLayout(Program *ProgramState, VkPipelineLayout *PipelineLayout) {
    VkPipelineLayoutCreateInfo PipelineLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 0,
        .pSetLayouts = NULL,

    };
    VkResult LayoutResult = vkCreatePipelineLayout(ProgramState->initialize.Device, &PipelineLayoutInfo, ProgramState->Allocator, PipelineLayout);
    
    if(LayoutResult != VK_SUCCESS)
    {Error("Failed to create layout", LayoutResult);}
    
    return (struct ErrorHandle){"Success"};
};

struct ErrorHandle CreateRenderPass(Program *ProgramState, VkRenderPass *RenderPass) {
    VkAttachmentDescription ColourAttachment = {
        .format = ProgramState->window.SurfaceFormat.format,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    };
    VkAttachmentReference ColourAttachmentReference = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,        
    };
    VkSubpassDescription SubPass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &ColourAttachmentReference,
    };
    VkRenderPassCreateInfo RenderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 1,
        .pAttachments = &ColourAttachment,
        .subpassCount = 1,
        .pSubpasses = &SubPass,

    };
    VkResult PassResult = vkCreateRenderPass(ProgramState->initialize.Device, &RenderPassInfo, ProgramState->Allocator, RenderPass);
    
    if(PassResult != VK_SUCCESS)
    {Error("Failed to create Render Pass", PassResult);}

    return (struct ErrorHandle){"Success"};
};
struct ErrorHandle CreateGraphicsPipeline(Program *ProgramState, struct Module *Modules, VkPipelineLayout PipelineLayout, VkRenderPass RenderPass, VkPipeline *GraphicsPipeline) {
    VkDynamicState Dynamics[] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineShaderStageCreateInfo ShaderStages[] = {
        (VkPipelineShaderStageCreateInfo){
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = Modules->VertexModule,
            .pName = (const char *)"main",
        },
        (VkPipelineShaderStageCreateInfo){
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = Modules->FragmentModule,
            .pName = (const char *)"main"
        }
    };

    VkPipelineDynamicStateCreateInfo DynamicState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = 2,
        .pDynamicStates = Dynamics
    };
    VkPipelineVertexInputStateCreateInfo VertexInput = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 0,
        .vertexAttributeDescriptionCount = 0,
    };
    VkPipelineInputAssemblyStateCreateInfo InputAssembly = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    };
    VkViewport ViewPort = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float)ProgramState->window.SurfaceCapabilities.currentExtent.width,
        .height = (float)ProgramState->window.SurfaceCapabilities.currentExtent.height,
        .maxDepth = 1.0f,
    };
    VkRect2D Scissors = {
        .extent = ProgramState->window.SurfaceCapabilities.currentExtent
    };
    VkPipelineViewportStateCreateInfo ViewPortInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .pViewports = &ViewPort,
        .scissorCount = 1,
        .pScissors = &Scissors,
    };
    VkPipelineRasterizationStateCreateInfo RasterizerInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .lineWidth = 1.0f,
    };
    VkPipelineMultisampleStateCreateInfo MultiSamplingInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 1.0f,
        .pSampleMask = NULL
    };
    VkPipelineColorBlendAttachmentState ColourBlend = {
        .blendEnable = VK_FALSE,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    };
    VkPipelineColorBlendStateCreateInfo ColourBlendInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &ColourBlend,

    };
    VkGraphicsPipelineCreateInfo PipelineInfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = 2,
        .pStages = ShaderStages,
        .pVertexInputState = &VertexInput,
        .pInputAssemblyState = &InputAssembly,
        .pViewportState = &ViewPortInfo,
        .pRasterizationState = &RasterizerInfo,
        .pMultisampleState = &MultiSamplingInfo,
        .pColorBlendState = &ColourBlendInfo,
        .pDynamicState = &DynamicState,
        .layout = PipelineLayout,
        .renderPass = RenderPass,
        .subpass = 0
    };

    VkResult PipelineResult = vkCreateGraphicsPipelines(ProgramState->initialize.Device, VK_NULL_HANDLE, 1, &PipelineInfo, ProgramState->Allocator, GraphicsPipeline);

    if(PipelineResult != VK_SUCCESS)
    {Error("Failed to create Graphics Pipeline", PipelineResult);}

    vkDestroyShaderModule(ProgramState->initialize.Device, Modules->VertexModule, ProgramState->Allocator);
    vkDestroyShaderModule(ProgramState->initialize.Device, Modules->FragmentModule, ProgramState->Allocator);
    return (struct ErrorHandle){"Success"};
};

struct ErrorHandle InitializeGraphicsPipeline(Program *ProgramState, uint32_t VertexCode[], size_t VertexSize, uint32_t FragmentCode[], size_t FragmentSize) {
    struct Module Modules;
    CreateShaderModule(ProgramState, VertexCode, VertexSize, FragmentCode, FragmentSize, &Modules);
    CreatePipelineLayout(ProgramState, &GraphicsPipeline.PipelineLayout);
    CreateRenderPass(ProgramState, &GraphicsPipeline.RenderPass);

    CreateGraphicsPipeline(ProgramState, &Modules, GraphicsPipeline.PipelineLayout, GraphicsPipeline.RenderPass, &GraphicsPipeline.GraphicsPipeline);
    return (struct ErrorHandle){"Success"};
};