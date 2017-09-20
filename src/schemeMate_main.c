#include "schemeMate_main.h"

static int running_mode = CONT_PARSE;

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

void restart_interpreter(int mode)
{
	if (mode == RECURSIVE)
		recursive_repl();
	else if (mode == CONT_PARSE) {
		PRINT(">>> ");
		execute_trampoline(contparse_repl_front);
	}
}

void init_all_systems(int mode)
{
	init_system(mode);
	init_memory(mode);
	init_environment(mode);
	init_evaluation(mode);
	if (mode == CONT_PARSE)
		init_trampoline(mode);
	init_functions(mode);
}

void restart_all_systems(int mode)
{
	shutdown_all_systems();

	// If no new mode is specified, use old one
	if (mode == 666)
		mode = running_mode;

	init_all_systems(mode);

	if (mode == RECURSIVE)
		recursive_repl();
	else if (mode == CONT_PARSE) {
		PRINT(">>> ");
		execute_trampoline(contparse_repl_front);
	}
}

void shutdown_all_systems()
{
	shutdown_memory();
	shutdown_environment();
	shutdown_evaluation();
	shutdown_trampoline();
}

void_ptr_ptr_func contparse_repl_front()
{
	sm_obj expr;
	sm_obj result;
	expr = sm_read(INPUT, true);
	PUSH_M(MAIN_ENV);
	PUSH_M(expr);
	SAVE_CP(contparse_repl_back);
	return contparse_initial_eval;
}

void_ptr_ptr_func contparse_repl_back()
{
	sm_obj result = POP_M();

	if (result != sm_eof() && result != NULL) {
		sm_print(result, true);
		PRINT("\n");
		PRINT(">>> ");
	}
	return contparse_repl_front;
}

int main(int argc, char *argv[])
{
	// check for command line options
	for (int i = 0; i < argc; ++i) {
		char* arg = argv[i];
		int a = i + 2;
		if (strcmp(arg, "--recursive") == 0)
			running_mode = RECURSIVE;
	}

	init_all_systems(running_mode);
	sm_selftest();

	if (running_mode == CONT_PARSE)
		PRINT_LINE("Welcome to schemeMate - Continuation Parsing Edition");
	else if (running_mode == RECURSIVE)
		PRINT_LINE("Welcome to schemeMate - Recursive Edition");

	if (running_mode == RECURSIVE)
		recursive_repl();
	else if (running_mode == CONT_PARSE) {
		PRINT(">>> ");
		execute_trampoline(contparse_repl_front);
	}

	shutdown_all_systems();
	exit(0);
}