#include "Gizmo.hpp"

#include "Renderer/Draw/DrawFunctions.hpp"
#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Vector/VectorMath.hpp"
#include "v2/Box/BoxMath.h"
#include "v2/Transform/TransformM.hpp"

using namespace _MathV2;

namespace _RenderV2
{


	void GizmoBuffer_alloc(GizmoBuffer* p_buffer)
	{
		_Core::VectorT_alloc(&p_buffer->Lines, 0);
		_Core::VectorT_alloc(&p_buffer->Vertices, 0);
	};

	void GizmoBuffer_clear(GizmoBuffer* p_buffer)
	{
		_Core::VectorT_clear(&p_buffer->Lines);
		_Core::VectorT_clear(&p_buffer->Vertices);
	};

	void GizmoBuffer_free(GizmoBuffer* p_buffer)
	{
		_Core::VectorT_free(&p_buffer->Lines);
		_Core::VectorT_free(&p_buffer->Vertices);
	};

	void Gizmo::render(GizmoRendererInput* p_input, Texture<3, char>* p_to, _MathV2::Rect<int>* p_to_clipRect, _Core::VectorT<RasterizationStep>* RasterizedPixelsBuffer)
	{
		_Core::VectorIteratorT<GizmoLine> l_gizmoLines = _Core::VectorT_buildIterator(&p_input->Buffer->Lines);
		while (_Core::VectorIteratorT_moveNext(&l_gizmoLines))
		{
			_MathV2::Vector4<float> tmp_vec4_0;

			_MathV2::Vector4<float> l_lineBegin;
			_MathV2::Vector4<float> l_lineEnd;

			// World to camera
			_MathV2::MatrixM::mul(p_input->CameraBuffer->ViewMatrix, &_Core::VectorT_at(&p_input->Buffer->Vertices, l_gizmoLines.Current->v1)->WorldPosition, &l_lineBegin);
			_MathV2::MatrixM::mul(p_input->CameraBuffer->ViewMatrix, &_Core::VectorT_at(&p_input->Buffer->Vertices, l_gizmoLines.Current->v2)->WorldPosition, &l_lineEnd);

			// Camera to clip
			l_lineBegin = *_MathV2::MatrixM::mul_homogeneous(p_input->CameraBuffer->ProjectionMatrix, &l_lineBegin, &tmp_vec4_0);
			l_lineEnd = *_MathV2::MatrixM::mul_homogeneous(p_input->CameraBuffer->ProjectionMatrix, &l_lineEnd, &tmp_vec4_0);

			// To pixel
			{
				l_lineBegin.z = 1.0f; l_lineBegin = *_MathV2::MatrixM::mul(p_input->GraphicsAPIToScreeMatrix, &l_lineBegin, &tmp_vec4_0);
				l_lineEnd.z = 1.0f; l_lineEnd = *_MathV2::MatrixM::mul(p_input->GraphicsAPIToScreeMatrix, &l_lineEnd, &tmp_vec4_0);
			}

			// Rasterize
			{
				DrawM::DrawLineClipped(_MathV2::VectorM::cast2(&l_lineBegin), _MathV2::VectorM::cast2(&l_lineEnd), RasterizedPixelsBuffer, p_to, p_to_clipRect, &l_gizmoLines.Current->Color);
			}
		}

		GizmoBuffer_clear(p_input->Buffer);
	};

	void Gizmo_drawLine_indices(GizmoBuffer* p_gizmo, GizmoVertexIndex* p_begin, GizmoVertexIndex* p_end, const _MathV2::Vector<3, char>* p_color)
	{
		GizmoLine l_line = {*p_begin , *p_end, *p_color};
		_Core::VectorT_pushBack(&p_gizmo->Lines, &l_line);
	}

	void Gizmo_pushVertex(GizmoBuffer* p_gizmo, const _MathV2::Vector<3, float>* p_position, GizmoVertexIndex* p_out_index)
	{
		GizmoVertex l_gizmoVertex{};
		l_gizmoVertex.WorldPosition = _MathV2::VectorM::cast(p_position, 1.0f);
		_Core::VectorT_pushBack(&p_gizmo->Vertices, &l_gizmoVertex);
		*p_out_index = p_gizmo->Vertices.Size - 1;
	};

	void Gizmo::drawLine(GizmoBuffer* p_gizmo, const _MathV2::Vector3<float>* p_begin, const _MathV2::Vector3<float>* p_end)
	{
		Vector3<char> l_color = { 255, 255, 255 };
		Gizmo::drawLine(p_gizmo, p_begin, p_end, &l_color);
	};

	void Gizmo::drawLine(GizmoBuffer* p_gizmo, const _MathV2::Vector3<float>* p_begin, const _MathV2::Vector3<float>* p_end, const _MathV2::Vector3<char>* p_color)
	{
		GizmoVertexIndex l_beginIndex;
		Gizmo_pushVertex(p_gizmo, p_begin, &l_beginIndex);
		GizmoVertexIndex l_endIndex;
		Gizmo_pushVertex(p_gizmo, p_end, &l_endIndex);
		Gizmo_drawLine_indices(p_gizmo, &l_beginIndex, &l_endIndex, p_color);
	};
	
	void Gizmo::drawPoint(GizmoBuffer* p_gizmo, const _MathV2::Vector3<float>* p_point)
	{
		Vector3<char> l_color = { 255, 255, 255 };
		Gizmo::drawPoint(p_gizmo, p_point, &l_color);
	};
	
	void Gizmo::drawPoint(GizmoBuffer* p_gizmo, const _MathV2::Vector3<float>* p_point, const _MathV2::Vector3<char>* p_color)
	{
		Vector3<float> tmp_vec3;
		Vector3<float> l_begin;
		Vector3<float> l_end;
		Vector3<float> l_deltaAdd;

		float l_lineLenght = 0.1f;

		{
			tmp_vec3 = { l_lineLenght, 0.0f, 0.0f };
			VectorM::add(p_point, &tmp_vec3, &l_begin);
			tmp_vec3 = { -1.0f * l_lineLenght, 0.0f, 0.0f };
			VectorM::add(p_point, &tmp_vec3, &l_end);

			GizmoVertexIndex l_beginIndex;
			Gizmo_pushVertex(p_gizmo, &l_begin, &l_beginIndex);

			GizmoVertexIndex l_endIndex;
			Gizmo_pushVertex(p_gizmo, &l_end, &l_endIndex);

			Gizmo_drawLine_indices(p_gizmo, &l_beginIndex, &l_endIndex, p_color);
		}
		{
			tmp_vec3 = { 0.0f, l_lineLenght, 0.0f };
			VectorM::add(p_point, &tmp_vec3, &l_begin);
			tmp_vec3 = { 0.0f, -1.0f * l_lineLenght, 0.0f };
			VectorM::add(p_point, &tmp_vec3, &l_end);

			GizmoVertexIndex l_beginIndex;
			Gizmo_pushVertex(p_gizmo, &l_begin, &l_beginIndex);

			GizmoVertexIndex l_endIndex;
			Gizmo_pushVertex(p_gizmo, &l_end, &l_endIndex);

			Gizmo_drawLine_indices(p_gizmo, &l_beginIndex, &l_endIndex, p_color);
		}
		{
			tmp_vec3 = { 0.0f, 0.0f, l_lineLenght };
			VectorM::add(p_point, &tmp_vec3, &l_begin);
			tmp_vec3 = { 0.0f, 0.0f, -1.0f * l_lineLenght };
			VectorM::add(p_point, &tmp_vec3, &l_end);

			GizmoVertexIndex l_beginIndex;
			Gizmo_pushVertex(p_gizmo, &l_begin, &l_beginIndex);

			GizmoVertexIndex l_endIndex;
			Gizmo_pushVertex(p_gizmo, &l_end, &l_endIndex);

			Gizmo_drawLine_indices(p_gizmo, &l_beginIndex, &l_endIndex, p_color);
		}
	};
	
	void Gizmo::drawBox(GizmoBuffer* p_gizmo, const _MathV2::Box* p_box, const _MathV2::Matrix<4, 4, float>* p_localToWorldMatrix, bool p_withCenter, const _MathV2::Vector3<char>* p_color)
	{
		_MathV2::Vector3<char> l_color = { 255, 255, 255 };
		if (p_color)
		{
			l_color = *p_color;
		}

		BoxPoints l_boxPoints; BoxPoints_mul(Box_extractPoints(p_box, &l_boxPoints), p_localToWorldMatrix, &l_boxPoints);

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
			Gizmo::drawPoint(p_gizmo, &l_boxPoints.Center, &l_color);
		}
	};
	
	void Gizmo::drawTransform(GizmoBuffer* p_gizmo, _MathV2::Transform* p_transform)
	{
		Vector3<float> tmp_vec3_0, tmp_vec3_1, tmp_vec3_2, tmp_vec3_3;
		Gizmo::drawTransform(p_gizmo, TransformM::getWorldPosition(p_transform, &tmp_vec3_0),
			TransformM::getRight(p_transform, &tmp_vec3_1), TransformM::getUp(p_transform, &tmp_vec3_2), TransformM::getForward(p_transform, &tmp_vec3_3));

	};
	
	void Gizmo::drawTransform(GizmoBuffer* p_gizmo, const _MathV2::Vector3<float>* p_center, const _MathV2::Vector3<float>* p_right, const _MathV2::Vector3<float>* p_up, const _MathV2::Vector3<float>* p_forward)
	{
		Vector3<char> tmp_vec3_1;
		Vector3<float> tmp_vec3_0;
		tmp_vec3_1 = { (char)255, 0, 0 }; Gizmo::drawLine(p_gizmo, p_center, VectorM::add(p_center, p_right, &tmp_vec3_0), &tmp_vec3_1);
		tmp_vec3_1 = { 0, (char)255, 0 }; Gizmo::drawLine(p_gizmo, p_center, VectorM::add(p_center, p_up, &tmp_vec3_0), &tmp_vec3_1);
		tmp_vec3_1 = { 0, 0, (char)255 }; Gizmo::drawLine(p_gizmo, p_center, VectorM::add(p_center, p_forward, &tmp_vec3_0), &tmp_vec3_1);
	};
}