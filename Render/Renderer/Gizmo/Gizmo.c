#include "Gizmo.h"

#include "v2/_interface/BoxC.h"
#include "v2/_interface/TransformC.h"
#include "v2/_interface/MatrixC.h"
#include "v2/_interface/VectorC.h"
#include "Renderer/Draw/DrawFunctions.h"
#include "DataStructures/ARRAY.h"



void GizmoBuffer_alloc(GizmoBuffer* p_buffer)
{
	Arr_Alloc((ARRAY_PTR)&p_buffer->Lines, sizeof(GizmoLine), 0);
	Arr_Alloc((ARRAY_PTR)&p_buffer->Vertices, sizeof(GizmoVertex), 0);
};

void GizmoBuffer_clear(GizmoBuffer* p_buffer)
{
	Arr_Clear((ARRAY_PTR)&p_buffer->Lines);
	Arr_Clear((ARRAY_PTR)&p_buffer->Vertices);
};

void GizmoBuffer_free(GizmoBuffer* p_buffer)
{
	Arr_Free((ARRAY_PTR)&p_buffer->Lines);
	Arr_Free((ARRAY_PTR)&p_buffer->Vertices);
};

void Gizmo_Render(GizmoRendererInput* p_input, TEXTURE3C_PTR p_to, RECTI_PTR p_to_clipRect, ARRAY_RASTERISATIONSTEP_PTR RasterizedPixelsBuffer)
{
	for (size_t i = 0; i < p_input->Buffer->Lines.Size; i++)
	{
		GizmoLine* l_line = &p_input->Buffer->Lines.Memory[i];
		VECTOR4F tmp_vec4_0;

		VECTOR4F l_lineBegin;
		VECTOR4F l_lineEnd;

		// World to camera
		Mat_Mul_M4F_V4F(p_input->CameraBuffer->ViewMatrix, &p_input->Buffer->Vertices.Memory[l_line->v1].WorldPosition, &l_lineBegin);
		Mat_Mul_M4F_V4F(p_input->CameraBuffer->ViewMatrix, &p_input->Buffer->Vertices.Memory[l_line->v2].WorldPosition, &l_lineEnd);

		// Camera to clip
		Mat_Mul_M4F_V4F_Homogeneous(p_input->CameraBuffer->ProjectionMatrix, &l_lineBegin, &tmp_vec4_0);
		l_lineBegin = tmp_vec4_0;
		Mat_Mul_M4F_V4F_Homogeneous(p_input->CameraBuffer->ProjectionMatrix, &l_lineEnd, &tmp_vec4_0);
		l_lineEnd = tmp_vec4_0;

		// To pixel
		{
			l_lineBegin.z = 1.0f;
			Mat_Mul_M4F_V4F(p_input->GraphicsAPIToScreeMatrix, &l_lineBegin, &tmp_vec4_0);
			l_lineBegin = tmp_vec4_0;
			l_lineEnd.z = 1.0f;
			Mat_Mul_M4F_V4F(p_input->GraphicsAPIToScreeMatrix, &l_lineEnd, &tmp_vec4_0);
			l_lineEnd = tmp_vec4_0;
		}

		// Rasterize
		{
			Draw_LineClipped(&l_lineBegin.Vec3.Vec2, &l_lineEnd.Vec3.Vec2, RasterizedPixelsBuffer, p_to, p_to_clipRect, &l_line->Color);
		}
	}

	GizmoBuffer_clear(p_input->Buffer);
};

void Gizmo_drawLine_indices(GizmoBuffer* p_gizmo, GizmoVertexIndex* p_begin, GizmoVertexIndex* p_end, const VECTOR3C* p_color)
{
	GizmoLine l_line = { *p_begin , *p_end, *p_color };
	Arr_PushBackRealloc((ARRAY_PTR)&p_gizmo->Lines, sizeof(l_line), (char*)&l_line);
}

void Gizmo_pushVertex(GizmoBuffer* p_gizmo, const VECTOR3F_PTR p_position, GizmoVertexIndex* p_out_index)
{
	GizmoVertex l_gizmoVertex = {0};
	l_gizmoVertex.WorldPosition.Vec3 = *p_position; l_gizmoVertex.WorldPosition.Vec3_w = 1.0f;
	Arr_PushBackRealloc((ARRAY_PTR)&p_gizmo->Vertices, sizeof(l_gizmoVertex), (char*)&l_gizmoVertex);
	*p_out_index = p_gizmo->Vertices.Size - 1;
};

void Gizmo_DrawLine(GizmoBuffer* p_gizmo, const VECTOR3F_PTR p_begin, const VECTOR3F_PTR p_end)
{
	VECTOR3C l_color = { (char)255, 255, 255 };
	Gizmo_DrawLine_Color(p_gizmo, p_begin, p_end, &l_color);
};

void Gizmo_DrawLine_Color(GizmoBuffer* p_gizmo, const VECTOR3F_PTR p_begin, const VECTOR3F_PTR p_end, const VECTOR3C_PTR p_color)
{
	GizmoVertexIndex l_beginIndex;
	Gizmo_pushVertex(p_gizmo, p_begin, &l_beginIndex);
	GizmoVertexIndex l_endIndex;
	Gizmo_pushVertex(p_gizmo, p_end, &l_endIndex);
	Gizmo_drawLine_indices(p_gizmo, &l_beginIndex, &l_endIndex, p_color);
};

void Gizmo_DrawPoint(GizmoBuffer* p_gizmo, const VECTOR3F_PTR p_point)
{
	VECTOR3C l_color = { (char)255, 255, 255 };
	Gizmo_DrawPoint_Color(p_gizmo, p_point, &l_color);
};

void Gizmo_DrawPoint_Color(GizmoBuffer* p_gizmo, const VECTOR3F_PTR p_point, const VECTOR3C_PTR p_color)
{
	VECTOR3F tmp_vec3;
	VECTOR3F l_begin;
	VECTOR3F l_end;

	float l_lineLenght = 0.1f;

	{
		tmp_vec3 = (VECTOR3F) { l_lineLenght, 0.0f, 0.0f };
		Vec_Add_3f_3f(p_point, &tmp_vec3, &l_begin);
		tmp_vec3 = (VECTOR3F) { -1.0f * l_lineLenght, 0.0f, 0.0f };
		Vec_Add_3f_3f(p_point, &tmp_vec3, &l_end);

		GizmoVertexIndex l_beginIndex;
		Gizmo_pushVertex(p_gizmo, &l_begin, &l_beginIndex);

		GizmoVertexIndex l_endIndex;
		Gizmo_pushVertex(p_gizmo, &l_end, &l_endIndex);

		Gizmo_drawLine_indices(p_gizmo, &l_beginIndex, &l_endIndex, p_color);
	}
	{
		tmp_vec3 = (VECTOR3F) { 0.0f, l_lineLenght, 0.0f };
		Vec_Add_3f_3f(p_point, &tmp_vec3, &l_begin);
		tmp_vec3 = (VECTOR3F) { 0.0f, -1.0f * l_lineLenght, 0.0f };
		Vec_Add_3f_3f(p_point, &tmp_vec3, &l_end);

		GizmoVertexIndex l_beginIndex;
		Gizmo_pushVertex(p_gizmo, &l_begin, &l_beginIndex);

		GizmoVertexIndex l_endIndex;
		Gizmo_pushVertex(p_gizmo, &l_end, &l_endIndex);

		Gizmo_drawLine_indices(p_gizmo, &l_beginIndex, &l_endIndex, p_color);
	}
	{
		tmp_vec3 = (VECTOR3F){ 0.0f, 0.0f, l_lineLenght };
		Vec_Add_3f_3f(p_point, &tmp_vec3, &l_begin);
		tmp_vec3 = (VECTOR3F){ 0.0f, 0.0f, -1.0f * l_lineLenght };
		Vec_Add_3f_3f(p_point, &tmp_vec3, &l_end);

		GizmoVertexIndex l_beginIndex;
		Gizmo_pushVertex(p_gizmo, &l_begin, &l_beginIndex);

		GizmoVertexIndex l_endIndex;
		Gizmo_pushVertex(p_gizmo, &l_end, &l_endIndex);

		Gizmo_drawLine_indices(p_gizmo, &l_beginIndex, &l_endIndex, p_color);
	}
};

void Gizmo_DrawBox(GizmoBuffer* p_gizmo, const BOXF_PTR p_box, const MATRIX4F_PTR p_localToWorldMatrix, bool p_withCenter, const VECTOR3C_PTR p_color)
{
	VECTOR3C l_color = { 255, 255, 255 };
	if (p_color)
	{
		l_color = *p_color;
	}

	BOXFPOINTS l_boxPoints; Box_ExtractPoints_F(p_box, &l_boxPoints); BoxPoints_Mul_F_M4F(&l_boxPoints, p_localToWorldMatrix, &l_boxPoints);

	GizmoVertexIndex LDF_index, LDB_index, LUF_index, RDF_index, LUB_index, RUF_index, RDB_index, RUB_index;
	{
		Gizmo_pushVertex(p_gizmo, &l_boxPoints.L_D_F, &LDF_index);
		Gizmo_pushVertex(p_gizmo, &l_boxPoints.L_D_B, &LDB_index);
		Gizmo_pushVertex(p_gizmo, &l_boxPoints.L_U_F, &LUF_index);
		Gizmo_pushVertex(p_gizmo, &l_boxPoints.R_D_F, &RDF_index);
		Gizmo_pushVertex(p_gizmo, &l_boxPoints.L_U_B, &LUB_index);
		Gizmo_pushVertex(p_gizmo, &l_boxPoints.R_U_F, &RUF_index);
		Gizmo_pushVertex(p_gizmo, &l_boxPoints.R_D_B, &RDB_index);
		Gizmo_pushVertex(p_gizmo, &l_boxPoints.R_U_B, &RUB_index);
	}

	Gizmo_drawLine_indices(p_gizmo, &LDF_index, &LDB_index, &l_color);
	Gizmo_drawLine_indices(p_gizmo, &LDF_index, &LUF_index, &l_color);
	Gizmo_drawLine_indices(p_gizmo, &LDF_index, &RDF_index, &l_color);
	Gizmo_drawLine_indices(p_gizmo, &LUF_index, &LUB_index, &l_color);
	Gizmo_drawLine_indices(p_gizmo, &LUF_index, &RUF_index, &l_color);
	Gizmo_drawLine_indices(p_gizmo, &LDB_index, &LUB_index, &l_color);
	Gizmo_drawLine_indices(p_gizmo, &LDB_index, &RDB_index, &l_color);
	Gizmo_drawLine_indices(p_gizmo, &RDF_index, &RUF_index, &l_color);
	Gizmo_drawLine_indices(p_gizmo, &RDF_index, &RDB_index, &l_color);
	Gizmo_drawLine_indices(p_gizmo, &RUB_index, &RDB_index, &l_color);
	Gizmo_drawLine_indices(p_gizmo, &RUB_index, &LUB_index, &l_color);
	Gizmo_drawLine_indices(p_gizmo, &RUB_index, &RUF_index, &l_color);

	if (p_withCenter)
	{
		Gizmo_DrawPoint_Color(p_gizmo, &l_boxPoints.Center, &l_color);
	}
};

void Gizmo_DrawTransform(GizmoBuffer* p_gizmo, TRANSFORM_PTR p_transform)
{
	VECTOR3F tmp_vec3_0, tmp_vec3_1, tmp_vec3_2, tmp_vec3_3;
	Transform_GetWorldPosition(p_transform, &tmp_vec3_0);
	Transform_GetRight(p_transform, &tmp_vec3_1);
	Transform_GetUp(p_transform, &tmp_vec3_2);
	Transform_GetForward(p_transform, &tmp_vec3_3);
	Gizmo_DrawTransform_Axis(p_gizmo, (const VECTOR3F_PTR)&tmp_vec3_0, (const VECTOR3F_PTR)&tmp_vec3_1, (const VECTOR3F_PTR)&tmp_vec3_2, (const VECTOR3F_PTR)&tmp_vec3_3);

};

void Gizmo_DrawTransform_Axis(GizmoBuffer* p_gizmo, const VECTOR3F_PTR p_center, const VECTOR3F_PTR p_right, const VECTOR3F_PTR p_up, const VECTOR3F_PTR p_forward)
{
	VECTOR3C tmp_vec3_1;
	VECTOR3F tmp_vec3_0;
	Vec_Add_3f_3f(p_center, p_right, &tmp_vec3_0);
	tmp_vec3_1 = (VECTOR3C){ (char)255, 0, 0 };
	Gizmo_DrawLine_Color(p_gizmo, p_center, &tmp_vec3_0, &tmp_vec3_1);
	Vec_Add_3f_3f(p_center, p_up, &tmp_vec3_0);
	tmp_vec3_1 = (VECTOR3C){ 0, (char)255, 0 };
	Gizmo_DrawLine_Color(p_gizmo, p_center, &tmp_vec3_0, &tmp_vec3_1);
	Vec_Add_3f_3f(p_center, p_forward, &tmp_vec3_0);
	tmp_vec3_1 = (VECTOR3C){ 0, 0, (char)255 };
	Gizmo_DrawLine_Color(p_gizmo, p_center, &tmp_vec3_0, &tmp_vec3_1);
};