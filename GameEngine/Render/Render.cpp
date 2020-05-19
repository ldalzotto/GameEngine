#include "Render.h"

namespace _GameEngine
{
    Render *Render_alloc()
    {
        Render *l_render = new Render();
        Window_init(&l_render->Window);
        return l_render;
    };

    void Render_free(Render *p_render)
    {
        delete p_render;
    };

    void Render_render(Render *p_render){

    };

} // namespace _GameEngine