#pragma once

#include "ar_environment.hpp"
#include "ar_writebyte.hpp"
#include <vector>

namespace AFunction_Lib
{
	void enable_afunc(Auroria_Env* A);

	struct AFunction {
		Bytecode data;
		Bytecode args;
		int req_args = 0;
		std::string name;
		bool is_executable = false;
		std::vector<std::string> variable_names;
		std::string return_value = "nil";
		int eac_top = 0;
	};

	struct {
		std::vector<AFunction> stack;
		std::vector<std::string> reserved_names;
	} AFunction_Stack;

	AFunction ar_af_getfunction(Auroria_Env* A, std::string name);

	void ar_af_reserve_name(std::string name);

	void ar_af_setlocalvariables(Auroria_Env* A, AFunction af);
	void ar_af_cleanlocalvariables(Auroria_Env* A, AFunction af);

	void ar_af_reversebytecode(AFunction& af);

	int ar_af_write_function(Auroria_Env* A, int idx, std::string name);
	void ar_af_inject_function(Auroria_Env* A, int idx, AFunction af);

	bool ar_af_isafunction(std::string name);
}