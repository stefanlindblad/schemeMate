#include "schemeMate_selftest.h"

void scm_selftest()
{
	SCM_OBJ result;
	result = scm_readString("123");
	ASSERT(result != NULL, "int check failed (NULL result)")
	ASSERT(result->scm_integer.iVal == 123, "int check failed")
	

	/*
	SCM_OBJ result2;
	result2 = scm_readString(" (a (b (c)))");
	(a(b(c))) [a|->] [->|EOF] [b|->] [->|EOF] [c|EOF]
	ASSERT(tagOf(result) == TAG_CONS, "empty list read check failed")
	ASSERT(isSymbol(car(result)), "car list read check failed")
	ASSERT(isCons(cdr(result)), "car list read check failed")
	ASSERT(isSymbol(car(cdr(result)), "car list read check failed"))
	*/
}