
#define FLOAT_TOLERANCE 0.000001f

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


typedef struct Vector4c_TYP
{
	union
	{
		char Points[4];
		struct { char x, y, z, w; };
	};
} Vector4c, * Vector4c_PTR;

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


typedef struct Plane_TYP
{
	Vector3f Normal;
	Vector3f Point;
}Plane, * Plane_PTR;

typedef struct Frustum_TYP
{
	Plane Left, Right, Bottom, Up, Near, Far;
}Frustum, *Frustum_PTR;





typedef struct CAMERABUFFER_TYP
{
	Matrix4f ViewMatrix;
	Matrix4f ProjectionMatrix;
	Vector4f WorldPosition;
	float Far;
	Frustum CameraFrustum;
}CAMERABUFFER, * CAMERABUFFER_PTR;

typedef struct WindowSize_TYP
{
	uint Width;
	uint Height;
	uint HalfWidth;
	uint HalfHeight;
	float TwoOnWidth;
	float TwoOnHeight;
} WindowSize, * WindowSize_PTR;

typedef struct Material_HANDLE_TYP
{
	size_t Handle;
}Material_HANDLE, * Material_HANDLE_PTR;


typedef struct Vertex_HANDLE_TYP
{
	size_t Handle;
}Vertex_HANDLE, * Vertex_HANDLE_PTR;

typedef size_t VertexIndex;

typedef struct Polygon_VertexIndex_TYP
{
	VertexIndex v1;
	VertexIndex v2;
	VertexIndex v3;
} Polygon_VertexIndex, * Polygon_VertexIndex_PTR;

typedef struct VertexPipeline_TYP
{
	Vertex_HANDLE Vertex;
	size_t AssociatedRenderableObjectPipeline;
	Vector4f WorldPosition;
	Vector4f CameraSpacePosition;
	Vector2i PixelPosition;
	char WorldPositionCalculated;
	char PixelPositionCalculated;
}VertexPipeline, * VertexPipeline_PTR;

typedef struct Vertex_TYP
{
	Vector4f LocalPosition;
}Vertex, * Vertex_PTR;

typedef struct PolygonPipelineV2_TYP
{
	char IsCulled;
	Polygon_VertexIndex VerticesPipelineIndex;

	Vector3f CalculatedFlatWorldNormal;

	size_t AssociatedRenderableObjectPipeline;
	size_t MeshPolygonIndex;
	Material_HANDLE Material;
}PolygonPipelineV2, * PolygonPipelineV2_PTR;


typedef struct RenderedObject_TYP
{
	void* Mesh;
	void* MeshBoundingBox;
	Matrix4f ModelMatrix;
	Material_HANDLE Material;
	char IsCulled;
}RenderedObject, * RenderedObject_PTR;

typedef struct RenderedObject_HANDLE_TYP
{
	size_t Handle;
}RenderedObject_HANDLE, * RenderedObject_HANDLE_PTR;

typedef struct RenderableObjectPipeline_TYP
{
	RenderedObject_HANDLE RenderedObject;

	size_t PolygonPipelineIndexBeginIncluded;
	size_t PolygonPipelineIndexEndExcluded;

	size_t VertexPipelineIndexBeginIncluded;
	size_t VertexPipelineIndexEndExcluded;
} RenderableObjectPipeline, * RenderableObjectPipeline_PTR;

typedef struct PolygonPipeline_CameraDistanceIndexed_TYP
{
	size_t Index;
	float DistanceFromCamera;
}PolygonPipeline_CameraDistanceIndexed, * PolygonPipeline_CameraDistanceIndexed_PTR;

typedef struct Polygon4fPTR_TYP
{
	union
	{
		struct
		{
			__global Vector4f* v1;
			__global Vector4f* v2;
			__global Vector4f* v3;
		};
		Vector4f_PTR Points[3];
	};
}Polygon4fPTR, * Polygon4fPTR_PTR;

void Vec_Mul_4f_1f(const Vector4f* p_left, float p_right,  Vector4f* p_out)
{
	p_out->x = p_left->x * p_right;
	p_out->y = p_left->y * p_right;
	p_out->z = p_left->z * p_right;
	p_out->w = p_left->w * p_right;
};

void Vec_Min_3f_3f(__global Vector3f* p_left, __global Vector3f* p_right, __global Vector3f* p_out)
{
	p_out->x = p_left->x - p_right->x;
	p_out->y = p_left->y - p_right->y;
	p_out->z = p_left->z - p_right->z;
}

void Vec_Min_4f_4f(__global Vector4f* p_left, __global Vector4f* p_right, Vector4f_PTR p_out)
{
	p_out->x = p_left->x - p_right->x;
	p_out->y = p_left->y - p_right->y;
	p_out->z = p_left->z - p_right->z;
	p_out->w = p_left->w - p_right->w;
};

float Vec_Length_3f(__global Vector3f* p_left)
{
	return sqrt((p_left->x * p_left->x) + (p_left->y * p_left->y) + (p_left->z * p_left->z));
};

void Vec_Normalize_3f(__global Vector3f* p_vec, __global Vector3f* p_out)
{
	float l_invertedLength = 1 / Vec_Length_3f(p_vec);
	p_out->x = p_vec->x * l_invertedLength;
	p_out->y = p_vec->y * l_invertedLength;
	p_out->z = p_vec->z * l_invertedLength;
};

void Vec_Cross_3f(const Vector3f_PTR p_left, const Vector3f_PTR p_right, __global Vector3f* p_out)
{
	p_out->x = (p_left->y * p_right->z) - (p_left->z * p_right->y);
	p_out->y = (p_left->z * p_right->x) - (p_left->x * p_right->z);
	p_out->z = (p_left->x * p_right->y) - (p_left->y * p_right->x);
};

float Vec_Dot_3f(const Vector3f_PTR p_left, __global Vector3f* p_right)
{
	return (p_left->Points[0] * p_right->Points[0]) + (p_left->Points[1] * p_right->Points[1]) + (p_left->Points[2] * p_right->Points[2]);
};

void Mat_Mul_M4F_V4F_ggg(__global const Matrix4f* p_left,__global const Vector4f* p_right, __global Vector4f* p_out)
{
	p_out->x = (p_left->_00 * p_right->x) + (p_left->_10 * p_right->y) + (p_left->_20 * p_right->z) + (p_left->_30 * p_right->w);
	p_out->y = (p_left->_01 * p_right->x) + (p_left->_11 * p_right->y) + (p_left->_21 * p_right->z) + (p_left->_31 * p_right->w);
	p_out->z = (p_left->_02 * p_right->x) + (p_left->_12 * p_right->y) + (p_left->_22 * p_right->z) + (p_left->_32 * p_right->w);
	p_out->w = (p_left->_03 * p_right->x) + (p_left->_13 * p_right->y) + (p_left->_23 * p_right->z) + (p_left->_33 * p_right->w);
};

void Mat_Mul_M4F_V4F_ggx(__global const Matrix4f* p_left,__global const Vector4f* p_right, Vector4f* p_out)
{
	p_out->x = (p_left->_00 * p_right->x) + (p_left->_10 * p_right->y) + (p_left->_20 * p_right->z) + (p_left->_30 * p_right->w);
	p_out->y = (p_left->_01 * p_right->x) + (p_left->_11 * p_right->y) + (p_left->_21 * p_right->z) + (p_left->_31 * p_right->w);
	p_out->z = (p_left->_02 * p_right->x) + (p_left->_12 * p_right->y) + (p_left->_22 * p_right->z) + (p_left->_32 * p_right->w);
	p_out->w = (p_left->_03 * p_right->x) + (p_left->_13 * p_right->y) + (p_left->_23 * p_right->z) + (p_left->_33 * p_right->w);
};

void Mat_Mul_M4F_V4F_Homogeneous(__global Matrix4f* p_projectionmatrix, __global Vector4f* p_pos, Vector4f* out_pos)
{
	Mat_Mul_M4F_V4F_ggx(p_projectionmatrix, p_pos, out_pos);
	Vec_Mul_4f_1f(out_pos, 1.0f / out_pos->w, out_pos);
};

void Polygon_CalculateNormal_V3FPTR(const Polygon4fPTR_PTR p_polygon, __global Vector3f* out_normal)
{
	Vec_Min_3f_3f(&p_polygon->v1->Vec3, &p_polygon->v2->Vec3, out_normal);
	Vector3f l_u = *out_normal;
	Vec_Min_3f_3f(&p_polygon->v1->Vec3, &p_polygon->v3->Vec3, out_normal);
	Vector3f l_v = *out_normal;
	Vec_Cross_3f(&l_u, &l_v, out_normal);
	Vec_Normalize_3f(out_normal, out_normal);
};

char BackFaceCulled_Normal3fPTR(__global Vector3f* p_worldNormal, __global Vector4f* p_polygonWorldPosition, __global Vector4f* p_cameraWorldPosition)
{
	Vector4f l_cameraToPolygon;
	Vec_Min_4f_4f(p_polygonWorldPosition, p_cameraWorldPosition, &l_cameraToPolygon);

	return Vec_Dot_3f(&l_cameraToPolygon.Vec3, p_worldNormal) > FLOAT_TOLERANCE;
}


inline void _i_VertexPipeline_CalculateWorldPosition(__global VertexPipeline* p_vertexPipeline, __global RenderedObject* p_renderableObject, __global Vertex* p_vertex)
{
	Mat_Mul_M4F_V4F_ggg(&p_renderableObject->ModelMatrix, &p_vertex->LocalPosition, &p_vertexPipeline->WorldPosition);
};

inline void WindowSize_GraphicsAPIToPixel(__global WindowSize* p_windowSize, float p_x, float p_y, __global int* out_x, __global int* out_y)
{
	//TODO -> Disable rounding ?
	*out_x = round(((p_x * p_windowSize->HalfWidth) + p_windowSize->HalfWidth));
	*out_y = round(((p_y * p_windowSize->HalfHeight) + p_windowSize->HalfHeight));
};

inline void _i_VertexPipeline_CalculatePixelPosition(__global VertexPipeline* p_vertexPipeline, __global CAMERABUFFER* p_cameraBuffer, __global WindowSize* p_windowSize)
{
		// World to Camera
		Mat_Mul_M4F_V4F_ggg(&p_cameraBuffer->ViewMatrix, &p_vertexPipeline->WorldPosition, &p_vertexPipeline->CameraSpacePosition);

		// Camera to clip
		Vector4f l_clipPosition;
		Mat_Mul_M4F_V4F_Homogeneous(&p_cameraBuffer->ProjectionMatrix, &p_vertexPipeline->CameraSpacePosition, &l_clipPosition);

		// To pixel
		WindowSize_GraphicsAPIToPixel(p_windowSize, l_clipPosition.x, l_clipPosition.y, &p_vertexPipeline->PixelPosition.x, &p_vertexPipeline->PixelPosition.y);
};

__kernel void Draw_Transform(__global VertexPipeline* p_vertexPipelines, __global RenderableObjectPipeline* p_renderableObjectsPipeline, 
			__global Vertex* p_vertices, __global RenderedObject* p_renderedObjects, __global CAMERABUFFER* p_cameraBuffer, __global WindowSize* p_windowSize)
{
	int i = get_global_id(0);

	__global VertexPipeline* l_vertexPipeline = &p_vertexPipelines[i];
	__global RenderableObjectPipeline* l_renderableObjectPipeline = &p_renderableObjectsPipeline[l_vertexPipeline->AssociatedRenderableObjectPipeline]; 
	__global RenderedObject* l_renderableObject = &p_renderedObjects[l_renderableObjectPipeline->RenderedObject.Handle];
	__global Vertex* l_vertex = &p_vertices[l_vertexPipeline->Vertex.Handle];

	_i_VertexPipeline_CalculateWorldPosition(l_vertexPipeline, l_renderableObject, l_vertex);
	_i_VertexPipeline_CalculatePixelPosition(l_vertexPipeline, p_cameraBuffer, p_windowSize);
	// Mat_Mul_M4F_V4F(&l_renderableObject->ModelMatrix, &l_vertex->LocalPosition, &l_vertexPipeline->WorldPosition);
}

__kernel void PolygonNormal_And_BackfaceCull(__global PolygonPipelineV2* p_polygonPipelines, __global VertexPipeline* p_vertexPipelines, __global CAMERABUFFER* p_cameraBuffer, 
		__global PolygonPipeline_CameraDistanceIndexed* p_polygonCameraDistanceIndexed)
{
	int i = get_global_id(0);
	
	__global PolygonPipelineV2* l_polygon = &p_polygonPipelines[i];

	__global VertexPipeline* l_v1 = &p_vertexPipelines[l_polygon->VerticesPipelineIndex.v1];
	__global VertexPipeline* l_v2 = &p_vertexPipelines[l_polygon->VerticesPipelineIndex.v2];
	__global VertexPipeline* l_v3 = &p_vertexPipelines[l_polygon->VerticesPipelineIndex.v3];


	Polygon4fPTR l_worldPositionPoly =
		{
			.v1 = &l_v1->WorldPosition,
			.v2 = &l_v2->WorldPosition,
			.v3 = &l_v3->WorldPosition
		};
	Polygon_CalculateNormal_V3FPTR(&l_worldPositionPoly, &l_polygon->CalculatedFlatWorldNormal);
	l_polygon->IsCulled = BackFaceCulled_Normal3fPTR(&l_polygon->CalculatedFlatWorldNormal, l_worldPositionPoly.v1, &p_cameraBuffer->WorldPosition);

	if(!l_polygon->IsCulled)
	{
		p_polygonCameraDistanceIndexed[i].Index = i;
		p_polygonCameraDistanceIndexed[i].DistanceFromCamera = (l_v1->CameraSpacePosition.z + l_v2->CameraSpacePosition.z + l_v3->CameraSpacePosition.z) * 0.333333f;
	}
}