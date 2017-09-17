#include "schemeMate_main.h"

void init_system() {
    INPUT = new_file_stream(stdin);
}

void repl()
{
	sm_obj expr;
	sm_obj result;
	PRINT(">>> ");

	while (true) {
		expr = sm_read(INPUT, true);
		result = sm_eval(expr, MAIN_ENV);
		if (result == sm_eof())
			continue;
		sm_print(result, true);
		PRINT("\n");
		PRINT(">>> ");
	}
}

int main(int argc, char *argv[])
{
	int RUNNING_MODE = CONT_PARSE;

	// check for command line options
	for (int i = 0; i < argc; ++i) {
		char* arg = argv[i];
		int a = i + 2;
		if (strcmp(arg, "--recursive") == 0)
			RUNNING_MODE = RECURSIVE;
	}
	init_system();
	init_memory();
	init_environment();
	init_evaluation();
	init_functions();
	sm_selftest();
	PRINT_LINE("Welcome to schemeMate [0.2]");
	if (RUNNING_MODE == RECURSIVE)
		repl();
	else if (RUNNING_MODE == CONT_PARSE)
		repl();
	exit(0);
}