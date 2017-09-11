#include "resource_manager.h"
#include "vk.h"

static Resource_Manager resource_manager;

Resource_Manager* get_resource_manager() {
    return &resource_manager;
}

void Resource_Manager::initialize(VkDevice device) {
    this->device = device;
}

void Resource_Manager::release_resources() {
    for (auto semaphore : semaphores) {
        vkDestroySemaphore(device, semaphore, nullptr);
    }
    semaphores.clear();

    for (auto command_pool : command_pools) {
        vkDestroyCommandPool(device, command_pool, nullptr);
    }
    command_pools.clear();

    for (auto descriptor_pool : descriptor_pools) {
        vkDestroyDescriptorPool(device, descriptor_pool, nullptr);
    }
    descriptor_pools.clear();

    for (auto buffer : buffers) {
        vkDestroyBuffer(device, buffer, nullptr);
    }
    buffers.clear();

    for (auto image : images) {
        vkDestroyImage(device, image, nullptr);
    }
    images.clear();

    for (auto image_view : image_views) {
        vkDestroyImageView(device, image_view, nullptr);
    }
    image_views.clear();

    for (auto sampler : samplers) {
        vkDestroySampler(device, sampler, nullptr);
    }
    samplers.clear();

    for (auto render_pass : render_passes) {
        vkDestroyRenderPass(device, render_pass, nullptr);
    }
    render_passes.clear();

    for (auto framebuffer : framebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
    framebuffers.clear();

    for (auto descriptor_set_layout : descriptor_set_layouts) {
        vkDestroyDescriptorSetLayout(device, descriptor_set_layout, nullptr);
    }
    descriptor_set_layouts.clear();

    for (auto pipeline_layout : pipeline_layouts) {
        vkDestroyPipelineLayout(device, pipeline_layout, nullptr);
    }
    pipeline_layouts.clear();

    for (auto pipeline : pipelines) {
        vkDestroyPipeline(device, pipeline, nullptr);
    }
    pipelines.clear();

    for (auto shader_module : shader_modules) {
        vkDestroyShaderModule(device, shader_module, nullptr);
    }
    shader_modules.clear();
}

VkSemaphore Resource_Manager::create_semaphore() {
    VkSemaphoreCreateInfo desc;
    desc.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    desc.pNext = nullptr;
    desc.flags = 0;

    VkSemaphore semaphore;
    VK_CHECK(vkCreateSemaphore(device, &desc, nullptr, &semaphore));
    semaphores.push_back(semaphore);
    return semaphore;
}

VkCommandPool Resource_Manager::create_command_pool(const VkCommandPoolCreateInfo& desc) {
    VkCommandPool command_pool;
    VK_CHECK(vkCreateCommandPool(device, &desc, nullptr, &command_pool));
    command_pools.push_back(command_pool);
    return command_pool;
}

VkDescriptorPool Resource_Manager::create_descriptor_pool(const VkDescriptorPoolCreateInfo& desc) {
    VkDescriptorPool descriptor_pool;
    VK_CHECK(vkCreateDescriptorPool(device, &desc, nullptr, &descriptor_pool));
    descriptor_pools.push_back(descriptor_pool);
    return descriptor_pool;
}

VkBuffer Resource_Manager::create_buffer(const VkBufferCreateInfo& desc) {
    VkBuffer buffer;
    VK_CHECK(vkCreateBuffer(device, &desc, nullptr, &buffer));
    buffers.push_back(buffer);
    return buffer;
}

VkImage Resource_Manager::create_image(const VkImageCreateInfo& desc) {
    VkImage image;
    VK_CHECK(vkCreateImage(device, &desc, nullptr, &image));
    images.push_back(image);
    return image;
}

VkImageView Resource_Manager::create_image_view(const VkImageViewCreateInfo& desc) {
    VkImageView image_view;
    VK_CHECK(vkCreateImageView(device, &desc, nullptr, &image_view));
    image_views.push_back(image_view);
    return image_view;
}

VkSampler Resource_Manager::create_sampler(const VkSamplerCreateInfo& desc) {
    VkSampler sampler;
    VK_CHECK(vkCreateSampler(device, &desc, nullptr, &sampler));
    samplers.push_back(sampler);
    return sampler;
}

VkRenderPass Resource_Manager::create_render_pass(const VkRenderPassCreateInfo& desc) {
    VkRenderPass render_pass;
    VK_CHECK(vkCreateRenderPass(device, &desc, nullptr, &render_pass));
    render_passes.push_back(render_pass);
    return render_pass;
}

VkFramebuffer Resource_Manager::create_framebuffer(const VkFramebufferCreateInfo& desc) {
    VkFramebuffer framebuffer;
    VK_CHECK(vkCreateFramebuffer(device, &desc, nullptr, &framebuffer));
    framebuffers.push_back(framebuffer);
    return framebuffer;
}

VkDescriptorSetLayout Resource_Manager::create_descriptor_set_layout(const VkDescriptorSetLayoutCreateInfo& desc) {
    VkDescriptorSetLayout descriptor_set_layout;
    VK_CHECK(vkCreateDescriptorSetLayout(device, &desc, nullptr, &descriptor_set_layout));
    descriptor_set_layouts.push_back(descriptor_set_layout);
    return descriptor_set_layout;
}

VkPipelineLayout Resource_Manager::create_pipeline_layout(const VkPipelineLayoutCreateInfo& desc) {
    VkPipelineLayout pipeline_layout;
    VK_CHECK(vkCreatePipelineLayout(device, &desc, nullptr, &pipeline_layout));
    pipeline_layouts.push_back(pipeline_layout);
    return pipeline_layout;
}

VkPipeline Resource_Manager::create_graphics_pipeline(const VkGraphicsPipelineCreateInfo& desc) {
    VkPipeline pipeline;
    VK_CHECK(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &desc, nullptr, &pipeline));
    pipelines.push_back(pipeline);
    return pipeline;
}

VkPipeline Resource_Manager::create_compute_pipeline(const VkComputePipelineCreateInfo& desc) {
    VkPipeline pipeline;
    VK_CHECK(vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &desc, nullptr, &pipeline));
    pipelines.push_back(pipeline);
    return pipeline;
}

VkShaderModule Resource_Manager::create_shader_module(const VkShaderModuleCreateInfo& desc) {
    VkShaderModule shader_module;
    VK_CHECK(vkCreateShaderModule(vk.device, &desc, nullptr, &shader_module));
    shader_modules.push_back(shader_module);
    return shader_module;
}
