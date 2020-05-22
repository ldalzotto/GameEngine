#pragma once

#include <vector>
#include <string>

namespace _GameEngine::_Utils::_File
{
	std::vector<char> readFile(const std::string& p_absoluteFilePath);
}