#pragma once

#include "vk.h"

class Vk_Demo {
public:
    Vk_Demo();
    ~Vk_Demo();

    void run();

private:
    void create_descriptor_sets();
    void create_pipeline_layouts();
    void create_shader_modules();
    void create_pipelines();

private:
	VkBuffer src_buffer = VK_NULL_HANDLE;
	void* src_buffer_ptr = nullptr;

	VkBuffer dst_buffer = VK_NULL_HANDLE;
	void* dst_buffer_ptr = nullptr;

    VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
	VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
    VkDescriptorSet descriptor_set = VK_NULL_HANDLE;

	VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;

	VkShaderModule compute_shader = VK_NULL_HANDLE;

	VkPipeline pipeline = VK_NULL_HANDLE;
};
