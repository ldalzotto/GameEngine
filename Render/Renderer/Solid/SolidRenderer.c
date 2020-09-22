#include "SolidRenderer.h"

#include "Heap/RenderHeap.h"
#include "Metrics/RenderTimeMetrics.h"
#include "Renderer/Draw/FlatShade.h"
#include "Renderer/Transformation/RenderableObjectTransform.h"
#include "Renderer/Transformation/RenderableObjectToPipeline.h"
#include "Renderer/Transformation/RenderableObjectCull.h"

// There is a unique directional light
RenderLights GRenderLights =
{
	.DirectionalLight = {.Direction = {1.0f, 0.0f, 0.0f}, .Intensity = 1.0f, .Color = {1.0f, 1.0f, 1.0f} },
	.AmbientLight = {.Color = {0.5f, 0.5f, 0.5f} }
};

void SolidRenderer_renderV2(const SolidRendererInput* p_input, Texture3f_PTR p_to, Recti_PTR p_to_clipRect, DepthBuffer_PTR p_depthBuffer, RendererPipeline_Memory_PTR p_memory)
{
#if RENDER_PERFORMANCE_TIMER
	TimeClockPrecision l_wireframeRenderBegin = Clock_currentTime_mics();
#endif


#if RENDER_PERFORMANCE_TIMER
	TimeClockPrecision tmp_timer, tmp_timer_2;
	tmp_timer = Clock_currentTime_mics();
#endif

	RenderableObject_CullObject(p_input->RenderableObjectsBuffer, p_input->CameraBuffer);

	RenderableObject_ToRenderPipeline(p_input->RenderableObjectsBuffer, p_memory, &RRenderHeap);

	RenderableObjectTransform_Input l_renderableObjectTransformInput = 
	{
		.RenderHeap = &RRenderHeap,
		.RendererPipelineMemory = p_memory,
		.CameraBuffer = p_input->CameraBuffer,
		.WindowSize = &p_input->WindowSize
	};
	RendereableObject_TransformPolygons(&l_renderableObjectTransformInput);


	DrawPolygFlatShadeTexturedInput l_drawInput = {
		.DepthBuffer = p_depthBuffer,
		.RendererPipelineMemory = p_memory,
		.RenderHeap = &RRenderHeap,
		.RenderLights = &GRenderLights,
		.RenderTarget = p_to,
		.TargetClip = p_to_clipRect
	};
	DrawPoly_FlatShade_Textured(&l_drawInput);


#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_IncrementCounter(&GWireframeRendererPerformace.AverageRasterization_TransformCoords);
#endif
#if RENDER_PERFORMANCE_TIMER && RENDER_PERFORMANCE_TIMER_PER_PIXEL
	PerformanceCounter_IncrementCounter(&GWireframeRendererPerformace.AverageRasterization_PolygonRasterize);
	PerformanceCounter_IncrementCounter(&GWireframeRendererPerformace.AverageRasterization_PixelShading);
#endif
#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageRasterization, Clock_currentTime_mics() - tmp_timer);
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageRender, Clock_currentTime_mics() - l_wireframeRenderBegin);
#endif
};
