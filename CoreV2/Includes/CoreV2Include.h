#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// Errors

#include "../Error/ReturnCodes.h"
#include "../Error/ErrorHandling.h"

// Functional objects
#include "../DataContainer/FunctionalObjets/VectorAccessor.h"
#include "../DataContainer/FunctionalObjets/VectorAccessorMacros.h"
#include "../DataContainer/FunctionalObjets/VectorIterator.h"
#include "../DataContainer/FunctionalObjets/VectorIteratorMacros.h"
#include "../DataContainer/FunctionalObjets/VectorWriter.h"
#include "../DataContainer/FunctionalObjets/VectorWriterMacros.h"

#include "../DataContainer/Algorithm/Compare/CompareComparators.h"
#include "../DataContainer/Algorithm/Compare/CompareComparatorsMacros.h"

#include "../DataContainer/Algorithm/Sort/SortComparators.h"
#include "../DataContainer/Algorithm/Sort/SortComparatorsMacros.h"
#include "../DataContainer/Algorithm/Sort/SortFunctions.h"

// Generic Array
#include "../DataContainer/Structures/GenericArray.h"

// DataContainer
#include "../DataContainer/Structures/Array.h"
#include "../DataContainer/Structures/ArrayMacros.h"

#include "../DataContainer/Structures/Vector.h"
#include "../DataContainer/Structures/VectorMacros.h"

#include "../DataContainer/Structures/SortedVector.h"
#include "../DataContainer/Structures/SortedVectorMacros.h"

// Algorithms
#include "../DataContainer/Algorithm/Compare/CompareAlgorithm.h"
#include "../DataContainer/Algorithm/Compare/CompareAlgorithmMacros.h"

#include "../DataContainer/Algorithm/Sort/SortingAlgorithm.h"
#include "../DataContainer/Algorithm/Sort/SortingAlgorithmMacro.h"

// Specific types
#include "../DataContainer/Algorithm/String/StringAlgorithm.h"
#include "../DataContainer/Algorithm/String/StringAlgorithmMacros.h"
#include "../DataContainer/Types/CoreString.h"