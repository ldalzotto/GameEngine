#pragma once

#include "DataContainer/Structures/VectorMacros.h"
#include "DataContainer/Structures/ArrayMacros.h"
#include "DataContainer/FunctionalObjets/VectorIteratorMacros.h"
#include "DataContainer/FunctionalObjets/VectorWriterMacros.h"
#include "DataContainer/Algorithm/SortingAlgorithmMacro.h"
#include "Error/ReturnCodes.h"

CORE_VECTORWRITER_DEFINE(Core_char_Vector, char);
CORE_DEFINE_VECTOR(char);
CORE_VECTORITERATOR_DEFINE(Core_char_Vector, char);

CORE_ALGORITHM_DEFINE(Core_char_Vector, char);


CORE_VECTORWRITER_DEFINE(Core_char_Array, char);
CORE_DEFINE_ARRAY(char);
CORE_VECTORITERATOR_DEFINE(Core_char_Array, char);

CORE_ALGORITHM_DEFINE(Core_char_Array, char);


#define CORE_STRING_CHAR_NB(in_string) (in_string)->Size - 1
#define CORE_STRING_CSTR(in_string) ((char*)(in_string)->Memory)
// void Core_char_Vector_append(Core_char_Vector* p_string, char* p_appendedString);