#pragma once

#include "vk_definitions.h"

#include <functional>
#include <string>
#include <vector>

void error(const std::string& message);

#define VK_CHECK(function_call) { \
    VkResult result = function_call; \
    if (result < 0) \
        error("Vulkan: error code " + std::to_string(result) + " returned by " + #function_call); \
}

void vk_initialize();

void vk_shutdown();

VkBuffer vk_create_host_visible_buffer(VkDeviceSize size, VkBufferUsageFlags usage, void** buffer_ptr);

void vk_record_buffer_memory_barrier(VkCommandBuffer cb, VkBuffer buffer,
                                     VkPipelineStageFlags src_stages, VkPipelineStageFlags dst_stages,
                                     VkAccessFlags src_access, VkAccessFlags dst_access);

// Vk_Instance contains vulkan resources that do not depend on applicaton logic.
// This structure is initialized/deinitialized by vk_initialize/vk_shutdown functions correspondingly.
struct Vk_Instance {
    HMODULE vulkan_library = NULL;

    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;

    uint32_t queue_family_index = 0;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue queue = VK_NULL_HANDLE;

    VkCommandPool command_pool = VK_NULL_HANDLE;
    VkCommandBuffer command_buffer = VK_NULL_HANDLE;

    //
    // Memory allocation.
    //
    struct Chunk {
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkDeviceSize used = 0;
        uint32_t memory_type_index = -1;
        void* data = nullptr; // only for host visible memory
    };
    std::vector<Chunk> device_local_chunks;
    std::vector<Chunk> host_visible_chunks;
};

extern Vk_Instance vk;

extern PFN_vkGetInstanceProcAddr                        vkGetInstanceProcAddr;

extern PFN_vkCreateInstance                             vkCreateInstance;
extern PFN_vkEnumerateInstanceExtensionProperties       vkEnumerateInstanceExtensionProperties;

extern PFN_vkCreateDevice                               vkCreateDevice;
extern PFN_vkDestroyInstance                            vkDestroyInstance;
extern PFN_vkEnumerateDeviceExtensionProperties         vkEnumerateDeviceExtensionProperties;
extern PFN_vkEnumeratePhysicalDevices                   vkEnumeratePhysicalDevices;
extern PFN_vkGetDeviceProcAddr                          vkGetDeviceProcAddr;
extern PFN_vkGetPhysicalDeviceFeatures                  vkGetPhysicalDeviceFeatures;
extern PFN_vkGetPhysicalDeviceFormatProperties          vkGetPhysicalDeviceFormatProperties;
extern PFN_vkGetPhysicalDeviceMemoryProperties          vkGetPhysicalDeviceMemoryProperties;
extern PFN_vkGetPhysicalDeviceProperties                vkGetPhysicalDeviceProperties;
extern PFN_vkGetPhysicalDeviceQueueFamilyProperties     vkGetPhysicalDeviceQueueFamilyProperties;
extern PFN_vkCreateWin32SurfaceKHR                      vkCreateWin32SurfaceKHR;
extern PFN_vkDestroySurfaceKHR                          vkDestroySurfaceKHR;
extern PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR    vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
extern PFN_vkGetPhysicalDeviceSurfaceFormatsKHR         vkGetPhysicalDeviceSurfaceFormatsKHR;
extern PFN_vkGetPhysicalDeviceSurfacePresentModesKHR    vkGetPhysicalDeviceSurfacePresentModesKHR;
extern PFN_vkGetPhysicalDeviceSurfaceSupportKHR         vkGetPhysicalDeviceSurfaceSupportKHR;

extern PFN_vkAllocateCommandBuffers                     vkAllocateCommandBuffers;
extern PFN_vkAllocateDescriptorSets                     vkAllocateDescriptorSets;
extern PFN_vkAllocateMemory                             vkAllocateMemory;
extern PFN_vkBeginCommandBuffer                         vkBeginCommandBuffer;
extern PFN_vkBindBufferMemory                           vkBindBufferMemory;
extern PFN_vkBindImageMemory                            vkBindImageMemory;
extern PFN_vkCmdBeginRenderPass                         vkCmdBeginRenderPass;
extern PFN_vkCmdBindDescriptorSets                      vkCmdBindDescriptorSets;
extern PFN_vkCmdBindIndexBuffer                         vkCmdBindIndexBuffer;
extern PFN_vkCmdBindPipeline                            vkCmdBindPipeline;
extern PFN_vkCmdBindVertexBuffers                       vkCmdBindVertexBuffers;
extern PFN_vkCmdBlitImage                               vkCmdBlitImage;
extern PFN_vkCmdClearAttachments                        vkCmdClearAttachments;
extern PFN_vkCmdCopyBufferToImage                       vkCmdCopyBufferToImage;
extern PFN_vkCmdCopyImage                               vkCmdCopyImage;
extern PFN_vkCmdCopyBuffer                              vkCmdCopyBuffer;
extern PFN_vkCmdDispatch                                vkCmdDispatch;
extern PFN_vkCmdDraw                                    vkCmdDraw;
extern PFN_vkCmdDrawIndexed                             vkCmdDrawIndexed;
extern PFN_vkCmdEndRenderPass                           vkCmdEndRenderPass;
extern PFN_vkCmdPipelineBarrier                         vkCmdPipelineBarrier;
extern PFN_vkCmdPushConstants                           vkCmdPushConstants;
extern PFN_vkCmdSetDepthBias                            vkCmdSetDepthBias;
extern PFN_vkCmdSetScissor                              vkCmdSetScissor;
extern PFN_vkCmdSetViewport                             vkCmdSetViewport;
extern PFN_vkCreateBuffer                               vkCreateBuffer;
extern PFN_vkCreateCommandPool                          vkCreateCommandPool;
extern PFN_vkCreateComputePipelines                     vkCreateComputePipelines;
extern PFN_vkCreateDescriptorPool                       vkCreateDescriptorPool;
extern PFN_vkCreateDescriptorSetLayout                  vkCreateDescriptorSetLayout;
extern PFN_vkCreateFence                                vkCreateFence;
extern PFN_vkCreateFramebuffer                          vkCreateFramebuffer;
extern PFN_vkCreateGraphicsPipelines                    vkCreateGraphicsPipelines;
extern PFN_vkCreateImage                                vkCreateImage;
extern PFN_vkCreateImageView                            vkCreateImageView;
extern PFN_vkCreatePipelineLayout                       vkCreatePipelineLayout;
extern PFN_vkCreateRenderPass                           vkCreateRenderPass;
extern PFN_vkCreateSampler                              vkCreateSampler;
extern PFN_vkCreateSemaphore                            vkCreateSemaphore;
extern PFN_vkCreateShaderModule                         vkCreateShaderModule;
extern PFN_vkDestroyBuffer                              vkDestroyBuffer;
extern PFN_vkDestroyCommandPool                         vkDestroyCommandPool;
extern PFN_vkDestroyDescriptorPool                      vkDestroyDescriptorPool;
extern PFN_vkDestroyDescriptorSetLayout                 vkDestroyDescriptorSetLayout;
extern PFN_vkDestroyDevice                              vkDestroyDevice;
extern PFN_vkDestroyFence                               vkDestroyFence;
extern PFN_vkDestroyFramebuffer                         vkDestroyFramebuffer;
extern PFN_vkDestroyImage                               vkDestroyImage;
extern PFN_vkDestroyImageView                           vkDestroyImageView;
extern PFN_vkDestroyPipeline                            vkDestroyPipeline;
extern PFN_vkDestroyPipelineLayout                      vkDestroyPipelineLayout;
extern PFN_vkDestroyRenderPass                          vkDestroyRenderPass;
extern PFN_vkDestroySampler                             vkDestroySampler;
extern PFN_vkDestroySemaphore                           vkDestroySemaphore;
extern PFN_vkDestroyShaderModule                        vkDestroyShaderModule;
extern PFN_vkDeviceWaitIdle                             vkDeviceWaitIdle;
extern PFN_vkEndCommandBuffer                           vkEndCommandBuffer;
extern PFN_vkFreeCommandBuffers                         vkFreeCommandBuffers;
extern PFN_vkFreeDescriptorSets                         vkFreeDescriptorSets;
extern PFN_vkFreeMemory                                 vkFreeMemory;
extern PFN_vkGetBufferMemoryRequirements                vkGetBufferMemoryRequirements;
extern PFN_vkGetDeviceQueue                             vkGetDeviceQueue;
extern PFN_vkGetImageMemoryRequirements                 vkGetImageMemoryRequirements;
extern PFN_vkGetImageSubresourceLayout                  vkGetImageSubresourceLayout;
extern PFN_vkMapMemory                                  vkMapMemory;
extern PFN_vkQueueSubmit                                vkQueueSubmit;
extern PFN_vkQueueWaitIdle                              vkQueueWaitIdle;
extern PFN_vkResetDescriptorPool                        vkResetDescriptorPool;
extern PFN_vkResetFences                                vkResetFences;
extern PFN_vkUnmapMemory                                vkUnmapMemory;
extern PFN_vkUpdateDescriptorSets                       vkUpdateDescriptorSets;
extern PFN_vkWaitForFences                              vkWaitForFences;
extern PFN_vkAcquireNextImageKHR                        vkAcquireNextImageKHR;
extern PFN_vkCreateSwapchainKHR                         vkCreateSwapchainKHR;
extern PFN_vkDestroySwapchainKHR                        vkDestroySwapchainKHR;
extern PFN_vkGetSwapchainImagesKHR                      vkGetSwapchainImagesKHR;
extern PFN_vkQueuePresentKHR                            vkQueuePresentKHR;
