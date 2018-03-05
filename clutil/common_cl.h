#ifndef COMMON_CL_H_
#define COMMON_CL_H_

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "clog/clog.h"

struct COMMON_CL_CONTEXT {
    cl_device_id device;
    cl_context context;
    cl_command_queue commands;

};

struct COMMON_CL_PROGRAM {
    struct COMMON_CL_CONTEXT *context;
    cl_program program;
    cl_kernel kernel;
};

int common_cl_pick_device(cl_platform_id *p_id, cl_device_id *id);

struct COMMON_CL_CONTEXT *common_cl_create_context(cl_device_id device);

struct COMMON_CL_PROGRAM *common_cl_compile_program(struct COMMON_CL_CONTEXT *context, char *path, char *kernel_name);

#endif
