__kernel void float_sorter(
	__global size_t* idx,
	__global float* val,
	unsigned group_size,
	int across
)
{
	size_t tid = get_global_id(0)*2;	
	
	size_t gid = tid / group_size;
	size_t cid = gid * group_size;
	size_t lid = (tid - cid) / 2;
	size_t cp2 = cid + group_size - lid - 1;
	size_t cp1 = across>0 ?  (cp2-(group_size /2)) : (cid + lid);
	//printf("cp1: %d -- cp2: %d\n", cp1, cp2);
	//barrier(CLK_GLOBAL_MEM_FENCE);
			
	if (val[idx[cp1]] > val[idx[cp2]]) {
		size_t tmp = idx[cp1];
		idx[cp1] = idx[cp2];
		idx[cp2] = tmp;
	}

}

// 0 1 2 3 4 5 6 7
// 0 2 4 6 8 10 12 14
// 0 1 4 5 8 9 12 13
