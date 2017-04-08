#include "schemeMate_memory.h"

static SCM_OBJ eof_singleton;
static SCM_OBJ *symbolTable = NULL;
static int numKnownSymbols = 0;
static int symbolTableSize = 0;

void init_system()
{
	symbolTable = (SCM_OBJ*)malloc(sizeof(SCM_OBJ) * INTIAL_SYMBOLTABLE_SIZE);
	symbolTableSize = INTIAL_SYMBOLTABLE_SIZE;
}

void deinit_system()
{
	// TODO: dealloc symbol table
}

SCM_OBJ new_integer(int iVal)
{
	SCM_OBJ o = (SCM_OBJ)malloc(sizeof(struct scm_integer));
	o->scm_integer.tag = TAG_INT;
	o->scm_integer.iVal = iVal;
	return o;
}

static SCM_OBJ get_symbol_or_null(char* chars)
{
	int i;
	for (i = 0; i < numKnownSymbols; i++)
	{
		SCM_OBJ currentObject = symbolTable[i];
		if (strcmp(currentObject->scm_symbol.chars, chars) == 0)
			return currentObject;
	}
	return NULL;
}

static void remember_symbol(SCM_OBJ obj)
{
	if (numKnownSymbols == numKnownSymbols) {
		int newSize = symbolTableSize * 2;
		symbolTable = (SCM_OBJ*)realloc(symbolTable, sizeof(SCM_OBJ) * newSize);
		symbolTableSize = newSize;
	}
	symbolTable[numKnownSymbols++] = obj;
}

SCM_OBJ really_new_symbol(char* chars)
{
	SCM_OBJ existingSymbol = get_symbol_or_null(chars);
	if (existingSymbol == NULL)
	{
		SCM_OBJ o = (SCM_OBJ)malloc(sizeof(struct scm_symbol) - 1 /* eins zuviel */ + strlen(chars) + 1 /* nullbyte */);
		remember_symbol(o);
		return o;
	}
	// strcopy usw
	//	o->scm_integer.iVal = iVal;
	return existingSymbol;
}

SCM_OBJ new_symbol(char* chars)
{
	really_new_symbol(chars);
	SCM_OBJ o = (SCM_OBJ)malloc(sizeof(struct scm_symbol) - 1 /* eins zuviel */ + strlen(chars) + 1 /* nullbyte */);
	o->scm_integer.tag = TAG_SYMBOL;


	// strcopy usw
//	o->scm_integer.iVal = iVal;
	return o;
}



SCM_OBJ new_EOF()
{
	if (eof_singleton == NULL)
	{
		SCM_OBJ o = (SCM_OBJ)malloc(sizeof(struct scm_integer));
		o->scm_integer.tag = TAG_EOF;
		eof_singleton = o;
	}
	return eof_singleton;
}

// singleton structure für die verschiedenen special typen mit array für alle singletons

SCM_OBJ new_nil()
{
	SCM_OBJ o = (SCM_OBJ)malloc(sizeof(struct scm_integer));
	o->scm_integer.tag = TAG_NIL;
	return o;
}

SCM_OBJ new_false()
{
	SCM_OBJ o = (SCM_OBJ)malloc(sizeof(struct scm_integer));
	o->scm_integer.tag = TAG_FALSE;
	return o;
}

SCM_OBJ new_cons(SCM_OBJ car, SCM_OBJ cdr)
{
	SCM_OBJ o = (SCM_OBJ)malloc(sizeof(struct scm_cons));
	o->scm_cons.tag = TAG_CONS;
	o->scm_cons.car = car;
	o->scm_cons.cdr = cdr;
	return o;
}

SCM_OBJ new_true()
{
	SCM_OBJ o = (SCM_OBJ)malloc(sizeof(struct scm_integer));
	o->scm_integer.tag = TAG_TRUE;
	return o;
}