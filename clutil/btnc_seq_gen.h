#ifndef __BTNC_SEQ_GEN_H
#define __BTNC_SEQ_GEN_H

#include <stddef.h>
#include <stdlib.h>

struct BTNC_LYR {
	size_t* cmp_idx_1;
	size_t* cmp_idx_2;
	unsigned char* dir;
	size_t size;
	size_t count;
};

struct BTNC_SEQ {
	struct BTNC_LYR* data;	
	size_t size;
};

struct BTNC_LYR create_btnc_lyr(size_t size);
size_t __gp2l(size_t n);
size_t __gpd2l(size_t n);
void __btnc_seq_mrg(size_t lo, size_t n, size_t depth, unsigned char dir, struct BTNC_SEQ seq);
void __btnc_seq_gen(size_t lo, size_t n, size_t depth, size_t predicted_depth, size_t mc, unsigned char dir, struct BTNC_SEQ seq);
struct BTNC_SEQ btnc_seq_gen(size_t size, unsigned char dir);
void btnc_seq_delete(struct BTNC_SEQ seq);

#endif
