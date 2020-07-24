#include "Camera.h"

#include "Math/Math.h"
#include "Math/Matrix/MatrixMath.h"
#include "Math/Vector/Vector.h"
#include "Math/Vector/VectorMath.h"
#include "Math/Segment/Segment.h"

#include "Render/RenderInterface.h"
#include "Render/VulkanObjects/SwapChain/SwapChain.h"
#include "Render/VulkanObjects/Hardware/Window/Window.h"

#include "ECS/Component.h"

namespace _GameEngine::_ECS
{
	ComponentType CameraType = "Camera";

	void camera_onSwapChainBuild(Camera* p_camera, _Render::RenderInterface* p_renderInterface)
	{
		Camera_buildProjectionMatrix(p_camera);
	};

	void camera_onDetached(Camera* p_camera)
	{
		_Core::ObserverT_unRegister(&p_camera->RenderInterface->SwapChain->OnSwapChainBuilded, (_Core::CallbackT<void, _Render::RenderInterface>*) & p_camera->OnSwapChainBuilded);
	};

	void Camera_init(Camera* p_camera, _Render::RenderInterface* p_renderInterface)
	{
		p_camera->RenderInterface = p_renderInterface;
		p_camera->OnSwapChainBuilded = { camera_onSwapChainBuild, p_camera };
		_Core::ObserverT_register(&p_camera->RenderInterface->SwapChain->OnSwapChainBuilded, (_Core::CallbackT<void, _Render::RenderInterface>*) & p_camera->OnSwapChainBuilded);

		p_camera->ComponentHeader.OnComponentFree = camera_onDetached;

		Camera_buildProjectionMatrix(p_camera);
	};

	void Camera_worldToClipMatrix(Camera* p_camera, _Math::Matrix4x4f* out_worldToClip)
	{
		_Math::Matrixf4x4_mul(&p_camera->ProjectionMatrix, &p_camera->ViewMatrix, out_worldToClip);
	};

	void Camera_buildProjectionMatrix(Camera* p_camera)
	{
		_Math::Matrixf4x4_perspective(45.0f * _Math::DEG_TO_RAD,
			p_camera->RenderInterface->SwapChain->SwapChainInfo.SwapExtend.width / (float)p_camera->RenderInterface->SwapChain->SwapChainInfo.SwapExtend.height, 0.1f, 200.0f, &p_camera->ProjectionMatrix);
	};

	void Camera_buildWorldSpaceRay(Camera* p_camera, _Math::Vector2f* p_screenPoint, _Math::Segment* out_ray)
	{
		_Math::Vector2f l_graphicsAPIPixelCoord;

		{
			_Math::Vector3f l_screenPoint3f = { p_screenPoint->x, p_screenPoint->y, 1.0f };
			_Math::Vector3f l_graphicsAPIPixelCoord3f;
			_Math::Matrix3x3f_mul(&p_camera->RenderInterface->Window->WindowToGraphicsAPIPixelCoordinates, &l_screenPoint3f, &l_graphicsAPIPixelCoord3f);
			l_graphicsAPIPixelCoord = { l_graphicsAPIPixelCoord3f.x, l_graphicsAPIPixelCoord3f.y };
		}

		_Math::Matrix4x4f l_clipToWorldMatrix;
		{
			_Math::Matrix4x4f l_wordlToClipMatrix;
			Camera_worldToClipMatrix(p_camera, &l_wordlToClipMatrix);
			_Math::Matrixf4x4_inv(&l_wordlToClipMatrix, &l_clipToWorldMatrix);
		}

		// Near plane
		_Math::Vector3f l_beginClipSpace = { l_graphicsAPIPixelCoord.x, l_graphicsAPIPixelCoord.y, -1.0f };
		_Math::Matrix4x4f_clipToWorld(&l_clipToWorldMatrix, &l_beginClipSpace, &out_ray->Begin);

		// Far plane
		_Math::Vector3f l_endClipSpace = { l_graphicsAPIPixelCoord.x, l_graphicsAPIPixelCoord.y, 1.0f };
		_Math::Matrix4x4f_clipToWorld(&l_clipToWorldMatrix, &l_endClipSpace, &out_ray->End);
	};
}