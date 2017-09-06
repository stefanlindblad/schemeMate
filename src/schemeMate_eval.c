#include "schemeMate_eval.h"

void init_evaluation()
{
    evalStackBottom = (sm_obj*) malloc(sizeof(sm_obj) * INIT_EVALUATION_STACK_SIZE);
    evalStackPointer = evalStackBottom;
    evalStackTop = &(evalStackBottom[INIT_EVALUATION_STACK_SIZE]);

	// Builtin Functions
	register_system_function("+", internal_plus);
	register_system_function("-", internal_minus);
	register_system_function("*", internal_mult);
	register_system_function("/", internal_div);
}

void sm_eval_intern(sm_obj o) 
{
    sm_obj obj;
    switch (get_tag(o)) {
		case TAG_SYMBOL:
			obj = get_binding(o, MAIN_ENV);
			if (obj == NULL)
				ERROR_CODE("Unknown variable given.", 44);
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
	    ERROR_CODE("Unknown function reference.", 53);
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
 
	if (argc < 1)
		ERROR_CODE("+ function expects at least 1 arguments", 45);

	while (--argc >= 0) {
		sm_obj next_value = POP();
		if(is_int(next_value))
			sum = sum + int_val(next_value);
		else
			ERROR_CODE("+ function works on numbers, received NaN.", 46);
	}
	PUSH(new_int(sum));
}

static void internal_minus(int argc)
{
	int result = 0;

	if (argc < 1)
		ERROR_CODE("- function expects at least 1 arguments", 45);

	sm_obj next_value = POP();
	if (is_int(next_value))
		result = - int_val(next_value);
	else
		ERROR_CODE("- function works on numbers, received NaN.", 46);

	if (argc == 1) {
		PUSH(new_int(result));
		return;
	}

	while (--argc > 1) {
	next_value = POP();

	if (is_int(next_value))
	    result = result - int_val(next_value);
	else
	    ERROR_CODE("- function works on numbers, received NaN.", 46);
	}


    next_value = POP();
    if (is_int(next_value))
		result = result + int_val(next_value);
    else
		ERROR_CODE("- function works on numbers, received NaN.", 46);

	PUSH(new_int(result));
}

// WIP
static void internal_mult(int argc)
{
	int result = 1;

	if (argc < 1)
		ERROR_CODE("* function expects at least 1 argument.", 45);

	while (--argc >= 0) {
		sm_obj next_value = POP();
		if (is_int(next_value))
			result = result * int_val(next_value);
		else
			ERROR_CODE("* function works on numbers, received NaN.", 46);
	}

	PUSH(new_int(result));
}

static void internal_div(int argc)
{
	int result = 0;
	int i = 1;

	if (argc < 1)
		ERROR_CODE("/ function expects at least 1 argument.", 45);

	if (argc == 1) {
		sm_obj value = POP();
		if (is_int(value)) {
			result = 1 / int_val(value);
			PUSH(new_int(result));
			return;
		}
		else
			ERROR_CODE("/ function works only on integers currently.", 46);
	}

	sm_obj counter = GET_N(argc);
	if (is_int(counter))
		result = int_val(counter);
	else
		ERROR_CODE("/ function works only on integers currently.", 46);

	for (; i < argc; i++) {
		sm_obj denominator = GET_N(i);
		if (is_int(denominator))
			result =  result / int_val(denominator);
		else
			ERROR_CODE("/ function works only on integers currently.", 46);
	}

	PUSH(new_int(result));
}