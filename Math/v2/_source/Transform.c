#include "v2/_interface/TransformC.h"

#include "v2/_interface/MatrixC.h"
#include "v2/_interface/VectorC.h"
#include "v2/_interface/QuaternionC.h"

void Arr_EraseCompare_TransformPtr_EqualsTransformPtr(Array_TransformHandle_PTR p_array, const Transform_PTR p_comparedTransform)
{
	for (size_t i = 0; i < p_array->Size; i++)
	{
		Transform_PTR l_transform = p_array->Memory[i];
		if (p_comparedTransform == l_transform)
		{
			Arr_Erase_TransformPtr(p_array, i);
			return;
		}
	}
}

void Transform_UpdateMatricesIfNecessary(Transform_PTR p_transform);

void Transform_MarkMatricsForRecalculation(Transform_PTR p_transform)
{
	p_transform->MatricesMustBeRecalculated = true;
	p_transform->UserFlag_HasChanged = true;
	for (size_t i = 0; i < p_transform->Childs.Size; i++)
	{
		Transform_MarkMatricsForRecalculation(p_transform->Childs.Memory[i]);
	}
};

void Transform_Alloc(Transform_PTR p_transform)
{
	Arr_Alloc_TransformPtr(&p_transform->Childs, 0);
	Transform_MarkMatricsForRecalculation(p_transform);
};

void Transform_Free(Transform_PTR p_transform)
{
	p_transform->Parent = NULL;
	Arr_Free_TransformPtr(&p_transform->Childs);
};

void Transform_AddChild(Transform_PTR p_transform, Transform_PTR p_newChild)
{
	if (p_newChild->Parent != p_transform)
	{
		if (p_newChild->Parent)
		{
			Arr_EraseCompare_TransformPtr_EqualsTransformPtr(&p_newChild->Parent->Childs, p_newChild);
		}

		p_newChild->Parent = p_transform;
		Arr_PushBackRealloc_TransformPtr(&p_transform->Childs, &p_newChild);

		Transform_MarkMatricsForRecalculation(p_newChild);
	}
};

void Transform_SetLocalPosition(Transform_PTR p_transform, const Vector3f_PTR p_localPosition)
{
	if (!Vec_Equals_3f(&p_transform->LocalPosition, p_localPosition))
	{
		Transform_MarkMatricsForRecalculation(p_transform);
		p_transform->LocalPosition = *p_localPosition;
	}
};

void Transform_SetLocalRotation(Transform_PTR p_transform, const Quaternion4f_PTR p_localRotation)
{
	if (!Quat_Equals(&p_transform->LocalRotation, p_localRotation))
	{
		Transform_MarkMatricsForRecalculation(p_transform);
		p_transform->LocalRotation = *p_localRotation;
	}
};

void Transform_SetLocalScale(Transform_PTR p_transform, const Vector3f_PTR p_localScale)
{
	if (!Vec_Equals_3f(&p_transform->LocalScale, p_localScale))
	{
		Transform_MarkMatricsForRecalculation(p_transform);
		p_transform->LocalScale = *p_localScale;
	}
};

void Transform_SetWorldPosition(Transform_PTR p_transform, const Vector3f_PTR p_worldPosition)
{
	if (p_transform->Parent == NULL)
	{
		Transform_SetLocalPosition(p_transform, p_worldPosition);
	}
	else
	{
		Matrix4f tmp_mat4; Vector4f tmp_vec4;
		Vector4f l_localPosition;
		Transform_GetWorldToLocalMatrix(p_transform->Parent, &tmp_mat4);
		tmp_vec4.Vec3 = *p_worldPosition; tmp_vec4.Vec3_w = 1.0f;
		Mat_Mul_M4F_V4F(&tmp_mat4, &tmp_vec4, &l_localPosition);
		Transform_SetLocalPosition(p_transform, &l_localPosition.Vec3);
	}
};

void Transform_AddToWorldPosition(Transform_PTR p_transform, const Vector3f_PTR p_worldPosition_delta)
{
	Vector3f tmp_vec3;
	Transform_GetWorldPosition(p_transform, &tmp_vec3);
	Vec_Add_3f_3f(&tmp_vec3, p_worldPosition_delta, &tmp_vec3);
	Transform_SetWorldPosition(p_transform, &tmp_vec3);
};


void Transform_SetWorldRotation(Transform_PTR p_transform, const Quaternion4f_PTR p_worldRotation)
{
	if (p_transform->Parent == NULL)
	{
		Transform_SetLocalRotation(p_transform, p_worldRotation);
	}
	else
	{
		Quaternion4f tmp_quat_0, tmp_quat_1;
		Quaternion4f l_settedLocalQuaternion;

		Transform_GetWorldRotation(p_transform->Parent, &tmp_quat_1);
		Quat_conjugate(&tmp_quat_1, &tmp_quat_0);
		Quat_Mul(&tmp_quat_0, p_worldRotation, &l_settedLocalQuaternion);
		Transform_SetLocalRotation(p_transform, &l_settedLocalQuaternion);
	}
};

void Transform_SetWorldScale(Transform_PTR p_transform, const Vector3f_PTR p_worldScale)
{
	if (p_transform->Parent == NULL)
	{
		Transform_SetLocalScale(p_transform, p_worldScale);
	}
	else
	{
		Vector3f tmp_vec3_0, tmp_vec3_1;
		Vector3f l_parentWorldScale;
		Transform_GetWorldScaleFactor(p_transform->Parent, &l_parentWorldScale);

		Vec_Inv_3f(&l_parentWorldScale, &tmp_vec3_0);
		Vec_Mul_3f_3f(p_worldScale, &tmp_vec3_0, &tmp_vec3_1);
		Transform_SetLocalScale(p_transform, &tmp_vec3_1);
	}
};

void Transform_UpdateMatricesIfNecessary(Transform_PTR p_transform)
{
	if (p_transform->MatricesMustBeRecalculated)
	{
		{
			Mat_TRS_Quat_M4F(
				&p_transform->LocalPosition,
				&p_transform->LocalRotation,
				&p_transform->LocalScale,
				&p_transform->LocalToWorldMatrix
			);

			if (p_transform->Parent)
			{
				Matrix4f tmp_mat4_0, tmp_mat4_1;
				Transform_GetLocalToWorldMatrix(p_transform->Parent, &tmp_mat4_0);
				Mat_Mul_M4F_M4F(&tmp_mat4_0, &p_transform->LocalToWorldMatrix, &tmp_mat4_1);
				p_transform->LocalToWorldMatrix = tmp_mat4_1;
			}
		}

		p_transform->MatricesMustBeRecalculated = false;
	}
};

void Transform_GetLocalToWorldMatrix(Transform_PTR p_transform, Matrix4f_PTR p_out)
{
	Transform_UpdateMatricesIfNecessary(p_transform);
	*p_out = p_transform->LocalToWorldMatrix;
};

void Transform_GetWorldToLocalMatrix(Transform_PTR p_transform, Matrix4f_PTR p_out)
{
	Matrix4f tmp_mat4;
	Transform_GetLocalToWorldMatrix(p_transform, &tmp_mat4);
	Mat_Inv_M4F(&tmp_mat4, p_out);
};

void Transform_CalculateMatrixToProjectFromTransformToAnother(Transform_PTR p_source, Transform_PTR p_target, Matrix4f_PTR p_out)
{
	Matrix4f tmp_mat4_1, tmp_mat4_2;
	Transform_GetLocalToWorldMatrix(p_source, &tmp_mat4_1);
	Transform_GetLocalToWorldMatrix(p_source, &tmp_mat4_2);
	Mat_Mul_M4F_M4F(&tmp_mat4_1, &tmp_mat4_2, p_out);
};

void Transform_GetWorldPosition(Transform_PTR p_transform, Vector3f_PTR p_out)
{
	Matrix4f tmp_mat4_1; Vector4f tmp_vec4_0;
	Transform_GetLocalToWorldMatrix(p_transform, &tmp_mat4_1);
	Mat_GetTranslation_M4F(&tmp_mat4_1, &tmp_vec4_0);
	*p_out = tmp_vec4_0.Vec3;
};

void Transform_GetWorldRotation(Transform_PTR p_transform, Quaternion4f_PTR p_out)
{
	if (p_transform->Parent)
	{
		Quaternion4f tmp_quat_0;
		Transform_GetWorldRotation(p_transform->Parent, &tmp_quat_0);
		Quat_Mul(&tmp_quat_0, &p_transform->LocalRotation, p_out);
	}
	else
	{
		*p_out = p_transform->LocalRotation;
	}
};

void Transform_GetWorldScaleFactor(Transform_PTR p_transform, Vector3f_PTR p_out)
{
	if (p_transform->Parent)
	{
		Vector3f tmp_vec3_0;
		Transform_GetWorldScaleFactor(p_transform->Parent, &tmp_vec3_0);
		Vec_Mul_3f_3f(&tmp_vec3_0, &p_transform->LocalScale, p_out);
	}
	else
	{
		*p_out = p_transform->LocalScale;
	}
};

void Transform_GetRight(Transform_PTR p_transform, Vector3f_PTR p_out)
{
	Matrix4f tmp_mat4_1;
	Transform_GetLocalToWorldMatrix(p_transform, &tmp_mat4_1);
	Vec_Normalize_3f(&tmp_mat4_1.Right.Vec3, p_out);
};

void Transform_GetUp(Transform_PTR p_transform, Vector3f_PTR p_out)
{
	Matrix4f tmp_mat4_1;
	Transform_GetLocalToWorldMatrix(p_transform, &tmp_mat4_1);
	Vec_Normalize_3f(&tmp_mat4_1.Up.Vec3, p_out);
};

void Transform_GetForward(Transform_PTR p_transform, Vector3f_PTR p_out)
{
	Matrix4f tmp_mat4_1;
	Transform_GetLocalToWorldMatrix(p_transform, &tmp_mat4_1);
	Vec_Normalize_3f(&tmp_mat4_1.Forward.Vec3, p_out);
};
