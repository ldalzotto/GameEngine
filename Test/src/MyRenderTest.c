
#include <stdlib.h> 

#include "RenderV2.h"
#include "AppEvent/AppEvent.h"
#include "v2/Math.h"
#include "v2/_interface/VectorStructuresC.h"
#include "v2/_interface/VectorC.h"
#include "v2/_interface/QuaternionC.h"
#include "v2/_interface/BoxC.h"
#include "v2/_interface/FrustumC.h"
#include "v2/_interface/MatrixC.h"

#include "File/TextureReader.h"

#include "Heap/RenderHeap.h"
#include "DataStructures/String.h"
#include "Objects/Resource/Mesh.h"
#include "Objects/Texture/TextureResourceProvider.h"
#include "Objects/RenderedObject.h"
#include "Clock/Clock.h"

#include "Asset/AssetPaths.h"
#include "File/ObjReader.h"

RenderV2 renderV2;
TimeClockPrecision LastFrameTime = 0;

int main(int argc, char* argv[])
{
	AppEvent_initialize();

	LastFrameTime = Clock_currentTime_mics();

	AssetPath_Initialize(argv[0]);
	RenderV2_initialize(&renderV2);

	Matrix4f l_modelMatrix = Matrix4f_IDENTITYF;
	// l_modelMatrix.Col3.z = 0.0f;
	// l_modelMatrix.Col3.x = 0.0f;
	// l_modelMatrix.Col3.y = 0.0f;
	MeshResource_HANDLE l_mesh;
	Assetpath l_meshAssetPath;
	AssetPath_GetAbsolutePath("Models/BigCube.obj", &l_meshAssetPath);
	// AssetPath_GetAbsolutePath("Models/16.09.obj", &l_meshAssetPath);
	// AssetPath_GetAbsolutePath("Models/Icosphere2.obj", &l_meshAssetPath);
	// AssetPath_GetAbsolutePath("Models/Plane.obj", &l_meshAssetPath);
	// AssetPath_GetAbsolutePath("Models/SingleTriangle.obj", &l_meshAssetPath);
	MeshResourceProvider_UseResource(&renderV2.Resources.MeshResourceProvider, &l_meshAssetPath, &l_mesh);
	RenderedObject l_renderableObject;
	RenderedObject_PTR l_renderableObject_ptr = &l_renderableObject;
	BoxF l_meshBoundingBox = {0};
	{
		Mesh_BuildBoundingBox(&l_mesh->Mesh, &l_meshBoundingBox);
	}

	Assetpath l_textureAssterPath;
	AssetPath_GetAbsolutePath("Textures/texture.png", &l_textureAssterPath);
	// AssetPath_GetAbsolutePath("Textures/16.09_diffuse.png", &l_textureAssterPath);
	TextureResource_PTR l_texture;
	TextureResourceProvider_UseResource(&renderV2.Resources.TextureResourceProvider, &l_textureAssterPath, &l_texture);

	Material_HANDLE l_materialHandle;
	PoolAllocator_AllocElement_Material(&RRenderHeap.MaterialAllocator, &l_materialHandle);
	RRenderHeap.MaterialAllocator.array.Memory[l_materialHandle.Handle] = (Material){ 
		.Type = MATERIAL_TYPE_FlatShaded_Textured,
		.BaseColor = (Color3f){ 1.0f, 1.0f, 1.0f },
		.DiffuseTexture = l_texture->Texture
	};

	// Arr_PushBackRealloc_Mater

	l_renderableObject = (RenderedObject){ .Mesh = &l_mesh->Mesh , .MeshBoundingBox = &l_meshBoundingBox, .ModelMatrix = l_modelMatrix, .Material = l_materialHandle };
	RendereableObject_PushToRenderEngine(&renderV2.GlobalBuffer.RenderedObjectBuffers, l_renderableObject_ptr);

	Vector3f l_cameraPos = { 9.0f, 9.0f, 9.0f }; Vector3f l_forward = { -0.572061539f, -0.587785244f, -0.572061360f }; Vector3f l_up = { -0.415627033f, 0.809017003f, -0.415626884f };
	Matrix4f l_viewMatrix;
	Mat_ViewMatrix_M4F(&l_cameraPos, &l_forward, &l_up, &l_viewMatrix);

	Matrix4f l_projectionMatrix;
	float l_cameraFar = 50.0f;
	Mat_Perspective_M4F(45.0f * DEG_TO_RAD,
		((float)renderV2.SwapChain.PresentTexture.Width / (float)renderV2.SwapChain.PresentTexture.Height), 0.1f, l_cameraFar, &l_projectionMatrix);

	Vector4f l_cameraWorldPosition = { 9.0f, 9.0f, 9.0f, 1.0f };
	Frustum l_cameraFrustum; Frustum_ExtractFromProjection((Matrix4f_PTR)&l_projectionMatrix, &l_cameraFrustum);

	renderV2.GlobalBuffer.CameraBuffer.ViewMatrix = (Matrix4f_PTR)&l_viewMatrix;
	renderV2.GlobalBuffer.CameraBuffer.ProjectionMatrix = (Matrix4f_PTR)&l_projectionMatrix;
	renderV2.GlobalBuffer.CameraBuffer.WorldPosition = l_cameraWorldPosition;
	renderV2.GlobalBuffer.CameraBuffer.Far = &l_cameraFar;
	renderV2.GlobalBuffer.CameraBuffer.CameraFrustum = &l_cameraFrustum;



	 while (!Window_askedForClose(&renderV2.AppWindow))
	 {
	
	 // for (size_t i = 0; i < 3; i++)
	 // {
		float l_deltaTime = (float)(Clock_currentTime_mics() - LastFrameTime);
		LastFrameTime = Clock_currentTime_mics();

		AppEvent_pool();

		Quaternion4f tmp_quat_0; Matrix3f tmp_mat3x3_0; Matrix4f tmp_mat4x4_0;
		Matrix4f l_rotation = Matrix4f_IDENTITYF;

		Quat_RotateAround((const Vector3f_PTR)&Vector3f_UP, 0.000001f * l_deltaTime, &tmp_quat_0);
		Quat_ExtractAxis(&tmp_quat_0, &tmp_mat3x3_0);
		Mat_RotationAxis_M4F(&tmp_mat3x3_0, &l_rotation);
		Mat_Mul_M4F_M4F(&l_renderableObject.ModelMatrix, (Matrix4f_PTR)&l_rotation, &tmp_mat4x4_0);
		l_renderableObject.ModelMatrix = tmp_mat4x4_0;

		RenderV2_render(&renderV2);
	}

	RendereableObject_EraseFromRenderEngine(&renderV2.GlobalBuffer.RenderedObjectBuffers, l_renderableObject_ptr);


	MeshResourceProvider_ReleaseResource(&renderV2.Resources.MeshResourceProvider, &l_mesh->Key);
	TextureResourceProvider_ReleaseResource(&renderV2.Resources.TextureResourceProvider, &l_texture->Key);
	RenderV2_free(&renderV2);

	AppEvent_free();
}