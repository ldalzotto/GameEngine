#pragma once

#if RENDER_PERFORMANCE_TIMER

#include "Clock/Clock.h"

typedef struct PerformanceCounter_TYP
{
	size_t SampleCount;
	TimeClockPrecision AccumulatedTime;
}PerformanceCounter, * PerformanceCounter_PTR;

typedef struct SolidRendererMetrics_TYP
{
	PerformanceCounter AverageRender;
	PerformanceCounter AverageDataSetup;
	PerformanceCounter AverageDataSetup_PushPipelineData;
	PerformanceCounter AverageLocalToWorld;
	PerformanceCounter AverageBackfaceCulling;
	PerformanceCounter AveragePreRasterizationLightCalculation;
	PerformanceCounter AveragePolygonSorting;
	PerformanceCounter AverageRasterization;
	PerformanceCounter AverageRasterization_TransformCoords;
	PerformanceCounter AverageRasterization_PolygonRasterize;
	PerformanceCounter AverageRasterization_PixelShading;
}SolidRendererMetrics, * SolidRendererMetrics_PTR;

extern SolidRendererMetrics GWireframeRendererPerformace;

#endif