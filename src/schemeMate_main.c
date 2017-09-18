#include "schemeMate_main.h"

void init_system(int RUNNING_MODE) {
    INPUT = new_file_stream(stdin);
}

void recursive_repl()
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

void_ptr_ptr_func contparse_repl_front()
{
	sm_obj expr;
	sm_obj result;
	expr = sm_read(INPUT, true);
	PUSH_M(MAIN_ENV);
	PUSH_M(expr);
	SAVE_CP((void_ptr_ptr_func) contparse_repl_back);
	return (void_ptr_ptr_func) contparse_initial_eval;
}

void_ptr_ptr_func contparse_repl_back()
{
	sm_obj result = POP_M();

	if (result != sm_eof()) {
		sm_print(result, true);
		PRINT("\n");
		PRINT(">>> ");
	}
	return (void_ptr_ptr_func) contparse_repl_front;
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

	init_system(RUNNING_MODE);
	init_memory(RUNNING_MODE);
	init_environment(RUNNING_MODE);
	init_evaluation(RUNNING_MODE);
	if (RUNNING_MODE == CONT_PARSE)
		init_trampoline();
	init_functions(RUNNING_MODE);
	sm_selftest();
	PRINT_LINE("Welcome to schemeMate [0.2]");
	if (RUNNING_MODE == RECURSIVE)
		recursive_repl();
	else if (RUNNING_MODE == CONT_PARSE) {
		PRINT(">>> ");
		execute_trampoline((void_ptr_ptr_func) contparse_repl_front);
	}
	exit(0);
}