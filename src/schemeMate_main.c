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
		PRINT(">>> ");
		expr = sm_read(INPUT);
		result = sm_eval(expr);
		sm_print(stdout, result);
		PRINT("\n");
	}
}

int main(int argc, char *argv[])
{
	init_system();
	init_memory();
	init_environment();
	init_evaluation();
	sm_selftest();
	PRINT_LINE("Welcome to schemeMate [0.1]");
	repl();
	exit(0);
}