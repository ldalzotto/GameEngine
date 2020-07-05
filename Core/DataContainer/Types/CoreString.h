#pragma once

#include <stdbool.h>

#include "DataContainer/Structures/VectorMacros.h"
#include "DataContainer/Structures/ArrayMacros.h"
#include "DataContainer/FunctionalObjets/VectorIteratorMacros.h"
#include "DataContainer/FunctionalObjets/VectorWriterMacros.h"
#include "DataContainer/FunctionalObjets/VectorAccessorMacros.h"
#include "DataContainer/Algorithm/Sort/SortingAlgorithmMacro.h"
#include "DataContainer/Algorithm/String/StringAlgorithmMacros.h"
#include "Error/ReturnCodes.h"

CORE_VECTORACCESSOR_DEFINE(Core_char_Vector, char);
CORE_VECTORWRITER_DEFINE(Core_char_Vector, char);
CORE_DEFINE_VECTOR(char);
CORE_VECTORITERATOR_DEFINE(Core_char_Vector, char);

CORE_SORT_ALGORITHM_DEFINE(Core_char_Vector, char, void);
CORE_STRING_ALGORITHM_DEFINE(Core_char_Vector);

CORE_VECTORACCESSOR_DEFINE(Core_char_Array, char);
CORE_VECTORWRITER_DEFINE(Core_char_Array, char);
CORE_DEFINE_ARRAY(char);
CORE_VECTORITERATOR_DEFINE(Core_char_Array, char);

CORE_SORT_ALGORITHM_DEFINE(Core_char_Array, char, void);
CORE_STRING_ALGORITHM_DEFINE(Core_char_Array);


#define CORE_STRING_CHAR_NB(in_string) (in_string)->Size - 1
#define CORE_STRING_CSTR(in_string) ((char*)(in_string)->Memory)
// void Core_char_Vector_append(Core_char_Vector* p_string, char* p_appendedString);