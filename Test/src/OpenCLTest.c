#include <CL/cl.h>

#include <stdio.h>
#include "Read/File/File.h"

int main()
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
	cl_device_id device;

	//devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, NULL);

	/*Step 3: Create context.*/
	cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);

	/*Step 4: Creating command queue associate with the context.*/
	cl_command_queue commandQueue = clCreateCommandQueue(context, device, 0, NULL);
	
	String l_clFile;
	File_readFile_string("E:/GameProjects/GameEngine/Test/src/OpenCLTest.cl", &l_clFile);

	cl_program program = clCreateProgramWithSource(context, 1, &l_clFile.array.Memory, &l_clFile.Size, &status);

	/*Step 6: Build program. */
	status = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

	if (status == CL_BUILD_PROGRAM_FAILURE)
	{
		// Determine the size of the log
		size_t log_size;
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

		// Allocate memory for the log
		char* log = (char*)malloc(log_size);

		// Get the log
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

		// Print the log
		printf("%s\n", log);
	}


	status = clReleaseContext(context);
}