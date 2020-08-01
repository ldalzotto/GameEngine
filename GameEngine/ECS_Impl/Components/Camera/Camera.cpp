#include "Camera.h"

#include "Math/Math.h"
#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Vector/Vector.hpp"
#include "v2/Vector/VectorMath.hpp"
#include "Math/Segment/Segment.hpp"

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

	void camera_onDetached(Camera* p_camera, ECS*)
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

	_MathV2::Matrix4x4<float> Camera_worldToClipMatrix(Camera* p_camera)
	{
		return _MathV2::MatrixM::mul(p_camera->ProjectionMatrix, p_camera->ViewMatrix);
	};

	void Camera_buildProjectionMatrix(Camera* p_camera)
	{
		p_camera->ProjectionMatrix = _MathV2::MatrixM::perspective<float>(45.0f * _Math::DEG_TO_RAD,
			((float)p_camera->RenderInterface->SwapChain->SwapChainInfo.SwapExtend.width / (float)p_camera->RenderInterface->SwapChain->SwapChainInfo.SwapExtend.height), 0.1f, 50.0f);
	};

	_Math::Segment& Camera_buildWorldSpaceRay(Camera* p_camera, _MathV2::Vector<2, float>& p_screenPoint)
	{
		_MathV2::Vector2<float> l_graphicsAPIPixelCoord = _MathV2::VectorM::cast(
			_MathV2::MatrixM::mul(
				*(_MathV2::Matrix3x3<float>*) & p_camera->RenderInterface->Window->WindowToGraphicsAPIPixelCoordinates,
				_MathV2::Vector3<float> {p_screenPoint.x, p_screenPoint.y, 1.0f})
		);

		_MathV2::Matrix4x4<float> l_clipToWorldMatrix = _MathV2::MatrixM::inv(Camera_worldToClipMatrix(p_camera));

		return
			_Math::Segment{
			/*Near plane*/ _MathV2::VectorM::cast(_MathV2::MatrixM::clipSpaceMul(l_clipToWorldMatrix, _MathV2::Vector4<float>{l_graphicsAPIPixelCoord.x, l_graphicsAPIPixelCoord.y, -1.0f, 1.0f})),
			/*Far plane*/ _MathV2::VectorM::cast(_MathV2::MatrixM::clipSpaceMul(l_clipToWorldMatrix, _MathV2::Vector4<float>{l_graphicsAPIPixelCoord.x, l_graphicsAPIPixelCoord.y, 1.0f, 1.0f}))
		};
	};
}