#ifndef EVAL_HEADER
#define EVAL_HEADER

#include "schemeMate_objects.h"

#define INIT_EVALUATION_STACK_SIZE 1024

static int callDepth = 0;

// inline functions

static inline void PUSH(sm_obj o, sm_stack s)
{
    (s->evalStackPointer++)->entry = o;
    if (s->evalStackPointer >= s->evalStackTop)
		  ERROR_CODE("Evaluation Stack Overflow!", 47);
}

// Returns the top element on the stack
static inline sm_obj POP(sm_stack s)
{
    if (s->evalStackPointer <= s->evalStackBottom)
          ERROR_CODE("Evaluation Stack Underflow!", 48);
    s->evalStackPointer--;
    return s->evalStackPointer->entry;
}

// Returns the nTh element down the stack
static inline sm_obj GET_N(int n, sm_stack s)
{
    if (s->evalStackPointer <= s->evalStackBottom)
		  ERROR_CODE("Evaluation Stack Underflow!", 48);
    return (s->evalStackPointer - n)->entry;
}

static inline void DROP(int amount, sm_stack s)
{
    s->evalStackPointer -= amount;
    if (s->evalStackPointer < s->evalStackBottom)
		ERROR_CODE("Evaluation Stack Underflow!", 48);
}

void init_evaluation();
sm_stack allocate_stack();
void sm_eval_intern(sm_obj o, sm_env env);
sm_obj sm_eval(sm_obj o, sm_env env);
sm_obj sm_eval_list(sm_obj o, sm_env env);
void register_system_syntax(char* name, void_func callable);
void register_system_function(char* name, void_func callable);

#endif // EVAL_HEADER
