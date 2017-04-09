#include "schemeMate_memory.h"

static sm_obj eof_singleton;
static sm_obj *symbolTable = NULL;
static int numKnownSymbols = 0;
static int symbolTableSize = 0;

void init_memory()
{
	symbolTable = (sm_obj*)malloc(sizeof(sm_obj) * INTIAL_SYMBOLTABLE_SIZE);
	symbolTableSize = INTIAL_SYMBOLTABLE_SIZE;
}

// utility functions to create the object types

sm_obj new_int(int value)
{
	sm_obj o = (sm_obj) malloc(sizeof(struct sm_int));
	o->sm_int.tag = TAG_INT;
	o->sm_int.iVal = value;
	return o;
}

sm_obj new_symbol(char* chars)
{
	sm_obj object = really_new_symbol(chars);
	
	return object;
}

sm_obj new_eof()
{
	if (eof_singleton == NULL)
	{
		sm_obj o = (sm_obj)malloc(sizeof(struct scm_integer));
		o->scm_integer.tag = TAG_EOF;
		eof_singleton = o;
	}
	return eof_singleton;
}

// singleton structure für die verschiedenen special typen mit array für alle singletons

sm_obj new_nil()
{
	sm_obj o = create_singleton(TAG_NIL);
	return o;
}

sm_obj new_false()
{
	sm_obj o = create_singleton(TAG_FALSE);
}

sm_obj new_true()
{
	sm_obj o = create_singleton(TAG_TRUE);
}

sm_obj new_cons(sm_obj car, sm_obj cdr)
{
	sm_obj o = (sm_obj)malloc(sizeof(struct scm_cons));
	o->scm_cons.tag = TAG_CONS;
	o->scm_cons.car = car;
	o->scm_cons.cdr = cdr;
	return o;
}

static sm_obj create_singleton(sm_tag tag) {
	static sm_obj singleton[TAG_MAX];
	if (singleton[tag] == NULL) {
		sm_obj o = (sm_obj) malloc(sizeof(struct sm_special));
		o->sm_int.tag = tag;
		singleton[tag] = o;
	}
	return singleton[tag];
}

static sm_obj get_symbol_or_null(char* chars)
{
	int i;
	for (i = 0; i < numKnownSymbols; i++)
	{
		sm_obj currentObject = symbolTable[i];
		if (strcmp(currentObject->scm_symbol.chars, chars) == 0)
			return currentObject;
	}
	return NULL;
}

static void remember_symbol(sm_obj obj)
{
	if (numKnownSymbols == numKnownSymbols) {
		int newSize = symbolTableSize * 2;
		symbolTable = (sm_obj*)realloc(symbolTable, sizeof(sm_obj) * newSize);
		symbolTableSize = newSize;
	}
	symbolTable[numKnownSymbols++] = obj;
}

sm_obj really_new_symbol(char* chars)
{
	sm_obj existingSymbol = get_symbol_or_null(chars);
	if (existingSymbol == NULL)
	{
		sm_obj o = (sm_obj)malloc(sizeof(struct scm_symbol) - 1 /* eins zuviel */ + strlen(chars) + 1 /* nullbyte */);
		remember_symbol(o);
		return o;
	}
	// strcopy usw
	//	o->scm_integer.iVal = iVal;
	return existingSymbol;
}

