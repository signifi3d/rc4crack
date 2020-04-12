#ifndef POS_STACK_H
#define POS_STACK_H

typedef struct pos {
	int res_pos;
	int chst_pos;
} pos;

typedef struct pos_stack_t {
	int max_size;
	int size;
	pos * stack;
} pos_stack_t;


int create_pos_stack(int size, pos_stack_t * init_stack);
int ps_push(int res, int chst, pos_stack_t * stack);
int ps_top_res(pos_stack_t * stack);
int ps_top_chst(pos_stack_t * stack);
int ps_pop(pos_stack_t * stack);
int ps_size(pos_stack_t * stack);

#endif
