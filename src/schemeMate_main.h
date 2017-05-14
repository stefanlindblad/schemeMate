#ifndef MAIN_HEADER
#define MAIN_HEADER

#include "schemeMate_objects.h"
#include "schemeMate_selftest.h"

// global special tokens
static sm_obj SM_NIL = NULL;
static sm_obj SM_TRUE = NULL;
static sm_obj SM_FALSE = NULL;
static sm_obj SM_VOID = NULL;
static sm_obj SM_EOF = NULL;
static sm_obj SM_QUOTE = NULL;
static sm_stream INPUT = NULL;

static void init_system();
static void repl();
void restart();
int main(int argc, char *argv[]);

#endif // MAIN_HEADER