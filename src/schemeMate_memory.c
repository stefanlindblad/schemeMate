#include "schemeMate_memory.h"

void init_memory(int running_mode)
{
	symbolTable = (sm_obj*) malloc(sizeof(sm_obj) * INITIAL_SYMBOLTABLE_SIZE);
	symbolTableSize = INITIAL_SYMBOLTABLE_SIZE;
	numKnownSymbols = 0;
    memset(symbolTable, 0, (sizeof(sm_obj) * INITIAL_SYMBOLTABLE_SIZE));
}

void shutdown_memory()
{
	free(symbolTable);
	symbolTable = NULL;
	symbolTableSize = INITIAL_SYMBOLTABLE_SIZE;
	numKnownSymbols = 0;
}

// utility functions to create the object types

sm_obj new_int(int value)
{
	sm_obj o = (sm_obj) malloc(sizeof(struct sm_int_type));
	o->sm_int.tag = TAG_INT;
	o->sm_int.iVal = value;
	return o;
}

sm_obj new_symbol(char* chars)
{
	sm_obj existingSymbol = get_symbol_or_null(chars);

    if (existingSymbol == NULL) {
		sm_obj newSymbol = really_new_symbol(chars);
		remember_symbol(newSymbol);
		return newSymbol;
    }
    return existingSymbol;
}

sm_obj new_cons(sm_obj car, sm_obj cdr)
{
	sm_obj o = (sm_obj)malloc(sizeof(struct sm_cons_type));
	o->sm_cons.tag = TAG_CONS;
	o->sm_cons.car = car;
	o->sm_cons.cdr = cdr;
	return o;
}

sm_obj new_string(char* chars) {
    unsigned int length = (unsigned int) strlen(chars);
	unsigned int bytes = (unsigned int) sizeof(struct sm_string_type)
		- 1 /* eins zuviel */ + length + 1 /* nullbyte */;
	sm_obj o = (sm_obj) malloc(bytes);
	o->sm_string.tag = TAG_STRING;
	strcpy(o->sm_string.string, chars);
	o->sm_string.string[length] = '\0';
	return o;
}

sm_obj sm_eof()
{
	sm_obj o = create_singleton(TAG_EOF);
	return o;
}

sm_obj sm_nil()
{
	sm_obj o = create_singleton(TAG_NIL);
	return o;
}

sm_obj sm_false()
{
	sm_obj o = create_singleton(TAG_FALSE);
	return o;
}

sm_obj sm_true()
{
	sm_obj o = create_singleton(TAG_TRUE);
	return o;
}

sm_obj sm_void()
{
	sm_obj o = create_singleton(TAG_VOID);
	return o;
}

sm_obj new_user_func(sm_obj args, sm_obj body)
{
    sm_obj o = (sm_obj) malloc(sizeof(struct sm_user_func_type));
    o->sm_user_func.tag = TAG_USER_FUNC;
    o->sm_user_func.args = args;
    o->sm_user_func.body = body;
    return o;
}

sm_obj new_sys_func(sm_func funcPtr, char *name)
{
	sm_obj o = (sm_obj) malloc(sizeof(struct sm_sys_func_type));
	o->sm_sys_func.tag = TAG_SYS_FUNC;
	o->sm_sys_func.code = funcPtr;
	o->sm_sys_func.name = name;
	return o;
}

sm_obj new_sys_syntax(sm_func syntaxPtr, char *name)
{
	sm_obj o = (sm_obj) malloc(sizeof(struct sm_sys_syntax_type));
	o->sm_sys_syntax.tag = TAG_SYS_SYNTAX;
	o->sm_sys_syntax.code = syntaxPtr;
	o->sm_sys_syntax.name = name;
	return o;
}

sm_obj really_new_symbol(char* chars)
{
	unsigned int length = (unsigned int) strlen(chars);
	unsigned int bytes = (unsigned int) sizeof(struct sm_symbol_type)
		- 1 /* eins zuviel */ + length + 1 /* nullbyte */;
	sm_obj o = (sm_obj) malloc(bytes);
	o->sm_symbol.tag = TAG_SYMBOL;
	strcpy(o->sm_symbol.chars, chars);
	o->sm_symbol.chars[length] = '\0';
	return o;
}

// STATIC FUNCTIONS
sm_stream new_string_stream(char* string)
{
	sm_stream s = (sm_stream) malloc(sizeof(struct sm_stream_type));
	s->type = STRING_STREAM;
	s->string = string;
	s->index = 0;
	return s;
}

sm_stream new_file_stream(FILE* file) {
	sm_stream s = (sm_stream) malloc(sizeof(struct sm_stream_type));
	s->type = FILE_STREAM;
	s->file = file;
	s->peek = 0;
	return s;
}

static sm_obj create_singleton(sm_tag tag)
{
	static sm_obj singleton[TAG_MAX];
	if (singleton[tag] == NULL) {
		sm_obj o = (sm_obj) malloc(sizeof(struct sm_special_type));
		o->sm_int.tag = tag;
		singleton[tag] = o;
	}
	return singleton[tag];
}

static sm_obj get_symbol_or_null(char* chars)
{
	int hash_id;
	int original_id;
    hash_id = str_hash(chars) % symbolTableSize;
	original_id = hash_id;

    while (true) {
		sm_obj symbol = symbolTable[hash_id];
		if (symbol == NULL)
	    	return NULL;
		if (strcmp(symbol->sm_symbol.chars, chars) == 0)
	    	return symbol;
		hash_id++;
		if (hash_id == symbolTableSize)
	    	hash_id = 0;
		if (hash_id == original_id)
	    	return NULL;
    }
}

static void remember_symbol(sm_obj obj)
{
	ASSERT_SYMBOL(obj);

	int hash_id;
	int original_id;
    hash_id = str_hash(obj->sm_symbol.chars) % symbolTableSize;
	original_id = hash_id;

    while (true) {
		sm_obj slotValue = symbolTable[hash_id];
		if (slotValue == NULL) {
	    	symbolTable[hash_id] = obj;
	    	numKnownSymbols++;
	    	check_table_size();
	    	return;
		}
		hash_id++;
		if (hash_id == symbolTableSize)
	    	hash_id = 0;
		if (hash_id == original_id)
			ERROR_CODE("The symbol table is full.", 49);
    }

	if (numKnownSymbols == symbolTableSize) {
		if (symbolTableSize == 0) {
	    	symbolTable = (sm_obj*) malloc(sizeof(sm_obj) * INITIAL_SYMBOLTABLE_SIZE);
	    	symbolTableSize = INITIAL_SYMBOLTABLE_SIZE;
		} else {
	    	int newSize = symbolTableSize * 2;
	    	symbolTable = (sm_obj*) realloc(symbolTable, sizeof(sm_obj) * newSize);
	    	symbolTableSize = newSize;
		}
	}
	symbolTable[numKnownSymbols++] = obj;
}

static void grow_symbol_table()
{
    int newSize = (symbolTableSize * 2) + 1;
    sm_obj *newTable = (sm_obj *)malloc(sizeof(sm_obj) * newSize);
    int i;
    memset(newTable, 0, (sizeof(sm_obj) * newSize));
    for (i = 0; i < symbolTableSize; i++) {
		sm_obj slotValue = symbolTable[i];
		if (slotValue != NULL) {
	    	int hash_id;
			int original_id;
	    	hash_id = str_hash(slotValue->sm_symbol.chars) % newSize;
			original_id = hash_id;
	    	while (true) {
				if (newTable[hash_id] == NULL) {
			    	newTable[hash_id] = slotValue;
			    	break;
				}
				hash_id++;
				if (hash_id == newSize)
		    		hash_id = 0;
				if (hash_id == original_id)
					ERROR_CODE("The new symbol table is full.", 49);
	    	}
		}
    }
    free(symbolTable);
    symbolTable = newTable;
    symbolTableSize = newSize;
}

// Grow the table when its more then 75% full
static void check_table_size()
{
    if (numKnownSymbols > (symbolTableSize * 3 / 4)) {
		grow_symbol_table();
    }
}

