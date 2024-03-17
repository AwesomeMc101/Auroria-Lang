#pragma once
#include "auroria_types.h"
#include "ar_opcodes.h"
#include "ar_error.hpp"

#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <iostream>



const int AR_STACK_NIL = -9403292345;
class Variable_Env {
	std::map<std::string, std::pair<AR_TYPE, std::string>> variables; //IDENTIFIER, <TYPE, VALUE>
public:
	void push_variable(std::string idx, AR_TYPE type, std::string value);

	bool isVariable(std::string idx);
	AR_TYPE ar_v_gettype(std::string idx);
	std::string ar_v_getvalue(std::string idx);
	void reserve_variable(std::string idx);

};

/* Extern libraries */
#define AR_MATH_LIB 9594
#define AR_CFUNC_LIB 5940
#define AR_IF_LIB 9490
#define AR_AFUNC_LIB 9432
#define AR_LOOP_LIB 9767

class Bytecode {
public:
	std::vector<std::string> data;
	std::vector<OpCode> op_set;
	void writeByte(OpCode op, std::string datav);
	void writeByteLoc(OpCode op, std::string datav, size_t loc);
	void deleteByte(size_t loc);
};


#define CENV_AFUNCTION 500
#define CENV_LOOP 600

class Auroria_Env //main environment
{
	//stack
public:
	Variable_Env* variables;
	std::vector<std::string> stack;
	std::vector<int> libraries;
	Bytecode* bytes;
	std::vector<Error> error_list;

	size_t nested_cenv = 0;
	std::vector<std::string> cenv_access_vector{};

	int lineinfo = 0;
	int cfunction_return = 0;

	std::vector<bool> if_results{};

	std::string primed_af_return = "nil";
	
	std::vector<char> AR_RESERVED_TOKENS = { '(',')','{','}',',','='};
	std::vector<std::string> AR_RESERVED_KEYS = {};

	void enable_lib(int lib);
	bool check_lib(int lib);
	//bool check_lib_token(int lib, char token);
	

};

Auroria_Env* ar_open();
void ar_pushstack(Auroria_Env* A, std::string value);
std::string ar_popstack(Auroria_Env* A);
size_t ar_stacksize(Auroria_Env* A);
void ar_sys_inclineinfo(Auroria_Env* A);
int ar_sys_getlineinfo(Auroria_Env* A);
void ar_e_pusherror(Auroria_Env* A, Error e);
void ar_setcfunctionreturn(Auroria_Env* A, int value);
void ar_resetcfunctionreturn(Auroria_Env* A);
int ar_getcfunctionreturn(Auroria_Env* A);

void ar_ce_pushaccess(Auroria_Env* A, std::string type);
std::string ar_ce_popaccess(Auroria_Env* A);
bool ar_ce_checkclose(Auroria_Env* A, std::string identifier);

bool ar_p_isnumber(Auroria_Env* A, std::string data);

void DBG_PRINT_STACK(Auroria_Env* A);
//void ar_indexvar(Auroria_Env* aE, const char* identifier);