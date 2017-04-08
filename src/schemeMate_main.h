#ifndef MAIN_HEADER
#define MAIN_HEADER

#include "schemeMate_objects.h"
#include "schemeMate_selftest.h"

#ifdef DEBUG
# define DBG_PRINT(x) fprintf x
#else
# define DBG_PRINT(x) /* as nothing */
#endif

sm_obj SM_NIL = NULL;
sm_obj SM_TRUE = NULL;
sm_obj SM_FALSE = NULL;
sm_obj SM_VOID = NULL;
sm_obj SM_EOF = NULL;
sm_obj SM_QUOTE = NULL;
static sm_stream INPUT = NULL;

static void init_system();
static void repl();
int main(int argc, char *argv[]);


#endif // MAIN_HEADER