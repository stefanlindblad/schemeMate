#ifndef MAIN_HEADER
#define MAIN_HEADER

#include "schemeMate_objects.h"
#include "schemeMate_selftest.h"

static sm_stream INPUT = NULL;

enum INTERPRETER_MODE {
    RECURSIVE = 1,
    CONT_PARSE = 2,
    MAX_MODE = 4
};

static void init_system();
static void repl();
void restart();
int main(int argc, char *argv[]);

#endif // MAIN_HEADER