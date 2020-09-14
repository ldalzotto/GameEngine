#include "Camera.h"

#include "v2/Math.h"
#include "v2/_interface/MatrixC.h"
#include "v2/_interface/FrustumC.h"
#include "v2/_interface/WindowSize.h"
#include "Objects/Window/Window.h"
#include "Objects/SwapChain/SwapChain.h"
#include "RenderV2Interface.h"
#include "Types_def.h"

void camera_onSwapChainBuild(Camera* p_camera, void* p_null)
{
	Camera_buildProjectionMatrix(p_camera);
};

void Camera_init(Camera_PTR p_camera, RenderV2Interface* p_renderInterface)
{
	p_camera->RenderInterface = p_renderInterface;
	p_camera->Header.ComponentType = CAMERA_TYPE;
	SwapChain_RegisterOnSwapChainBuilded(p_camera->RenderInterface->SwapChain, (void(*)(void*, void*)) camera_onSwapChainBuild, p_camera);

	Camera_buildProjectionMatrix(p_camera);
};

void Camera_OnComponentDetached(Camera_PTR p_camera)
{
	SwapChain_UnRegisterOnSwapChainBuilded(p_camera->RenderInterface->SwapChain, (void(*)(void*, void*)) camera_onSwapChainBuild, p_camera);
};

void Camera_buildProjectionMatrix(Camera_PTR p_camera)
{
	Mat_Perspective_M4F(45.0f * DEG_TO_RAD,
		((float)p_camera->RenderInterface->SwapChain->PresentTexture.Width / (float)p_camera->RenderInterface->SwapChain->PresentTexture.Height), 0.1f, 50.0f, &p_camera->ProjectionMatrix);
	Frustum_ExtractFromProjection((Matrix4f_PTR)&p_camera->ProjectionMatrix, &p_camera->CameraFrustum);

};

Matrix4f Camera_worldToClipMatrix(Camera_PTR p_camera)
{
	Matrix4f l_return;
	Mat_Mul_M4F_M4F(&p_camera->ProjectionMatrix, &p_camera->ViewMatrix, &l_return);
	return l_return;
};

Segment_Vector3f Camera_buildWorldSpaceRay(Camera_PTR p_camera, Vector2f_PTR p_screenPoint)
{
	Vector4f tmp_vec4_0, tmp_vec4_1, tmp_vec4_2, tmp_vec4_3;


	Vector2f l_graphicsAPIPixelCoord;
	WindowSize_PixelToGraphicsAPI(&p_camera->RenderInterface->AppWindow->WindowSize, (int)p_screenPoint->x, (int)p_screenPoint->y, &l_graphicsAPIPixelCoord.x, &l_graphicsAPIPixelCoord.y);


	Matrix4f l_worldToClipMatrix = Camera_worldToClipMatrix(p_camera);
	Matrix4f l_clipToWorldMatrix; Mat_Inv_M4F(&l_worldToClipMatrix, &l_clipToWorldMatrix);

	tmp_vec4_0 = (Vector4f){ l_graphicsAPIPixelCoord.x, l_graphicsAPIPixelCoord.y, -1.0f, 1.0f };
	tmp_vec4_1 = (Vector4f){ l_graphicsAPIPixelCoord.x, l_graphicsAPIPixelCoord.y, 1.0f, 1.0f };

	Segment_Vector3f l_return;
	Mat_Mul_M4F_V4F_Homogeneous(&l_clipToWorldMatrix, &tmp_vec4_0, &tmp_vec4_2);
	Mat_Mul_M4F_V4F_Homogeneous(&l_clipToWorldMatrix, &tmp_vec4_1, &tmp_vec4_3);
	l_return.Begin = tmp_vec4_2.Vec3;
	l_return.End = tmp_vec4_3.Vec3;

	return l_return;
};