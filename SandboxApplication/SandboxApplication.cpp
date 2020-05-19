
#include "SandboxApplication.h"
#include "Log/Log.h"

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
	catch (const std::exception& e)
	{
		_Log::LogInstance->ClientLogger->error(e.what());
		app_free(App);
		return EXIT_FAILURE;
	}
	catch (...) 
	{
		_Log::LogInstance->ClientLogger->error("Unexpected Error");
		app_free(App);
		return EXIT_FAILURE;
	}

	app_free(App);
	return EXIT_SUCCESS;
};