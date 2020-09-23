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
	.DirectionalLight = {.Direction = {0.707f, 0.707f, 0.0f}, .Intensity = 1.0f, .Color = {1.0f, 1.0f, 1.0f} },
	.AmbientLight = {.Color = {0.3f, 0.3f, 0.6f} }
};

void DrawObjects_NoShade_NotTextured(const SolidRendererInput* p_input, Texture3f_PTR p_to, Recti_PTR p_to_clipRect, DepthBuffer_PTR p_depthBuffer, RendererPipeline_Memory_PTR p_memory)
{

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

	DrawPoly_NoShade_NotTextured(&l_drawInput);

}

void DrawObjects_FlatShade_Textured(const SolidRendererInput* p_input, Texture3f_PTR p_to, Recti_PTR p_to_clipRect, DepthBuffer_PTR p_depthBuffer, RendererPipeline_Memory_PTR p_memory)
{
#if RENDER_PERFORMANCE_TIMER
	TimeClockPrecision l_wireframeRenderBegin = Clock_currentTime_mics();
#endif


#if RENDER_PERFORMANCE_TIMER
	TimeClockPrecision tmp_timer;
	tmp_timer = Clock_currentTime_mics();
#endif

	RenderableObject_CullObject(p_input->RenderableObjectsBuffer, p_input->CameraBuffer);

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageObjectCull, Clock_currentTime_mics() - tmp_timer);
	tmp_timer = Clock_currentTime_mics();
#endif

	RenderableObject_ToRenderPipeline(p_input->RenderableObjectsBuffer, p_memory, &RRenderHeap);

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AveragePipelineMapping, Clock_currentTime_mics() - tmp_timer);
	tmp_timer = Clock_currentTime_mics();
#endif

	RenderableObjectTransform_Input l_renderableObjectTransformInput = 
	{
		.RenderHeap = &RRenderHeap,
		.RendererPipelineMemory = p_memory,
		.CameraBuffer = p_input->CameraBuffer,
		.WindowSize = &p_input->WindowSize
	};
	RendereableObject_TransformPolygons(&l_renderableObjectTransformInput);

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageTransform, Clock_currentTime_mics() - tmp_timer);
	tmp_timer = Clock_currentTime_mics();
#endif

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
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageRasterize, Clock_currentTime_mics() - tmp_timer);
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageRender, Clock_currentTime_mics() - l_wireframeRenderBegin);
#endif

};

void DrawObjects_FlatShade_NotTextured(const SolidRendererInput* p_input, Texture3f_PTR p_to, Recti_PTR p_to_clipRect, DepthBuffer_PTR p_depthBuffer, RendererPipeline_Memory_PTR p_memory)
{
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
	DrawPoly_FlatShade_NotTextured(&l_drawInput);

};
