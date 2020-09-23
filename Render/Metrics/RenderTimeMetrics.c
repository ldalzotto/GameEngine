#if RENDER_PERFORMANCE_TIMER

#include "RenderTimeMetrics.h"
#include <stdio.h>

SolidRendererMetrics GWireframeRendererPerformace = { 0 };

TimeClockPrecision PerformanceCounter_GetAverage(PerformanceCounter_PTR p_performanceCounter)
{
	if (p_performanceCounter->SampleCount == 0) { return 0; }
	else { return p_performanceCounter->AccumulatedTime / p_performanceCounter->SampleCount; }
};

void PerformanceCounter_PushSample(PerformanceCounter_PTR p_performanceCounter, TimeClockPrecision p_sampleTime)
{
	p_performanceCounter->SampleCount += 1;
	p_performanceCounter->AccumulatedTime += p_sampleTime;
};

void PerformanceCounter_IncrementCounter(PerformanceCounter_PTR p_performanceCounter)
{
	p_performanceCounter->SampleCount += 1;
};

void PerformanceCounter_AddTime(PerformanceCounter_PTR p_performanceCounter, TimeClockPrecision p_sampleTime)
{
	p_performanceCounter->AccumulatedTime += p_sampleTime;
};

void SolidRendererMetrics_Print(SolidRendererMetrics_PTR p_wireframeRenderPerformance)
{
	printf("SolidRenderer_renderV2 : %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageRender));
	printf("  -> Object cull : %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageObjectCull));
	printf("  -> Pipeline mapping : %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AveragePipelineMapping));
	printf("  -> Transform : %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageTransform));
	printf("  -> Rasterize : %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageRasterize));
};

#endif