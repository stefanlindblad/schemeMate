#include "schemeMate_printer.h"

void sm_print(FILE* file, sm_obj o)
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
		sm_printList(file, o);
		break;
	case TAG_EOF:
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
	    sm_print(file, o->sm_user_func.args);
	    fprintf(file, " ");
	    sm_printRest(file, o->sm_user_func.body);
		break;

	default:
		fprintf(file, "<UNKNOWN CHARACTER>");
		break;
	}
}

void sm_printList(FILE* file, sm_obj obj)
{
	fprintf(file, "(");
	sm_printRest(file, obj);
}

void sm_printRest(FILE* file, sm_obj cons)
{
    sm_print(file, car(cons));
    if (is_nil(cdr(cons))) {
		fprintf(file, ")");
		return;
    }
    if (!is_cons(cdr(cons))) {
		fprintf(file, " . ");
		sm_print(file, cdr(cons));
		fprintf(file, ")");
		return;
    }
    fprintf(file, " ");
    sm_printRest(file, cdr(cons));
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

void error(char *message, char *file, int line)
{
    fprintf(stdout, "%s:%d: %s\n", file, line, message);
    abort();
}
