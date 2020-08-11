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

#include "Clock/Clock.hpp"

#include "Renderer/Wireframe/WireframeRenderer.hpp"
#include "Objects/Mesh.hpp"

#include "File/ObjReader.hpp"

using namespace _RenderV2;

RenderV2 renderV2;
_Core::TimeClockPrecision LastFrameTime = 0;

int main()
{


	_Core::AppEvent_initialize();

	LastFrameTime = _Core::Clock_currentTime_mics();

	RenderV2_initialize(&renderV2);


	_MathV2::Matrix<4, 4, float> l_modelMatrix = _MathV2::Matrix4x4f_Identity;
	l_modelMatrix.Points[3][0] = 0.0f;
	_Core::VectorT<RenderableObject> l_renderableObjects;
	_Core::VectorT_alloc(&l_renderableObjects, 0);

	_Core::VectorT<Mesh> l_loadedMeshes{};
	_Core::VectorT_alloc(&l_loadedMeshes, 0);
	{
		ObjReader_loadMeshes("C:/Users/loicd/Desktop/CubeTest.obj", &_Core::VectorT_buildInsertor(&l_loadedMeshes));

		_Core::VectorIteratorT<Mesh> l_meshIt = _Core::VectorT_buildIterator(&l_loadedMeshes);
		while (_Core::VectorIteratorT_moveNext(&l_meshIt))
		{
			_MathV2::Box l_meshBoundingBox;
			_MathV2::Box_build(&l_meshBoundingBox, (_Core::VectorT<_MathV2::Vector3<float>>*) & l_meshIt.Current->Vertices);

			RenderableObject l_renderableObject = { l_meshIt.Current , &l_meshBoundingBox, &l_modelMatrix };
			_Core::VectorT_pushBack(&l_renderableObjects, &l_renderableObject);
		}
	}

	_MathV2::Matrix<4, 4, float> l_viewMatrix = { 0.7071f, 0.4156f, 0.5720f, 0.00f, 0.00f, -0.8090f, 0.5877f, -0.00f, -0.7071f, 0.4156f, 0.5720f, 0.00f, 0.00f, -0.2001f, -15.5871f, 1.00f };
	_MathV2::Matrix<4, 4, float> l_projectionMatrix = { 1.7275f, 0.00f, 0.00f, 0.00f, 0.00f, 2.4142f, 0.00f, 0.00f, 0.00f, 0.00f, (1.0f / 1.0040f), -1.0000f, 0.00f, 0.00f, -0.2004f, 0.00f };

	_MathV2::Vector<4, float> l_cameraWorldPosition = { 9.0f, 9.0f, 9.0f, 1.0f };

	while (!_RenderV2::Window_askedForClose(&renderV2.AppWindow))
	{
		float l_deltaTime = _Core::Clock_currentTime_mics() - LastFrameTime;
		LastFrameTime = _Core::Clock_currentTime_mics();

		_Core::AppEvent_pool();

		_MathV2::Quaternion<float> tmp_quat_0; _MathV2::Matrix3x3<float> tmp_mat3x3_0; _MathV2::Matrix4x4<float> tmp_mat4x4_0;
		_MathV2::Matrix4x4<float> l_rotation = _MathV2::Matrix4x4f_Identity;
		_MathV2::MatrixM::buildRotationMatrix(_MathV2::QuaternionM::extractAxis(_MathV2::QuaternionM::rotateAround(&_MathV2::UP, 0.05f * l_deltaTime * 0.0001f, &tmp_quat_0), &tmp_mat3x3_0), &l_rotation);
		_MathV2::MatrixM::mul(&l_modelMatrix, &l_rotation, &tmp_mat4x4_0);
		l_modelMatrix = tmp_mat4x4_0;

		WireframeRendererInput l_input{};
		l_input.RendererConfiguration.ObjectCullEnabled = true;
		l_input.RenderableObjects = &l_renderableObjects;
		l_input.ProjectionMatrix = &l_projectionMatrix;
		l_input.ViewMatrix = &l_viewMatrix;
		l_input.GraphicsAPIToScreeMatrix = &renderV2.AppWindow.GraphicsAPIToWindowPixelCoordinates;
		l_input.CameraWorldPosition = &l_cameraWorldPosition;

		RenderV2_render(&renderV2, &l_input);
	}


	_Core::VectorT_free(&l_loadedMeshes);
	RenderV2_free(&renderV2);

	_Core::AppEvent_free();
}