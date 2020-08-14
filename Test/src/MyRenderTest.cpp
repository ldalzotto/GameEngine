#include "RenderV2.hpp"

#include <cstdlib>
#include <stdlib.h> 

#include "AppEvent/AppEvent.hpp"
#include "DataStructures/Specifications/VectorT.hpp"

#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Quaternion/QuaternionMath.hpp"
#include "v2/Vector/VectorMath.hpp"
#include "v2/Box/Box.hpp"
#include "v2/Box/BoxMath.h"
#include "v2/Frustum/FrustumMath.hpp"

#include "Clock/Clock.hpp"

#include "Objects/Resource/Mesh.hpp"
#include "Objects/RenderedObject.hpp"

#include "v2/Clip/ClipMath.hpp"
#include "File/ObjReader.hpp"

using namespace _RenderV2;

RenderV2 renderV2;
_Core::TimeClockPrecision LastFrameTime = 0;

int main(int argc, char* argv[])
{
	_Core::AppEvent_initialize();

	LastFrameTime = _Core::Clock_currentTime_mics();

	RenderV2_initialize(&renderV2);

	_MathV2::Matrix<4, 4, float> l_modelMatrix = _MathV2::Matrix4x4f_Identity;
	// l_modelMatrix.Points[3][0] = 9.0f;

	MeshResourceKey l_meshResourceKey;
	_Core::String_alloc(&l_meshResourceKey.MeshPathAbsolute, 0); _Core::String_append(&l_meshResourceKey.MeshPathAbsolute, "E:/GameProjects/GameEngine/Assets/Models/ForwardArrow.obj");
	MeshResource* l_mesh = _Core::ResourceProviderT_useResource(&renderV2.Resources.MeshResourceProvider, &l_meshResourceKey, MeshResourceKey_getHashCode(&l_meshResourceKey));
	RenderedObject* l_renderableObject = new RenderedObject();
	_MathV2::Box l_meshBoundingBox{};
	{
		_MathV2::Box_build(&l_meshBoundingBox, (_Core::VectorT<_MathV2::Vector3<float>>*) & l_mesh->Mesh.Vertices);
		*l_renderableObject = { &l_mesh->Mesh , &l_meshBoundingBox, l_modelMatrix };
		_Core::VectorT_pushBack(&renderV2.GlobalBuffer.RenderedObjectsBuffer.RenderedObjects, &l_renderableObject);
	}

	_MathV2::Matrix<4, 4, float> l_viewMatrix = { 0.7071f, 0.4156f, 0.5720f, 0.00f, 0.00f, -0.8090f, 0.5877f, -0.00f, -0.7071f, 0.4156f, 0.5720f, 0.00f, 0.00f, -0.2001f, -15.5871f, 1.00f };
	// _MathV2::Matrix<4, 4, float> l_projectionMatrix = { 1.7275f, 0.00f, 0.00f, 0.00f, 0.00f, 2.4142f, 0.00f, 0.00f, 0.00f, 0.00f, (1.0f / 1.0040f), -1.0000f, 0.00f, 0.00f, -0.2004f, 0.00f };
	_MathV2::Matrix<4, 4, float> l_projectionMatrix = { 1.5088f, 0.00f, 0.00f, 0.00f, 0.00f, 2.4142f, 0.00f, 0.00f, 0.00f, 0.00f, (1.0f / 1.0040f), -1.0000f, 0.00f, 0.00f, -0.2004f, 0.00f };
	_MathV2::Vector<4, float> l_cameraWorldPosition = { 9.0f, 9.0f, 9.0f, 1.0f };
	_MathV2::Frustum l_cameraFrustum = _MathV2::FrustumM::extractFrustumFromProjection(&l_projectionMatrix);

	renderV2.GlobalBuffer.CameraBuffer.ViewMatrix = &l_viewMatrix;
	renderV2.GlobalBuffer.CameraBuffer.ProjectionMatrix = &l_projectionMatrix;
	renderV2.GlobalBuffer.CameraBuffer.WorldPosition = l_cameraWorldPosition;
	renderV2.GlobalBuffer.CameraBuffer.CameraFrustum = &l_cameraFrustum;

	

	while (!_RenderV2::Window_askedForClose(&renderV2.AppWindow))
	{
		float l_deltaTime = _Core::Clock_currentTime_mics() - LastFrameTime;
		LastFrameTime = _Core::Clock_currentTime_mics();

		_Core::AppEvent_pool();

		_MathV2::Quaternion<float> tmp_quat_0; _MathV2::Matrix3x3<float> tmp_mat3x3_0; _MathV2::Matrix4x4<float> tmp_mat4x4_0;
		_MathV2::Matrix4x4<float> l_rotation = _MathV2::Matrix4x4f_Identity;
		_MathV2::MatrixM::buildRotationMatrix(_MathV2::QuaternionM::extractAxis(_MathV2::QuaternionM::rotateAround(&_MathV2::UP, 0.000001f * l_deltaTime, &tmp_quat_0), &tmp_mat3x3_0), &l_rotation);
		_MathV2::MatrixM::mul(&l_modelMatrix, &l_rotation, &tmp_mat4x4_0);
		l_modelMatrix = tmp_mat4x4_0;

		RenderV2_render(&renderV2);
	}


	_Core::ResourceProviderT_releaseResource(&renderV2.Resources.MeshResourceProvider, MeshResourceKey_getHashCode(&l_mesh->Key));
	RenderV2_free(&renderV2);

	_Core::AppEvent_free();
}