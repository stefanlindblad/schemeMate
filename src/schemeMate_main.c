#include "schemeMate_main.h"

#define stdInputStream

static void repl()
{
	fprintf(stdout, ">");
	SCM_OBJ expr = SCM_read(stdInputStream);
	scm_print(expr);
}


int main(int argc, char *argv[])
{
	init_system();
	printf("Welcome to our (incomplete) Scheme\n");
	scm_selftest();
	deinit_system();
	exit(0);
}