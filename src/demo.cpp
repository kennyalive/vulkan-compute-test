#include "demo.h"
#include "resource_manager.h"
#include "vk.h"

#include <array>

const int buffer_size = 1024 * 1024 * sizeof(int32_t);

Vk_Demo::Vk_Demo() {
    vk_initialize();
    get_resource_manager()->initialize(vk.device);

    src_buffer = vk_create_host_visible_buffer(buffer_size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, &src_buffer_ptr);
    dst_buffer = vk_create_host_visible_buffer(buffer_size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, &dst_buffer_ptr);

    auto p_src_data = static_cast<int32_t*>(src_buffer_ptr);
    for (int32_t i = 0; i < buffer_size / sizeof(int32_t); i++) {
        p_src_data[i] = i;
    }

    create_descriptor_sets();
    create_pipeline_layouts();
    create_shader_modules();
    create_pipelines();
}

Vk_Demo::~Vk_Demo() {
    VK_CHECK(vkDeviceWaitIdle(vk.device));
    get_resource_manager()->release_resources();
    vk_shutdown();
}

void Vk_Demo::create_descriptor_sets() {
    //
    // Descriptor pool.
    //
    {
        VkDescriptorPoolSize pool_size;
        pool_size.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        pool_size.descriptorCount = 2;

        VkDescriptorPoolCreateInfo desc;
        desc.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        desc.pNext = nullptr;
        desc.flags = 0;
        desc.maxSets = 1;
        desc.poolSizeCount = 1;
        desc.pPoolSizes = &pool_size;

        descriptor_pool = get_resource_manager()->create_descriptor_pool(desc);
    }

    //
    // Descriptor set layouts.
    //
    {
        VkDescriptorSetLayoutBinding descriptor_bindings[2];
        descriptor_bindings[0].binding = 0;
        descriptor_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptor_bindings[0].descriptorCount = 1;
        descriptor_bindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
        descriptor_bindings[0].pImmutableSamplers = nullptr;

        descriptor_bindings[1].binding = 1;
        descriptor_bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptor_bindings[1].descriptorCount = 1;
        descriptor_bindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
        descriptor_bindings[1].pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutCreateInfo desc;
        desc.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        desc.pNext = nullptr;
        desc.flags = 0;
        desc.bindingCount = 2;
        desc.pBindings = descriptor_bindings;
        descriptor_set_layout = get_resource_manager()->create_descriptor_set_layout(desc);
    }

    //
    // Descriptor sets.
    //
    {
        VkDescriptorSetAllocateInfo desc;
        desc.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        desc.pNext = nullptr;
        desc.descriptorPool = descriptor_pool;
        desc.descriptorSetCount = 1;
        desc.pSetLayouts = &descriptor_set_layout;
        VK_CHECK(vkAllocateDescriptorSets(vk.device, &desc, &descriptor_set));

        VkBuffer buffers[2] = { src_buffer, dst_buffer };

        for (int i = 0; i < 2; i++) {
            VkDescriptorBufferInfo buffer_info;
            buffer_info.buffer = buffers[i];
            buffer_info.offset = 0;
            buffer_info.range = buffer_size;

            VkWriteDescriptorSet descriptor_write;
            descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptor_write.pNext = nullptr;
            descriptor_write.dstSet = descriptor_set;
            descriptor_write.dstBinding = i;
            descriptor_write.dstArrayElement = 0;
            descriptor_write.descriptorCount = 1;
            descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            descriptor_write.pImageInfo = nullptr;
            descriptor_write.pBufferInfo = &buffer_info;
            descriptor_write.pTexelBufferView = nullptr;

            vkUpdateDescriptorSets(vk.device, 1, &descriptor_write, 0, nullptr);
        }
    }
}

void Vk_Demo::create_pipeline_layouts() {
    VkPipelineLayoutCreateInfo desc;
    desc.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    desc.pNext = nullptr;
    desc.flags = 0;
    desc.setLayoutCount = 1;
    desc.pSetLayouts = &descriptor_set_layout;
    desc.pushConstantRangeCount = 0;
    desc.pPushConstantRanges = nullptr;
    pipeline_layout = get_resource_manager()->create_pipeline_layout(desc);
}

void Vk_Demo::create_shader_modules() {
    auto create_shader_module = [](uint8_t* bytes, long long count) {
        if (count % 4 != 0) {
            error("Vulkan: SPIR-V binary buffer size is not multiple of 4");
        }
        VkShaderModuleCreateInfo desc;
        desc.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        desc.pNext = nullptr;
        desc.flags = 0;
        desc.codeSize = count;
        desc.pCode = reinterpret_cast<const uint32_t*>(bytes);

        return get_resource_manager()->create_shader_module(desc);
    };

    extern unsigned char shader_comp_spv[];
    extern long long shader_comp_spv_size;
    compute_shader = create_shader_module(shader_comp_spv, shader_comp_spv_size);
}

void Vk_Demo::create_pipelines() {
    VkPipelineShaderStageCreateInfo shader_stage;
    shader_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage.pNext = nullptr;
    shader_stage.flags = 0;
    shader_stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    shader_stage.module = compute_shader;
    shader_stage.pName = "main";
    shader_stage.pSpecializationInfo = nullptr;

    VkComputePipelineCreateInfo desc;
    desc.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    desc.pNext = nullptr;
    desc.flags = 0;
    desc.stage = shader_stage;
    desc.layout = pipeline_layout;
    desc.basePipelineHandle = VK_NULL_HANDLE;
    desc.basePipelineIndex = -1;

    pipeline = get_resource_manager()->create_compute_pipeline(desc);
}

void Vk_Demo::run() {
    VkCommandBufferBeginInfo begin_info;
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.pNext = nullptr;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    begin_info.pInheritanceInfo = nullptr;

    VK_CHECK(vkBeginCommandBuffer(vk.command_buffer, &begin_info));

    // Ensure that written data is visible to compute stage.
    vk_record_buffer_memory_barrier(vk.command_buffer, src_buffer,
        VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);

    // Run computations.
    vkCmdBindDescriptorSets(vk.command_buffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline_layout, 0, 1, &descriptor_set, 0, nullptr);
    vkCmdBindPipeline(vk.command_buffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);
    vkCmdDispatch(vk.command_buffer, 1024, 1, 1);

    VK_CHECK(vkEndCommandBuffer(vk.command_buffer));

    VkSubmitInfo submit_info;
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = nullptr;
    submit_info.waitSemaphoreCount = 0;
    submit_info.pWaitSemaphores = nullptr;
    submit_info.pWaitDstStageMask = nullptr;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &vk.command_buffer;
    submit_info.signalSemaphoreCount = 0;
    submit_info.pSignalSemaphores = nullptr;
    VK_CHECK(vkQueueSubmit(vk.queue, 1, &submit_info, VK_NULL_HANDLE));

    vkDeviceWaitIdle(vk.device);

    bool success = (memcmp(src_buffer_ptr, dst_buffer_ptr, buffer_size) == 0);
    printf("Validation result: %s\n", success ? "CORRECT" : "ERROR");
}
