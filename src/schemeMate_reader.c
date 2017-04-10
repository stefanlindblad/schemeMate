#include "schemeMate_reader.h"

sm_obj sm_read(sm_stream input)
{
	buffer b;
	sm_char nextChar;
	allocBuffer(&b, INIT_BUFFER_SIZE);
	nextChar = skipSeparators(input);
	if (nextChar == EOF_CHAR)
	{
		return new_eof();
	}
	
	
	for (;;) {
		if (isSeparator(nextChar) || nextChar == '(' || nextChar == ')') {
			break;
		}
		putBuffer(&b, nextChar);
		nextChar = readCharacter(input);
	}
	unreadCharacter(input);
	//if (allDigits(b.memory))
	//{
	//	a2i(b.memory)
	//}

	//if(inputToken[0] == '#')
	//{
	//	if (strcmp(inputToken, "#t") == 0)
	//	{
	//		return new_true();
	//	}
	//	if (strcmp(inputToken, "#f") == 0)
	//	{
	//		return new_false();
	//	}
	//}

	return new_integer(123);
}


long a2l (char* cp)
{ 
	long val = 0;
	char c;
	while (( c = *cp++) != '\0')
		val = val * 10 + (c - '\0');
}

sm_obj scm_readString(char * input)
{
	return scm_read( new_stringStream(input) );
}

static sm_obj scm_readList(SCM_STREAM inStream)
{
	sm_char nextChar;
	sm_obj car;
	sm_obj cdr;
	nextChar = skipSeparators(inStream);
	if (nextChar == ')')
	{
		return new_nil();
	}
	unreadCharacter(inStream);
	car = scm_read(inStream);
	cdr = scm_readList(inStream);
	return new_cons(car, cdr);

	//FATAL("unimplemented");
}



static int skipSeparators(SCM_STREAM inStream)
{
	sm_char nextChar;

	do {
		nextChar = readCharacter(inStream);
		if (nextChar == EOF_CHAR) return EOF_CHAR;
	} while (isSeparator(nextChar));
	return nextChar;
}

static BOOL isSeparator(int aChar)
{
	switch (aChar)
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

static int readCharacter(SCM_STREAM inStream)
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
		sm_char nextChar = fgetc(inStream->fileStream);
		if (inStream->peekChar != 0)
		{
			
		}
		if (nextChar < 0)
			return EOF_CHAR;
		return nextChar;
	}
	default:
		ASSERT(0 == 0, "UNKOWN STREAM TYPE!");
		return 0;
	}
}

static void unreadCharacter(SCM_STREAM inStream, sm_char char_to_unread)
{
	switch (inStream->type)
	{
	case STRING_STREAM:
	{
		inStream->index--;
		return;
	}
	case FILE_STREAM:
		ASSERT(inStream->peekChar == 0, "CANNOT UNREAD FIRST CHARACTER");
		{
		inStream->peekChar = char_to_unread;
		return;

		}
	default:
		ASSERT(0 == 0, "UNKOWN STREAM TYPE!");
	}
}

 
SCM_STREAM new_filestream(FILE* inFile)
{
	scm_stream s = (scm_stream) malloc(sizeof(struct scm_stream_struct));

}

