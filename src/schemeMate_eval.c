#include "schemeMate_eval.h"

void init_evaluation()
{
    evalStackBottom = (sm_obj*) malloc(sizeof(sm_obj) * INIT_EVALUATION_STACK_SIZE);
    evalStackPointer = evalStackBottom;
    evalStackTop = &(evalStackBottom[INIT_EVALUATION_STACK_SIZE]);

	// Builtin Functions
	register_system_function("+", internal_plus);
}

void sm_eval_intern(sm_obj o) 
{
    sm_obj obj;
    switch (get_tag(o)) {
		case TAG_SYMBOL:
			obj = get_binding(o, MAIN_ENV);
	    	if (obj == NULL) 
		 		ERROR("Could not find a binding");
	    	PUSH(obj);
	    	return;
		case TAG_CONS:
	    	callDepth++;
	    	sm_eval_list(o);
	    	callDepth--;
	    	return;
		default:
	    	PUSH(o);
	    	return;
    }
	return POP();
}

sm_obj sm_eval(sm_obj o)
{
	sm_eval_intern(o);
	return POP();
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
		    PUSH(sm_eval(nextArg));
		    argc++;
		}
		(*obj->sm_sys_func.code)(argc);
		return;
	}
	case TAG_SYS_SYNTAX:
	    (*obj->sm_sys_syntax.code)(args);
	    return;

	default:
	    ERROR("Could not retrieve system function.");
    }
}

void register_system_function(char* name, void_func callable)
{
    sm_obj function = new_sys_func(callable, name);
    PUSH(function);
    sm_obj key = new_symbol(name);
    function = POP();
    add_binding(key, function, MAIN_ENV);
}

static void internal_plus(int argc)
{
	int sum = 0;
 
	while(--argc >= 0) {
		sm_obj next_value = POP();
		if(is_int(next_value))
			sum = sum + int_val(next_value);
		else
			ERROR("Argument to + is NaN");
	}
	PUSH(new_int(sum));
}
