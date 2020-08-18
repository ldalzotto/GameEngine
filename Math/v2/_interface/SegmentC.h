#pragma once

#include "v2/_source/MatrixC_def.h"
#include "SegmentC_def.h"

void Seg_Direction_V2F(const SEGMENT_VECTOR2F_PTR p_segment, VECTOR2F_PTR p_out);
void Seg_ToVector_V2F(const SEGMENT_VECTOR2F_PTR p_segment, VECTOR2F_PTR p_out);
void Seg_Mul_V2F_V2F(const SEGMENT_VECTOR2F_PTR p_segment, const VECTOR2F_PTR p_right, SEGMENT_VECTOR2F_PTR p_out);

void Seg_Direction_V3F(const SEGMENT_VECTOR3F_PTR p_segment, VECTOR3F_PTR p_out);
void Seg_ToVector_V3F(const SEGMENT_VECTOR3F_PTR p_segment, VECTOR3F_PTR p_out);
void Seg_Mul_V3F_V3F(const SEGMENT_VECTOR3F_PTR p_segment, const VECTOR3F_PTR p_right, SEGMENT_VECTOR3F_PTR p_out);

void Seg_Direction_V4F(const SEGMENT_VECTOR4F_PTR p_segment, VECTOR4F_PTR p_out);
void Seg_ToVector_V4F(const SEGMENT_VECTOR4F_PTR p_segment, VECTOR4F_PTR p_out);
void Seg_Mul_V4F_V4F(const SEGMENT_VECTOR4F_PTR p_segment, const VECTOR4F_PTR p_right, SEGMENT_VECTOR4F_PTR p_out);
void Seg_Mul_V4F_M4F(const SEGMENT_VECTOR4F_PTR p_segment, const MATRIX4F_PTR p_right, SEGMENT_VECTOR4F_PTR p_out);