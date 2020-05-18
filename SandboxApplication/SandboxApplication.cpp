#include "SandboxApplication.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

using namespace _GameEngine;

int main()
{
    App = app_alloc();

    App->Log->CoreLogger->warn("Initialized Log!");
    App->Log->ClientLogger->info("Initialized Log!");

    try
    {
        app_run(App);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        app_free(App);
        return EXIT_FAILURE;
    }

    app_free(App);
    return EXIT_SUCCESS;
};