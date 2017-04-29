#include "schemeMate_eval.h"

static sm_obj eof_singleton;
static sm_obj *symbolTable = NULL;
static int numKnownSymbols = 0;
static int symbolTableSize = 0;

static struct environment* allocateEnvironment(int size) {
	int nBytes;
	struct environment* env;
	numBytes = sizeof()

}

static sm_obj evalPlus(sm_obj args) {
	int sum = 0;
	sm_obj nextCell = args;
 
	while(nextCell != sm_nil()) {
		sm_obj nextArg = car(nextCell);
		sm_obj valueOfNext;

		valueOfNext = scm_eval(nextCell);
		breakpoint();

		nextCell = cdr(nextCell);
	}
	breakpoint();
}

static sm_obj evalMinus(sm_obj args) {
	breakpoint();
}


sm_obj evalList(sm_obj expr) {

	sm_obj func = CAR(expr);
	sm_obj args = CDR(expr);

	if (func == new_symbol("+")) {
		return evalPlus(args);
	}
	if (func == new_symbol("-")) {
		return evalMinus(args);
	}

	ERROR("Is not implemented yet");
}

