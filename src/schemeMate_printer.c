#include "schemeMate_printer.h"

void sm_print(FILE* file, sm_obj o, bool oneLineMode)
{
	switch (get_tag(o))
	{
	case TAG_INT:
		fprintf(file, "%ld", int_val(o));
		break;
	case TAG_FLOAT:
		fprintf(file, "%lf", float_val(o));
		break;
	case TAG_NIL:
		fprintf(file, "()");
		break;
	case TAG_FALSE:
		fprintf(file, "#f");
		break;
	case TAG_TRUE:
		fprintf(file, "#t");
		break;
	case TAG_SYMBOL:
		fprintf(file, "%s", symbol_val(o));
		break;
	case TAG_STRING:
		fprintf(file, "%s", string_val(o));
		break;
	case TAG_CONS:
		sm_printList(file, o, oneLineMode);
		break;
	case TAG_EOF:
		if (oneLineMode)
			return;
		fprintf(file, "#eof");
		break;
	case TAG_OBJ:
		fprintf(file, "%f", float_val(o));
		break;
	case TAG_VOID:
		fprintf(file, "#void");
		break;
	case TAG_SYS_FUNC:
		fprintf(file, "<BUILTIN FUNC>");
		break;
	case TAG_SYS_SYNTAX:
	    fprintf(file, "<BUILTIN SYNTAX>");
	    break;
	case TAG_USER_FUNC:
		fprintf(file, "(lambda ");
	    sm_print(file, o->sm_user_func.args, oneLineMode);
	    fprintf(file, " ");
	    sm_printRest(file, o->sm_user_func.body, oneLineMode);
		break;

	default:
		fprintf(file, "<UNKNOWN CHARACTER>");
		break;
	}
}

void sm_printList(FILE* file, sm_obj obj, bool oneLineMode)
{
	fprintf(file, "(");
	sm_printRest(file, obj, oneLineMode);
}

void sm_printRest(FILE* file, sm_obj cons, bool oneLineMode)
{
    sm_print(file, car(cons), oneLineMode);
    if (is_nil(cdr(cons))) {
		fprintf(file, ")");
		return;
    }
    if (!is_cons(cdr(cons))) {
		fprintf(file, " . ");
		sm_print(file, cdr(cons), oneLineMode);
		fprintf(file, ")");
		return;
    }
    fprintf(file, " ");
    sm_printRest(file, cdr(cons), oneLineMode);
}

void print(char *message)
{
	fprintf(stdout, "%s", message);
}

void print_line(char *message)
{
	fprintf(stdout, "%s\n", message);
}

void warn(char *message, char *file, int line)
{
	fprintf(stdout, "%s:%d: %s\n", file, line, message);
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
    fprintf(stdout, "%s:%d: %s\n", file, line, message);
	fprintf(stdout, "schemeMate exited with error code %d, press ENTER to continue.\n", exit_code);
	sm_read(new_file_stream(stdin), true);
	exit(exit_code);
}
