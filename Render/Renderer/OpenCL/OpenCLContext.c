#include "OpenCLContext.h"
#include <stdlib.h>
#include <stdio.h>
#include "Read/File/File.h"
#include "DataStructures/STRING.h"
#include <CL/cl.h>

#if HAS_OPENCL

void OpenCLContext_LoadProgram(OpenCLContext_PTR p_openclContext)
{
	cl_int status;
	String l_programString;
	File_readFile_string("E:/GameProjects/GameEngine/Render/Renderer/OpenCL/OpenCLProgram.cl", &l_programString);
	cl_program program = clCreateProgramWithSource(p_openclContext->Context, 1, &l_programString.Memory, &l_programString.Size, &status);
	
	if (status == CL_BUILD_PROGRAM_FAILURE)
	{
		// Determine the size of the log
		size_t log_size;
		clGetProgramBuildInfo(program, p_openclContext->Device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

		// Allocate memory for the log
		char* log = (char*)malloc(log_size);

		// Get the log
		clGetProgramBuildInfo(program, p_openclContext->Device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

		// Print the log
		printf("%s\n", log);
	}

	char* build_options = "";
	status = clBuildProgram(program, 1, &p_openclContext->Device, build_options, NULL, NULL);

	if (status != CL_SUCCESS)
	{
		// Determine the size of the log
		size_t log_size;
		clGetProgramBuildInfo(program, p_openclContext->Device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

		// Allocate memory for the log
		char* log = (char*)malloc(log_size);

		// Get the log
		clGetProgramBuildInfo(program, p_openclContext->Device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

		// Print the log
		printf("%s\n", log);
	}

	/*Step 8: Create kernel object */
	TransformKernel = clCreateKernel(program, "Draw_Transform", &status);
	PolygonNormalAndBackfaceCullKernel = clCreateKernel(program, "PolygonNormal_And_BackfaceCull", &status);
	
	String_Free(&l_programString);

};


void OpenCLContext_Initialize(OpenCLContext_PTR p_openclContext)
{
	cl_uint numPlatforms; //the NO. of platforms
	cl_platform_id platform = NULL; //the chosen platform
	cl_int status = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (status != CL_SUCCESS)
	{
		printf("Error: Getting platforms!");
		return 0;
	}


	/*For clarity, choose the first available platform. */
	if (numPlatforms > 0)
	{
		cl_platform_id* platforms =
			(cl_platform_id*)malloc(numPlatforms * sizeof(cl_platform_id));
		status = clGetPlatformIDs(numPlatforms, platforms, NULL);
		platform = platforms[0];
		free(platforms);
	}

	/*Step 2:Query the platform and choose the first GPU device if has one.Otherwise use the CPU as device.*/

	//devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &p_openclContext->Device, NULL);

	/*Step 3: Create context.*/
	p_openclContext->Context = clCreateContext(NULL, 1, &p_openclContext->Device, NULL, NULL, NULL);

	/*Step 4: Creating command queue associate with the context.*/
	p_openclContext->CommandQueue = clCreateCommandQueue(p_openclContext->Context, p_openclContext->Device, 0, NULL);

	OpenCLContext_LoadProgram(p_openclContext);
};
#endif