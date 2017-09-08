#include "schemeMate_functions.h"

void init_functions()
{
	// Register Basic Syntax
	register_system_syntax("define", internal_define);

	// Register Math Functions
	register_system_function("+", internal_plus);
	register_system_function("-", internal_minus);
	register_system_function("*", internal_mult);
	register_system_function("/", internal_div);
	register_system_function("max", internal_max);
	register_system_function("min", internal_min);
	register_system_function("magnitude", internal_magnitude);
	register_system_function("modulo", internal_modulo);

	// Register Logic Functions
	register_system_function("eq?", internal_mem_equal);
	register_system_function("=", internal_int_equal);
	register_system_function("string=", internal_string_equal);
	register_system_function("<", internal_lessthan);
	register_system_function(">", internal_greaterthan);
	register_system_function(">=", internal_greaterequal);
	register_system_function("<=", internal_lessequal);
	register_system_function("or", internal_logical_or);
	register_system_function("and", internal_logical_and);
	register_system_function("not", internal_logical_not);

	// Register Type Check Functions
	register_system_function("int?", internal_is_int);
	register_system_function("string?", internal_is_string);
	register_system_function("zero?", internal_is_zero);
	register_system_function("pos?", internal_is_positive);
	register_system_function("neg?", internal_is_negative);
}

static void internal_define(sm_obj args)
{
	sm_obj literal = car(args);
	sm_obj data = cdr(args);

	if (is_symbol(literal)) {
		sm_obj value = sm_eval(data, MAIN_ENV);
		add_binding(literal, value, &MAIN_ENV);
		PUSH(sm_void(), MAIN_STACK);
		return;
	}

	if (is_cons(literal)) {
		sm_obj name = car(literal);

		if (is_symbol(name)) {
			sm_obj args = cdr(literal);
			sm_obj func = new_user_func(args, data);
			add_binding(name, func, &MAIN_ENV);
			PUSH(sm_void(), MAIN_STACK);
			return;
		}
	}
}

static void internal_plus(int argc)
{
	int sum = 0;
 
	if (argc < 1)
		ERROR_CODE("+ function expects at least 1 arguments", 45);

	while (--argc >= 0) {
		sm_obj next_value = POP(MAIN_STACK);
		if(is_int(next_value))
			sum = sum + int_val(next_value);
		else
			ERROR_CODE("+ function works on numbers, received NaN.", 46);
	}
	PUSH(new_int(sum), MAIN_STACK);
}

static void internal_minus(int argc)
{
	int result = 0;

	if (argc < 1)
		ERROR_CODE("- function expects at least 1 arguments", 45);

	sm_obj next_value = POP(MAIN_STACK);
	if (is_int(next_value))
		result = - int_val(next_value);
	else
		ERROR_CODE("- function works on numbers, received NaN.", 46);

	if (argc == 1) {
		PUSH(new_int(result), MAIN_STACK);
		return;
	}

	while (--argc > 1) {
	next_value = POP(MAIN_STACK);

	if (is_int(next_value))
	    result = result - int_val(next_value);
	else
	    ERROR_CODE("- function works on numbers, received NaN.", 46);
	}


    next_value = POP(MAIN_STACK);
    if (is_int(next_value))
		result = result + int_val(next_value);
    else
		ERROR_CODE("- function works on numbers, received NaN.", 46);

	PUSH(new_int(result), MAIN_STACK);
}

static void internal_mult(int argc)
{
	int result = 1;

	if (argc < 1)
		ERROR_CODE("* function expects at least 1 argument.", 45);

	while (--argc >= 0) {
		sm_obj next_value = POP(MAIN_STACK);
		if (is_int(next_value))
			result = result * int_val(next_value);
		else
			ERROR_CODE("* function works on numbers, received NaN.", 46);
	}

	PUSH(new_int(result), MAIN_STACK);
}

static void internal_div(int argc)
{
	int result = 0;
	int i = 1;

	if (argc < 1)
		ERROR_CODE("/ function expects at least 1 argument.", 45);

	if (argc == 1) {
		sm_obj value = POP(MAIN_STACK);
		if (is_int(value)) {
			result = 1 / int_val(value);
			PUSH(new_int(result), MAIN_STACK);
			return;
		}
		else
			ERROR_CODE("/ function works only on numbers currently.", 46);
	}

	sm_obj counter = GET_N(argc, MAIN_STACK);
	if (is_int(counter))
		result = int_val(counter);
	else
		ERROR_CODE("/ function works only on numbers currently.", 46);

	for (; i < argc; i++) {
		sm_obj denominator = GET_N(i, MAIN_STACK);
		if (is_int(denominator))
			result =  result / int_val(denominator);
		else
			ERROR_CODE("/ function works only on numbers currently.", 46);
	}

	PUSH(new_int(result), MAIN_STACK);
}

static void internal_max(int argc)
{
	int result = -2147483647;
	int value;

	if (argc < 1)
		ERROR_CODE("max function expects at least 1 argument.", 45);

	while (--argc >= 0) {
		sm_obj candidate = POP(MAIN_STACK);
		if (is_int(candidate)) {
			value = int_val(candidate);
			if (value > result)
				result = value;	
		}
		else
			ERROR_CODE("max function works on numbers, received NaN.", 46);
	}

	PUSH(new_int(result), MAIN_STACK);
}

static void internal_min(int argc)
{
	int result = 2147483647;
	int value;

	if (argc < 1)
		ERROR_CODE("min function expects at least 1 argument.", 45);

	while (--argc >= 0) {
		sm_obj candidate = POP(MAIN_STACK);
		if (is_int(candidate)) {
			value = int_val(candidate);
			if (value < result)
				result = value;	
		}
		else
			ERROR_CODE("min function works on numbers, received NaN.", 46);
	}

	PUSH(new_int(result), MAIN_STACK);
}
	
static void internal_magnitude(int argc)
{
	int value;

	if (argc != 1)
		ERROR_CODE("magnitude function expects exactly 1 argument.", 45);

	sm_obj candidate = POP(MAIN_STACK);
	if (is_int(candidate)) {
		value = int_val(candidate);
		if (value < 0)
			value = -value;
		PUSH(new_int(value), MAIN_STACK);
		return;
	}
	else
		ERROR_CODE("magnitude function works on numbers, received NaN.", 46);
}

static void internal_modulo(int argc)
{
	int dividend, divisor, result;

	if (argc != 2)
		ERROR_CODE("modulo function expects exactly 2 arguments.", 45);
	
	sm_obj obj_divisor = POP(MAIN_STACK);
	sm_obj obj_dividend = POP(MAIN_STACK);
	
	if (is_int(obj_dividend) && is_int(obj_divisor)) {
		dividend = int_val(obj_dividend);
		divisor = int_val(obj_divisor);

		result = dividend % divisor;
		PUSH(new_int(result), MAIN_STACK);
		return;
	}
	else
		ERROR_CODE("modulo function works on numbers, received NaN.", 46);
}

static void internal_mem_equal(int argc)
{
	if (argc < 2)
		ERROR_CODE("eq? function expects at minimum 2 arguments.", 45);
	
	sm_obj object = POP(MAIN_STACK);

	while (--argc >= 1) {
		sm_obj next = POP(MAIN_STACK);
		if (object != next) {
			PUSH(sm_false(), MAIN_STACK);
			return;
		}
	}
	PUSH(sm_true(), MAIN_STACK);
}

static void internal_int_equal(int argc)
{
	int value, compare;

	if (argc < 2)
		ERROR_CODE("= function expects at minimum 2 arguments.", 45);
	
	sm_obj object = POP(MAIN_STACK);
	if (is_int(object))
		value = int_val(object);
	else
		ERROR_CODE("= function works on numbers, received NaN.", 46);

	while (--argc >= 1) {
		sm_obj next = POP(MAIN_STACK);
		if (is_int(next))
			compare = int_val(next);
		else
			ERROR_CODE("= function works on numbers, received NaN.", 46);

		if (compare != value) {
			PUSH(sm_false(), MAIN_STACK);
			return;
		}
	}
	PUSH(sm_true(), MAIN_STACK);
}

static void internal_string_equal(int argc)
{
	char *value;
	char *compare;
	
		if (argc < 2)
			ERROR_CODE("string= function expects at minimum 2 arguments.", 45);
		
		sm_obj object = POP(MAIN_STACK);

		if (is_string(object))
			value = string_val(object);
		else
			ERROR_CODE("strings= function works on strings, received NaN.", 46);
	
		while (--argc >= 1) {
			sm_obj next = POP(MAIN_STACK);
			if (is_string(next))
				compare = string_val(next);
			else
				ERROR_CODE("strings= function works on strings, received NaN.", 46);
			
			if (strcmp(compare, value)) {
				PUSH(sm_false(), MAIN_STACK);
				return;
			}
		}
		PUSH(sm_true(), MAIN_STACK);
}

static void internal_lessthan(int argc)
{
	int one, two;

	if (argc != 2)
		ERROR_CODE("< function expects exactly 2 arguments.", 45);

	sm_obj obj1 = POP(MAIN_STACK);
	sm_obj obj2 = POP(MAIN_STACK);

	if (is_int(obj1) && is_int(obj2)) {
		one = int_val(obj2);
		two = int_val(obj1);

		if (one < two) {
			PUSH(sm_true(), MAIN_STACK);
			return;
		}
		else
			PUSH(sm_false(), MAIN_STACK);
	}
	else
		ERROR_CODE("< function works on numbers, received NaN.", 46);
}

static void internal_greaterthan(int argc)
{
	int one, two;

	if (argc != 2)
		ERROR_CODE("> function expects exactly 2 arguments.", 45);

	sm_obj obj1 = POP(MAIN_STACK);
	sm_obj obj2 = POP(MAIN_STACK);

	if (is_int(obj1) && is_int(obj2)) {
		one = int_val(obj2);
		two = int_val(obj1);

		if (one > two) {
			PUSH(sm_true(), MAIN_STACK);
			return;
		}
		else
			PUSH(sm_false(), MAIN_STACK);
	}
	else
		ERROR_CODE("> function works on numbers, received NaN.", 46);
}

static void internal_greaterequal(int argc)
{
	int one, two;

	if (argc != 2)
		ERROR_CODE(">= function expects exactly 2 arguments.", 45);

	sm_obj obj1 = POP(MAIN_STACK);
	sm_obj obj2 = POP(MAIN_STACK);

	if (is_int(obj1) && is_int(obj2)) {
		one = int_val(obj2);
		two = int_val(obj1);

		if (one >= two) {
			PUSH(sm_true(), MAIN_STACK);
			return;
		}
		else
			PUSH(sm_false(), MAIN_STACK);
	}
	else
		ERROR_CODE(">= function works on numbers, received NaN.", 46);
}

static void internal_lessequal(int argc)
{
	int one, two;

	if (argc != 2)
		ERROR_CODE("<= function expects exactly 2 arguments.", 45);

	sm_obj obj1 = POP(MAIN_STACK);
	sm_obj obj2 = POP(MAIN_STACK);

	if (is_int(obj1) && is_int(obj2)) {
		one = int_val(obj2);
		two = int_val(obj1);

		if (one <= two) {
			PUSH(sm_true(), MAIN_STACK);
			return;
		}
		else
			PUSH(sm_false(), MAIN_STACK);
	}
	else
		ERROR_CODE("<= function works on numbers, received NaN.", 46);
}

static void internal_logical_or(int argc)
{
	if (argc != 2)
		ERROR_CODE("(or) function expects exactly 2 arguments.", 45);

	sm_obj obj1 = POP(MAIN_STACK);
	sm_obj obj2 = POP(MAIN_STACK);

	if (obj1 == sm_true() || obj2 == sm_true()) {
		PUSH(sm_true(), MAIN_STACK);
		return;
	}

	if ((is_int(obj1) && int_val(obj1) > 0) || (is_int(obj2) && int_val(obj2) > 0)) {
		PUSH(sm_true(), MAIN_STACK);
		return;
	}

	PUSH(sm_false(), MAIN_STACK);
}

static void internal_logical_and(int argc)
{
	if (argc != 2)
		ERROR_CODE("(and) function expects exactly 2 arguments.", 45);

	sm_obj obj1 = POP(MAIN_STACK);
	sm_obj obj2 = POP(MAIN_STACK);

	if (obj1 == sm_true() && obj2 == sm_true()) {
		PUSH(sm_true(), MAIN_STACK);
		return;
	}

	if ((is_int(obj1) && int_val(obj1) > 0) && (is_int(obj2) && int_val(obj2) > 0)) {
		PUSH(sm_true(), MAIN_STACK);
		return;
	}

	PUSH(sm_false(), MAIN_STACK);
}

static void internal_logical_not(int argc)
{
	if (argc != 1)
		ERROR_CODE("(not) function expects exactly 1 argument.", 45);

	sm_obj obj = POP(MAIN_STACK);

	if (obj == sm_true()) {
		PUSH(sm_false(), MAIN_STACK);
		return;
	}

	if (is_int(obj) && int_val(obj) > 0) {
		PUSH(sm_false(), MAIN_STACK);
		return;
	}

	PUSH(sm_true(), MAIN_STACK);
}

static void internal_is_int(int argc)
{
	if (argc < 1)
		ERROR_CODE("int? function expects at least 1 argument.", 45);

	while (--argc >= 0) {
		sm_obj value = POP(MAIN_STACK);
		if(!is_int(value)) {
			PUSH(sm_false(), MAIN_STACK);
			return;
		}
	}
	PUSH(sm_true(), MAIN_STACK);
}

static void internal_is_string(int argc)
{
	if (argc < 1)
		ERROR_CODE("string? function expects at least 1 argument.", 45);

	while (--argc >= 0) {
		sm_obj value = POP(MAIN_STACK);
		if(!is_string(value)) {
			PUSH(sm_false(), MAIN_STACK);
			return;
		}
	}
	PUSH(sm_true(), MAIN_STACK);
}

static void internal_is_zero(int argc)
{
	if (argc < 1)
		ERROR_CODE("zero? function expects at least 1 argument.", 45);

	while (--argc >= 0) {
		sm_obj value = POP(MAIN_STACK);
		if(!is_int(value)) {
			PUSH(sm_false(), MAIN_STACK);
			return;
		}
		else if(int_val(value) != 0) {
			PUSH(sm_false(), MAIN_STACK);
			return;
		}
	}
	PUSH(sm_true(), MAIN_STACK);
}

static void internal_is_positive(int argc)
{
	if (argc < 1)
		ERROR_CODE("positive? function expects at least 1 argument.", 45);

	while (--argc >= 0) {
		sm_obj value = POP(MAIN_STACK);
		if(!is_int(value)) {
			PUSH(sm_false(), MAIN_STACK);
			return;
		}
		else if(int_val(value) <= 0) {
			PUSH(sm_false(), MAIN_STACK);
			return;
		}
	}
	PUSH(sm_true(), MAIN_STACK);
}

static void internal_is_negative(int argc)
{
	if (argc < 1)
		ERROR_CODE("negative? function expects at least 1 argument.", 45);

	while (--argc >= 0) {
		sm_obj value = POP(MAIN_STACK);
		if(!is_int(value)) {
			PUSH(sm_false(), MAIN_STACK);
			return;
		}
		else if(int_val(value) > 0) {
			PUSH(sm_false(), MAIN_STACK);
			return;
		}
	}
	PUSH(sm_true(), MAIN_STACK);
}
