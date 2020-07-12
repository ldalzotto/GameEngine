#pragma once

#include "ErrorHandling/ReturnCodes.h"

struct Core_GenericArray;

Core_ReturnCodes Core_File_readFile_byte(struct Core_GenericArray* p_absoluteFilePath, struct Core_GenericArray* out_file_byte);
Core_ReturnCodes Core_File_readFile_string(struct Core_GenericArray* p_absoluteFilePath, struct Core_GenericArray* out_file_string);
