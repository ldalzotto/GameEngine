#include "Math/Vector/Vector.h"
#include "Math/Matrix/MatrixMath.h"

#include "Render/VulkanObjects/SwapChain/SwapChain.h"
#include "Render/RenderInterface.h"

#include "Math/Quaternion/Quaternion.h"
#include "Math/Quaternion/QuaternionMath.h"
#include "ECS_Impl/Components/Camera/Camera.h"

#include "Math/Math.h"
#include "glm/matrix.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/quaternion.hpp"

// using namespace _GameEngine::_Test;
using namespace _GameEngine;

int main()
{
	_Render::SwapChain l_swapChain{};
	_Core::ObserverT_alloc(&l_swapChain.OnSwapChainBuilded);
	l_swapChain.SwapChainInfo.SwapExtend.width = 800;
	l_swapChain.SwapChainInfo.SwapExtend.height = 600;

	_Render::RenderInterface l_renderInterface;
	l_renderInterface.SwapChain = &l_swapChain;

	{

		_ECS::Camera l_camera{};
		_ECS::Camera_init(&l_camera, &l_renderInterface);

		{
			_Math::Vector3f l_position = { 0.0f, 0.0f, 0.0f };
			_Math::Vector3f l_scale = { 1.0f, 1.0f, 1.0f };
			_Math::Matrif4x4_buildTRSV2(&l_position, &_Math::RIGHT, &_Math::UP, &_Math::FORWARD, &l_scale, &l_camera.ViewMatrix);
		}

		_Math::Matrix4x4f l_worldToClipMatrix, l_clipToWorldMatrix;
		_ECS::Camera_worldToClipMatrix(&l_camera, &l_worldToClipMatrix);
		_Math::Matrixf4x4_inv(&l_worldToClipMatrix, &l_clipToWorldMatrix);

		{
			_Math::Vector3f l_clipPos = { 0.75f, -0.5f, 1.0f };
			_Math::Vector3f l_comparedClip;
			_Math::Vector3f l_worldPos;
			_Math::Matrix4x4f_clipToWorld(&l_clipToWorldMatrix, &l_clipPos, &l_worldPos);
			_Math::Matrix4x4f_worldToClip(&l_worldToClipMatrix, &l_worldPos, &l_comparedClip);
		}

		float l_z_m100;
		float l_z_m050;
		float l_z_000;
		float l_z_050;
		float l_z_100;


		{
			_Math::Vector3f l_clipPos = { 1.0f, 1.0f, -1.0f };
			_Math::Vector3f l_worldPos;
			_Math::Matrix4x4f_clipToWorld(&l_clipToWorldMatrix, &l_clipPos, &l_worldPos);
			l_z_m100 = l_worldPos.z;
		}

		{
			_Math::Vector3f l_clipPos = { 1.0f, 1.0f, -0.5f };
			_Math::Vector3f l_worldPos;
			_Math::Matrix4x4f_clipToWorld(&l_clipToWorldMatrix, &l_clipPos, &l_worldPos);
			l_z_m050 = l_worldPos.z;
		}

		{
			_Math::Vector3f l_clipPos = { 1.0f, 1.0f, 0.0f };
			_Math::Vector3f l_worldPos;
			_Math::Matrix4x4f_clipToWorld(&l_clipToWorldMatrix, &l_clipPos, &l_worldPos);
			l_z_000 = l_worldPos.z;
		}

		{
			_Math::Vector3f l_clipPos = { 1.0f, 1.0f, 0.5f };
			_Math::Vector3f l_worldPos;
			_Math::Matrix4x4f_clipToWorld(&l_clipToWorldMatrix, &l_clipPos, &l_worldPos);
			l_z_050 = l_worldPos.z;
		}

		{
			_Math::Vector3f l_clipPos = { 1.0f, 1.0f, 1.0f };
			_Math::Vector3f l_worldPos;
			_Math::Matrix4x4f_clipToWorld(&l_clipToWorldMatrix, &l_clipPos, &l_worldPos);
			l_z_100 = l_worldPos.z;
		}

	}


	{
		_ECS::Camera l_camera{};
		_ECS::Camera_init(&l_camera, &l_renderInterface);

		{
			_Math::Vector3f l_position = { -2.0f, -2.0f, -2.0f };
			_Math::Quaternionf l_rotation;
			_Math::Quaternion_fromEulerAngles(_Math::Vector3f{ 15.0f, 9954.0f, 14.0f }, &l_rotation);
			_Math::Vector3f l_scale = { 1.0f, 1.0f, 1.0f };
			_Math::Matrif4x4_buildTRS(&l_position, &l_rotation, &l_scale, &l_camera.ViewMatrix);
			// _Math::Matrif4x4_buildTRSV2(&l_position, &_Math::RIGHT, &_Math::UP, &_Math::FORWARD, &l_scale, &l_camera.ViewMatrix);
		}

		_Math::Matrix4x4f l_worldToClipMatrix, l_clipToWorldMatrix;
		_ECS::Camera_worldToClipMatrix(&l_camera, &l_worldToClipMatrix);
		_Math::Matrixf4x4_inv(&l_worldToClipMatrix, &l_clipToWorldMatrix);

		{
			_Math::Vector3f l_clipPos = { 0.75f, -0.5f, 1.0f };
			_Math::Vector3f l_comparedClip;
			_Math::Vector3f l_worldPos;
			_Math::Matrix4x4f_clipToWorld(&l_clipToWorldMatrix, &l_clipPos, &l_worldPos);
			_Math::Matrix4x4f_worldToClip(&l_worldToClipMatrix, &l_worldPos, &l_comparedClip);
		}

		//Direction
		{
			_Math::Vector3f l_zero;
			_Math::Vector3f l_forwardWorld = _Math::FORWARD;
			_Math::Vector3f l_forawrd_clipSpace_begin, l_forawrd_clipSpace_end;
			_Math::Matrix4x4f_worldToClip(&l_clipToWorldMatrix, &l_zero, &l_forawrd_clipSpace_begin);
			_Math::Matrix4x4f_worldToClip(&l_clipToWorldMatrix, &l_forwardWorld, &l_forawrd_clipSpace_end);

		}
	}
}