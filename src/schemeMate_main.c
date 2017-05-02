#include "schemeMate_main.h"

static void init_system() {
    INPUT = new_file_stream(stdin);
    SM_NIL = sm_nil();
    SM_TRUE = sm_true();
    SM_FALSE = sm_false();
    SM_VOID = sm_void();
    SM_EOF = sm_eof();
}

static void repl()
{
	sm_obj expr;
	sm_obj result;

	while (true) {
		fprintf(stdout, ">>> ");
		expr = sm_read(INPUT);
		result = sm_eval(expr);
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
	init_environment();
	init_evaluation();
	printf("Welcome to schemeMate [0.1]\n");
	if (argc > 1 && strcmp(argv[1], "--selftest") != 0)
		sm_selftest();
	repl();
	exit(0);
}