#ifndef MAIN_HEADER
#define MAIN_HEADER

#include "schemeMate_objects.h"
#include "schemeMate_selftest.h"
#include "schemeMate_eval.h"
#include "schememate_trampoline.h"

static sm_stream INPUT = NULL;
extern void_ptr_ptr_func contparse_initial_eval();

static enum INTERPRETER_MODE {
    RECURSIVE = 1,
    CONT_PARSE = 2,
    MAX_MODE = 4
};

static void init_system(int running_mode);
static void recursive_repl();
static void_ptr_ptr_func contparse_repl_front();
static void_ptr_ptr_func contparse_repl_back();
void init_all_systems(int running_mode);
void restart_all_systems();
void shutdown_all_systems();
int main(int argc, char *argv[]);

#endif // MAIN_HEADER