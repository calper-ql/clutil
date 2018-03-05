__kernel void float_sorter(
		__global size_t* idx,
		__global float* val,
		__global size_t* c1,
		__global size_t* c2,
		__global uchar* dir
		)
{
	size_t gi = get_global_id(0);	
	if( dir[gi] == ( val[idx[c1[gi]]] > val[idx[c2[gi]]] ) ){
		size_t t = idx[c1[gi]];
		idx[c1[gi]] = idx[c2[gi]];
		idx[c2[gi]] = t;
 	}
}
