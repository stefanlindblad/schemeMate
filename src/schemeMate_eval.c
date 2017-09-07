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

void sm_eval_intern(sm_obj o) 
{
    sm_obj obj;
    switch (get_tag(o)) {
		case TAG_SYMBOL:
			obj = get_binding(o, MAIN_ENV);
			if (obj == NULL)
				ERROR_CODE("Unknown variable given.", 44);
			PUSH(obj, MAIN_STACK);
	    	return;
		case TAG_CONS:
	    	callDepth++;
	    	sm_eval_list(o);
	    	callDepth--;
	    	return;
		default:
			PUSH(o, MAIN_STACK);
	    	return;
    }
	return POP(MAIN_STACK);
}

sm_obj sm_eval(sm_obj o)
{
	sm_eval_intern(o);
	return POP(MAIN_STACK);
}

sm_obj sm_eval_list(sm_obj o) 
{
	sm_obj func = car(o);
	sm_obj args = cdr(o);
	sm_obj obj = sm_eval(func);

	switch (get_tag(obj)) {
	case TAG_SYS_FUNC:
	{
		int argc = 0;
		while (!is_nil(args)) {
		    sm_obj nextArg = car(args);
		    args = cdr(args);
		    PUSH(sm_eval(nextArg), MAIN_STACK);
		    argc++;
		}
		(*obj->sm_sys_func.code)(argc);
		return;
	}
	case TAG_SYS_SYNTAX:
	    (*obj->sm_sys_syntax.code)(args);
	    return;

	default:
	    ERROR_CODE("Unknown function reference.", 53);
    }
}

void register_system_function(char* name, void_func callable)
{
    sm_obj function = new_sys_func(callable, name);
    PUSH(function, MAIN_STACK);
    sm_obj key = new_symbol(name);
    function = POP(MAIN_STACK);
    add_binding(key, function, &MAIN_ENV);
}
