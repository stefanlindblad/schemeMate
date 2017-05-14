#ifndef EVAL_HEADER
#define EVAL_HEADER

#include "schemeMate_objects.h"

#define INIT_EVALUATION_STACK_SIZE 1024

static int callDepth = 0;
static sm_obj* evalStackBottom = NULL;
static sm_obj* evalStackPointer = NULL;
static sm_obj* evalStackTop = NULL;

// inline functions

static inline void PUSH(sm_obj o)
{
    *evalStackPointer++ = o;
    if (evalStackPointer >= evalStackTop) 
		ERROR_CODE("Evaluation Stack Overflow!", 47);
}

static inline sm_obj POP()
{
    if (evalStackPointer <= evalStackBottom) 
		ERROR_CODE("Evaluation Stack Underflow!", 48);
    return *--evalStackPointer;
}

static inline void DROP(int amount)
{
    evalStackPointer -= amount;
    if (evalStackPointer < evalStackBottom)
		ERROR_CODE("Evaluation Stack Underflow!", 48);
}

void init_evaluation();
void sm_eval_intern(sm_obj o);
sm_obj sm_eval(sm_obj o);
sm_obj sm_eval_list(sm_obj o);
void register_system_function(char* name, void_func callable);
static void internal_plus(int argc);
static void internal_minus(int argc);

#endif // EVAL_HEADER
