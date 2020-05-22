#include "CommandPool.h"

#include <stdexcept>

#include "Log/Log.h"

namespace _GameEngine::_Render::_CommandBuffer
{
    void CommandPool_Init(CommandPool* p_commandPool, CommandPoolDependencies* p_commandPoolDependencies)
    {
        p_commandPool->CommanPoolDependencies = *p_commandPoolDependencies;

        _Device::QueueFamily p_graphicsQueueFamily = p_commandPool->CommanPoolDependencies.Device->PhysicalDevice.QueueFamilies.Graphics;

        VkCommandPoolCreateInfo l_commandPoolCreateInfo{};
        l_commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        l_commandPoolCreateInfo.queueFamilyIndex = p_graphicsQueueFamily.QueueIndex;
        l_commandPoolCreateInfo.flags = 0;

        if (vkCreateCommandPool(p_commandPool->CommanPoolDependencies.Device->LogicalDevice.LogicalDevice, &l_commandPoolCreateInfo, nullptr, &p_commandPool->CommandPool)
            != VK_SUCCESS)
        {
            throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create command pool!"));
        }
    };

    void CommandPool_free(CommandPool* p_commandPool)
    {
        vkDestroyCommandPool(p_commandPool->CommanPoolDependencies.Device->LogicalDevice.LogicalDevice, p_commandPool->CommandPool, nullptr);
    }
}