#ifndef __BTNC_SEQ_GEN_H
#define __BTNC_SEQ_GEN_H

#include <stddef.h>
#include <stdlib.h>

struct BTNC_LYR {
	size_t* cmp_idx_1 = NULL;
	size_t* cmp_idx_2 = NULL;
	unsigned char* dir = NULL;
	size_t size = 0;
	size_t count = 0;
};

struct BTNC_LYR create_btnc_lyr(size_t size){
	size_t* c1 = (size_t*) calloc(size, sizeof(size_t));
	size_t* c2 = (size_t*) calloc(size, sizeof(size_t));
	unsigned char* dir = (unsigned char*) calloc(size ,sizeof(unsigned char));
	BTNC_LYR bl;
	bl.cmp_idx_1 = c1;
	bl.cmp_idx_2 = c2;
	bl.dir = dir;
	bl.size = size;
	bl.count = 0;
	return bl;
};

struct BTNC_SEQ {
	BTNC_LYR* data;	
	size_t size;
};

size_t __gp2l(size_t n){
	size_t k=1;
	while (k>0 && k<n) k = k << 1;
	return k >> 1;
}

size_t __gpd2l(size_t n){
	size_t k=1;
	size_t shift_count=0;
	while (k>0 && k<n){
		k = k << 1;
		shift_count++;
	}
	if(shift_count == 0) return shift_count;
	return shift_count-1;
}

void __btnc_seq_mrg(size_t lo, size_t n, size_t depth, bool dir, BTNC_SEQ seq){
	if(n > 1){
		size_t m = __gp2l(n);
		for (size_t i=lo; i<lo+n-m; i++) {
			size_t idx = seq.data[depth].count;
			seq.data[depth].cmp_idx_1[idx] = i;
			seq.data[depth].cmp_idx_2[idx] = i+m;
			seq.data[depth].dir[idx] = dir ? 1:0;
			seq.data[depth].count++;
		}
		__btnc_seq_mrg(lo, m, depth+1, dir, seq);
		__btnc_seq_mrg(lo+m, n-m, depth+1, dir, seq);
	}
}

void __btnc_seq_gen(size_t lo, size_t n, size_t depth, size_t predicted_depth, size_t mc, bool dir, BTNC_SEQ seq){
	if(n > 1){
		size_t m = n/2;
		__btnc_seq_gen(lo, m, depth+1, predicted_depth-1, mc-predicted_depth, !dir, seq);
		__btnc_seq_gen(lo+m, n-m, depth+1, predicted_depth-1 ,mc-predicted_depth, dir, seq);
		__btnc_seq_mrg(lo, n, mc-predicted_depth, dir, seq);
	}
}

BTNC_SEQ btnc_seq_gen(size_t size, bool dir){
	BTNC_SEQ seq;
	size_t predicted_depth = __gpd2l(size)+1;
	size_t mc = predicted_depth;

	for(size_t i = 0; i < predicted_depth; i++) mc += i;	

	seq.data = (BTNC_LYR*) malloc(sizeof(BTNC_LYR)*mc);
	seq.size = mc;

	for(size_t i = 0; i < mc; i++){
		seq.data[i] = create_btnc_lyr(size);
	}	

	__btnc_seq_gen(0, size, 0, predicted_depth, mc, dir, seq);

	return seq;
}

void btnc_seq_delete(BTNC_SEQ seq){
	for(size_t i = 0; i < seq.size; i++){
		free(seq.data[i].cmp_idx_1);
		free(seq.data[i].cmp_idx_2);
		free(seq.data[i].dir);
	}
	free(seq.data);
}

#endif
