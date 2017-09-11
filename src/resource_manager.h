#pragma once

#include "vk_definitions.h"
#include <vector>

class Resource_Manager {
public:
    void initialize(VkDevice device);
    void release_resources();

    VkSemaphore create_semaphore();
    VkCommandPool create_command_pool(const VkCommandPoolCreateInfo& desc);
    VkDescriptorPool create_descriptor_pool(const VkDescriptorPoolCreateInfo& desc);
    VkBuffer create_buffer(const VkBufferCreateInfo& desc);
    VkImage create_image(const VkImageCreateInfo& desc);
    VkImageView create_image_view(const VkImageViewCreateInfo& desc);
    VkSampler create_sampler(const VkSamplerCreateInfo& desc);
    VkRenderPass create_render_pass(const VkRenderPassCreateInfo& desc);
    VkFramebuffer create_framebuffer(const VkFramebufferCreateInfo& desc);
    VkDescriptorSetLayout create_descriptor_set_layout(const VkDescriptorSetLayoutCreateInfo& desc);
    VkPipelineLayout create_pipeline_layout(const VkPipelineLayoutCreateInfo& desc);
    VkPipeline create_graphics_pipeline(const VkGraphicsPipelineCreateInfo& desc);
    VkPipeline create_compute_pipeline(const VkComputePipelineCreateInfo& desc);
    VkShaderModule create_shader_module(const VkShaderModuleCreateInfo& desc);

private:
    VkDevice device = VK_NULL_HANDLE;
    std::vector<VkSemaphore> semaphores;
    std::vector<VkCommandPool> command_pools;
    std::vector<VkDescriptorPool> descriptor_pools;
    std::vector<VkBuffer> buffers;
    std::vector<VkImage> images;
    std::vector<VkImageView> image_views;
    std::vector<VkSampler> samplers;
    std::vector<VkDescriptorSetLayout> descriptor_set_layouts;
    std::vector<VkRenderPass> render_passes;
    std::vector<VkFramebuffer> framebuffers;
    std::vector<VkPipelineLayout> pipeline_layouts;
    std::vector<VkPipeline> pipelines;
    std::vector<VkShaderModule> shader_modules;
};

Resource_Manager* get_resource_manager();
