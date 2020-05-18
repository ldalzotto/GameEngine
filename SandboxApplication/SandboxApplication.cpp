#include "SandboxApplication.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

using namespace _GameEngine;

int main()
{
    App = app_alloc();

    try
    {
        app_run(App);
    }
    catch (const std::exception &e)
    {
        App->Log->ClientLogger->error(e.what());
        app_free(App);
        return EXIT_FAILURE;
    }

    app_free(App);
    return EXIT_SUCCESS;
};