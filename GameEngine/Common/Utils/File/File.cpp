#include "File.h"

extern "C"
{
#include "Log/LogFormatting.h"
}

#include <stdexcept>
#include <fstream>
#include <ios>

namespace _GameEngine::_Utils
{
	void File_readFileV2(Core_GenericArray* p_absoluteFilePath, Core_GenericArray* out_file)
	{
		std::ifstream file((char*)p_absoluteFilePath->Memory, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			Core_GenericArray l_error; Core_string_alloc(&l_error, 100);
			Core_string_append(&l_error, "Failed to open file : ");
			Core_string_append(&l_error, (char*)p_absoluteFilePath->Memory);
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE_STRING(&l_error));
		}

		size_t l_fileSize = (size_t)file.tellg();
		Core_Array_alloc(out_file, sizeof(char), l_fileSize);
		out_file->Size = out_file->Capacity;
		
		file.seekg(0);
		file.read((char*)out_file->Memory, l_fileSize);
		file.close();
	};
}