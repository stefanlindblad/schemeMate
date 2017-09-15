#ifndef FUNCTIONS_HEADER
#define FUNCTIONS_HEADER

#include "schemeMate_objects.h"
#include "schemeMate_eval.h"

void init_functions();

// Basic Syntax
static void internal_define(sm_obj args);
static void internal_set(sm_obj args);
static void internal_lambda(sm_obj args);
static void internal_display(sm_obj args);
static void internal_car(sm_obj args);
static void internal_cdr(sm_obj args);
static void internal_cons(sm_obj args);

// Math Functionality
static void internal_plus(int argc);
static void internal_minus(int argc);
static void internal_mult(int argc);
static void internal_div(int argc);
static void internal_max(int argc);
static void internal_min(int argc);
static void internal_magnitude(int argc);
static void internal_modulo(int argc);

// Logic Functionality
static void internal_mem_equal(int argc);
static void internal_int_equal(int argc);
static void internal_string_equal(int argc);
static void internal_lessthan(int argc);
static void internal_greaterthan(int argc);
static void internal_greaterequal(int argc);
static void internal_lessequal(int argc);
static void internal_logical_or(int argc);
static void internal_logical_and(int argc);
static void internal_logical_not(int argc);

// Type Check Functionality
static void internal_is_int(int argc);
static void internal_is_string(int argc);
static void internal_is_zero(int argc);
static void internal_is_positive(int argc);
static void internal_is_negative(int argc);

// Helper Functionality
static void internal_exit(int argc);

#endif // FUNCTIONS_HEADER
