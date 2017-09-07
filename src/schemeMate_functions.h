#ifndef FUNCTIONS_HEADER
#define FUNCTIONS_HEADER

#include "schemeMate_objects.h"
#include "schemeMate_eval.h"

void init_functions();

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
//static void internal_lessthan(int argc);
//static void internal_greaterthan(int argc);
//static void internal_greaterequal(int argc);
//static void internal_lessequal(int argc);
//static void internal_logical_or(int argc);
//static void internal_logical_and(int argc);
//static void internal_logical_not(int argc);
//static void internal_is_int(int argc);
//static void internal_is_string(int argc);
//static void internal_is_zero(int argc);
//static void internal_is_positive(int argc);
//static void internal_is_negative(int argc);

#endif // FUNCTIONS_HEADER
