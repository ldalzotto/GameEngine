#include "File.h"

#include "DataStructures/StringAlgorithm.h"
#include "MYLog/MYLog.h"

#include <stdexcept>
#include <fstream>
#include <ios>

namespace _GameEngine::_Utils
{
	std::vector<char> File_readFile(const std::string& p_absoluteFilePath)
	{
		std::ifstream file(p_absoluteFilePath, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to open file : " + p_absoluteFilePath));
		}

		size_t l_fileSize = (size_t)file.tellg();
		std::vector<char> buffer(l_fileSize);

		file.seekg(0);
		file.read(buffer.data(), l_fileSize);
		file.close();

		return buffer;
	}

	void File_readFileV2(_Core::String* p_absoluteFilePath, _Core::String* out_file)
	{
		std::ifstream file(p_absoluteFilePath->c_str(), std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			_Core::String l_error;
			l_error.alloc(p_absoluteFilePath->Vector.size() + 50);
			_Core::String_append(&l_error, "Failed to open file : ");
			_Core::String_append(&l_error, p_absoluteFilePath->c_str(), _Core::String_charNb(p_absoluteFilePath));
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE(l_error.c_str()));
		}

		size_t l_fileSize = (size_t)file.tellg();
		out_file->alloc(l_fileSize);
		

		file.seekg(0);
		file.read(out_file->c_str(), l_fileSize);
		file.close();
	};
}