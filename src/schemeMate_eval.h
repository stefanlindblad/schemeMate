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
		ERROR("Evaluation Stack Overflow!");
}

static inline sm_obj POP()
{
    if (evalStackPointer <= evalStackBottom) 
		ERROR("Evaluation Stack Underflow!");
    return *--evalStackPointer;
}

static inline void DROP(int amount)
{
    evalStackPointer -= amount;
    if (evalStackPointer < evalStackBottom)
		ERROR("Evaluation Stack Underflow!");
}

void init_evaluation();
void sm_eval_intern(sm_obj o);
sm_obj sm_eval(sm_obj o);
sm_obj sm_eval_list(sm_obj o);
static void internal_plus(int argc);

#endif // EVAL_HEADER
