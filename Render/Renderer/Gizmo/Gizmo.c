#include "Gizmo.h"

#include "v2/_interface/BoxC.h"
#include "v2/_interface/TransformC.h"
#include "v2/_interface/MatrixC.h"
#include "v2/_interface/VectorC.h"
#include "v2/_interface/WindowSize.h"
#include "Renderer/Draw/DrawFunctions.h"
#include "Raster/Rasterizer.h"
#include "DataStructures/ARRAY.h"


void GizmoBuffer_alloc(GizmoBuffer* p_buffer)
{
	Arr_Alloc(&p_buffer->Lines.array, sizeof(GizmoLine), 0);
	Arr_Alloc(&p_buffer->Vertices.array, sizeof(GizmoVertex), 0);
};

void GizmoBuffer_clear(GizmoBuffer* p_buffer)
{
	Arr_Clear(&p_buffer->Lines.array);
	Arr_Clear(&p_buffer->Vertices.array);
};

void GizmoBuffer_free(GizmoBuffer* p_buffer)
{
	Arr_Free(&p_buffer->Lines.array);
	Arr_Free(&p_buffer->Vertices.array);
};

void Gizmo_Render(GizmoRendererInput* p_input, Texture3f_PTR p_to, Recti_PTR p_to_clipRect)
{
	for (size_t i = 0; i < p_input->Buffer->Lines.Size; i++)
	{
		GizmoLine* l_line = &p_input->Buffer->Lines.Memory[i];
		Vector4f tmp_vec4_0;

		Vector4f l_lineBegin;
		Vector4f l_lineEnd;

		// World to camera
		Mat_Mul_M4F_V4F(&p_input->CameraBuffer->ViewMatrix, &p_input->Buffer->Vertices.Memory[l_line->v1].WorldPosition, &l_lineBegin);
		Mat_Mul_M4F_V4F(&p_input->CameraBuffer->ViewMatrix, &p_input->Buffer->Vertices.Memory[l_line->v2].WorldPosition, &l_lineEnd);

		// Camera to clip
		Mat_Mul_M4F_V4F_Homogeneous(&p_input->CameraBuffer->ProjectionMatrix, &l_lineBegin, &tmp_vec4_0);
		l_lineBegin = tmp_vec4_0;
		Mat_Mul_M4F_V4F_Homogeneous(&p_input->CameraBuffer->ProjectionMatrix, &l_lineEnd, &tmp_vec4_0);
		l_lineEnd = tmp_vec4_0;

		// To pixel

		Vector2i l_lineBegin_pixel;
		Vector2i l_lineEnd_pixel;
		{
			WindowSize_GraphicsAPIToPixel(&p_input->WindowSize, l_lineBegin.x, l_lineBegin.y, &l_lineBegin_pixel.x, &l_lineBegin_pixel.y);
			WindowSize_GraphicsAPIToPixel(&p_input->WindowSize, l_lineEnd.x, l_lineEnd.y, &l_lineEnd_pixel.x, &l_lineEnd_pixel.y);
		}

		// Rasterize
		{
			Draw_LineClipped(&l_lineBegin_pixel, &l_lineEnd_pixel, p_to, p_to_clipRect, &l_line->Color);
		}
	}

	GizmoBuffer_clear(p_input->Buffer);
};

void Gizmo_drawLine_indices(GizmoBuffer* p_gizmo, GizmoVertexIndex* p_begin, GizmoVertexIndex* p_end, const Color3f_PTR p_color)
{
	GizmoLine l_line = { *p_begin , *p_end, *p_color };
	Arr_PushBackRealloc(&p_gizmo->Lines.array, sizeof(l_line), (char*)&l_line);
}

void Gizmo_pushVertex(GizmoBuffer* p_gizmo, const Vector3f_PTR p_position, GizmoVertexIndex* p_out_index)
{
	GizmoVertex l_gizmoVertex = {0};
	l_gizmoVertex.WorldPosition.Vec3 = *p_position; l_gizmoVertex.WorldPosition.Vec3_w = 1.0f;
	Arr_PushBackRealloc(&p_gizmo->Vertices.array, sizeof(l_gizmoVertex), (char*)&l_gizmoVertex);
	*p_out_index = p_gizmo->Vertices.Size - 1;
};

void Gizmo_DrawLine(GizmoBuffer* p_gizmo, const Vector3f_PTR p_begin, const Vector3f_PTR p_end)
{
	Color3f l_color = { 1.0f, 1.0f, 1.0f };
	Gizmo_DrawLine_Color(p_gizmo, p_begin, p_end, &l_color);
};

void Gizmo_DrawLine_Color(GizmoBuffer* p_gizmo, const Vector3f_PTR p_begin, const Vector3f_PTR p_end, const Color3f_PTR p_color)
{
	GizmoVertexIndex l_beginIndex;
	Gizmo_pushVertex(p_gizmo, p_begin, &l_beginIndex);
	GizmoVertexIndex l_endIndex;
	Gizmo_pushVertex(p_gizmo, p_end, &l_endIndex);
	Gizmo_drawLine_indices(p_gizmo, &l_beginIndex, &l_endIndex, p_color);
};

void Gizmo_DrawPoint(GizmoBuffer* p_gizmo, const Vector3f_PTR p_point)
{
	Color3f l_color = { 1.0f, 1.0f, 1.0f };
	Gizmo_DrawPoint_Color(p_gizmo, p_point, &l_color);
};

void Gizmo_DrawPoint_Color(GizmoBuffer* p_gizmo, const Vector3f_PTR p_point, const Color3f_PTR p_color)
{
	Vector3f tmp_vec3;
	Vector3f l_begin;
	Vector3f l_end;

	float l_lineLenght = 0.1f;

	{
		tmp_vec3 = (Vector3f) { l_lineLenght, 0.0f, 0.0f };
		Vec_Add_3f_3f(p_point, &tmp_vec3, &l_begin);
		tmp_vec3 = (Vector3f) { -1.0f * l_lineLenght, 0.0f, 0.0f };
		Vec_Add_3f_3f(p_point, &tmp_vec3, &l_end);

		GizmoVertexIndex l_beginIndex;
		Gizmo_pushVertex(p_gizmo, &l_begin, &l_beginIndex);

		GizmoVertexIndex l_endIndex;
		Gizmo_pushVertex(p_gizmo, &l_end, &l_endIndex);

		Gizmo_drawLine_indices(p_gizmo, &l_beginIndex, &l_endIndex, p_color);
	}
	{
		tmp_vec3 = (Vector3f) { 0.0f, l_lineLenght, 0.0f };
		Vec_Add_3f_3f(p_point, &tmp_vec3, &l_begin);
		tmp_vec3 = (Vector3f) { 0.0f, -1.0f * l_lineLenght, 0.0f };
		Vec_Add_3f_3f(p_point, &tmp_vec3, &l_end);

		GizmoVertexIndex l_beginIndex;
		Gizmo_pushVertex(p_gizmo, &l_begin, &l_beginIndex);

		GizmoVertexIndex l_endIndex;
		Gizmo_pushVertex(p_gizmo, &l_end, &l_endIndex);

		Gizmo_drawLine_indices(p_gizmo, &l_beginIndex, &l_endIndex, p_color);
	}
	{
		tmp_vec3 = (Vector3f){ 0.0f, 0.0f, l_lineLenght };
		Vec_Add_3f_3f(p_point, &tmp_vec3, &l_begin);
		tmp_vec3 = (Vector3f){ 0.0f, 0.0f, -1.0f * l_lineLenght };
		Vec_Add_3f_3f(p_point, &tmp_vec3, &l_end);

		GizmoVertexIndex l_beginIndex;
		Gizmo_pushVertex(p_gizmo, &l_begin, &l_beginIndex);

		GizmoVertexIndex l_endIndex;
		Gizmo_pushVertex(p_gizmo, &l_end, &l_endIndex);

		Gizmo_drawLine_indices(p_gizmo, &l_beginIndex, &l_endIndex, p_color);
	}
};

void Gizmo_DrawBox(GizmoBuffer* p_gizmo, const BoxF_PTR p_box, const Matrix4f_PTR p_localToWorldMatrix, bool p_withCenter, const Color3f_PTR p_color)
{
	Color3f l_color = { 1.0f, 1.0f, 1.0f };
	if (p_color)
	{
		l_color = *p_color;
	}

	BoxFPoints l_boxPoints; Box_ExtractPoints_F(p_box, &l_boxPoints); BoxPoints_Mul_F_M4F(&l_boxPoints, p_localToWorldMatrix, &l_boxPoints);

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

void Gizmo_DrawTransform(GizmoBuffer* p_gizmo, Transform_PTR p_transform)
{
	Vector3f tmp_vec3_0, tmp_vec3_1, tmp_vec3_2, tmp_vec3_3;
	Transform_GetWorldPosition(p_transform, &tmp_vec3_0);
	Transform_GetRight(p_transform, &tmp_vec3_1);
	Transform_GetUp(p_transform, &tmp_vec3_2);
	Transform_GetForward(p_transform, &tmp_vec3_3);
	Gizmo_DrawTransform_Axis(p_gizmo, (const Vector3f_PTR)&tmp_vec3_0, (const Vector3f_PTR)&tmp_vec3_1, (const Vector3f_PTR)&tmp_vec3_2, (const Vector3f_PTR)&tmp_vec3_3);

};

void Gizmo_DrawTransform_Axis(GizmoBuffer* p_gizmo, const Vector3f_PTR p_center, const Vector3f_PTR p_right, const Vector3f_PTR p_up, const Vector3f_PTR p_forward)
{
	Color3f tmp_vec3_1;
	Vector3f tmp_vec3_0;
	Vec_Add_3f_3f(p_center, p_right, &tmp_vec3_0);
	tmp_vec3_1 = (Color3f){ 1.0f, 0, 0 };
	Gizmo_DrawLine_Color(p_gizmo, p_center, &tmp_vec3_0, &tmp_vec3_1);
	Vec_Add_3f_3f(p_center, p_up, &tmp_vec3_0);
	tmp_vec3_1 = (Color3f){ 0, 1.0f, 0 };
	Gizmo_DrawLine_Color(p_gizmo, p_center, &tmp_vec3_0, &tmp_vec3_1);
	Vec_Add_3f_3f(p_center, p_forward, &tmp_vec3_0);
	tmp_vec3_1 = (Color3f){ 0, 0, 1.0f };
	Gizmo_DrawLine_Color(p_gizmo, p_center, &tmp_vec3_0, &tmp_vec3_1);
};