
#define RENDER_V2 1

#include "RenderV2.hpp"

#include <cstdlib>

#include "AppEvent/AppEvent.hpp"
#include "DataStructures/Specifications/VectorT.hpp"

#include "v2/Matrix/MatrixMath.hpp"

#include "Renderer/Wireframe/WireframeRenderer.hpp"
#include "Objects/Mesh.hpp"
#include "Objects/Texture/TextureM.hpp"

using namespace _RenderV2;

RenderV2 renderV2;

int main()
{
	_Core::AppEvent_initialize();

	RenderV2_initialize(&renderV2);

	Mesh l_cubeMesh;
	_Core::ArrayT_alloc(&l_cubeMesh.Vertices, 8);
	_Core::ArrayT_alloc(&l_cubeMesh.Polygons, 12);

	_Core::ArrayT_pushBack(&l_cubeMesh.Vertices, { {0.0f, 0.0f, 0.0f} });
	_Core::ArrayT_pushBack(&l_cubeMesh.Vertices, { {1.0f, 0.0f, 0.0f} });
	_Core::ArrayT_pushBack(&l_cubeMesh.Vertices, { {1.0f, 1.0f, 0.0f} });
	_Core::ArrayT_pushBack(&l_cubeMesh.Vertices, { {0.0f, 1.0f, 0.0f} });
	_Core::ArrayT_pushBack(&l_cubeMesh.Vertices, { {0.0f, 1.0f, 1.0f} });
	_Core::ArrayT_pushBack(&l_cubeMesh.Vertices, { {1.0f, 1.0f, 1.0f} });
	_Core::ArrayT_pushBack(&l_cubeMesh.Vertices, { {1.0f, 0.0f, 1.0f} });
	_Core::ArrayT_pushBack(&l_cubeMesh.Vertices, { {0.0f, 0.0f, 1.0f} });


	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 0), _Core::ArrayT_at(&l_cubeMesh.Vertices, 2), _Core::ArrayT_at(&l_cubeMesh.Vertices, 1) });
	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 0), _Core::ArrayT_at(&l_cubeMesh.Vertices, 3), _Core::ArrayT_at(&l_cubeMesh.Vertices, 2) });

	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 2), _Core::ArrayT_at(&l_cubeMesh.Vertices, 3), _Core::ArrayT_at(&l_cubeMesh.Vertices, 4) });
	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 2), _Core::ArrayT_at(&l_cubeMesh.Vertices, 4), _Core::ArrayT_at(&l_cubeMesh.Vertices, 5) });

	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 1), _Core::ArrayT_at(&l_cubeMesh.Vertices, 2), _Core::ArrayT_at(&l_cubeMesh.Vertices, 5) });
	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 1), _Core::ArrayT_at(&l_cubeMesh.Vertices, 5), _Core::ArrayT_at(&l_cubeMesh.Vertices, 6) });

	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 0), _Core::ArrayT_at(&l_cubeMesh.Vertices, 7), _Core::ArrayT_at(&l_cubeMesh.Vertices, 4) });
	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 0), _Core::ArrayT_at(&l_cubeMesh.Vertices, 4), _Core::ArrayT_at(&l_cubeMesh.Vertices, 3) });

	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 5), _Core::ArrayT_at(&l_cubeMesh.Vertices, 4), _Core::ArrayT_at(&l_cubeMesh.Vertices, 7) });
	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 5), _Core::ArrayT_at(&l_cubeMesh.Vertices, 7), _Core::ArrayT_at(&l_cubeMesh.Vertices, 6) });

	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 0), _Core::ArrayT_at(&l_cubeMesh.Vertices, 6), _Core::ArrayT_at(&l_cubeMesh.Vertices, 7) });
	_Core::ArrayT_pushBack(&l_cubeMesh.Polygons, { _Core::ArrayT_at(&l_cubeMesh.Vertices, 0), _Core::ArrayT_at(&l_cubeMesh.Vertices, 1), _Core::ArrayT_at(&l_cubeMesh.Vertices, 6) });

	_MathV2::Matrix<4, 4, float> l_modelMatrix = _MathV2::Matrix4x4f_Identity;
	_Core::VectorT<PolygonInput> l_polygonInput;
	_Core::VectorT_alloc(&l_polygonInput, 0);
	for (size_t i = 0; i < l_cubeMesh.Polygons.Size; i++)
	{
		_Core::VectorT_pushBack(&l_polygonInput, { _Core::ArrayT_at(&l_cubeMesh.Polygons, i), &l_modelMatrix });
	}

	_MathV2::Matrix<4, 4, float> l_viewMatrix = { 0.7071f, 0.4156f, 0.5720f, 0.00f, 0.00f, -0.8090f, 0.5877f, -0.00f, -0.7071f, 0.4156f, 0.5720f, 0.00f, 0.00f, -0.2001f, -15.5871f, 1.00f };
	_MathV2::Matrix<4, 4, float> l_projectionMatrix = { 1.7275f, 0.00f, 0.00f, 0.00f, 0.00f, 2.4142f, 0.00f, 0.00f, 0.00f, 0.00f, -1.0040f, -1.0000f, 0.00f, 0.00f, -0.2004f, 0.00f };

	Texture<3, char> l_renderTexture{};
	l_renderTexture.Width = 800; l_renderTexture.Height = 600;
	TextureM::allocPixels(&l_renderTexture);

	while (!_RenderV2::Window_askedForClose(&renderV2.AppWindow))
	{
		_Core::AppEvent_pool();

		_MathV2::Vector3<char> l_color = { 255, 0, 255 };
		TextureM::fill(&l_renderTexture, &l_color);

		WireframeRendererInput l_input{};
		l_input.Polygons = &l_polygonInput;
		l_input.ProjectionMatrix = &l_projectionMatrix; //TODO -> copy from other test
		l_input.ViewMatrix = &l_viewMatrix; //TODO -> copy from other tests
		l_input.GraphicsAPIToScreeMatrix = &renderV2.AppWindow.GraphicsAPIToWindowPixelCoordinates;
		WirerameRenderer_render(&l_input, &l_renderTexture);

		Window_presentTexture(&renderV2.AppWindow, &l_renderTexture);
	}

	TextureM::freePixels(&l_renderTexture);
	RenderV2_free(&renderV2);

	_Core::AppEvent_free();
}