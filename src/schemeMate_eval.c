#include "schemeMate_eval.h"

static SCM_OBJ eof_singleton;
static SCM_OBJ *symbolTable = NULL;
static int numKnownSymbols = 0;
static int symbolTableSize = 0;

static struct environment* allocateEnvironment(int size) {
	int nBytes;
	struct environment* env;
	numBytes = sizeof()

}

static SCM_OBJ evalPlus(SCM_OBJ args) {
	int sum = 0;
	SCM_OBJ nextCell = args;
 
	while(nextCell != new_nil()) {
		SCM_OBJ nextArg = car(nextCell);
		SCM_OBJ valueOfNext;

		valueOfNext = scm_eval(nextCell);
		breakpoint();

		nextCell = cdr(nextCell);
	}
	breakpoint();
}

static SCM_OBJ evalMinus(SCM_OBJ args) {
	breakpoint();
}


SCM_OBJ evalList(SCM_OBJ expr) {

	SCM_OBJ func = CAR(expr);
	SCM_OBJ args = CDR(expr);

	if (func == new_symbol("+")) {
		return evalPlus(args);
	}
	if (func == new_symbol("-")) {
		return evalMinus(args);
	}



	report_error("Is not implemented yet");
}

