//
// Created by Can Alper on 1/2/18.
//

#ifndef CL_BUFFER_H
#define CL_BUFFER_H

#include <vector>
#include <map>
#include <iostream>

extern "C" {
#include "common_cl.h"
}

template<class T>
class CLBuffer {
	private:
		size_t size;
		cl_mem mem;
		std::vector<T> values;

		COMMON_CL_CONTEXT *context;

	public:
		CLBuffer(COMMON_CL_CONTEXT *context);

		~CLBuffer();

		cl_int add(size_t expand_size);

		void write();

		cl_mem* device_data();

		std::vector<T> *local_data();

		std::vector<T> *read();

		size_t get_size();

};

template<class T>
CLBuffer<T>::CLBuffer(COMMON_CL_CONTEXT *context) {
	this->context = context;
	this->size = 0;
}

template<class T>
CLBuffer<T>::~CLBuffer() {
	if(size > 0) clReleaseMemObject(mem);	
}

template<class T>
cl_int CLBuffer<T>::add(size_t expand_size) {
	cl_int err;

	cl_mem new_mem = clCreateBuffer(context->context, CL_MEM_READ_WRITE, (size + expand_size) * sizeof(T), NULL, &err);
	if(err != CL_SUCCESS) return err;

	if(size > 0){
		err = clEnqueueCopyBuffer(context->commands, mem, new_mem, 0, 0, size * sizeof(T), 0, NULL, NULL);
		if(err != CL_SUCCESS) {
			clReleaseMemObject(new_mem);
			return err;
		}
	}

	clFinish(context->commands);
	if(size > 0) clReleaseMemObject(mem);

	mem = new_mem;
	size += expand_size;
	values.resize(size);

	return CL_SUCCESS;
}

template<class T>
void CLBuffer<T>::write() {
	clEnqueueWriteBuffer(context->commands, mem, false, 0, size*sizeof(T), &values.data()[0], 0, NULL, NULL);
	clFinish(context->commands);
}

template<class T>
std::vector<T> *CLBuffer<T>::read() {
	clEnqueueReadBuffer(context->commands, mem, false, 0, size*sizeof(T), &values.data()[0], 0, NULL, NULL);
	clFinish(context->commands);
	return &values;	
}

template<class T>
std::vector<T> *CLBuffer<T>::local_data(){
	return &values;
}

template<class T>
cl_mem* CLBuffer<T>::device_data() {
	return &mem;
}

template<class T>
size_t CLBuffer<T>::get_size() {
	return size;
}

#endif //HIVEENGINE_CL_BUFFER_H
