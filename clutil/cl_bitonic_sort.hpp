#ifndef CL_BITONIC_SORT_HPP
#define CL_BITONIC_SORT_HPP

#include "btnc_seq_gen.h"
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
		CLBuffer<size_t>** ca1;
		CLBuffer<size_t>** ca2;
		CLBuffer<unsigned char>** dira;
		COMMON_CL_PROGRAM* sorter;
		size_t size;
		BTNC_SEQ seq;

	public:
		CLBitonicSorter(COMMON_CL_PROGRAM* sorter, size_t size){
			this->sorter = sorter;
			this->size = size;
			seq = btnc_seq_gen(size, true);

			ca1 = (CLBuffer<size_t>**)malloc(sizeof(CLBuffer<size_t>*)*seq.size);
			ca2 = (CLBuffer<size_t>**)malloc(sizeof(CLBuffer<size_t>*)*seq.size);
			dira = (CLBuffer<unsigned char>**)malloc(sizeof(CLBuffer<unsigned char>*)*seq.size);

			for(size_t i = 0; i < seq.size; i++){
				CLBuffer<size_t>* c1 = new CLBuffer<size_t>(sorter->context);
				c1->add(seq.data[i].count);
				std::memcpy(c1->local_data()->data(), seq.data[i].cmp_idx_1, sizeof(size_t)*seq.data[i].count);

				CLBuffer<size_t>* c2 = new CLBuffer<size_t>(sorter->context);
				c2->add(seq.data[i].count);
				std::memcpy(c2->local_data()->data(), seq.data[i].cmp_idx_2, sizeof(size_t)*seq.data[i].count);

				CLBuffer<unsigned char>* dir = new CLBuffer<unsigned char>(sorter->context);
				dir->add(seq.data[i].count);
				std::memcpy(dir->local_data()->data(), seq.data[i].dir, sizeof(unsigned char)*seq.data[i].count);

				c1->write();
				c2->write();
				dir->write();

				ca1[i] = c1;
				ca2[i] = c2;
				dira[i] = dir;
			}


		}

		~CLBitonicSorter(){
			delete ca1;
			delete ca2;
			delete dira;
			btnc_seq_delete(seq);
		}


		CLBitonicSorterOutput sort(CLBuffer<size_t>* idx, CLBuffer<float>* val){
			CLBitonicSorterOutput out;
			out.err = 0;
			
			if(idx == nullptr){
				out.err = -1;
				return out;
			}

			if(val == nullptr){
				out.err = -2;
				return out;
			}

			if(idx->get_size() != val->get_size()){
				out.err = 1;
				return out;
			}

			if(idx->get_size() != size){
				out.err = 2;
				return out;
			}


			auto start = std::chrono::steady_clock::now();
			for(size_t i = 0; i < seq.size; i++){
				auto cerr = clSetKernelArg(sorter->kernel, 0, sizeof(cl_mem), idx->device_data());
				if( cerr != CL_SUCCESS){
					out.err = 3;
					out.cl_err = cerr;
					return out;
				}

				cerr = clSetKernelArg(sorter->kernel, 1, sizeof(cl_mem), val->device_data());
				if( cerr != CL_SUCCESS){
					out.err = 4;
					out.cl_err = cerr;
					return out;
				}

				cerr = clSetKernelArg(sorter->kernel, 2, sizeof(cl_mem), ca1[i]->device_data());
				if( cerr != CL_SUCCESS){
					out.err = 5;
					out.cl_err = cerr;
					return out;
				}

				cerr = clSetKernelArg(sorter->kernel, 3, sizeof(cl_mem), ca2[i]->device_data());
				if( cerr != CL_SUCCESS){
					out.err = 6;
					out.cl_err = cerr;
					return out;
				}

				cerr = clSetKernelArg(sorter->kernel, 4, sizeof(cl_mem), dira[i]->device_data());
				if( cerr != CL_SUCCESS){
					out.err = 7;
					out.cl_err = cerr;
					return out;
				}

				size_t compute_size = seq.data[i].count;
				cerr = clEnqueueNDRangeKernel(sorter->context->commands, sorter->kernel, 1, NULL, &compute_size, 0, 0, NULL, NULL);
				if( cerr != CL_SUCCESS){
					out.err = 8;
					out.cl_err = cerr;
					return out;
				}

				clFinish(sorter->context->commands);
			}
			auto end = std::chrono::steady_clock::now();

			size_t usecs = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
			out.duration_usec = usecs;

			return out;

		}

};

#endif
