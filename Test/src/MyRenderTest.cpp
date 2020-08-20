#include "RenderV2.hpp"

#include <cstdlib>
#include <stdlib.h> 

#include "DataStructures/Specifications/VectorT.hpp"

extern "C"
{
#include "AppEvent/AppEvent.h"
#include "v2/Math.h"
#include "v2/_interface/VectorC.h"
#include "v2/_interface/QuaternionC.h"
#include "v2/_interface/BoxC.h"
#include "v2/_interface/FrustumC.h"
#include "v2/_interface/MatrixC.h"

#include "DataStructures/STRING.h"
}

#include "Clock/Clock.hpp"

#include "Objects/RenderedObject.hpp"

#include "File/ObjReader.h"

using namespace _RenderV2;

RenderV2 renderV2;
_Core::TimeClockPrecision LastFrameTime = 0;

int main(int argc, char* argv[])
{
	STRING l_str; String_Alloc(&l_str, 0);
	String_AppendRaw(&l_str, "Ta mere");
	String_AppendRaw(&l_str, " en slip.");

	const char* l_steez = "bonjour";
	STRINGSLICE l_slice = { (char*)l_steez, 2, 5};
	String_AppendSlice(&l_str, &l_slice);

	/*
	MESHRESOURCE_PROVIDER l_prov;
	MeshResourceProvider_Alloc(&l_prov);
	MESHRESOURCE_KEY l_k;
	MESHRESOURCE_PTR l_ins;
	MeshResourceProvider_UseResource(&l_prov, &l_k, &l_ins);
	MeshResourceProvider_Free(&l_prov);
	*/


	AppEvent_initialize();

	LastFrameTime = _Core::Clock_currentTime_mics();

	RenderV2_initialize(&renderV2);

	MATRIX4F l_modelMatrix = MATRIX4F_IDENTITYF;
	// l_modelMatrix.Col3.x = 12.0f;

	MESHRESOURCE_HANDLE l_mesh;
	MeshResourceProvider_UseResource(&renderV2.Resources.MeshResourceProvider, "C:/Users/loicd/Desktop/BigCube.obj", &l_mesh);
	RenderedObject l_renderableObject;
	RenderedObject* l_renderableObject_ptr = &l_renderableObject;
	BOXF l_meshBoundingBox{};
	{
		ARRAY_VECTOR3F l_vertices = { .Memory = (VECTOR3F_PTR)l_mesh->Mesh.Vertices.Memory, .Size = l_mesh->Mesh.Vertices.Size, .Capacity = l_mesh->Mesh.Vertices.Capacity };
		Box_Build_F(&l_meshBoundingBox, &l_vertices);
		l_renderableObject = { &l_mesh->Mesh , &l_meshBoundingBox, l_modelMatrix };
		_Core::VectorT_pushBack(&renderV2.GlobalBuffer.RenderedObjectsBuffer.RenderedObjects, &l_renderableObject_ptr);
	}

	MATRIX4F l_viewMatrix = { 0.7071f, 0.4156f, 0.5720f, 0.00f, 0.00f, -0.8090f, 0.5877f, -0.00f, -0.7071f, 0.4156f, 0.5720f, 0.00f, 0.00f, -0.2001f, -15.5871f, 1.00f };
	MATRIX4F l_projectionMatrix;
	Mat_Perspective_M4F(45.0f * DEG_TO_RAD,
		((float)renderV2.SwapChain.PresentTexture.Width / (float)renderV2.SwapChain.PresentTexture.Height), 0.1f, 50.0f, &l_projectionMatrix);

	VECTOR4F l_cameraWorldPosition = { 9.0f, 9.0f, 9.0f, 1.0f };
	FRUSTUM l_cameraFrustum; Frustum_ExtractFromProjection((MATRIX4F_PTR)&l_projectionMatrix, &l_cameraFrustum);

	renderV2.GlobalBuffer.CameraBuffer.ViewMatrix = (MATRIX4F_PTR)&l_viewMatrix;
	renderV2.GlobalBuffer.CameraBuffer.ProjectionMatrix = (MATRIX4F_PTR)&l_projectionMatrix;
	renderV2.GlobalBuffer.CameraBuffer.WorldPosition = l_cameraWorldPosition;
	renderV2.GlobalBuffer.CameraBuffer.CameraFrustum = &l_cameraFrustum;



	while (!_RenderV2::Window_askedForClose(&renderV2.AppWindow))
	{
		float l_deltaTime = _Core::Clock_currentTime_mics() - LastFrameTime;
		LastFrameTime = _Core::Clock_currentTime_mics();

		AppEvent_pool();

		QUATERNION4F tmp_quat_0; MATRIX3F tmp_mat3x3_0; MATRIX4F tmp_mat4x4_0;
		MATRIX4F l_rotation = MATRIX4F_IDENTITYF;

		Quat_RotateAround((const VECTOR3F_PTR)&VECTOR3F_UP, 0.000001f * l_deltaTime, &tmp_quat_0);
		Quat_ExtractAxis(&tmp_quat_0, &tmp_mat3x3_0);
		Mat_RotationAxis_M4F(&tmp_mat3x3_0, &l_rotation);
		Mat_Mul_M4F_M4F(&l_renderableObject.ModelMatrix, (MATRIX4F_PTR)&l_rotation, &tmp_mat4x4_0);
		l_renderableObject.ModelMatrix = tmp_mat4x4_0;
		
		RenderV2_render(&renderV2);
	}

	MeshResourceProvider_ReleaseResource(&renderV2.Resources.MeshResourceProvider, &l_mesh->Key);
	RenderV2_free(&renderV2);

	AppEvent_free();
}