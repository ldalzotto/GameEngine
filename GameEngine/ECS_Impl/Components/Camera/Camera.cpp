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
		Core_Observer_register(&l_camera->RenderInterface->SwapChain->OnSwapChainBuilded, &l_camera->OnSwapChainBuilded);
	};

	void Camera_init(Camera* p_camera, Component* p_associatedComponent, _Render::RenderInterface* p_renderInterface)
	{
		p_camera->RenderInterface = p_renderInterface;
		p_camera->OnSwapChainBuilded.Closure = p_camera;
		p_camera->OnSwapChainBuilded.Function = camera_onSwapChainBuild;
		Core_Observer_register(&p_camera->RenderInterface->SwapChain->OnSwapChainBuilded, &p_camera->OnSwapChainBuilded);

		p_camera->OnComponentDetached.Closure = p_camera;
		p_camera->OnComponentDetached.Function = camera_onDetached;
		Core_Observer_register(&p_associatedComponent->OnComponentFree, &p_camera->OnComponentDetached);

		Camera_buildProjectionMatrix(p_camera);
	};

	void Camera_buildProjectionMatrix(Camera* p_camera)
	{
		_Math::Matrixf4x4_perspective(45.0f * _Math::DEG_TO_RAD,
			p_camera->RenderInterface->SwapChain->SwapChainInfo.SwapExtend.width / (float)p_camera->RenderInterface->SwapChain->SwapChainInfo.SwapExtend.height, 0.1f, 200.0f, &p_camera->ProjectionMatrix);
	};

}