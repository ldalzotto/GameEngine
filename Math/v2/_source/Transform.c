#include "v2/_interface/TransformC.h"

#include "v2/_interface/MatrixC.h"
#include "v2/_interface/VectorC.h"
#include "v2/_interface/QuaternionC.h"

void Arr_EraseCompare_TransformPtr_EqualsTransformPtr(ARRAY_TRANSFORMHANDLE_PTR p_array, const TRANSFORM_PTR p_comparedTransform)
{
	ARRAY_TRANSFORMHANDLE_ITERATOR l_it; Arr_BuildIterator_TransformPtr(p_array, &l_it);
	while (Iter_MoveNext_TransformPtr(&l_it))
	{
		if (p_comparedTransform == *l_it.Current)
		{
			Arr_Erase_TransformPtr(p_array, l_it.CurrentIndex);
			return;
		}
	}
}

void Transform_UpdateMatricesIfNecessary(TRANSFORM_PTR p_transform);

void Transform_MarkMatricsForRecalculation(TRANSFORM_PTR p_transform)
{
	p_transform->MatricesMustBeRecalculated = true;
	p_transform->UserFlag_HasChanged = true;
	for (size_t i = 0; i < p_transform->Childs.Size; i++)
	{
		Transform_MarkMatricsForRecalculation(p_transform->Childs.Memory[i]);
	}
};

void Transform_Alloc(TRANSFORM_PTR p_transform)
{
	Arr_Alloc_TransformPtr(&p_transform->Childs, 0);
	Transform_MarkMatricsForRecalculation(p_transform);
};

void Transform_Free(TRANSFORM_PTR p_transform)
{
	Arr_Free_TransformPtr(&p_transform->Childs);
};

void Transform_AddChild(TRANSFORM_PTR p_transform, TRANSFORM_PTR p_newChild)
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

void Transform_SetLocalPosition(TRANSFORM_PTR p_transform, const VECTOR3F_PTR p_localPosition)
{
	if (!Vec_Equals_3f(&p_transform->LocalPosition, p_localPosition))
	{
		Transform_MarkMatricsForRecalculation(p_transform);
		p_transform->LocalPosition = *p_localPosition;
	}
};

void Transform_SetLocalRotation(TRANSFORM_PTR p_transform, const QUATERNION4F_PTR p_localRotation)
{
	if (!Quat_Equals(&p_transform->LocalRotation, p_localRotation))
	{
		Transform_MarkMatricsForRecalculation(p_transform);
		p_transform->LocalRotation = *p_localRotation;
	}
};

void Transform_SetLocalScale(TRANSFORM_PTR p_transform, const VECTOR3F_PTR p_localScale)
{
	if (!Vec_Equals_3f(&p_transform->LocalScale, p_localScale))
	{
		Transform_MarkMatricsForRecalculation(p_transform);
		p_transform->LocalScale = *p_localScale;
	}
};

void Transform_SetWorldPosition(TRANSFORM_PTR p_transform, const VECTOR3F_PTR p_worldPosition)
{
	if (p_transform->Parent == NULL)
	{
		Transform_SetLocalPosition(p_transform, p_worldPosition);
	}
	else
	{
		MATRIX4F tmp_mat4; VECTOR4F tmp_vec4;
		VECTOR4F l_localPosition;
		Transform_GetWorldToLocalMatrix(p_transform->Parent, &tmp_mat4);
		tmp_vec4.Vec3 = *p_worldPosition; tmp_vec4.Vec3_w = 1.0f;
		Mat_Mul_M4F_V4F(&tmp_mat4, &tmp_vec4, &l_localPosition);
		Transform_SetLocalPosition(p_transform, &l_localPosition.Vec3);
	}
};

void Transform_AddToWorldPosition(TRANSFORM_PTR p_transform, const VECTOR3F_PTR p_worldPosition_delta)
{
	VECTOR3F tmp_vec3;
	Transform_GetWorldPosition(p_transform, &tmp_vec3);
	Vec_Add_3f_3f(&tmp_vec3, p_worldPosition_delta, &tmp_vec3);
	Transform_SetWorldPosition(p_transform, &tmp_vec3);
};


void Transform_SetWorldRotation(TRANSFORM_PTR p_transform, const QUATERNION4F_PTR p_worldRotation)
{
	if (p_transform->Parent == NULL)
	{
		Transform_SetLocalRotation(p_transform, p_worldRotation);
	}
	else
	{
		QUATERNION4F tmp_quat_0, tmp_quat_1;
		QUATERNION4F l_settedLocalQuaternion;

		Transform_GetWorldRotation(p_transform->Parent, &tmp_quat_1);
		Quat_conjugate(&tmp_quat_1, &tmp_quat_0);
		Quat_Mul(&tmp_quat_0, p_worldRotation, &l_settedLocalQuaternion);
		Transform_SetLocalRotation(p_transform, &l_settedLocalQuaternion);
	}
};

void Transform_SetWorldScale(TRANSFORM_PTR p_transform, const VECTOR3F_PTR p_worldScale)
{
	if (p_transform->Parent == NULL)
	{
		Transform_SetLocalScale(p_transform, p_worldScale);
	}
	else
	{
		VECTOR3F tmp_vec3_0, tmp_vec3_1;
		VECTOR3F l_parentWorldScale;
		Transform_GetWorldScaleFactor(p_transform->Parent, &l_parentWorldScale);

		Vec_Inv_3f(&l_parentWorldScale, &tmp_vec3_0);
		Vec_Mul_3f_3f(p_worldScale, &tmp_vec3_0, &tmp_vec3_1);
		Transform_SetLocalScale(p_transform, &tmp_vec3_1);
	}
};

void Transform_UpdateMatricesIfNecessary(TRANSFORM_PTR p_transform)
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
				MATRIX4F tmp_mat4_0, tmp_mat4_1;
				Transform_GetLocalToWorldMatrix(p_transform->Parent, &tmp_mat4_0);
				Mat_Mul_M4F_M4F(&tmp_mat4_0, &p_transform->LocalToWorldMatrix, &tmp_mat4_1);
				p_transform->LocalToWorldMatrix = tmp_mat4_1;
			}
		}

		p_transform->MatricesMustBeRecalculated = false;
	}
};

void Transform_GetLocalToWorldMatrix(TRANSFORM_PTR p_transform, MATRIX4F_PTR p_out)
{
	Transform_UpdateMatricesIfNecessary(p_transform);
	*p_out = p_transform->LocalToWorldMatrix;
};

void Transform_GetWorldToLocalMatrix(TRANSFORM_PTR p_transform, MATRIX4F_PTR p_out)
{
	MATRIX4F tmp_mat4;
	Transform_GetLocalToWorldMatrix(p_transform, &tmp_mat4);
	Mat_Inv_M4F(&tmp_mat4, p_out);
};

void Transform_CalculateMatrixToProjectFromTransformToAnother(TRANSFORM_PTR p_source, TRANSFORM_PTR p_target, MATRIX4F_PTR p_out)
{
	MATRIX4F tmp_mat4_1, tmp_mat4_2;
	Transform_GetLocalToWorldMatrix(p_source, &tmp_mat4_1);
	Transform_GetLocalToWorldMatrix(p_source, &tmp_mat4_2);
	Mat_Mul_M4F_M4F(&tmp_mat4_1, &tmp_mat4_2, p_out);
};

void Transform_GetWorldPosition(TRANSFORM_PTR p_transform, VECTOR3F_PTR p_out)
{
	MATRIX4F tmp_mat4_1; VECTOR4F tmp_vec4_0;
	Transform_GetLocalToWorldMatrix(p_transform, &tmp_mat4_1);
	Mat_GetTranslation_M4F(&tmp_mat4_1, &tmp_vec4_0);
	*p_out = tmp_vec4_0.Vec3;
};

void Transform_GetWorldRotation(TRANSFORM_PTR p_transform, QUATERNION4F_PTR p_out)
{
	if (p_transform->Parent)
	{
		QUATERNION4F tmp_quat_0;
		Transform_GetWorldRotation(p_transform->Parent, &tmp_quat_0);
		Quat_Mul(&tmp_quat_0, &p_transform->LocalRotation, p_out);
	}
	else
	{
		*p_out = p_transform->LocalRotation;
	}
};

void Transform_GetWorldScaleFactor(TRANSFORM_PTR p_transform, VECTOR3F_PTR p_out)
{
	if (p_transform->Parent)
	{
		VECTOR3F tmp_vec3_0;
		Transform_GetWorldScaleFactor(p_transform->Parent, &tmp_vec3_0);
		Vec_Mul_3f_3f(&tmp_vec3_0, &p_transform->LocalScale, p_out);
	}
	else
	{
		*p_out = p_transform->LocalScale;
	}
};

void Transform_GetRight(TRANSFORM_PTR p_transform, VECTOR3F_PTR p_out)
{
	MATRIX4F tmp_mat4_1;
	Transform_GetLocalToWorldMatrix(p_transform, &tmp_mat4_1);
	Vec_Normalize_3f(&tmp_mat4_1.Right.Vec3, p_out);
};

void Transform_GetUp(TRANSFORM_PTR p_transform, VECTOR3F_PTR p_out)
{
	MATRIX4F tmp_mat4_1;
	Transform_GetLocalToWorldMatrix(p_transform, &tmp_mat4_1);
	Vec_Normalize_3f(&tmp_mat4_1.Up.Vec3, p_out);
};

void Transform_GetForward(TRANSFORM_PTR p_transform, VECTOR3F_PTR p_out)
{
	MATRIX4F tmp_mat4_1;
	Transform_GetLocalToWorldMatrix(p_transform, &tmp_mat4_1);
	Vec_Normalize_3f(&tmp_mat4_1.Forward.Vec3, p_out);
};
