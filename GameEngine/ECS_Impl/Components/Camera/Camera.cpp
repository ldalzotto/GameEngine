#include "Camera.h"

#include "VulkanObjects/SwapChain/SwapChain.h"

#include "ECS/Component.h"
#include "Render/RenderInterface.h"
#include "Math/Math.h"
#include "Math/Matrix/MatrixMath.h"
#include "Math/Vector/Vector.h"

namespace _GameEngine::_ECS
{
	ComponentType CameraType = "Camera";

	void camera_onSwapChainBuild(void* p_camera, void* p_swapChain)
	{
		Camera* l_camera = (Camera*)p_camera;
		Camera_buildProjectionMatrix(l_camera);
	};

	void camera_onDetached(void* p_camera, void* p_notUsed)
	{
		Camera* l_camera = (Camera*)p_camera;
		_Utils::Observer_unRegister(&l_camera->RenderInterface->SwapChain->OnSwapChainBuilded, &l_camera->OnSwapChainBuilded);
	};

	void Camera_init(Camera* p_camera, Component* p_associatedComponent, _Render::RenderInterface* p_renderInterface)
	{
		p_camera->RenderInterface = p_renderInterface;
		p_camera->OnSwapChainBuilded.Closure = p_camera;
		p_camera->OnSwapChainBuilded.Callback = camera_onSwapChainBuild;
		_Utils::Observer_register(&p_camera->RenderInterface->SwapChain->OnSwapChainBuilded, &p_camera->OnSwapChainBuilded);

		p_camera->OnComponentDetached.Closure = p_camera;
		p_camera->OnComponentDetached.Callback = camera_onDetached;
		_Utils::Observer_register(&p_associatedComponent->ComponentFreeEvent, &p_camera->OnComponentDetached);

		Camera_buildProjectionMatrix(p_camera);
	};

	void Camera_buildProjectionMatrix(Camera* p_camera)
	{
		_Math::Matrixf4x4_perspective(45.0f * _Math::DEG_TO_RAD,
			p_camera->RenderInterface->SwapChain->SwapChainInfo.SwapExtend.width / (float)p_camera->RenderInterface->SwapChain->SwapChainInfo.SwapExtend.height, 0.1f, 10.0f, &p_camera->ProjectionMatrix);
	};

	void Camera_convertWindowPixelPositionToNormalizedScreenRenderCoordinate(Camera* p_camera, _Math::Vector2f* p_screenSpaceCoordinates, _Math::Vector2f* out_renderNormalized)
	{
		out_renderNormalized->x = p_screenSpaceCoordinates->x  / (float)p_camera->RenderInterface->SwapChain->SwapChainInfo.SwapExtend.width;
		out_renderNormalized->y = p_screenSpaceCoordinates->y / (float)p_camera->RenderInterface->SwapChain->SwapChainInfo.SwapExtend.height;

		// center is 0,0
		out_renderNormalized->x -= 0.5f;
		out_renderNormalized->y -= 0.5f;

		// Corners are 1,1
		out_renderNormalized->x *= 2.0f;
		out_renderNormalized->y *= 2.0f;
	};

}