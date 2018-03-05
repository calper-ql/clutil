//
// Created by Can Alper on 1/2/18.
//

#include "common_cl.h"
#include <stdio.h>

struct COMMON_CL_CONTEXT *common_cl_create_context(cl_device_id device) {
    clog_raw_log("", "CREATING CONTEXT", CLOG_ANSI_YELLOW);
    printf("\n");
    clog_add_space();
    clog_add_space();

    char buffer[2048];
    size_t length;
    cl_int err;

    err = clGetDeviceInfo(device, CL_DEVICE_VERSION, 2048, buffer, &length);
    if (err != CL_SUCCESS) {
        clog_remove_space();
        clog_remove_space();
        clog_failure("device version test");
        return NULL;
    }
    unsigned major;
    unsigned minor;
    if (sscanf(buffer, "OpenCL %d.%d", &major, &minor) != 2) {
        clog_failure("version parser");
    }
    clog_raw_log("DEVICE VERSION: ", "OPENCL", CLOG_ANSI_CYAN);
    printf("%d.%d\n", major, minor);

    struct COMMON_CL_CONTEXT *context = (struct COMMON_CL_CONTEXT *) malloc(sizeof(struct COMMON_CL_CONTEXT));
    context->device = device;

    context->context = clCreateContext(0, 1, &device, NULL, NULL, &err);
    if (err != CL_SUCCESS) {
        clog_failure("create program");
        clog_remove_space();
        clog_remove_space();
        free(context);
        return NULL;
    }
    clog_success("create program");

#ifdef CL_VERSION_2_0
    if(major >= 2){
        cl_queue_properties clcqp = 0;
        context->commands = clCreateCommandQueueWithProperties(context->context, context->device, &clcqp, &err);
    } else {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        cl_command_queue_properties clcqp = 0;
        context->commands = clCreateCommandQueue(context->context, context->device, clcqp, &err);
#pragma GCC diagnostic push
    }
#else
    cl_command_queue_properties clcqp = 0;
    context->commands = clCreateCommandQueue(context->context, context->device, clcqp, &err);
#endif

    if (err != CL_SUCCESS) {
        clog_failure("create command queue");
        clog_remove_space();
        clog_remove_space();
        free(context);
        return NULL;
    }

    clog_success("create command queue");
    clog_remove_space();
    clog_remove_space();
    return context;
}
