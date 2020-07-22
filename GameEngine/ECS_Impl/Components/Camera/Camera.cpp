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
		_Core::ObserverT_register(&p_camera->RenderInterface->SwapChain->OnSwapChainBuilded, (_Core::CallbackT<void, _Render::RenderInterface>*) &p_camera->OnSwapChainBuilded);

		p_camera->ComponentHeader.OnComponentFree = camera_onDetached;

		Camera_buildProjectionMatrix(p_camera);
	};

	void Camera_buildProjectionMatrix(Camera* p_camera)
	{
		_Math::Matrixf4x4_perspective(45.0f * _Math::DEG_TO_RAD,
			p_camera->RenderInterface->SwapChain->SwapChainInfo.SwapExtend.width / (float)p_camera->RenderInterface->SwapChain->SwapChainInfo.SwapExtend.height, 0.1f, 200.0f, &p_camera->ProjectionMatrix);
	};

}