#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "DataStructures/GenericArray.h"

void Core_string_append(struct Core_GenericArray* p_string, char* p_appended);
bool Core_string_find(struct Core_GenericArray* p_string, char* p_comparedStr, size_t p_startIndex, size_t* p_outfoundIndex);