#pragma once

#include <vector>
#include <string>

#include "DataStructures/String.h"

namespace _GameEngine::_Utils
{
	std::vector<char> File_readFile(const std::string& p_absoluteFilePath);
	void File_readFileV2(_Core::String* p_absoluteFilePath, _Core::String* out_file);
}