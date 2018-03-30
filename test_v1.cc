#include <iostream>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <iomanip>

#include "clutil/cl_buffer.hpp"
#include "clutil/cl_bitonic_sort.hpp"

using namespace std;

float random_float(float min, float max);

#define TEST_SIZE_CL_BUFFER (0x1<<19)

int main() {
	cout << "-- CL-Util --" << endl;

	std::vector<float> real_vector;

	cl_platform_id platform;
	cl_device_id device;
	if (common_cl_pick_device(&platform, &device)) return 2;

	auto context = common_cl_create_context(device);

	CLBuffer<float> val(context);
	CLBuffer<size_t> idx(context);

	auto sorter = common_cl_compile_program(context, (char*)"clutil/CL_CODE/float_sorter.cl", (char*)"float_sorter");

	srand(time(NULL));

	for(size_t i = 0; i < TEST_SIZE_CL_BUFFER; i++){
		real_vector.push_back(random_float(-100, 100));//random_float(-1000.0, 1000.0));
	}

	auto add_err = val.add(TEST_SIZE_CL_BUFFER);
	if(add_err != CL_SUCCESS) return add_err;

	add_err = idx.add(TEST_SIZE_CL_BUFFER);
	if(add_err != CL_SUCCESS) return add_err;

	auto idx_vec = idx.read();
	auto val_vec = val.read();

	for(size_t i = 0; i < TEST_SIZE_CL_BUFFER; i++){
		idx_vec->data()[i] = i;
		val_vec->data()[i] = real_vector[i];
		//std::cout << real_vector[i] << " ";
	}
	//std::cout << std::endl;

	idx.write();
	val.write();

	std::cout << "Size: " << TEST_SIZE_CL_BUFFER << " " << std::endl;

	CLBitonicSorter cl_sorter(sorter);
	auto out = cl_sorter.sort(&idx, &val);

	std::cout << "finished function call in: " << out.duration_usec/1e6 << " s" << std::endl;

	idx_vec = idx.read();
	val_vec = val.read();
	float cursor = (*val_vec)[(*idx_vec)[0]];

	//for(size_t i = 0; i < TEST_SIZE_CL_BUFFER; i++){
	//	std::cout << (*idx_vec)[i]  << " ";
	//}
	//std::cout << std::endl;

	for (size_t i = 0; i < TEST_SIZE_CL_BUFFER; i++) {
		//std::cout << std::setprecision(3) << (*val_vec)[(*idx_vec)[i]] << " ";
	}
	std::cout << std::endl;
	
	for(size_t i = 1; i < idx_vec->size(); i++){
		float next = (*val_vec)[(*idx_vec)[i]];
		if(cursor > next){
			std::cout << "Sort failure... " << i << " c:" << cursor << " n:" << next << std::endl;
			return 4;
		}
		cursor = next;
	}	

	std::cout << "Sorted..." << std::endl;

	return 0;
}

/*****************************************/
/************** DEFINITIONS **************/
/*****************************************/

float random_float(float min, float max) {
	return min + static_cast<float>(rand()) / (RAND_MAX / (max - min));
}

