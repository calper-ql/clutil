#include "common_cl.h"
#include <stdio.h>

char *__cccp_load_file(char *path) {
    FILE *source;
    char *source_str;

    clog_wait(path, "LOADING");
    source = fopen(path, "r");
    if (source) {
        fseek(source, 0L, SEEK_END);
        size_t size = ftell(source);
        if (size == 0) {
            clog_failure("file size is 0");
            return NULL;
        }
        rewind(source);
        source_str = (char *) malloc(sizeof(char) * size + 1);
        for (size_t i = 0; i < size; i++) {
            source_str[i] = fgetc(source);
        }
        source_str[size] = '\0';
        clog_raw_log(path, "LOADED", CLOG_ANSI_MAGENTA);
        printf("\n-------------------------\n");
        printf("%s\n--------------------------\n", source_str);

    } else {
        clog_failure(path);
        return NULL;
    }

    return source_str;
}

struct COMMON_CL_PROGRAM *common_cl_compile_program(struct COMMON_CL_CONTEXT *context, char *path, char *kernel_name) {
    clog_raw_log(path, "COMPILING", CLOG_ANSI_YELLOW);
    printf("\n");
    clog_add_space();
    clog_add_space();
    char *source_str = __cccp_load_file(path);
    if (!source_str) {
        clog_remove_space();
        clog_remove_space();
        return NULL;
    }

    char buffer[2048];
    size_t length;
    cl_int err;

    struct COMMON_CL_PROGRAM *program = (struct COMMON_CL_PROGRAM *) malloc(sizeof(struct COMMON_CL_PROGRAM));
    program->context = context;

    program->program = clCreateProgramWithSource(context->context, 1, (const char **) &source_str, NULL, &err);
    if (err != CL_SUCCESS) {
        clog_failure("create program");
        clog_remove_space();
        clog_remove_space();
        free(context);
        free(source_str);
        return NULL;
    }
    clog_success("create program");

    err = clBuildProgram(program->program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        clog_failure("build program\n");
        size_t len;
        char error_buffer[2048 * 20];
        printf("err:%d\n----------\n", err);
        clGetProgramBuildInfo(program->program, context->device, CL_PROGRAM_BUILD_LOG, sizeof(error_buffer),
                              error_buffer, &len);
        printf("%s\n----------\n", error_buffer);
        clog_remove_space();
        clog_remove_space();
        free(context);
        free(source_str);
        return NULL;
    }
    clog_success("build program");

    program->kernel = clCreateKernel(program->program, kernel_name, &err);
    if (err != CL_SUCCESS) {
        clog_failure("create kernel");
        clog_remove_space();
        clog_remove_space();
        free(context);
        free(source_str);
        return NULL;
    }
    clog_success("create kernel");

    clog_remove_space();
    clog_remove_space();
    free(source_str);
    return program;
}