#pragma once

extern "C"
{
#include "Include/CoreV2.h"
}

namespace _GameEngine::_Utils
{
	void File_readFileV2(Core_GenericArray* p_absoluteFilePath, Core_GenericArray* out_file);
}