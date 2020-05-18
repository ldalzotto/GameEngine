#pragma once

#include "Window.h"

namespace _GameEngine
{

    struct Render
    {
        Window Window;
    };

    Render *Render_alloc();
    void Render_free(Render *p_render);

} // namespace _GameEngine
