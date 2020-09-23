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
	PerformanceCounter AverageObjectCull;
	PerformanceCounter AveragePipelineMapping;
	PerformanceCounter AverageTransform;
	PerformanceCounter AverageRasterize;
}SolidRendererMetrics, * SolidRendererMetrics_PTR;

extern SolidRendererMetrics GWireframeRendererPerformace;

#endif