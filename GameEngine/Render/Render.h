#pragma once

#include "Window.h"
#include "vulkan/vulkan.h"

namespace _GameEngine
{

    struct Render
    {
        Window Window;
        VkInstance Instance;
    };

    Render *Render_alloc();
    void Render_free(Render *p_render);

    void Render_render(Render *p_render);
} // namespace _GameEngine
