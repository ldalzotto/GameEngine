#include "RMatrix.h"

#include <string.h>

namespace _MathV2
{
	void RMatrix_3x3_buildFromColumn(const float p_col1[3], const float p_col2[3], const float p_col3[3], float p_out[3][3])
	{
		memcpy(p_out, p_col1, sizeof(float) * 3);
		memcpy((float*)((char*)p_out + (sizeof(float) * 3)), p_col2, sizeof(float) * 3);
		memcpy((float*)((char*)p_out + (sizeof(float) * 6)), p_col3, sizeof(float) * 3);
	};
}