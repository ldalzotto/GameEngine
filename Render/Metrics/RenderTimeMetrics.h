#pragma once

#if RENDER_PERFORMANCE_TIMER

#include "Clock/Clock.h"

typedef struct PerformanceCounter_TYP
{
	size_t SampleCount;
	TimeClockPrecision AccumulatedTime;
}PerformanceCounter, * PerformanceCounter_PTR;

void PerformanceCounter_PushSample(PerformanceCounter_PTR p_performanceCounter, TimeClockPrecision p_sampleTime)
{
	p_performanceCounter->SampleCount += 1;
	p_performanceCounter->AccumulatedTime += p_sampleTime;
};

typedef struct SolidRendererMetrics_TYP
{
	PerformanceCounter AverageRender;
	PerformanceCounter AverageDataSetup;
	PerformanceCounter AverageDataSetup_PushPipelineData;
	PerformanceCounter AverageLocalToWorld;
	PerformanceCounter AverageBackfaceCulling;
	PerformanceCounter AverageRasterization;
	PerformanceCounter AverageRasterization_PixelDrawing;
}SolidRendererMetrics, * SolidRendererMetrics_PTR;

SolidRendererMetrics GWireframeRendererPerformace = { 0 };

TimeClockPrecision PerformanceCounter_GetAverage(PerformanceCounter_PTR p_performanceCounter)
{
	return p_performanceCounter->AccumulatedTime / p_performanceCounter->SampleCount;
};

void SolidRendererMetrics_Print(SolidRendererMetrics_PTR p_wireframeRenderPerformance)
{
	printf("SolidRenderer_renderV2 : %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageRender));
	printf("  -> Data Setup %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageDataSetup));
	printf("     -> Pushing pipeline data %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageDataSetup_PushPipelineData));
	printf("  -> Local To World %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageLocalToWorld));
	printf("  -> Backface Culling %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageBackfaceCulling));
	printf("  -> Rasterization %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageRasterization));
	printf("     -> PixelDraw %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageRasterization_PixelDrawing));
};

#endif