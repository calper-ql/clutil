#ifndef CL_BITONIC_SORT_HPP
#define CL_BITONIC_SORT_HPP

#include <chrono>
#include <thread>

#include "cl_buffer.hpp"
#include <cstring>
#include <chrono>

struct CLBitonicSorterOutput {
	int err;
	cl_int cl_err;
	size_t duration_usec;
};

class CLBitonicSorter {
private:
	COMMON_CL_PROGRAM * sorter;

public:
	CLBitonicSorter(COMMON_CL_PROGRAM* sorter) {
		this->sorter = sorter;
	}

	~CLBitonicSorter() {

	}

	size_t __gpd2l(size_t n) {
		size_t k = 1;
		size_t shift_count = 0;
		while (k>0 && k<n) {
			k = k << 1;
			shift_count++;
		}
		if (shift_count == 0) return shift_count;
		return shift_count;
	}

	CLBitonicSorterOutput sort(CLBuffer<size_t>* idx, CLBuffer<float>* val) {
		CLBitonicSorterOutput out;
		out.err = 0;

		if (idx == nullptr) {
			out.err = -1;
			return out;
		}

		if (val == nullptr) {
			out.err = -2;
			return out;
		}

		if (idx->get_size() != val->get_size()) {
			out.err = 1;
			return out;
		}
		bool powerOfTwo = !(val->get_size() == 0) && !(val->get_size() & (val->get_size() - 1));
		if (!powerOfTwo) {
			out.err = 2;
			return out;
		}

		unsigned bitonic_len = __gpd2l(val->get_size());
		unsigned group_size = 2;

		auto start = std::chrono::steady_clock::now();

		for (size_t i = 0; i < bitonic_len; i++) {
			unsigned local_group_size = group_size;
			for (size_t k = 0; k < i + 1; k++) {
				cl_mem cpy1 = idx->device_data();
				auto cerr = clSetKernelArg(sorter->kernel, 0, sizeof(cl_mem), &cpy1);
				if (cerr != CL_SUCCESS) {
					out.err = 3;
					out.cl_err = cerr;
					return out;
				}

				cl_mem cpy2 = val->device_data();
				cerr = clSetKernelArg(sorter->kernel, 1, sizeof(cl_mem), &cpy2);
				if (cerr != CL_SUCCESS) {
					out.err = 4;
					out.cl_err = cerr;
					return out;
				}

				int across = k;
				clSetKernelArg(sorter->kernel, 2, sizeof(unsigned), &local_group_size);
				clSetKernelArg(sorter->kernel, 3, sizeof(int), &across);

				size_t compute_size = val->get_size() / 2;
				cerr = clEnqueueNDRangeKernel(sorter->context->commands, sorter->kernel, 1, NULL, &compute_size, 0, 0, NULL, NULL);
				if (cerr != CL_SUCCESS) {
					out.err = 8;
					out.cl_err = cerr;
					return out;
				}
				
				local_group_size /= 2;
				//std::cout << "---" << std::endl;
			}
			group_size *= 2;
			//std::cout << "|||" << std::endl;
		}
		clFinish(sorter->context->commands);

		auto end = std::chrono::steady_clock::now();

		size_t usecs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		out.duration_usec = usecs;

		return out;

	}

};

#endif
