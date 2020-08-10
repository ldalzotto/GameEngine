#include "RenderV2.hpp"

#include <cstdlib>

#include "AppEvent/AppEvent.hpp"
#include "DataStructures/Specifications/VectorT.hpp"

#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Quaternion/QuaternionMath.hpp"
#include "v2/Vector/VectorMath.hpp"
#include "v2/Box/Box.hpp"

#include "Clock/Clock.hpp"

#include "Renderer/Wireframe/WireframeRenderer.hpp"
#include "Objects/Mesh.hpp"
#include "Objects/Texture/TextureM.hpp"

using namespace _RenderV2;

RenderV2 renderV2;
_Core::TimeClockPrecision LastFrameTime = 0;

int main()
{
	_Core::AppEvent_initialize();

	LastFrameTime = _Core::Clock_currentTime_mics();

	RenderV2_initialize(&renderV2);

	Mesh l_cubeMesh;
	_Core::ArrayT_alloc(&l_cubeMesh.Vertices, 8);
	_Core::ArrayT_alloc(&l_cubeMesh.Polygons, 12);

	_Core::ArrayT_pushBack(&l_cubeMesh.Vertices, { {-0.5f, 0.5f, -0.5f} });
	_Core::ArrayT_pushBack(&l_cubeMesh.Vertices, { {-0.5f, -0.5f, -0.5f} });
	_Core::ArrayT_pushBack(&l_cubeMesh.Vertices, { {-0.5f, -0.5f, 0.5f} });
	_Core::ArrayT_pushBack(&l_cubeMesh.Vertices, { {-0.5f, 0.5f, 0.5f} });
	_Core::ArrayT_pushBack(&l_cubeMesh.Vertices, { {0.5f, 0.5f, 0.5f} });
	_Core::ArrayT_pushBack(&l_cubeMesh.Vertices, { {0.5f, -0.5f, 0.5f} });
	_Core::ArrayT_pushBack(&l_cubeMesh.Vertices, { {0.5f, -0.5f, -0.5f} });
	_Core::ArrayT_pushBack(&l_cubeMesh.Vertices, { {0.5f, 0.5f, -0.5f} });


	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 1), _Core::ArrayT_at(&l_cubeMesh.Vertices, 3), _Core::ArrayT_at(&l_cubeMesh.Vertices, 0) });
	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 3), _Core::ArrayT_at(&l_cubeMesh.Vertices, 5), _Core::ArrayT_at(&l_cubeMesh.Vertices, 4) });

	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 2), _Core::ArrayT_at(&l_cubeMesh.Vertices, 6), _Core::ArrayT_at(&l_cubeMesh.Vertices, 5) });
	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 7), _Core::ArrayT_at(&l_cubeMesh.Vertices, 5), _Core::ArrayT_at(&l_cubeMesh.Vertices, 6) });

	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 0), _Core::ArrayT_at(&l_cubeMesh.Vertices, 4), _Core::ArrayT_at(&l_cubeMesh.Vertices, 7) });
	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 1), _Core::ArrayT_at(&l_cubeMesh.Vertices, 7), _Core::ArrayT_at(&l_cubeMesh.Vertices, 6) });

	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 1), _Core::ArrayT_at(&l_cubeMesh.Vertices, 2), _Core::ArrayT_at(&l_cubeMesh.Vertices, 3) });
	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 3), _Core::ArrayT_at(&l_cubeMesh.Vertices, 2), _Core::ArrayT_at(&l_cubeMesh.Vertices, 5) });

	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 2), _Core::ArrayT_at(&l_cubeMesh.Vertices, 1), _Core::ArrayT_at(&l_cubeMesh.Vertices, 6) });
	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 7), _Core::ArrayT_at(&l_cubeMesh.Vertices, 4), _Core::ArrayT_at(&l_cubeMesh.Vertices, 5) });

	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 0), _Core::ArrayT_at(&l_cubeMesh.Vertices, 3), _Core::ArrayT_at(&l_cubeMesh.Vertices, 4) });
	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 1), _Core::ArrayT_at(&l_cubeMesh.Vertices, 0), _Core::ArrayT_at(&l_cubeMesh.Vertices, 7) });

	_MathV2::Box l_meshBoundingBox = { {0.0f, 0.0f, 0.0f},{1.0f, 1.0f, 1.0f} };

	_MathV2::Matrix<4, 4, float> l_modelMatrix = _MathV2::Matrix4x4f_Identity;
	// l_modelMatrix.Points[3][0] = 3000.0f; l_modelMatrix.Points[3][1] = 3000.0f; l_modelMatrix.Points[3][2] = 3000.0f;
	_Core::VectorT<RenderableObject> l_renderableObjects;
	_Core::VectorT_alloc(&l_renderableObjects, 0);
	
	_Core::VectorT_pushBack(&l_renderableObjects, RenderableObject{ &l_cubeMesh , &l_meshBoundingBox, &l_modelMatrix });

	// In the view matrix, the forward vector is no more inverted
	_MathV2::Matrix<4, 4, float> l_viewMatrix = { -0.7071f, 0.4156f, -0.5720f, 0.00f, 0.00f, -0.8090f, -0.5877f, -0.00f, 0.7071f, 0.4156f, -0.5720f, 0.00f, 0.00f, -0.2001f, 15.5871f, 1.00f };
	// In the projection matrix, the z value is no more negated.
	_MathV2::Matrix<4, 4, float> l_projectionMatrix = { 1.7275f, 0.00f, 0.00f, 0.00f, 0.00f, 2.4142f, 0.00f, 0.00f, 0.00f, 0.00f, (1.0f / 1.0040f), 1.0000f, 0.00f, 0.00f, -0.2004f, 0.00f };
	_MathV2::Vector<4, float> l_cameraWorldPosition = { 9.0f, 9.0f, 9.0f, 1.0f };
	Texture<3, char> l_renderTexture{};
	l_renderTexture.Width = 800; l_renderTexture.Height = 600;
	TextureM::allocPixels(&l_renderTexture);

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


		_MathV2::Vector3<char> l_color = { 0, 0, 0 };
		TextureM::fill(&l_renderTexture, &l_color);

		WireframeRendererInput l_input{};
		l_input.RenderableObjects = &l_renderableObjects;
		l_input.ProjectionMatrix = &l_projectionMatrix;
		l_input.ViewMatrix = &l_viewMatrix;
		l_input.GraphicsAPIToScreeMatrix = &renderV2.AppWindow.GraphicsAPIToWindowPixelCoordinates;
		l_input.CameraWorldPosition = &l_cameraWorldPosition;
		WirerameRenderer_render(&l_input, &l_renderTexture);

		Window_presentTexture(&renderV2.AppWindow, &l_renderTexture);
	}

	TextureM::freePixels(&l_renderTexture);
	RenderV2_free(&renderV2);

	_Core::AppEvent_free();
}