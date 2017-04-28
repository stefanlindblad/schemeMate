#include "schemeMate_reader.h"

static void alloc_buffer(buffer *b, int initSize)
{
    b->memory = malloc(initSize);
    b->size = initSize;
    b->filled = 0;
}

static void grow_buffer(buffer *b)
{
    int newSize = b->size * 2;
    b->memory = realloc(b->memory, newSize);
    b->size = newSize;
}

static void put_buffer(buffer *b, sm_char ch)
{
    if ((b->filled+1) == b->size) {
		grow_buffer(b);
    }
    b->memory[b->filled++] = (char) ch;
    b->memory[b->filled] = '\0';
}

long long a2l(char* cp)
{ 
	long long val = 0;
	char c;
	while ((c = *cp++) != '\0')
		val = val * 10 + (c - '0');
	return val;
}

static sm_char skipSeparators(sm_stream inStream)
{
	sm_char nextChar;
	do {
		nextChar = readCharacter(inStream);
		if (nextChar == EOF_CHAR) {
			return EOF_CHAR;
		}
	} while (isSeparator(nextChar));
	return nextChar;
}

static sm_bool isSeparator(sm_char c)
{
	switch (c)
	{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			return 1;
		default:
			return 0;
	}
}

static sm_bool all_digits(char* cp)
{
	char c;

	if(*cp == '\0')
		return 0;

	while ((c = *cp++) != '\0') {
		if ((c < '0') || (c > '9'))
			return 0;
	}
    return 1;
}

static sm_char readCharacter(sm_stream inStream)
{
	switch (inStream->type)
	{
		case STRING_STREAM:
		{
			int i = inStream->index;
			sm_char nextChar = inStream->theString[i++];
			if (nextChar == '\0') {
				return EOF_CHAR;
			}
			inStream->index = i;
			return nextChar;
		}
		case FILE_STREAM:
		{
			sm_char nextChar;
			if (inStream->peek != 0) {
		    	nextChar = inStream->peek;
		    	inStream->peek = 0;
		    	return nextChar;
			}
			nextChar = fgetc(inStream->fileStream);
			if (nextChar < 0)
				return EOF_CHAR;
			return nextChar;
		}
		default:
			ERROR("UNKOWN STREAM TYPE!");

		// Should not end here, pure compiler satisfaction
		return 0;
	}
}

static void unreadCharacter(sm_stream inStream, sm_char char_to_unread)
{
	switch (inStream->type)
	{
		case STRING_STREAM:
		{
			inStream->index--;
			return;
		}
		case FILE_STREAM:
		{
			ASSERT(inStream->peek == 0, "CANNOT UNREAD FIRST CHARACTER");
			inStream->peek = char_to_unread;
			return;
		}
	default:
		ERROR("UNKOWN STREAM TYPE!");
	}
}

static sm_obj sm_readList(sm_stream inStream)
{
	sm_char nextChar;
	sm_obj car;
	sm_obj cdr;
	nextChar = skipSeparators(inStream);
	if (nextChar == EOF_CHAR) {
		ERROR("Opening bracket ( is missing closing bracket )!");
		return new_eof();
	}
	if (nextChar == ')')
	{
		return new_nil();
	}
	unreadCharacter(inStream, nextChar);
	car = sm_read(inStream);
	cdr = sm_readList(inStream);
	return new_cons(car, cdr);
}

sm_obj sm_readString(sm_stream inStream)
{
	buffer b;
    sm_char nextChar;
    char *string;
    alloc_buffer(&b, INIT_BUFFER_SIZE);
    for (;;) {
		nextChar = readCharacter(inStream);

		if (nextChar == EOF_CHAR) {
			ERROR("Unterminated String!");
			return new_eof();
		}
		if (nextChar == '"') {
			string = b.memory;
			return new_string(string);
		}
		if (nextChar == '\\') {
	    	sm_char escapedChar;
	    	escapedChar = readCharacter(inStream);
	    	switch (escapedChar)
	    	{
				case 'n':
		    		nextChar = '\n';
		    		break;
				default:
		    		break;
	    	}
		}
		put_buffer(&b, nextChar);
    }
}

sm_obj sm_readCString(char* input) {
    return sm_read(new_string_stream(input));
}

sm_obj sm_read(sm_stream inStream)
{
	buffer b;
    sm_char nextChar;
    char *input;
    alloc_buffer(&b, INIT_BUFFER_SIZE);
    nextChar = skipSeparators(inStream);
    if (nextChar == EOF_CHAR) {
		return new_eof();
    }
    if (nextChar == '(') {
		sm_obj result = sm_readList(inStream);
		if (result->sm_any.tag == TAG_EOF) {
			ERROR("Opening bracket ( is missing closing bracket )!");
			return new_eof();
		}
		return result;
	}
    if (nextChar == '"') {
		return sm_readString(inStream);
    }
    if (nextChar == '\'') {
		sm_obj quotedExpr;
		quotedExpr = sm_read(inStream);
		return new_cons(new_symbol("quote"), new_cons(quotedExpr, new_nil()));
    }

    for (;;) {
		if (nextChar == EOF_CHAR)
	    	break;
		if (isSeparator(nextChar) || (nextChar == '(') || (nextChar == ')')) {
	    	break;
		}
		put_buffer(&b, nextChar);
		nextChar = readCharacter(inStream);
    }
    unreadCharacter(inStream, nextChar);

    if (all_digits(b.memory)) {
		long long iVal = a2l(b.memory);
		return new_int(iVal);
    }
    input = b.memory;

    if (input[0] == '-') {
		if (all_digits(input+1)) {
	    	long iVal = a2l(input+1);
	    	return new_int(-iVal);
		}
    }

    if (input[0] == '#') {
		if (strcmp(input, "#t") == 0)
	    	return new_true();
		if (strcmp(input, "#f") == 0)
	    	return new_false();
		if (strcmp(input, "#void") == 0)
	    	return new_void();
    }

    if (strcmp(input, "nil") == 0) {
		return new_nil();
    }

    return new_symbol(input);
}
