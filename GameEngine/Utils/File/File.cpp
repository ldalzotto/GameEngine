#include "File.h"

#include "Log/Log.h"

#include <stdexcept>
#include <fstream>
#include <ios>

namespace _GameEngine::_Utils::_File
{
	std::vector<char> readFile(const std::string& p_absoluteFilePath)
	{
		std::ifstream file(p_absoluteFilePath, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to open file : " + p_absoluteFilePath));
		}

		size_t l_fileSize = (size_t)file.tellg();
		std::vector<char> buffer(l_fileSize);

		file.seekg(0);
		file.read(buffer.data(), l_fileSize);
		file.close();

		return buffer;
	}
}