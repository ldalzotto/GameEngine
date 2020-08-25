
#include <stdlib.h> 

#include "RenderV2.h"
#include "AppEvent/AppEvent.h"
#include "v2/Math.h"
#include "v2/_interface/VectorC.h"
#include "v2/_interface/QuaternionC.h"
#include "v2/_interface/BoxC.h"
#include "v2/_interface/FrustumC.h"
#include "v2/_interface/MatrixC.h"

#include "DataStructures/String.h"
#include "Objects/RenderedObject.h"
#include "Clock/Clock.h"

#include "Renderer/Gizmo/Gizmo.h"

#include "File/ObjReader.h"

RenderV2 renderV2;
TimeClockPrecision LastFrameTime = 0;

int main(int argc, char* argv[])
{
	AppEvent_initialize();

	LastFrameTime = Clock_currentTime_mics();

	RenderV2_initialize(&renderV2);

	Matrix4f l_viewMatrix = { 0.7071f, 0.4156f, 0.5720f, 0.00f, 0.00f, -0.8090f, 0.5877f, -0.00f, -0.7071f, 0.4156f, 0.5720f, 0.00f, 0.00f, -0.2001f, -15.5871f, 1.00f };

	{
		Matrix4f tmp_mat4_0;
		Vector3f l_source = { 9.0f, 9.0f, 9.0f }; Vector3f l_target = (Vector3f){ 0.0f, 0.0f,0.0f };
		Vector3f l_one = { 1.0f, 1.0f, 1.0f };
		Matrix3f l_axis;
		Vector3f l_down = { 0.0f, -1.0f, 0.0f };
		Mat_LookAtRotation_F(&l_source, &l_target, &l_down, &l_axis);
		
		Mat_TRS_Axis_M4F(&l_source, &l_axis, &l_one, &tmp_mat4_0);



		Mat_Inv_M4F(&tmp_mat4_0, &l_viewMatrix);
	}

	Matrix4f l_projectionMatrix;
	Mat_Perspective_M4F(45.0f * DEG_TO_RAD,
		((float)renderV2.SwapChain.PresentTexture.Width / (float)renderV2.SwapChain.PresentTexture.Height), 0.1f, 50.0f, &l_projectionMatrix);

	Vector4f l_cameraWorldPosition = { 9.0f, 9.0f, 9.0f, 1.0f };
	Frustum l_cameraFrustum; Frustum_ExtractFromProjection((Matrix4f_PTR)&l_projectionMatrix, &l_cameraFrustum);

	renderV2.GlobalBuffer.CameraBuffer.ViewMatrix = (Matrix4f_PTR)&l_viewMatrix;
	renderV2.GlobalBuffer.CameraBuffer.ProjectionMatrix = (Matrix4f_PTR)&l_projectionMatrix;
	renderV2.GlobalBuffer.CameraBuffer.WorldPosition = l_cameraWorldPosition;
	renderV2.GlobalBuffer.CameraBuffer.CameraFrustum = &l_cameraFrustum;



	while (!Window_askedForClose(&renderV2.AppWindow))
	{
		float l_deltaTime = (float)(Clock_currentTime_mics() - LastFrameTime);
		LastFrameTime = Clock_currentTime_mics();

		AppEvent_pool();

		
		Vector3f l_begin = { 0.0f, 0.0f, 0.0f };
		Vector3f l_end = { 0.0f, 10.0f, 0.0f };
		Gizmo_DrawLine(&renderV2.GizmoBuffer, &l_begin, &l_end);

		RenderV2_render(&renderV2);
	}

	// MeshResourceProvider_ReleaseResource(&renderV2.Resources.MeshResourceProvider, &l_mesh->Key);
	RenderV2_free(&renderV2);

	AppEvent_free();
}