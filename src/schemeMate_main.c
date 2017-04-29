#include "schemeMate_main.h"

#define SELFTEST

static void init_system() {
    INPUT = new_file_stream(stdin);
    SM_NIL = new_nil();
    SM_TRUE = new_true();
    SM_FALSE = new_false();
    SM_VOID = new_void();
    SM_EOF = new_eof();
}

static void repl()
{
	sm_obj expr;
	sm_obj result;

	while (true) {
		fprintf(stdout, ">>> ");
		expr = sm_read(INPUT);
		//result = sm_eval(expr);
		if (get_tag(expr) != TAG_VOID) {
			sm_print(stdout, expr);
	    	fprintf(stdout, "\n");
		}
	}
}

int main(int argc, char *argv[])
{
	init_system();
	init_memory();
	printf("Welcome to schemeMate [0.1]\n");
	#ifdef SELFTEST
		sm_selftest();
	#endif
	repl();
	exit(0);
}