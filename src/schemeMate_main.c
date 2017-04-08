#include "schemeMate_main.h"

static void initializeSystem() {
    INPUT = new_fileStream(stdin);
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
		expr = sm_read(stdInputStream);
		result = sm_eval(expr);
		if (tag(result) != TAG_VOID) {
	    	sm_print(stdout, result, PRINT_WRITE);
	    	fprintf(stdout, "\n");
		}
	}
}


int main(int argc, char *argv[])
{
	init_system();
	printf("Welcome to schemeMate [0.1 ]\n");
	selftest();
	repl();
	deinit_system();
	exit(0);
}