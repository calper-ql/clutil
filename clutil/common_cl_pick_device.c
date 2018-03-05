#include "common_cl.h"
#include <stdio.h>

int common_cl_pick_device(cl_platform_id *p_id, cl_device_id *id) {
    if (id == NULL || p_id == NULL) {
        clog_failure("NULL POINTER IS PASSED INTO PICK DEVICE...");
        return 9;
    }
    clog_wait("PLEASE SELECT DEVICE:", "SELECTOR");

    size_t length;
    char buffer[4028];
    char count_buffer[1024];

    cl_int err;

    cl_platform_id platforms[255];
    cl_device_id devices[255];
    cl_uint platform_count;
    cl_uint device_count;

    unsigned int selected_platform;
    unsigned int selected_device;

    clGetPlatformIDs(255, platforms, &platform_count);
    if (platform_count == 0) {
        clog_failure("NO OPENCL PLATFORMS...");
        return 1;
    }

    for (cl_uint i = 0; i < platform_count; i++) {
        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 4028, buffer, &length);
        if (err == CL_SUCCESS) {
            snprintf(count_buffer, 1024, "%d", i);
            clog_raw_log(buffer, count_buffer, CLOG_ANSI_RED);
            printf("\n");
            clog_add_space();
            clog_add_space();
            err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 255, devices, &device_count);
            if (err == CL_SUCCESS || device_count == 0) {
                for (cl_uint k = 0; k < device_count; k++) {
                    err = clGetDeviceInfo(devices[k], CL_DEVICE_NAME, 4028, buffer, &length);
                    snprintf(count_buffer, 1024, "%d", k);
                    if (err != CL_SUCCESS) {
                        clog_failure("Couldn't get device name...");
                    } else {
                        clog_raw_log(buffer, count_buffer, CLOG_ANSI_GREEN);
                    }
                    printf("\n");
                }
            } else {
                clog_failure("No devices found for this platform...");
            }
            clog_remove_space();
            clog_remove_space();
        }
    }

    if (platform_count == 1) {
        selected_platform = 0;
    } else {
        printf("%sSELECT PLATFORM:%s ", CLOG_ANSI_RED, CLOG_ANSI_YELLOW);
        int err = scanf("%d", &selected_platform);
        if (selected_platform >= platform_count || err == 0) {
            clog_failure("No such platform...");
            return 2;
        }
    }

    err = clGetDeviceIDs(platforms[selected_platform], CL_DEVICE_TYPE_ALL, 255, devices, &device_count);
    if (err != CL_SUCCESS) {
        clog_failure("Couldn't get device list...");
        return 4;
    }

    if (device_count == 0) {
        clog_failure("No devices in selected platform...");
        return 5;
    }

    if (device_count == 1) {
        selected_device = 0;
    } else {
        printf("%sSELECT DEVICE:%s ", CLOG_ANSI_GREEN, CLOG_ANSI_YELLOW);
        int err = scanf("%d", &selected_device);
        if (selected_device >= device_count || err == 0) {
            clog_failure("No such device...");
            return 3;
        }
    }

    printf(CLOG_ANSI_DEFAULT);

    p_id[0] = platforms[selected_platform];
    id[0] = devices[selected_device];
    return 0;
}
