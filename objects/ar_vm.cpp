#include "ar_vm.hpp"

bool Callstack::check_callstack(Auroria_Env* A, size_t stackSize)	//on OP_PUSH
{
	if (callStack.size())
	{
		Call ci = callStack.back();
		//std::cout << stackSize << " compared to required " << ci.args;
		//return (ci.args <= stackSize);
		//if (ci.args <= stackSize) { 
		if(stackSize >= ci.args){
			if (stackSize > ci.args) { 
				ar_e_pusherror(A, ar_e_newerror(ar_sys_getlineinfo(A), WERROR_9405)); 
			} 
			return true;  //enough args to complete function
		}
	}
	ar_e_pusherror(A, ar_e_newerror(ar_sys_getlineinfo(A), ERROR_9403));
	return false;
}
bool Callstack::push_callclosure(Call closure)
{
	//std::cout << "\nNEWCALL CLOSURE: " << closure.data << "\n";
	
	if ((!closure.data.empty())) {
		//std::cout << "\nclosure " << closure.data << " pushed. ARGS: " << closure.args << "\n";
		callStack.push_back(closure);
		return true;
	}
	return false;
}

bool Callstack::erase_callclosure(std::string name)
{
	for (int i = 0; i < callStack.size(); i++)
	{
		Call closure = callStack[i];
		if (closure.data == name)
		{
			callStack.erase(callStack.begin() + i);
			return true;
		}
			
	}
	return false;
}



void Callstack::new_environment()
{
	//printf("\n\n\n NEW ENVIRONMENT \n\n\n");
	env_arg_counts.push_back(0);
}

void Callstack::dbg_printcallstack()
{
	for (Call ci : callStack)
	{
		//std::cout << "\nCALL " << ci.data << "\n";
	}
}

unsigned int Callstack::eac_top()
{
	size_t size = env_arg_counts.size();
	if (!size)
	{
		env_arg_counts.push_back(0);
		return 0;
	}

	return (size - 1);
}

size_t Callstack::get_eac_top_value()
{
	return env_arg_counts[eac_top()];
}

size_t Callstack::get_eac_value(int sub_idx)
{
	size_t top = eac_top();
	//std::cout << "EAX TOP: " << top << "\n";
	if(sub_idx > top) {return env_arg_counts[top]; }

//	std::cout << "\nEAC RETURN: " << env_arg_counts[top - sub_idx];


	return env_arg_counts[top - sub_idx];
}

size_t Callstack::get_stack_size()
{
	return callStack.size();
}

Call Callstack::pop_callstack()
{
	if (!callStack.size()) { return *(new Call); }
	Call ci = callStack.back();
	callStack.pop_back();
	return ci;
}


bool ar_domathcall(Auroria_Env* A, Callstack* cs)
{
	if (cs->check_callstack(A, cs->env_arg_counts[cs->eac_top()]))
	{
		Call ci = cs->pop_callstack();
		switch (ci.OpType)
		{
		case OP_CONCAT:
			Mathematic::ar_math_concat(A);
			return true;
		case OP_STRSUB:
			Mathematic::ar_math_strsub(A);
			return true;
		case OP_ADD:
			Mathematic::ar_math_add(A);
			return true;
		case OP_SUB:
			Mathematic::ar_math_sub(A);
			return true;
		case OP_MUL:
			Mathematic::ar_math_mul(A);
			return true;
		case OP_DIV:
			Mathematic::ar_math_div(A);
			return true;
		case OP_MOD:
			Mathematic::ar_math_mod(A);
			return true;
		case OP_POW:
			Mathematic::ar_math_pow(A);
			return true;
		case OP_LOADVAL:
		{
			std::string top = ar_popstack(A); //value
			//std::string stop = ar_popstack(A); //name
			std::string var_name = ci.data;
			A->variables->push_variable(var_name, 0, top);
			//std::cout << "mathloadval set " << var_name << " to " << top;
			return true;
		}
		case OP_BLT: //<
		case OP_BLTEQ: //<=
		case OP_BGT: //>
		case OP_BGTEQ: //>=
		case OP_BDEQ: //==
		case OP_BNEQ: //!=
		case OP_BNOT: //!
		{
			ar_if_comparisonclosure(A, ci.OpType);
			return true;
		}
		}
		cs->push_callclosure(ci); //REPUSH CLOSURE IF IT ISNT A MATH CALL (WOULD'VE RETURNED IF MATH)
	}
	return false;
}

bool ar_docaller(Auroria_Env* A, Callstack* cs, int& i)
{
	bool valid = cs->check_callstack(A, cs->env_arg_counts[cs->eac_top()]);
	if (!valid) { return false; }
	if (valid == true);
	{
		
		Call ci = cs->pop_callstack();
		//printf("\nCALL VALID FOR %s", ci.data);
		//std::cout << "\nCALL VALID FOR " << ci.data << "\n";
		switch (ci.OpType)
		{
		case OP_CCALL:
			CFunction::ar_cf_runcfunction(A, ci.data);
			break;
		case OP_CALL:
			//AFunction_Lib::inject_afunction fr
			AFunction_Lib::ar_af_inject_function(A, i, AFunction_Lib::ar_af_getfunction(A, ci.data));
			return false; //STOP OP_CARGS FROM CLOSING OUR ENVIRONMENT AND CUTTING OFF OUR INPUT VALUES.
			break;
		case OP_FORLOOP:
			//printf("generate new loop");
			LoopLibrary::new_loop(A, i);
			break;
		case OP_CONCAT:
			Mathematic::ar_math_concat(A);
			break;
		case OP_STRSUB:
			Mathematic::ar_math_strsub(A);
			break;
		case OP_ADD:
			Mathematic::ar_math_add(A);
			break;
		case OP_SUB:
			Mathematic::ar_math_sub(A);
			break;
		case OP_MUL:
			Mathematic::ar_math_mul(A);
			break;
		case OP_DIV:
			Mathematic::ar_math_div(A);
			break;
		case OP_MOD:
			Mathematic::ar_math_mod(A);
			break;
		case OP_POW:
			Mathematic::ar_math_pow(A);
			break;
		case OP_LOADVAL:
		{
			std::string top = ar_popstack(A); //value
			//std::string stop = ar_popstack(A); //name
			std::string var_name = ci.data;
			A->variables->push_variable(var_name, 0, top);
			std::cout << "loadval set " << var_name << " to " << top;
			break;
		}
		case OP_BLT: //<
		case OP_BLTEQ: //<=
		case OP_BGT: //>
		case OP_BGTEQ: //>=
		case OP_BDEQ: //==
		case OP_BNEQ: //!=
		case OP_BNOT: //!
		{
			ar_if_comparisonclosure(A, ci.OpType);
			break;
		}
		case OP_IF:
		{
			ar_if_execute(A, i);
			break;
		}
		case OP_ELSE:
		{
			ar_else_execute(A, i);
			break;
		}
		}
		return true;
	}
	printf("\nCALLFAIL\n");
	return false;
}

void ar_m_execute(Auroria_Env* A)
{
	Callstack* cs = new Callstack;

	for (int i = 0; i < A->bytes->op_set.size(); i++)
	{
		OpCode op = A->bytes->op_set[i];
		std::string data = A->bytes->data[i];

		//std::cout << "\n[" << i << "]" << " Instr: " << op << " && Data: " << data << "\n";

		switch (op)
		{
		case OP_CCALL: {
			/* this can only happen if CF Lib is enabled. */
			Call new_ci;
			new_ci.args = CFunction::ar_cf_getargs(data);
			new_ci.data = data;
			new_ci.OpType = op;
			cs->push_callclosure(new_ci);

			break;
		}
		case OP_CALL: {
			Call new_ci;
			//AFunction_Lib::AFunction af = AFunction_Lib::ar_af_getfunction(A, data);
			new_ci.args = 0; //arg check redone by OP_PRECALL
			new_ci.data = data;
			new_ci.OpType = op;
			cs->push_callclosure(new_ci);
			cs->new_environment(); //done anyway by argset ://
			ar_ce_pushaccess(A, data);

			break;
		}
		case OP_IF:
		{
			Call new_ci;
			new_ci.args = 1;
			new_ci.data = data;
			new_ci.OpType = op;
			cs->push_callclosure(new_ci);

			break;
		}
		case OP_ELSE:
		{
			Call new_ci;
			new_ci.args = 0;
			new_ci.data = data;
			new_ci.OpType = op;
			cs->push_callclosure(new_ci);

			break;
		}
		case OP_PUSH:
		{
			cs->env_arg_counts[cs->eac_top()] += 1;
			//A->stack.push_back(data);
			ar_pushstack(A, data);
			DBG_PRINT_STACK(A);
			
			break;
		}
		case OP_ARGSET:
		{
			//cs->env_arg_counts.push_back(0);
			cs->new_environment();
			break;
		}
		case OP_PRELOOP:
		{
			LoopLibrary::LoopData* LD = LoopLibrary::get_loop_by_idx(A, std::stoi(data)); //its not possible to have a non-artificial preloop, so data is always int
			//ar_ce_pushaccess(A, data);
			switch (LD->iteration_type) {
			case 0:
				A->variables->push_variable(LD->variable_index_title, 0, "0");
				break;
			case 1://string iteration
			{
				std::string t(1, LD->type_1_iterator[0]);
				A->variables->push_variable(LD->variable_index_title, 1, t);
				break;
			}
			}
			break;
		}
		case OP_ENV:
			//A->nested_cenv++;
			break;
		case OP_CENV:
		{
			
			break;
		}
		case OP_TAILLOOP:
		{
			//std::cout << "Tailloop";
			LoopLibrary::LoopData* LD = LoopLibrary::get_loop_by_idx(A, std::stoi(data));
			LD->performed_loops++;
			switch (LD->iteration_type) {
			case 0:
				A->variables->push_variable(LD->variable_index_title, 0, std::to_string(LD->performed_loops));
				break;
			case 1:
			{
				std::string t(1, LD->type_1_iterator[LD->performed_loops]);
				A->variables->push_variable(LD->variable_index_title, 0, t);
				break;
			}
			}
			
			//std::cout << "MAX loops: " << LD->max_loops;
			if (LD->performed_loops < LD->max_loops)
			{
				//std::cout << "reset loops";
				//std::cout << "\nvm index: " << LD->vm_index << "\n";
				i = LD->vm_index;
				break;
			}
			LoopLibrary::erase_loop_by_idx(A, std::stoi(data));
			break;
		}
		case OP_CARGS:
		{
			size_t old_stack_size = ar_stacksize(A);
			if (cs->get_stack_size()) {
				if (ar_docaller(A, cs, i)) {
					cs->env_arg_counts.resize(cs->env_arg_counts.size() - 1); //delete most recent space
					cs->env_arg_counts[cs->eac_top()] += ar_getcfunctionreturn(A); //append new value count to new eac top
					//std::cout << "\nInc eac: " << abs((int)(ar_stacksize(A) - old_stack_size)) << "\n";
				}
			}
			break;
		}
		case OP_CONCAT:
		case OP_STRSUB:
		case OP_ADD:
		case OP_SUB:
		case OP_MUL:
		case OP_DIV:
		case OP_POW:
		case OP_MOD: {
			Call* new_ci = new Call;
			new_ci->args = 2; //A + B
			new_ci->data = data;
			new_ci->OpType = op;
			cs->push_callclosure(*new_ci);
		}
			break;
		case OP_SYSLINEINFO:
			ar_sys_inclineinfo(A);
			break;
		case OP_PRECALL:
		{
			AFunction_Lib::AFunction af = AFunction_Lib::ar_af_getfunction(A, data); //precall is passed the function name as data.
			size_t args = cs->get_eac_value(0); //one environment down... the () after function call creates an unwanted env

			printf("precall");
			cs->dbg_printcallstack();

			if (args < af.req_args) //not enough function arguments
			{
			//	std::cout << "precall " << args << " < " << af.req_args << "\n";
				Error e = ar_e_newerror(A->lineinfo, ERROR_3406);
				ar_e_pusherror(A, e);

				//cancel function execution
				while (op != OP_TAILCALL)
				{
					i++;
					op = A->bytes->op_set[i];
					data = A->bytes->data[i];
				}
				break;
			}

			cs->new_environment();
			AFunction_Lib::ar_af_setlocalvariables(A, af);

			break;
		}
		case OP_TAILCALL:
		{
			AFunction_Lib::AFunction af = AFunction_Lib::ar_af_getfunction(A, data); //tailcall's data is function name.
			AFunction_Lib::ar_af_cleanlocalvariables(A, af); //destroy local variables


			cs->env_arg_counts.resize(cs->env_arg_counts.size() - 1); //delete most recent space
			if (A->primed_af_return != "nil")
			{
				cs->env_arg_counts[cs->eac_top()] += 1; //consider just setcfuncreturn
				ar_pushstack(A, A->primed_af_return);
			}
			
			ar_resetcfunctionreturn(A);
			
			for (std::string var_name : af.variable_names) //clean up leaked functions because of OP_LOADVAL w/ arguments
			{
				cs->erase_callclosure(var_name);
			}
		//	ar_docaller(A, cs, i);
			break;
			
		}
		case OP_FORLOOP:
		{
			Call* new_ci = new Call;
			new_ci->args = 2; //amount, index variable
			new_ci->OpType = op;
			new_ci->data = data;
			cs->push_callclosure(*new_ci);
		//	printf("new loop closure");
			break;
		}
			
		case OP_NEWFUNC:
		{
			cs->erase_callclosure(data); //erase the new variable function since its a charset.
			int new_loc = AFunction_Lib::ar_af_write_function(A, i, data);
			i = new_loc;
			break;
		}
		case OP_LOADVAL: //set var
		{
			//printf("loadval");
			Call* new_ci = new Call;
			new_ci->args = 2; //name, value
			new_ci->OpType = op;
			new_ci->data = data;
			cs->push_callclosure(*new_ci);
			break;
		}
		case OP_VAR:
		//	A->stack.push_back(A->variables->ar_v_getvalue(data));
			cs->env_arg_counts[cs->eac_top()] += 1;
			ar_pushstack(A, A->variables->ar_v_getvalue(data));
		//dont break here, let VAR also do a mathcall
		case OP_CHAR:
		case OP_STRING:
		case OP_FLOAT:
		case OP_NIL:
		case OP_BOOL:
		case OP_INTEGER:
			if (ar_domathcall(A, cs)) {
				cs->env_arg_counts.resize(cs->env_arg_counts.size() - 1); //delete most recent space
				cs->env_arg_counts[cs->eac_top()] += 1; //append 1 to new eac top
			}
			break;
		case OP_BLT: //<
		case OP_BLTEQ: //<=
		case OP_BGT: //>
		case OP_BGTEQ: //>=
		case OP_BDEQ: //==
		case OP_BNEQ: //!=
		case OP_BNOT: //!
		{
			Call* new_ci = new Call;
			new_ci->args = 2; //value, value
			new_ci->OpType = op;
			new_ci->data = data;
			cs->push_callclosure(*new_ci);
			break;
		}

		}
	}
}
