#pragma once

#include "v2/_interface/MatrixC_def.h"
#include "SegmentC_def.h"

void Seg_Direction_V2F(const Segment_Vector2f_PTR p_segment, Vector2f_PTR p_out);
void Seg_ToVector_V2F(const Segment_Vector2f_PTR p_segment, Vector2f_PTR p_out);
void Seg_Mul_V2F_V2F(const Segment_Vector2f_PTR p_segment, const Vector2f_PTR p_right, Segment_Vector2f_PTR p_out);

void Seg_Direction_V3F(const Segment_Vector3f_PTR p_segment, Vector3f_PTR p_out);
void Seg_ToVector_V3F(const Segment_Vector3f_PTR p_segment, Vector3f_PTR p_out);
void Seg_Mul_V3F_V3F(const Segment_Vector3f_PTR p_segment, const Vector3f_PTR p_right, Segment_Vector3f_PTR p_out);
void Seg_Mul_V3F_M3F(const Segment_Vector3f_PTR p_segment, const Matrix3f_PTR p_right, Segment_Vector3f_PTR p_out);

void Seg_Direction_V4F(const Segment_Vector4f_PTR p_segment, Vector4f_PTR p_out);
void Seg_ToVector_V4F(const Segment_Vector4f_PTR p_segment, Vector4f_PTR p_out);
void Seg_Mul_V4F_V4F(const Segment_Vector4f_PTR p_segment, const Vector4f_PTR p_right, Segment_Vector4f_PTR p_out);
void Seg_Mul_V4F_M4F(const Segment_Vector4f_PTR p_segment, const Matrix4f_PTR p_right, Segment_Vector4f_PTR p_out);