#include "schemeMate_selftest.h"

void sm_selftest()
{
	sm_obj rslt, sym_abcde1, sym_abcde2, sym_abcde3,
	    sym_bbb1, sym_bbb2, symx1, symx2, l2, l3;

    ASSERT( a2l("0") == 0, "a2l error");
    ASSERT( a2l("1") == 1, "a2l error");
    ASSERT( a2l("123456") == 123456, "a2l error");
    ASSERT( a2l("999999") == 999999, "a2l error");
    ASSERT( a2l("2432902008176640000") == 2432902008176640000, "a2l error");

    rslt = sm_readCString("");
    ASSERT(rslt != NULL, "empty read check failed (NULL result)")
    ASSERT(rslt->sm_any.tag == TAG_EOF, "empty read check failed")

    rslt = sm_readCString("    ");
    ASSERT(rslt != NULL, "empty read check failed (NULL result)")
    ASSERT(rslt->sm_any.tag == TAG_EOF, "empty read check failed")

    rslt = sm_readCString("123");
    ASSERT(rslt != NULL, "int check failed (NULL result)")
    ASSERT(rslt->sm_any.tag == TAG_INT, "int tag check failed")
    ASSERT(rslt->sm_int.iVal == 123, "int check failed")

    rslt = sm_readCString("        123");
    ASSERT(rslt->sm_any.tag == TAG_INT, "int tag check failed")
    ASSERT(rslt->sm_int.iVal == 123, "int check failed")

    rslt = sm_readCString("999");
    ASSERT(rslt->sm_any.tag == TAG_INT, "int tag check failed")
    ASSERT(rslt->sm_int.iVal == 999, "int check failed")

    rslt = sm_readCString("99999999");
    ASSERT(rslt->sm_any.tag == TAG_INT, "int tag check failed")
    ASSERT(rslt->sm_int.iVal == 99999999, "int check failed")

    rslt = sm_readCString("-99999999");
    ASSERT(rslt->sm_any.tag == TAG_INT, "int tag check failed")
    ASSERT(rslt->sm_int.iVal == -99999999, "int check failed")

    rslt = sm_readCString(" #t ");
    ASSERT(rslt->sm_any.tag == TAG_TRUE, "true tag check failed")

    rslt = sm_readCString(" #f ");
    ASSERT(rslt->sm_any.tag == TAG_FALSE, "false tag check failed")

    rslt = sm_readCString(" nil ");
    ASSERT(rslt->sm_any.tag == TAG_NIL, "nil tag check failed")

    rslt = sm_readCString(" abc ");
    ASSERT(rslt->sm_any.tag == TAG_SYMBOL, "symbol tag check failed")
    ASSERT(strcmp(rslt->sm_symbol.chars, "abc")==0, "symbol check failed")

    rslt = sm_readCString(" if ");
    ASSERT(rslt->sm_any.tag == TAG_SYMBOL, "symbol tag check failed")
    ASSERT(strcmp(rslt->sm_symbol.chars, "if")==0, "symbol check failed")

    rslt = sm_readCString(" define ");
    ASSERT(rslt->sm_any.tag == TAG_SYMBOL, "symbol tag check failed")
    ASSERT(strcmp(rslt->sm_symbol.chars, "define")==0, "symbol check failed")

    rslt = sm_readCString(" 1+ ");
    ASSERT(rslt->sm_any.tag == TAG_SYMBOL, "symbol tag check failed")
    ASSERT(strcmp(rslt->sm_symbol.chars, "1+")==0, "symbol check failed")

    rslt = sm_readCString(" --->---<--- ");
    ASSERT(rslt->sm_any.tag == TAG_SYMBOL, "symbol tag check failed")
    ASSERT(strcmp(rslt->sm_symbol.chars, "--->---<---")==0, "symbol check failed")

    sym_abcde1 = sm_readCString("abcde");
    sym_abcde2 = sm_readCString("abcde");
    ASSERT(sym_abcde1 == sym_abcde2, "symbol identity check failed")

    sym_bbb1 = sm_readCString("   bbb ");
    sym_bbb2 = sm_readCString("bbb ");
    ASSERT(sym_bbb1 == sym_bbb2, "symbol identity check failed")

    sym_abcde3 = sm_readCString("abcde    ");
    ASSERT(sym_abcde3 == sym_abcde1, "symbol identity check failed")

    symx1 = sm_readCString("abcde ");
    symx2 = sm_readCString(" bbb ");
    ASSERT(symx1 != symx2, "symbol identity check failed")

    rslt = sm_readCString("()");
    ASSERT(rslt == sm_nil(), "empty list read check failed")
    ASSERT(get_tag(rslt) == TAG_NIL, "empty list read check failed")
    ASSERT(has_tag(rslt, TAG_NIL), "empty list read check failed")

    rslt = sm_readCString(" (123) ");
    ASSERT(get_tag(rslt) == TAG_CONS, "empty list read check failed")
    ASSERT(get_tag(rslt->sm_cons.car) == TAG_INT, "car list read check failed")
    ASSERT(get_tag(rslt->sm_cons.cdr) == TAG_NIL, "cdr list read check failed")
    ASSERT(int_val(car(rslt)) == 123, "inval list read check failed")

    rslt = sm_readCString(" (a b c) ");
    ASSERT(get_tag(rslt) == TAG_CONS, "empty list read check failed")
    ASSERT(is_symbol(car(rslt)), "car list read check failed")
    ASSERT(is_cons(cdr(rslt)), "car list read check failed")
    ASSERT(is_symbol(car(cdr(rslt))), "car list read check failed")
    ASSERT(is_cons(cdr(cdr(rslt))), "car list read check failed")
    ASSERT(is_symbol(car(cdr(cdr(rslt)))), "car list read check failed")
    ASSERT(is_nil(cdr(cdr(cdr(rslt)))), "car list read check failed")

    rslt = sm_readCString(" (a (b (c))) ");
    ASSERT(get_tag(rslt) == TAG_CONS, "empty list read check failed")
    ASSERT(is_symbol(car(rslt)), "car list read check failed")
    ASSERT(is_cons(cdr(rslt)), " read check failed")
    ASSERT(is_nil(cdr(cdr(rslt))), " read check failed")
    l2 = car(cdr(rslt));
    ASSERT(is_symbol(car(l2)), "read check failed")
    ASSERT(is_cons(cdr(l2)), " read check failed")
    l3 = car(cdr(l2));
    ASSERT(is_symbol(car(l3)), "read check failed")

    add_binding(new_symbol("a"), new_int(120), MAIN_ENV);
    add_binding(new_symbol("b"), new_int(200), MAIN_ENV);

    rslt = get_binding(new_symbol("a"), MAIN_ENV);
    ASSERT(is_int(rslt), "get_binding does not work as expected");
    ASSERT(int_val(rslt) == 120, "get_binding does not work as expected");

    rslt = get_binding(new_symbol("b"), MAIN_ENV);
    ASSERT(is_int(rslt), "get_binding does not work as expected");
    ASSERT(int_val(rslt) == 200, "get_binding does not work as expected");

    rslt = get_binding(new_symbol("c"), MAIN_ENV);
    ASSERT(rslt == NULL, "get_binding does not work as expected");

    PRINT("Successfully finished the Interpreter Selftest.");
}