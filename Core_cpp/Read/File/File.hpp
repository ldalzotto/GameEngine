#pragma once

namespace _Core
{
	struct String;
}

namespace _Core
{
	void File_readFile_byte(char* p_absoluteFilePath, String* out_file_byte);
	void File_readFile_string(char* p_absoluteFilePath, String* out_file_string);
}

