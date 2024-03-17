#pragma once

/* OPSET includes all library opcodes */
typedef enum {
	OP_MOVE, //VAR, VAR
	OP_LOADVAL, //VAR, VALUE
	OP_LOADNIL, //VAR, nil
	OP_VAR,

	OP_ADD, //NUM, NUM
	OP_SUB,//NUM, NUM
	OP_MUL, //NUM, NUM
	OP_DIV, //NUM, NUM
	OP_MOD, //NUM, NUM
	OP_POW, //NUM, NUM

	//string
	OP_CONCAT, //STR, STR
	OP_STRSUB, //STR, NUM

	OP_BOOL,
	OP_FLOAT,
	OP_STRING,
	OP_INTEGER,
	OP_NIL,
	OP_CHAR,

	OP_GETARG,

	OP_CALL, //FUNCTION
	OP_TAILCALL, //FUNCTION POSTCHECK
	OP_PRECALL, //FUNCTION PRECHECK

	OP_PUSH, //VALUE
	OP_POP, //NOTHING
	OP_RETURN, //VALUE

	OP_BAND, //bitwise and
	OP_BOR, //bitwise or
	OP_BGT, //bitwise greater than
	OP_BLT, //bitwise less than
	OP_BEQ, //bitwise equal to =
	OP_BDEQ, //bitwise equal to (comp) ==
	OP_BNEQ, //bitwise not equal to
	OP_BGTEQ, //bitwise greater than or equal to
	OP_BLTEQ, //bitwise less than or equal to
	OP_BNOT, //bitwise NOT !

	OP_FORLOOP, //NUM, VAR
	OP_PRELOOP,
	OP_TAILLOOP,


	OP_IF,
	OP_ELSE,

	OP_SYSLINEINFO,

	OP_CCALL, //CFUNCTION

	OP_ENV, //new env {}
	OP_CENV, //close env

	OP_NEWFUNC,

	OP_ARGSET, //new argset (
	OP_CARGS
} OpCode;