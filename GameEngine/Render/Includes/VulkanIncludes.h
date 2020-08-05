#pragma once

#ifdef _WIN32
#include "Include/PlatformInclude.hpp"
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "vulkan/vulkan.h"