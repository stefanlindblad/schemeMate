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
	sm_obj expr, obj;
	sm_env env;

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
	sm_obj data, func, func_args;
	sm_env env;

	func = POP_M();
	data = POP_M();
	env = POP_M();
	func_args = cdr(data);

	switch (get_tag(func)) {
	case TAG_SYS_SYNTAX:
		return (void_ptr_ptr_func) (*func->sm_sys_syntax.code)(func_args, env);
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
	sm_obj data, func, eval_arg, rest_args, num_args;
	sm_env env;

	eval_arg = POP_M();
	num_args = POP_M();
	int nargs = int_val(num_args) + 1;
	rest_args = POP_M();
	data = POP_M();
	env = POP_M();
	func = POP_M();
	PUSH_M(eval_arg);

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

	PUSH_M(env);
	PUSH_M(func);
	PUSH_M(new_int(nargs));
	SAVE_CP(contparse_user_func_eval);
	return contparse_initial_eval;
}

void_ptr_ptr_func contparse_user_func_eval()
{
	sm_obj func, nargs_obj;
	sm_env env;

	nargs_obj = POP_M();
	func = POP_M();
	env = POP_M();
	int nargs = int_val(nargs_obj);

	// Create environment for user function
	sm_obj func_args = func->sm_user_func.args;
	sm_obj func_body = func->sm_user_func.body;
	sm_obj body_result = sm_nil();
	sm_env func_env = allocate_env(INIT_USER_ENV_SIZE, MAIN_ENV);

	// Register bindings for arguments in function env
	sm_obj next_arg = func_args;
	while (!is_nil(next_arg)) {
		sm_obj name = car(next_arg);
		sm_obj next_arg = cdr(next_arg);

		sm_obj data = POP_M();
		add_binding(name, data, func_env);
	}
}

// Built in cp based syntax

// Helper function for contparse set/define to avoid code duplication
static void assign_symbol(sm_obj literal, sm_obj args, sm_env env)
{
	sm_obj data = cdr(args);

	if (is_symbol(literal)) {
		sm_obj object = car(data);
		sm_obj value = sm_eval(object, env);
		add_binding(literal, value, &env);
		PUSH(sm_void(), MAIN_STACK);
		return;
	}

	if (is_cons(literal)) {
		sm_obj object = car(literal);
		if (is_symbol(object)) {
			sm_obj args = cdr(literal);
			sm_obj func = new_user_func(args, data);
			add_binding(object, func, &env);
			PUSH(sm_void(), MAIN_STACK);
			return;
		}
	}
}

void_ptr_ptr_func contparse_define_front(sm_obj args, sm_env env)
{
	sm_obj literal = car(car(args));
	sm_obj entry = sm_nil();

	if (is_symbol(literal))
		entry = get_binding(literal, env);

	if (is_cons(literal))
		entry = get_binding(car(literal), env);

	if (entry != NULL)
		ERROR_CODE("define tried to redefine existing symbol, use set! instead.", 54);

	PUSH_M(env);
	PUSH_M(literal);
	PUSH_M(cdr(args));
	SAVE_CP(contparse_initial_eval);
	return contparse_define_back;
}

void_ptr_ptr_func contparse_define_back()
{
	sm_obj obj, args;
	sm_env env;

	args = POP_M();
	obj = POP_M();
	env = POP_M();

	assign_symbol(obj, args, env);
	PUSH_M(sm_void());
	return LOAD_CP();
}
