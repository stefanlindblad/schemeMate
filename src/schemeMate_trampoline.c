#include "schemeMate_trampoline.h"

void init_trampoline()
{
	continuation_stack = (void_ptr_ptr_func*) malloc(sizeof(void_ptr_ptr_func) * INIT_CONT_STACK_SIZE);
	continuation_stack_pointer = continuation_stack;
	continuation_stack_top = &(continuation_stack[INIT_CONT_STACK_SIZE]);
}

void execute_trampoline(void_ptr_ptr_func function)
{
	while (function != NULL) {
		function = (void_ptr_ptr_func)((*function)());
	}
}

void grow_cp_stack()
{
	int size = continuation_stack_pointer - continuation_stack;
	continuation_stack = (void_ptr_ptr_func*) realloc(continuation_stack, sizeof(void_ptr_ptr_func) * (size*2));
	continuation_stack_pointer = &continuation_stack[size];
	continuation_stack_top = &(continuation_stack[size*2]);
}

void_ptr_ptr_func contparse_initial_eval()
{
    sm_obj expr, env, obj;

	expr = POP_M();
    env = POP_M();

    switch (get_tag(expr)) {
	case TAG_SYMBOL:
		obj = get_binding(expr, env);
	    if (obj == NULL)
			ERROR_CODE("Unknown variable or function given.", 44);
	    PUSH_M(obj);
	    return LOAD_CP();

	case TAG_CONS:
	    PUSH_M(env);
	    PUSH_M(expr);
	    PUSH_M(env);
	    PUSH_M(car(expr));
	    //SAVE_CP(contparse_eval_two);
	    return contparse_initial_eval;

	default:
	    PUSH_M(expr);
	    return LOAD_CP();
	}
}
