#include "schemeMate_eval.h"

sm_stack MAIN_STACK = NULL;

void init_evaluation()
{
	MAIN_STACK = allocate_stack();
}

sm_stack allocate_stack()
{
	unsigned bytes;
	sm_stack stack;
	sm_stack_entry entries;

	// allocate memory for the entries
	bytes = INIT_EVALUATION_STACK_SIZE * (unsigned) sizeof(struct sm_stack_entry_struct);
	entries = (sm_stack_entry) malloc(bytes);
	memset(entries, 0, bytes);

	// allocate the stack
	stack = (sm_stack) malloc(sizeof(struct sm_stack_struct));
	stack->evalStackBottom = entries;
    stack->evalStackPointer = stack->evalStackBottom;
    stack->evalStackTop = &(stack->evalStackBottom[INIT_EVALUATION_STACK_SIZE]);
	return stack;
}

void sm_eval_intern(sm_obj o, sm_env env) 
{
    sm_obj obj;
    switch (get_tag(o)) {
		case TAG_SYMBOL:
			obj = get_binding(o, env);
			if (obj == NULL)
				ERROR_CODE("Unknown variable given.", 44);
			PUSH(obj, MAIN_STACK);
	    	return;
		case TAG_CONS:
	    	callDepth++;
	    	sm_eval_list(o, env);
	    	callDepth--;
	    	return;
		default:
			PUSH(o, MAIN_STACK);
	    	return;
    }
	return POP(MAIN_STACK);
}

sm_obj sm_eval(sm_obj o, sm_env env)
{
	sm_eval_intern(o, env);
	return POP(MAIN_STACK);
}

sm_obj sm_eval_list(sm_obj o, sm_env env) 
{
	sm_obj func = car(o);
	sm_obj args = cdr(o);
	sm_obj obj = sm_eval(func, env);

	switch (get_tag(obj)) {
	case TAG_SYS_FUNC:
	{
		int argc = 0;
		while (!is_nil(args)) {
		    sm_obj nextArg = car(args);
		    args = cdr(args);
		    PUSH(sm_eval(nextArg, env), MAIN_STACK);
		    argc++;
		}
		(*obj->sm_sys_func.code)(argc);
		return;
	}
	case TAG_SYS_SYNTAX:
	{
		(*obj->sm_sys_syntax.code)(args);
	    return;
	}
	case TAG_USER_FUNC:
	{
		sm_obj func_args = obj->sm_user_func.args;
		sm_obj func_body = obj->sm_user_func.body;
		sm_obj body_result = sm_nil();
		sm_env func_env = allocate_env(20); // TODO: check for element size and set accordingly

		// Compare layout arguments with given arguments and store evaluated data
		while (func_args != sm_nil()) {

			sm_obj arg_name = car(func_args);
			func_args = cdr(func_args);
			sm_obj arg_input = car(args);
			args = cdr(args);

			sm_obj arg_value = sm_eval(arg_input, func_env);
			add_binding(arg_name, arg_value, &func_env);
		}

		// Same thing for the body data
		while (func_body != sm_nil()) {
			sm_obj body_data = car(func_body);
			func_body = cdr(func_body);
			body_result = sm_eval(body_data, func_env);
		}
		PUSH(body_result, MAIN_STACK);
	    return;
	}
	default:
	    ERROR_CODE("Unknown function reference.", 53);
    }
}

void register_system_syntax(char* name, void_func callable)
{
    sm_obj syntax = new_sys_syntax(callable, name);
    PUSH(syntax, MAIN_STACK);
    sm_obj key = new_symbol(name);
    syntax = POP(MAIN_STACK);
    add_binding(key, syntax, &MAIN_ENV);
}

void register_system_function(char* name, void_func callable)
{
    sm_obj function = new_sys_func(callable, name);
    PUSH(function, MAIN_STACK);
    sm_obj key = new_symbol(name);
    function = POP(MAIN_STACK);
    add_binding(key, function, &MAIN_ENV);
}

void register_user_function(char* name, void_func callable)
{
    sm_obj function = new_sys_func(callable, name);
    PUSH(function, MAIN_STACK);
    sm_obj key = new_symbol(name);
    function = POP(MAIN_STACK);
    add_binding(key, function, &MAIN_ENV);
}
