#pragma once

#include <stdlib.h>
#include <stdbool.h> 

void Core_string_append(struct Core_GenericArray* p_string, struct Core_VectorWriter* p_writer, char* p_appended);
bool Core_string_find(struct Core_GenericArray* p_string, struct Core_VectorAccessor* p_accessor, char* p_comparedStr, size_t p_startIndex, size_t* p_outfoundIndex);