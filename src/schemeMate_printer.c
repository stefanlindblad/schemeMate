#include "schemeMate_printer.h"

void sm_print(sm_obj o, bool oneLineMode)
{
	switch (get_tag(o))
	{
	case TAG_INT:
		printf("%ld", int_val(o));
		break;
	case TAG_NIL:
		printf("()");
		break;
	case TAG_FALSE:
		printf("#f");
		break;
	case TAG_TRUE:
		printf("#t");
		break;
	case TAG_SYMBOL:
		printf("%s", symbol_val(o));
		break;
	case TAG_STRING:
		printf("'%s'", string_val(o));
		break;
	case TAG_CONS:
		sm_printList(o, oneLineMode);
		break;
	case TAG_EOF:
		if (oneLineMode)
			return;
		printf("#eof");
		break;
	case TAG_VOID:
		if (oneLineMode)
			return;
		printf("#void");
		break;
	case TAG_SYS_FUNC:
		printf("<BUILTIN FUNC>");
		break;
	case TAG_SYS_SYNTAX:
	    printf("<BUILTIN SYNTAX>");
	    break;
	case TAG_USER_FUNC:
		printf("(lambda ");
	    sm_print(o->sm_user_func.args, oneLineMode);
	    printf(" ");
	    sm_printRest(o->sm_user_func.body, oneLineMode);
		break;

	default:
		printf("<UNKNOWN CHARACTER>");
		break;
	}
}

void sm_printList(sm_obj obj, bool oneLineMode)
{
	printf("(");
	sm_printRest(obj, oneLineMode);
}

void sm_printRest(sm_obj cons, bool oneLineMode)
{
    sm_print(car(cons), oneLineMode);
    if (is_nil(cdr(cons))) {
		printf(")");
		return;
    }
    if (!is_cons(cdr(cons))) {
		printf(" . ");
		sm_print(cdr(cons), oneLineMode);
		printf(")");
		return;
    }
    printf(" ");
    sm_printRest(cdr(cons), oneLineMode);
}

void print(char *message)
{
	printf("%s", message);
}

void print_line(char *message)
{
	printf("%s\n", message);
}

void warn(char *message, char *file, int line)
{
	printf("%s:%d: %s\n", file, line, message);
}

void read_enter_and_return(sm_stream inStream)
{
	sm_char input;
	while(true) {
		input = fgetc(inStream->file);
		if (input < 0) // enter
			break;
		else
			input = fgetc(inStream->file);
	}
}

void error(char *message, char *file, int line, int exit_code)
{
    printf("%s:%d: %s\n", file, line, message);
	printf("schemeMate exited with error code %d, press ENTER to continue.\n", exit_code);
	restart_all_systems(666);
}
