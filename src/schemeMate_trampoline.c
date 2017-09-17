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
	    SAVE_CP(contparse_func_eval);
	    return contparse_initial_eval;

	default:
	    PUSH_M(expr);
	    return LOAD_CP();
	}
}

void_ptr_ptr_func contparse_func_eval()
{
	sm_obj env, data, func, func_args;

	func = POP_M();
	data = POP_M();
	env = POP_M();
	func_args = cdr(data);

	switch (get_tag(func)) {
	case TAG_SYS_SYNTAX:
		return (void_ptr_ptr_func) (*func->sm_sys_syntax.code)(func_args);
	case TAG_SYS_FUNC:
	case TAG_USER_FUNC:
	{
		if (is_nil(func_args) && get_tag(func) == TAG_SYS_FUNC) {
			(*func->sm_sys_func.code)(0);
			return LOAD_CP();
		}
		
		// Function with first argument
		PUSH_M(func);
		PUSH_M(env);
		PUSH_M(data);
		PUSH_M(cdr(func_args));
		PUSH_M(new_int(0)); // Counter for first argument

		PUSH_M(env);
		PUSH_M(car(func_args));
		SAVE_CP(contparse_args_eval);
		return contparse_initial_eval;
	}
	default:
		ERROR_CODE("Unknown function reference.", 53);
		return NULL;
	}
}

void_ptr_ptr_func contparse_args_eval()
{
	sm_obj env, data, func, eval_arg, rest_args, num_args;

	eval_arg = POP_M();
	num_args = POP_M();
	int nargs = int_val(num_args) + 1;
	rest_args = POP_M();
	data = POP_M();
	env = POP_M();
	func = POP_M();

	// This is the 1+x argument but not the last
	if (!is_nil(rest_args)) {
		PUSH_M(func);
		PUSH_M(env);
		PUSH_M(data);
		PUSH_M(cdr(rest_args));
		PUSH_M(new_int(nargs));

		PUSH_M(env);
		PUSH_M(car(rest_args));
		SAVE_CP(contparse_args_eval);
		return contparse_initial_eval;
	}

	// All arguments have been evaluated, we can execute
	if (func->sm_any.tag == TAG_SYS_FUNC) {
		(*func->sm_sys_func.code)(nargs);
		return LOAD_CP();
	}

}
