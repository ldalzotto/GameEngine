
extern "C"
{
#include "v2/_source/VectorC.h"
#include "v2/_source/QuaternionC.h"
}

#include "v2/Quaternion/RQuaternion.h"

int main()
{

	VECTOR3F l_euler = {0.62831f, 3.92699f, 0.000f};
	QUATERNION4F l_quat;
	_MathV2::RQuaternion_fromEulerAngle((float*)&l_euler, (float*)&l_quat);
	QUATERNION4F l_quat2;
	Quat_FromEulerAngle(&l_euler, &l_quat2);
	return 0;
}