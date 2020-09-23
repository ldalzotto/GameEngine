#pragma once

#if HAS_OPENCL

typedef char *OpenCLDevice;
typedef char *OpenCLCContext;
typedef char *OpenCLCommandQueue;
typedef char* OpenCLKernel;

typedef struct OpenCLContext_TYP
{
	OpenCLDevice Device;
	OpenCLCContext Context;
	OpenCLCommandQueue CommandQueue;
}OpenCLContext, * OpenCLContext_PTR;

#endif