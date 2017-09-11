#include "vk.h"
#include "resource_manager.h"

#include <array>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <vector>

const int DEVICE_LOCAL_CHUNK_SIZE = 16 * 1024 * 1024;
const int HOST_VISIBLE_CHUNK_SIZE = 8 * 1024 * 1024;

Vk_Instance vk;

//
// Vulkan API functions used by the renderer.
//
PFN_vkGetInstanceProcAddr                       vkGetInstanceProcAddr;

PFN_vkCreateInstance                            vkCreateInstance;
PFN_vkEnumerateInstanceExtensionProperties      vkEnumerateInstanceExtensionProperties;

PFN_vkCreateDevice                              vkCreateDevice;
PFN_vkDestroyInstance                           vkDestroyInstance;
PFN_vkEnumerateDeviceExtensionProperties        vkEnumerateDeviceExtensionProperties;
PFN_vkEnumeratePhysicalDevices                  vkEnumeratePhysicalDevices;
PFN_vkGetDeviceProcAddr                         vkGetDeviceProcAddr;
PFN_vkGetPhysicalDeviceFeatures                 vkGetPhysicalDeviceFeatures;
PFN_vkGetPhysicalDeviceFormatProperties         vkGetPhysicalDeviceFormatProperties;
PFN_vkGetPhysicalDeviceMemoryProperties         vkGetPhysicalDeviceMemoryProperties;
PFN_vkGetPhysicalDeviceProperties               vkGetPhysicalDeviceProperties;
PFN_vkGetPhysicalDeviceQueueFamilyProperties    vkGetPhysicalDeviceQueueFamilyProperties;
PFN_vkCreateWin32SurfaceKHR                     vkCreateWin32SurfaceKHR;
PFN_vkDestroySurfaceKHR                         vkDestroySurfaceKHR;
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR   vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR        vkGetPhysicalDeviceSurfaceFormatsKHR;
PFN_vkGetPhysicalDeviceSurfacePresentModesKHR   vkGetPhysicalDeviceSurfacePresentModesKHR;
PFN_vkGetPhysicalDeviceSurfaceSupportKHR        vkGetPhysicalDeviceSurfaceSupportKHR;

PFN_vkAllocateCommandBuffers                    vkAllocateCommandBuffers;
PFN_vkAllocateDescriptorSets                    vkAllocateDescriptorSets;
PFN_vkAllocateMemory                            vkAllocateMemory;
PFN_vkBeginCommandBuffer                        vkBeginCommandBuffer;
PFN_vkBindBufferMemory                          vkBindBufferMemory;
PFN_vkBindImageMemory                           vkBindImageMemory;
PFN_vkCmdBeginRenderPass                        vkCmdBeginRenderPass;
PFN_vkCmdBindDescriptorSets                     vkCmdBindDescriptorSets;
PFN_vkCmdBindIndexBuffer                        vkCmdBindIndexBuffer;
PFN_vkCmdBindPipeline                           vkCmdBindPipeline;
PFN_vkCmdBindVertexBuffers                      vkCmdBindVertexBuffers;
PFN_vkCmdBlitImage                              vkCmdBlitImage;
PFN_vkCmdClearAttachments                       vkCmdClearAttachments;
PFN_vkCmdCopyBufferToImage                      vkCmdCopyBufferToImage;
PFN_vkCmdCopyImage                              vkCmdCopyImage;
PFN_vkCmdCopyBuffer                             vkCmdCopyBuffer;
PFN_vkCmdDispatch                               vkCmdDispatch;
PFN_vkCmdDraw                                   vkCmdDraw;
PFN_vkCmdDrawIndexed                            vkCmdDrawIndexed;
PFN_vkCmdEndRenderPass                          vkCmdEndRenderPass;
PFN_vkCmdPipelineBarrier                        vkCmdPipelineBarrier;
PFN_vkCmdPushConstants                          vkCmdPushConstants;
PFN_vkCmdSetDepthBias                           vkCmdSetDepthBias;
PFN_vkCmdSetScissor                             vkCmdSetScissor;
PFN_vkCmdSetViewport                            vkCmdSetViewport;
PFN_vkCreateBuffer                              vkCreateBuffer;
PFN_vkCreateCommandPool                         vkCreateCommandPool;
PFN_vkCreateComputePipelines                    vkCreateComputePipelines;
PFN_vkCreateDescriptorPool                      vkCreateDescriptorPool;
PFN_vkCreateDescriptorSetLayout                 vkCreateDescriptorSetLayout;
PFN_vkCreateFence                               vkCreateFence;
PFN_vkCreateFramebuffer                         vkCreateFramebuffer;
PFN_vkCreateGraphicsPipelines                   vkCreateGraphicsPipelines;
PFN_vkCreateImage                               vkCreateImage;
PFN_vkCreateImageView                           vkCreateImageView;
PFN_vkCreatePipelineLayout                      vkCreatePipelineLayout;
PFN_vkCreateRenderPass                          vkCreateRenderPass;
PFN_vkCreateSampler                             vkCreateSampler;
PFN_vkCreateSemaphore                           vkCreateSemaphore;
PFN_vkCreateShaderModule                        vkCreateShaderModule;
PFN_vkDestroyBuffer                             vkDestroyBuffer;
PFN_vkDestroyCommandPool                        vkDestroyCommandPool;
PFN_vkDestroyDescriptorPool                     vkDestroyDescriptorPool;
PFN_vkDestroyDescriptorSetLayout                vkDestroyDescriptorSetLayout;
PFN_vkDestroyDevice                             vkDestroyDevice;
PFN_vkDestroyFence                              vkDestroyFence;
PFN_vkDestroyFramebuffer                        vkDestroyFramebuffer;
PFN_vkDestroyImage                              vkDestroyImage;
PFN_vkDestroyImageView                          vkDestroyImageView;
PFN_vkDestroyPipeline                           vkDestroyPipeline;
PFN_vkDestroyPipelineLayout                     vkDestroyPipelineLayout;
PFN_vkDestroyRenderPass                         vkDestroyRenderPass;
PFN_vkDestroySampler                            vkDestroySampler;
PFN_vkDestroySemaphore                          vkDestroySemaphore;
PFN_vkDestroyShaderModule                       vkDestroyShaderModule;
PFN_vkDeviceWaitIdle                            vkDeviceWaitIdle;
PFN_vkEndCommandBuffer                          vkEndCommandBuffer;
PFN_vkFreeCommandBuffers                        vkFreeCommandBuffers;
PFN_vkFreeDescriptorSets                        vkFreeDescriptorSets;
PFN_vkFreeMemory                                vkFreeMemory;
PFN_vkGetBufferMemoryRequirements               vkGetBufferMemoryRequirements;
PFN_vkGetDeviceQueue                            vkGetDeviceQueue;
PFN_vkGetImageMemoryRequirements                vkGetImageMemoryRequirements;
PFN_vkGetImageSubresourceLayout                 vkGetImageSubresourceLayout;
PFN_vkMapMemory                                 vkMapMemory;
PFN_vkQueueSubmit                               vkQueueSubmit;
PFN_vkQueueWaitIdle                             vkQueueWaitIdle;
PFN_vkResetDescriptorPool                       vkResetDescriptorPool;
PFN_vkResetFences                               vkResetFences;
PFN_vkUnmapMemory                               vkUnmapMemory;
PFN_vkUpdateDescriptorSets                      vkUpdateDescriptorSets;
PFN_vkWaitForFences                             vkWaitForFences;
PFN_vkAcquireNextImageKHR                       vkAcquireNextImageKHR;
PFN_vkCreateSwapchainKHR                        vkCreateSwapchainKHR;
PFN_vkDestroySwapchainKHR                       vkDestroySwapchainKHR;
PFN_vkGetSwapchainImagesKHR                     vkGetSwapchainImagesKHR;
PFN_vkQueuePresentKHR                           vkQueuePresentKHR;
////////////////////////////////////////////////////////////////////////////

static uint32_t find_memory_type(VkPhysicalDevice physical_device, uint32_t memory_type_bits, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {
        if ((memory_type_bits & (1 << i)) != 0 &&
            (memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    error("Vulkan: failed to find matching memory type with requested properties");
    return -1;
}

struct Allocation {
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkDeviceSize offset = 0;
    void* data = nullptr;
};

static Allocation allocate_memory(const VkMemoryRequirements& memory_requirements, bool host_visible) {
    Vk_Instance::Chunk* chunk = nullptr;

    auto& chunks = host_visible ? vk.host_visible_chunks : vk.device_local_chunks;
    VkDeviceSize chunk_size = host_visible ? HOST_VISIBLE_CHUNK_SIZE : DEVICE_LOCAL_CHUNK_SIZE;

    // Try to find an existing chunk of sufficient capacity.
    const auto mask = ~(memory_requirements.alignment - 1);
    for (size_t i = 0; i < chunks.size(); i++) {
        if (((1 << chunks[i].memory_type_index) & memory_requirements.memoryTypeBits) == 0)
            continue;

        // ensure that memory region has proper alignment
        VkDeviceSize offset = (chunks[i].used + memory_requirements.alignment - 1) & mask;

        if (offset + memory_requirements.size <= chunk_size) {
            chunks[i].used = offset + memory_requirements.size;
            chunk = &chunks[i];
            break;
        }
    }

    // Allocate a new chunk in case we couldn't find suitable existing chunk.
    if (chunk == nullptr) {
        VkMemoryAllocateInfo alloc_info;
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.pNext = nullptr;
        alloc_info.allocationSize = std::max(chunk_size, memory_requirements.size);
        alloc_info.memoryTypeIndex = find_memory_type(vk.physical_device, memory_requirements.memoryTypeBits,
            host_visible
                ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
                : VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        VkDeviceMemory memory;
        VK_CHECK(vkAllocateMemory(vk.device, &alloc_info, nullptr, &memory));

        chunks.push_back(Vk_Instance::Chunk());
        chunk = &chunks.back();
        chunk->memory = memory;
        chunk->used = memory_requirements.size;
        chunk->memory_type_index = alloc_info.memoryTypeIndex;

        if (host_visible) {
            VK_CHECK(vkMapMemory(vk.device, chunk->memory, 0, VK_WHOLE_SIZE, 0, &chunk->data));
        }
    }

    Allocation alloc;
    alloc.memory = chunk->memory;
    alloc.offset = chunk->used - memory_requirements.size;
    alloc.data = static_cast<uint8_t*>(chunk->data) + alloc.offset;
    return alloc;
}

static void create_instance() {
    const char* instance_extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    };

    uint32_t count = 0;
    VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr));
    std::vector<VkExtensionProperties> extension_properties(count);
    VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &count, extension_properties.data()));

    for (auto name : instance_extensions) {
        bool supported = false;
        for (const auto& property : extension_properties) {
            if (!strcmp(property.extensionName, name)) {
                supported = true;
                break;
            }
        }
        if (!supported)
            error("Vulkan: required instance extension is not available: " + std::string(name));
    }

    VkApplicationInfo app_info;
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = nullptr;
    app_info.pApplicationName = nullptr;
    app_info.applicationVersion = 0;
    app_info.pEngineName = nullptr;
    app_info.engineVersion = 0;
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo desc;
    desc.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    desc.pNext = nullptr;
    desc.flags = 0;
    desc.pApplicationInfo = &app_info;
    desc.enabledLayerCount = 0;
    desc.ppEnabledLayerNames = nullptr;
    desc.enabledExtensionCount = sizeof(instance_extensions)/sizeof(instance_extensions[0]);
    desc.ppEnabledExtensionNames = instance_extensions;

    VK_CHECK(vkCreateInstance(&desc, nullptr, &vk.instance));
}

static void create_device() {
    // select physical device
    {
        uint32_t count;
        VK_CHECK(vkEnumeratePhysicalDevices(vk.instance, &count, nullptr));

        if (count == 0)
            error("Vulkan: no physical device found");

        std::vector<VkPhysicalDevice> physical_devices(count);
        VK_CHECK(vkEnumeratePhysicalDevices(vk.instance, &count, physical_devices.data()));
        vk.physical_device = physical_devices[0];
    }

    // select queue family
    {
        uint32_t queue_family_count;
        vkGetPhysicalDeviceQueueFamilyProperties(vk.physical_device, &queue_family_count, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(vk.physical_device, &queue_family_count, queue_families.data());

        // select queue family with presentation and graphics support
        vk.queue_family_index = -1;

        for (uint32_t i = 0; i < queue_family_count; i++) {
            if ((queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0) {
                vk.queue_family_index = i;

                bool pure_compute = (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0;
                if (pure_compute)
                    break;
            }
        }
        if (vk.queue_family_index == -1)
            error("Vulkan: failed to find queue family");
    }

    // create VkDevice
    {
        uint32_t count = 0;
        VK_CHECK(vkEnumerateDeviceExtensionProperties(vk.physical_device, nullptr, &count, nullptr));
        std::vector<VkExtensionProperties> extension_properties(count);
        VK_CHECK(vkEnumerateDeviceExtensionProperties(vk.physical_device, nullptr, &count, extension_properties.data()));

        const float priority = 1.0;
        VkDeviceQueueCreateInfo queue_desc;
        queue_desc.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_desc.pNext = nullptr;
        queue_desc.flags = 0;
        queue_desc.queueFamilyIndex = vk.queue_family_index;
        queue_desc.queueCount = 1;
        queue_desc.pQueuePriorities = &priority;

        VkDeviceCreateInfo device_desc;
        device_desc.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_desc.pNext = nullptr;
        device_desc.flags = 0;
        device_desc.queueCreateInfoCount = 1;
        device_desc.pQueueCreateInfos = &queue_desc;
        device_desc.enabledLayerCount = 0;
        device_desc.ppEnabledLayerNames = nullptr;
        device_desc.enabledExtensionCount = 0;
        device_desc.ppEnabledExtensionNames = nullptr;
        device_desc.pEnabledFeatures = nullptr;
        VK_CHECK(vkCreateDevice(vk.physical_device, &device_desc, nullptr, &vk.device));
    }
}

#define INIT_INSTANCE_FUNCTION(func) func = (PFN_ ## func)vkGetInstanceProcAddr(vk.instance, #func);
#define INIT_DEVICE_FUNCTION(func) func = (PFN_ ## func)vkGetDeviceProcAddr(vk.device, #func);

static void init_vulkan_library() {
    // Win32 Vulkan specific code
    vk.vulkan_library = LoadLibrary(L"vulkan-1.dll");
    if (vk.vulkan_library == NULL)
        error("Could not load vulkan dll");
    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(vk.vulkan_library, "vkGetInstanceProcAddr");

    //
    // Get functions that do not depend on VkInstance (vk.instance == nullptr at this point).
    //
    INIT_INSTANCE_FUNCTION(vkCreateInstance)
    INIT_INSTANCE_FUNCTION(vkEnumerateInstanceExtensionProperties)

    //
    // Get instance level functions.
    //
    create_instance();
    INIT_INSTANCE_FUNCTION(vkCreateDevice)
    INIT_INSTANCE_FUNCTION(vkDestroyInstance)
    INIT_INSTANCE_FUNCTION(vkEnumerateDeviceExtensionProperties)
    INIT_INSTANCE_FUNCTION(vkEnumeratePhysicalDevices)
    INIT_INSTANCE_FUNCTION(vkGetDeviceProcAddr)
    INIT_INSTANCE_FUNCTION(vkGetPhysicalDeviceFeatures)
    INIT_INSTANCE_FUNCTION(vkGetPhysicalDeviceFormatProperties)
    INIT_INSTANCE_FUNCTION(vkGetPhysicalDeviceMemoryProperties)
    INIT_INSTANCE_FUNCTION(vkGetPhysicalDeviceProperties)
    INIT_INSTANCE_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties)
    INIT_INSTANCE_FUNCTION(vkCreateWin32SurfaceKHR)
    INIT_INSTANCE_FUNCTION(vkDestroySurfaceKHR)
    INIT_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
    INIT_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR)
    INIT_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfacePresentModesKHR)
    INIT_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR)

    //
    // Get device level functions.
    //
    create_device();
    INIT_DEVICE_FUNCTION(vkAllocateCommandBuffers)
    INIT_DEVICE_FUNCTION(vkAllocateDescriptorSets)
    INIT_DEVICE_FUNCTION(vkAllocateMemory)
    INIT_DEVICE_FUNCTION(vkBeginCommandBuffer)
    INIT_DEVICE_FUNCTION(vkBindBufferMemory)
    INIT_DEVICE_FUNCTION(vkBindImageMemory)
    INIT_DEVICE_FUNCTION(vkCmdBeginRenderPass)
    INIT_DEVICE_FUNCTION(vkCmdBindDescriptorSets)
    INIT_DEVICE_FUNCTION(vkCmdBindIndexBuffer)
    INIT_DEVICE_FUNCTION(vkCmdBindPipeline)
    INIT_DEVICE_FUNCTION(vkCmdBindVertexBuffers)
    INIT_DEVICE_FUNCTION(vkCmdBlitImage)
    INIT_DEVICE_FUNCTION(vkCmdClearAttachments)
    INIT_DEVICE_FUNCTION(vkCmdCopyBufferToImage)
    INIT_DEVICE_FUNCTION(vkCmdCopyBuffer)
    INIT_DEVICE_FUNCTION(vkCmdCopyImage)
    INIT_DEVICE_FUNCTION(vkCmdDispatch)
    INIT_DEVICE_FUNCTION(vkCmdDraw)
    INIT_DEVICE_FUNCTION(vkCmdDrawIndexed)
    INIT_DEVICE_FUNCTION(vkCmdEndRenderPass)
    INIT_DEVICE_FUNCTION(vkCmdPipelineBarrier)
    INIT_DEVICE_FUNCTION(vkCmdPushConstants)
    INIT_DEVICE_FUNCTION(vkCmdSetDepthBias)
    INIT_DEVICE_FUNCTION(vkCmdSetScissor)
    INIT_DEVICE_FUNCTION(vkCmdSetViewport)
    INIT_DEVICE_FUNCTION(vkCreateBuffer)
    INIT_DEVICE_FUNCTION(vkCreateCommandPool)
    INIT_DEVICE_FUNCTION(vkCreateComputePipelines)
    INIT_DEVICE_FUNCTION(vkCreateDescriptorPool)
    INIT_DEVICE_FUNCTION(vkCreateDescriptorSetLayout)
    INIT_DEVICE_FUNCTION(vkCreateFence)
    INIT_DEVICE_FUNCTION(vkCreateFramebuffer)
    INIT_DEVICE_FUNCTION(vkCreateGraphicsPipelines)
    INIT_DEVICE_FUNCTION(vkCreateImage)
    INIT_DEVICE_FUNCTION(vkCreateImageView)
    INIT_DEVICE_FUNCTION(vkCreatePipelineLayout)
    INIT_DEVICE_FUNCTION(vkCreateRenderPass)
    INIT_DEVICE_FUNCTION(vkCreateSampler)
    INIT_DEVICE_FUNCTION(vkCreateSemaphore)
    INIT_DEVICE_FUNCTION(vkCreateShaderModule)
    INIT_DEVICE_FUNCTION(vkDestroyBuffer)
    INIT_DEVICE_FUNCTION(vkDestroyCommandPool)
    INIT_DEVICE_FUNCTION(vkDestroyDescriptorPool)
    INIT_DEVICE_FUNCTION(vkDestroyDescriptorSetLayout)
    INIT_DEVICE_FUNCTION(vkDestroyDevice)
    INIT_DEVICE_FUNCTION(vkDestroyFence)
    INIT_DEVICE_FUNCTION(vkDestroyFramebuffer)
    INIT_DEVICE_FUNCTION(vkDestroyImage)
    INIT_DEVICE_FUNCTION(vkDestroyImageView)
    INIT_DEVICE_FUNCTION(vkDestroyPipeline)
    INIT_DEVICE_FUNCTION(vkDestroyPipelineLayout)
    INIT_DEVICE_FUNCTION(vkDestroyRenderPass)
    INIT_DEVICE_FUNCTION(vkDestroySampler)
    INIT_DEVICE_FUNCTION(vkDestroySemaphore)
    INIT_DEVICE_FUNCTION(vkDestroyShaderModule)
    INIT_DEVICE_FUNCTION(vkDeviceWaitIdle)
    INIT_DEVICE_FUNCTION(vkEndCommandBuffer)
    INIT_DEVICE_FUNCTION(vkFreeCommandBuffers)
    INIT_DEVICE_FUNCTION(vkFreeDescriptorSets)
    INIT_DEVICE_FUNCTION(vkFreeMemory)
    INIT_DEVICE_FUNCTION(vkGetBufferMemoryRequirements)
    INIT_DEVICE_FUNCTION(vkGetDeviceQueue)
    INIT_DEVICE_FUNCTION(vkGetImageMemoryRequirements)
    INIT_DEVICE_FUNCTION(vkGetImageSubresourceLayout)
    INIT_DEVICE_FUNCTION(vkMapMemory)
    INIT_DEVICE_FUNCTION(vkQueueSubmit)
    INIT_DEVICE_FUNCTION(vkQueueWaitIdle)
    INIT_DEVICE_FUNCTION(vkResetDescriptorPool)
    INIT_DEVICE_FUNCTION(vkResetFences)
    INIT_DEVICE_FUNCTION(vkUnmapMemory)
    INIT_DEVICE_FUNCTION(vkUpdateDescriptorSets)
    INIT_DEVICE_FUNCTION(vkWaitForFences)
    INIT_DEVICE_FUNCTION(vkAcquireNextImageKHR)
    INIT_DEVICE_FUNCTION(vkCreateSwapchainKHR)
    INIT_DEVICE_FUNCTION(vkDestroySwapchainKHR)
    INIT_DEVICE_FUNCTION(vkGetSwapchainImagesKHR)
    INIT_DEVICE_FUNCTION(vkQueuePresentKHR)
}

#undef INIT_INSTANCE_FUNCTION
#undef INIT_DEVICE_FUNCTION

static void deinit_vulkan_library() {
    vkCreateInstance                            = nullptr;
    vkEnumerateInstanceExtensionProperties		= nullptr;

    vkCreateDevice								= nullptr;
    vkDestroyInstance							= nullptr;
    vkEnumerateDeviceExtensionProperties		= nullptr;
    vkEnumeratePhysicalDevices					= nullptr;
    vkGetDeviceProcAddr							= nullptr;
    vkGetPhysicalDeviceFeatures					= nullptr;
    vkGetPhysicalDeviceFormatProperties			= nullptr;
    vkGetPhysicalDeviceMemoryProperties			= nullptr;
    vkGetPhysicalDeviceProperties				= nullptr;
    vkGetPhysicalDeviceQueueFamilyProperties	= nullptr;
    vkCreateWin32SurfaceKHR						= nullptr;
    vkDestroySurfaceKHR							= nullptr;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR	= nullptr;
    vkGetPhysicalDeviceSurfaceFormatsKHR		= nullptr;
    vkGetPhysicalDeviceSurfacePresentModesKHR	= nullptr;
    vkGetPhysicalDeviceSurfaceSupportKHR		= nullptr;

    vkAllocateCommandBuffers					= nullptr;
    vkAllocateDescriptorSets					= nullptr;
    vkAllocateMemory							= nullptr;
    vkBeginCommandBuffer						= nullptr;
    vkBindBufferMemory							= nullptr;
    vkBindImageMemory							= nullptr;
    vkCmdBeginRenderPass						= nullptr;
    vkCmdBindDescriptorSets						= nullptr;
    vkCmdBindIndexBuffer						= nullptr;
    vkCmdBindPipeline							= nullptr;
    vkCmdBindVertexBuffers						= nullptr;
    vkCmdBlitImage								= nullptr;
    vkCmdClearAttachments						= nullptr;
    vkCmdCopyBufferToImage						= nullptr;
    vkCmdCopyImage								= nullptr;
    vkCmdDispatch                               = nullptr;
    vkCmdDraw									= nullptr;
    vkCmdDrawIndexed							= nullptr;
    vkCmdEndRenderPass							= nullptr;
    vkCmdPipelineBarrier						= nullptr;
    vkCmdPushConstants							= nullptr;
    vkCmdSetDepthBias							= nullptr;
    vkCmdSetScissor								= nullptr;
    vkCmdSetViewport							= nullptr;
    vkCreateBuffer								= nullptr;
    vkCreateCommandPool							= nullptr;
    vkCreateComputePipelines                    = nullptr;
    vkCreateDescriptorPool						= nullptr;
    vkCreateDescriptorSetLayout					= nullptr;
    vkCreateFence								= nullptr;
    vkCreateFramebuffer							= nullptr;
    vkCreateGraphicsPipelines					= nullptr;
    vkCreateImage								= nullptr;
    vkCreateImageView							= nullptr;
    vkCreatePipelineLayout						= nullptr;
    vkCreateRenderPass							= nullptr;
    vkCreateSampler								= nullptr;
    vkCreateSemaphore							= nullptr;
    vkCreateShaderModule						= nullptr;
    vkDestroyBuffer								= nullptr;
    vkDestroyCommandPool						= nullptr;
    vkDestroyDescriptorPool						= nullptr;
    vkDestroyDescriptorSetLayout				= nullptr;
    vkDestroyDevice								= nullptr;
    vkDestroyFence								= nullptr;
    vkDestroyFramebuffer						= nullptr;
    vkDestroyImage								= nullptr;
    vkDestroyImageView							= nullptr;
    vkDestroyPipeline							= nullptr;
    vkDestroyPipelineLayout						= nullptr;
    vkDestroyRenderPass							= nullptr;
    vkDestroySampler							= nullptr;
    vkDestroySemaphore							= nullptr;
    vkDestroyShaderModule						= nullptr;
    vkDeviceWaitIdle							= nullptr;
    vkEndCommandBuffer							= nullptr;
    vkFreeCommandBuffers						= nullptr;
    vkFreeDescriptorSets						= nullptr;
    vkFreeMemory								= nullptr;
    vkGetBufferMemoryRequirements				= nullptr;
    vkGetDeviceQueue							= nullptr;
    vkGetImageMemoryRequirements				= nullptr;
    vkGetImageSubresourceLayout					= nullptr;
    vkMapMemory									= nullptr;
    vkQueueSubmit								= nullptr;
    vkQueueWaitIdle								= nullptr;
    vkResetDescriptorPool						= nullptr;
    vkResetFences								= nullptr;
    vkUnmapMemory                               = nullptr;
    vkUpdateDescriptorSets						= nullptr;
    vkWaitForFences								= nullptr;
    vkAcquireNextImageKHR						= nullptr;
    vkCreateSwapchainKHR						= nullptr;
    vkDestroySwapchainKHR						= nullptr;
    vkGetSwapchainImagesKHR						= nullptr;
    vkQueuePresentKHR							= nullptr;
}

void vk_initialize() {
    init_vulkan_library();

    vkGetDeviceQueue(vk.device, vk.queue_family_index, 0, &vk.queue);

    //
    // Command pool.
    //
    {
        VkCommandPoolCreateInfo desc;
        desc.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        desc.pNext = nullptr;
        desc.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        desc.queueFamilyIndex = vk.queue_family_index;

        VK_CHECK(vkCreateCommandPool(vk.device, &desc, nullptr, &vk.command_pool));
    }

    //
    // Command buffer.
    //
    {
        VkCommandBufferAllocateInfo alloc_info;
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.pNext = nullptr;
        alloc_info.commandPool = vk.command_pool;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandBufferCount = 1;
        VK_CHECK(vkAllocateCommandBuffers(vk.device, &alloc_info, &vk.command_buffer));
    }
}

void vk_shutdown() {
    vkDeviceWaitIdle(vk.device);

    for (const auto& chunk : vk.device_local_chunks) {
        vkFreeMemory(vk.device, chunk.memory, nullptr);
    }
    vk.device_local_chunks.clear();

    for (const auto& chunk : vk.host_visible_chunks) {
        vkFreeMemory(vk.device, chunk.memory, nullptr);
    }
    vk.host_visible_chunks.clear();

    vkDestroyCommandPool(vk.device, vk.command_pool, nullptr);

    vkDestroyDevice(vk.device, nullptr);
    vkDestroyInstance(vk.instance, nullptr);

    deinit_vulkan_library();
}

void vk_record_buffer_memory_barrier(VkCommandBuffer cb, VkBuffer buffer,
        VkPipelineStageFlags src_stages, VkPipelineStageFlags dst_stages,
        VkAccessFlags src_access, VkAccessFlags dst_access) {

    VkBufferMemoryBarrier barrier;
    barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;;
    barrier.pNext = nullptr;
    barrier.srcAccessMask = src_access;
    barrier.dstAccessMask = dst_access;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.buffer = buffer;
    barrier.offset = 0;
    barrier.size = VK_WHOLE_SIZE;

    vkCmdPipelineBarrier(cb, src_stages, dst_stages, 0, 0, nullptr, 1, &barrier, 0, nullptr);
}

VkBuffer vk_create_host_visible_buffer(VkDeviceSize size, VkBufferUsageFlags usage, void** buffer_ptr) {
    VkBufferCreateInfo desc;
    desc.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    desc.pNext = nullptr;
    desc.flags = 0;
    desc.size = size;
    desc.usage = usage;
    desc.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    desc.queueFamilyIndexCount = 0;
    desc.pQueueFamilyIndices = nullptr;

    VkBuffer buffer = get_resource_manager()->create_buffer(desc);

    VkMemoryRequirements memory_requirements;
    vkGetBufferMemoryRequirements(vk.device, buffer, &memory_requirements);
    auto alloc = allocate_memory(memory_requirements, true);
    VK_CHECK(vkBindBufferMemory(vk.device, buffer, alloc.memory, alloc.offset));
    *buffer_ptr = alloc.data;

    return buffer;
}
