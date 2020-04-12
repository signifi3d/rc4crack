#include "pos_stack.h"
#include <stdlib.h>


int create_pos_stack(int size, pos_stack_t * init_stack) {
	//init_stack = (pos_stack_t*)malloc(sizeof(pos_stack_t));
	init_stack->max_size = size;
	init_stack->size = 0;
	init_stack->stack = malloc(sizeof(pos) * size);
	return 0;
}

int ps_push(int res, int chst, pos_stack_t * stack) {
	if (stack->size == stack->max_size)
		return -1;

	stack->stack[stack->size].res_pos = res;
	stack->stack[stack->size].chst_pos = chst;
	stack->size++;

	return 0;
}

int ps_top_res(pos_stack_t * stack) {
	if (stack->size == 0)
		return -1;
	return stack->stack[stack->size-1].res_pos;
}

int ps_top_chst(pos_stack_t * stack) {
	if (stack->size == 0)
		return -1;
	return stack->stack[stack->size-1].chst_pos;
}

int ps_pop(pos_stack_t * stack) {
	if (stack->size == 0)
		return -1;
	return --stack->size;
}

int ps_size(pos_stack_t * stack) {
	return stack->size;
}
