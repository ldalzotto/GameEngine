#include "Camera.h"

#include "v2/Math.h"
#include "v2/Vector/Vector.hpp"
#include "v2/Vector/VectorMath.hpp"
extern "C"
{
#include "v2/_interface/MatrixC.h"
#include "v2/_interface/FrustumC.h"
}

#include "RenderV2Interface.hpp"
#include "Objects/SwapChain/SwapChain.hpp"
#include "Objects/Window/Window.hpp"
#include "ECS/Component.h"

namespace _GameEngine::_ECS
{
	ComponentType CameraType = "Camera";

	void camera_onSwapChainBuild(Camera* p_camera, _RenderV2::RenderV2Interface* p_renderInterface)
	{
		Camera_buildProjectionMatrix(p_camera);
	};

	void camera_onDetached(Camera* p_camera, ECS*)
	{
		_Core::ObserverT_unRegister(&p_camera->RenderInterface->SwapChain->OnSwapChainBuilded, (_Core::CallbackT<void, _RenderV2::RenderV2Interface>*) & p_camera->OnSwapChainBuilded);
	};

	void Camera_init(Camera* p_camera, _RenderV2::RenderV2Interface* p_renderInterface)
	{
		p_camera->RenderInterface = p_renderInterface;
		p_camera->OnSwapChainBuilded = { camera_onSwapChainBuild, p_camera };
		_Core::ObserverT_register(&p_camera->RenderInterface->SwapChain->OnSwapChainBuilded, (_Core::CallbackT<void, _RenderV2::RenderV2Interface>*) & p_camera->OnSwapChainBuilded);

		p_camera->ComponentHeader.OnComponentFree = camera_onDetached;

		Camera_buildProjectionMatrix(p_camera);

	};

	MATRIX4F Camera_worldToClipMatrix(Camera* p_camera)
	{
		MATRIX4F l_return;
		Mat_Mul_M4F_M4F(&p_camera->ProjectionMatrix, &p_camera->ViewMatrix, &l_return);
		return l_return;
	};

	void Camera_buildProjectionMatrix(Camera* p_camera)
	{
		Mat_Perspective_M4F(45.0f * _MathV2::DEG_TO_RAD,
			((float)p_camera->RenderInterface->SwapChain->PresentTexture.Width / (float)p_camera->RenderInterface->SwapChain->PresentTexture.Height), 0.1f, 50.0f, &p_camera->ProjectionMatrix);
		Frustum_ExtractFromProjection((MATRIX4F_PTR)&p_camera->ProjectionMatrix, &p_camera->CameraFrustum);
	};

	SEGMENT_VECTOR3F Camera_buildWorldSpaceRay(Camera* p_camera, VECTOR2F_PTR p_screenPoint)
	{
		VECTOR3F tmp_vec3_0, tmp_vec3_1; VECTOR4F tmp_vec4_0, tmp_vec4_1, tmp_vec4_2, tmp_vec4_3;

		tmp_vec3_0 = { p_screenPoint->x, p_screenPoint->y, 1.0f };
		Mat_Mul_M3F_V3F(&p_camera->RenderInterface->AppWindow->WindowToGraphicsAPIPixelCoordinates,
			&tmp_vec3_0,
			&tmp_vec3_1);
		VECTOR2F l_graphicsAPIPixelCoord = tmp_vec3_1.Vec2;

		MATRIX4F l_clipToWorldMatrix; Mat_Inv_M4F((MATRIX4F_PTR)&Camera_worldToClipMatrix(p_camera), &l_clipToWorldMatrix);

		tmp_vec4_0 = { l_graphicsAPIPixelCoord.x, l_graphicsAPIPixelCoord.y, -1.0f, 1.0f };
		tmp_vec4_1 = { l_graphicsAPIPixelCoord.x, l_graphicsAPIPixelCoord.y, 1.0f, 1.0f };

		SEGMENT_VECTOR3F l_return;
		Mat_Mul_M4F_V4F_Homogeneous(&l_clipToWorldMatrix, &tmp_vec4_0, &tmp_vec4_2);
		Mat_Mul_M4F_V4F_Homogeneous(&l_clipToWorldMatrix, &tmp_vec4_1, &tmp_vec4_3);
		l_return.Begin = tmp_vec4_2.Vec3;
		l_return.End = tmp_vec4_3.Vec3;

		return l_return;
	};
}