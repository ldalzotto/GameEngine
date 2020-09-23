typedef struct Vector2f_TYP
{
	union
	{
		float Points[2];
		struct { float x, y; };
	};
} Vector2f, * Vector2f_PTR;

typedef struct Vector3f_TYP
{
	union
	{
		float Points[3];
		struct { float x, y, z; };
		struct { Vector2f Vec2; float Vec2_w; };
	};
} Vector3f, *Vector3f_PTR;

typedef struct Vector4f_TYP
{
	union
	{
		float Points[4];
		struct { float x, y, z, w; };
		struct { Vector3f Vec3; float Vec3_w; };
	};
} Vector4f, * Vector4f_PTR;

typedef struct Vector2d_TYP
{
	union
	{
		struct { double x, y; };
		double Points[2];
	};
} Vector2d, * Vector2d_PTR;

typedef struct Vector2i_TYP
{
	union
	{
		struct { int x, y; };
		int Points[2];
	};
} Vector2i, * Vector2i_PTR;

typedef struct Vector3c_TYP
{
	union
	{
		char Points[3];
		struct { char x, y, z; };
	};
} Vector3c, * Vector3c_PTR;


typedef struct Matrix3f_TYP
{
	union
	{
		float Points[9];
		struct { float _00, _01, _02, _10, _11, _12, _20, _21, _22; };
		struct { Vector3f Col0, Col1, Col2; };
		struct { Vector3f Right, Up, Forward; };
	};
} Matrix3f, * Matrix3f_PTR;

typedef struct Matrix4f_TYP
{
	union
	{
		float Points[16];
		struct { float _00, _01, _02, _03, _10, _11, _12, _13, _20, _21, _22, _23, _30, _31, _32, _33; };
		struct { Vector4f Col0, Col1, Col2, Col3; };
		struct { Vector4f Right, Up, Forward, Col3_Direction; };
	};
} Matrix4f, * Matrix4f_PTR;


void Mat_Mul_M4F_M4F(const Matrix4f_PTR p_left, const Matrix4f_PTR p_right, Matrix4f_PTR p_out)
{
	p_out->_00 = (p_left->_00 * p_right->_00) + (p_left->_10 * p_right->_01) + (p_left->_20 * p_right->_02) + (p_left->_30 * p_right->_03);
	p_out->_01 = (p_left->_01 * p_right->_00) + (p_left->_11 * p_right->_01) + (p_left->_21 * p_right->_02) + (p_left->_31 * p_right->_03);
	p_out->_02 = (p_left->_02 * p_right->_00) + (p_left->_12 * p_right->_01) + (p_left->_22 * p_right->_02) + (p_left->_32 * p_right->_03);
	p_out->_03 = (p_left->_03 * p_right->_00) + (p_left->_13 * p_right->_01) + (p_left->_23 * p_right->_02) + (p_left->_33 * p_right->_03);

	p_out->_10 = (p_left->_00 * p_right->_10) + (p_left->_10 * p_right->_11) + (p_left->_20 * p_right->_12) + (p_left->_30 * p_right->_13);
	p_out->_11 = (p_left->_01 * p_right->_10) + (p_left->_11 * p_right->_11) + (p_left->_21 * p_right->_12) + (p_left->_31 * p_right->_13);
	p_out->_12 = (p_left->_02 * p_right->_10) + (p_left->_12 * p_right->_11) + (p_left->_22 * p_right->_12) + (p_left->_32 * p_right->_13);
	p_out->_13 = (p_left->_03 * p_right->_10) + (p_left->_13 * p_right->_11) + (p_left->_23 * p_right->_12) + (p_left->_33 * p_right->_13);

	p_out->_20 = (p_left->_00 * p_right->_20) + (p_left->_10 * p_right->_21) + (p_left->_20 * p_right->_22) + (p_left->_30 * p_right->_23);
	p_out->_21 = (p_left->_01 * p_right->_20) + (p_left->_11 * p_right->_21) + (p_left->_21 * p_right->_22) + (p_left->_31 * p_right->_23);
	p_out->_22 = (p_left->_02 * p_right->_20) + (p_left->_12 * p_right->_21) + (p_left->_22 * p_right->_22) + (p_left->_32 * p_right->_23);
	p_out->_23 = (p_left->_03 * p_right->_20) + (p_left->_13 * p_right->_21) + (p_left->_23 * p_right->_22) + (p_left->_33 * p_right->_23);

	p_out->_30 = (p_left->_00 * p_right->_30) + (p_left->_10 * p_right->_31) + (p_left->_20 * p_right->_32) + (p_left->_30 * p_right->_33);
	p_out->_31 = (p_left->_01 * p_right->_30) + (p_left->_11 * p_right->_31) + (p_left->_21 * p_right->_32) + (p_left->_31 * p_right->_33);
	p_out->_32 = (p_left->_02 * p_right->_30) + (p_left->_12 * p_right->_31) + (p_left->_22 * p_right->_32) + (p_left->_32 * p_right->_33);
	p_out->_33 = (p_left->_03 * p_right->_30) + (p_left->_13 * p_right->_31) + (p_left->_23 * p_right->_32) + (p_left->_33 * p_right->_33);
};

float Vec_Dot_3f(const Vector3f_PTR p_left, const Vector3f_PTR p_right)
{
	return (p_left->Points[0] * p_right->Points[0]) + (p_left->Points[1] * p_right->Points[1]) + (p_left->Points[2] * p_right->Points[2]);
};


__kernel void MatrixTestGPU(__global Matrix4f* p_left, __global Matrix4f* p_right, __global Matrix4f* out_matrices)
{
	int i = get_global_id(0);
	// Vector3f l_left = p_left[i];
	// Vector3f l_right = p_right[i];
	// out_matrices[i] = Vec_Dot_3f(&l_left, &l_right);

	Matrix4f l_left = p_left[i];
	Matrix4f l_right = p_right[i];
	Matrix4f l_out;
	Mat_Mul_M4F_M4F(&l_left, &l_right, &l_out);
	Mat_Mul_M4F_M4F(&l_left, &l_right, &l_out);
	Mat_Mul_M4F_M4F(&l_left, &l_right, &l_out);
	Mat_Mul_M4F_M4F(&l_left, &l_right, &l_out);
	Mat_Mul_M4F_M4F(&l_left, &l_right, &l_out);
	Mat_Mul_M4F_M4F(&l_left, &l_right, &l_out);
	Mat_Mul_M4F_M4F(&l_left, &l_right, &l_out);
	Mat_Mul_M4F_M4F(&l_left, &l_right, &l_out);
	Mat_Mul_M4F_M4F(&l_left, &l_right, &l_out);
	out_matrices[i] = l_out;
};