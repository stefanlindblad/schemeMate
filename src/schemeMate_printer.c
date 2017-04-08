#include "schemeMate_printer.h"

void scm_print(FILE* file, SCM_OBJ o)
{
	switch (tagOf(o))
	{
	case TAG_INT:
		fprintf(stdout, "%ld", intValue(o));
		break;
	case TAG_FLOAT:
		fprintf(stdout, "%lf", floatValue(o));
		break;
	case TAG_NIL:
		fprintf(stdout, "()");
		break;
	case TAG_FALSE:
		fprintf(stdout, "#f");
		break;
	case TAG_TRUE:
		fprintf(stdout, "#t");
		break;
	case TAG_SYMBOL:
		fprintf(stdout, "%s", symbolValue(o));
		break;
	case TAG_STRING:
		fprintf(stdout, "<STRING>");
		break;
	case TAG_CONS:
		scm_printList(file, o);
		break;
	case TAG_EOF:
		fprintf(stdout, "#eof");
		break;
	case TAG_OBJ:
		fprintf(stdout, "%f", floatValue(o));
		break;
	default:
		fprintf(stdout, "<UNKNOWN CHARACTER>");
		break;
	}
}

void print(sizeof(SCM_OBJ));

void scm_printList(SCM_OBJ obj)
{
	fprintf(stdout, "(");
	scm_print(car(obj));
	if (isNil(cdr(obj)))
		fprintf(stdout, ")");
	else
		scm_printList
}

void error(char *file, int line, char *message) {
    fprintf(stderr, "%s[%d]: %s\n", file, line, message);
    abort();
}

static inline double
floatValue(SCM_OBJ obj)
{
	ASSERT(isFloat(obj), "IS NOT A FLOAT VALUE")
	return (obj->scm_float.fVal);
}