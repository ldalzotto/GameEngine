#include "Segment.h"

#include "Math/Matrix/Matrix.h"
#include "Math/Vector/VectorMath.h"
#include "Math/Matrix/MatrixMath.h"

namespace _GameEngine::_Math
{

	_Math::Vector3f Segment_direction(Segment* p_segment)
	{
		_Math::Vector3f l_direction;
		_Math::Vector3f_min(&p_segment->End, &p_segment->Begin, &l_direction);
		_Math::Vector3f_normalize(&l_direction);
		return l_direction;
	};

	_Math::Vector3f Segment_toVector(Segment* p_segment)
	{
		_Math::Vector3f l_vector;
		_Math::Vector3f_min(&p_segment->End, &p_segment->Begin, &l_vector);
		return l_vector;
	};

	float Segment_length(Segment* p_segment)
	{
		_Math::Vector3f l_vector = Segment_toVector(p_segment);
		return _Math::Vector3f_length(&l_vector);
	};

	void Segment_mul(Segment* p_segment, _Math::Matrix4x4f* p_matrix, Segment* p_out)
	{
		_Math::Vector4f l_begin4f, l_end4f;
		{
			_Math::Vector4f_build(&p_segment->Begin, 1.0f, &l_begin4f);
			_Math::Vector4f_build(&p_segment->End, 1.0f, &l_end4f);
		}

		_Math::Vector4f l_projectedBegin, l_projectedEnd;
		_Math::Matrixf4x4_mul(p_matrix, &l_begin4f, &l_projectedBegin);
		_Math::Matrixf4x4_mul(p_matrix, &l_end4f, &l_projectedEnd);

		p_out->Begin = { l_projectedBegin.x, l_projectedBegin.y, l_projectedBegin.z };
		p_out->End = { l_projectedEnd.x, l_projectedEnd.y, l_projectedEnd.z };
	};

	void Segment_mul(Segment* p_segment, _Math::Matrix3x3f* p_matrix, Segment* p_out)
	{
		_Math::Matrix3x3f_mul(p_matrix, &p_segment->Begin, &p_out->Begin);
		_Math::Matrix3x3f_mul(p_matrix, &p_segment->End, &p_out->End);
	};
}