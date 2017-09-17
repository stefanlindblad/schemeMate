#ifndef MAIN_HEADER
#define MAIN_HEADER

#include "schemeMate_objects.h"
#include "schemeMate_selftest.h"
#include "schemeMate_eval.h"
#include "schememate_trampoline.h"

static sm_stream INPUT = NULL;
extern void_ptr_ptr_func contparse_initial_eval();

enum INTERPRETER_MODE {
    RECURSIVE = 1,
    CONT_PARSE = 2,
    MAX_MODE = 4
};

static void init_system();
static void recursive_repl();
static void_ptr_ptr_func contparse_repl_front();
static void_ptr_ptr_func contparse_repl_back();
void restart();
int main(int argc, char *argv[]);

#endif // MAIN_HEADER