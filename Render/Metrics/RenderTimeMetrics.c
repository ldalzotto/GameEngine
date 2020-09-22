#if RENDER_PERFORMANCE_TIMER

#include "RenderTimeMetrics.h"
#include <stdio.h>

SolidRendererMetrics GWireframeRendererPerformace = { 0 };

TimeClockPrecision PerformanceCounter_GetAverage(PerformanceCounter_PTR p_performanceCounter)
{
	return p_performanceCounter->AccumulatedTime / p_performanceCounter->SampleCount;
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
	// printf("  -> Data Setup %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageDataSetup));
	// printf("     -> Pushing pipeline data %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageDataSetup_PushPipelineData));
	// printf("  -> Local To World %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageLocalToWorld));
	// printf("  -> Backface Culling %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageBackfaceCulling));
	// printf("  -> Pre Rasterization Lighting %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AveragePreRasterizationLightCalculation));
	// printf("  -> Polygon Sorting %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AveragePolygonSorting));
	// printf("  -> Rasterization %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageRasterization));
	// printf("     -> TransformCoords %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageRasterization_TransformCoords));
#if RENDER_PERFORMANCE_TIMER_PER_PIXEL
	printf("     -> PolygonRasterize %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageRasterization_PolygonRasterize));
	printf("     -> PixelShading %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageRasterization_PixelShading));
#endif
};

#endif