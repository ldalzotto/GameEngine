#include "File.h"

#include "DataStructures/StringAlgorithm.h"
#include "MYLog/MYLog.h"

#include <stdexcept>
#include <fstream>
#include <ios>

namespace _GameEngine::_Utils
{
	void File_readFileV2(_CoreV3::ArrayT<char>* p_absoluteFilePath, _CoreV3::ArrayT<char>* out_file)
	{
		std::ifstream file(p_absoluteFilePath->Memory, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			_CoreV3::String l_error = _CoreV3::PushBackArrays(_CoreV3::Alloc<char>(20), STR("Failed to open file : "), (_CoreV3::GenericArray*) p_absoluteFilePath);
			throw std::runtime_error(l_error.Memory);
		}

		size_t l_fileSize = (size_t)file.tellg();
		_CoreV3::Alloc(out_file, l_fileSize);
		out_file->Size = out_file->Capacity;
		
		file.seekg(0);
		file.read(out_file->Memory, l_fileSize);
		file.close();
	};
}