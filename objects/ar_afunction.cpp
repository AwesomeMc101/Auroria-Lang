#pragma once
#include "ar_afunction.hpp"

namespace AFunction_Lib
{
	void enable_afunc(Auroria_Env* A)
	{
		A->AR_RESERVED_KEYS.push_back("return");
		A->enable_lib(AR_AFUNC_LIB);
	}

	AFunction ar_af_getfunction(Auroria_Env* A, std::string name)
	{
		for (AFunction af : AFunction_Stack.stack)
		{
			if (af.name == name)
			{
				return af;
			}
		}
		return *(new AFunction);
	}

	bool ar_af_isafunction(std::string name)
	{
		for (AFunction af : AFunction_Stack.stack)
		{
			if (af.name == name)
			{
				return true;
			}
		}

		for (std::string reserved_name : AFunction_Stack.reserved_names)
		{
			if (reserved_name == name) { return true; }
		}

		return false;
	}

	void ar_af_reserve_name(std::string name)
	{
		AFunction_Stack.reserved_names.push_back(name);
	}

	void ar_af_reversebytecode(AFunction &af)
	{
		std::reverse(af.data.data.begin(), af.data.data.end());
		std::reverse(af.data.op_set.begin(), af.data.op_set.end());


		std::reverse(af.args.data.begin(), af.args.data.end());
		std::reverse(af.args.op_set.begin(), af.args.op_set.end());
	}

	int ar_af_write_function(Auroria_Env* A, int idx, std::string name)
	{
		AFunction* af = new AFunction;
		int new_idx = idx;

		af->name = name;

		bool writing_args = true;
		size_t nests_passed = 0;
		size_t required_nests = 0;
		for (int i = idx; i < A->bytes->data.size(); i++)
		{
			OpCode op = A->bytes->op_set[i];
			std::string data = A->bytes->data[i];
			//std::cout << "\nNEW OP " << op << " | " << data << " | " << writing_args << "\n";

			/*while (true)
			{
				af->args.writeByte(op, data);
				i++;
				if (i >= A->bytes->op_set.size()) { printf("NO FUCNTION OPEN");  return idx; }
				op = A->bytes->op_set[i];
				data = A->bytes->data[i];
				if (op == OP_ENV) { break; }
				
			}*/
			if (i == 0) { continue; }
			if (op == OP_ENV)
			{
				//printf("switchup");
				//std::cout << "INC RN " << required_nests << "\n";
				required_nests++;
				if (!writing_args)
				{
					af->data.writeByte(op, data);
				}
				writing_args = false;
				continue;
			}

			if (op != OP_CENV && op != OP_ENV)
			{
				switch (writing_args)
				{
				case true:
					//std::cout << "WRITEARG: " << op << " | " << data << "\n";
					af->args.writeByte(op, data);
					break;
				case false:
					af->data.writeByte(op, data);
					break;
				}
				
			}

			if (op == OP_CENV)
			{
				//close environment
				//required_nests--;
				nests_passed++;

				//std::cout << "DEC RN< INC NP " << required_nests << " || " << nests_passed << "\n";
				if (nests_passed >= required_nests) {
					//A->nested_cenv--;
					new_idx = i;
					break;
				}
				else
				{
					af->data.writeByte(op, data);
				}
			}
		}

		//for (OpCode op : af->data.op_set)
		for(int x = 0; x < af->args.op_set.size(); x++)
		{
			OpCode op = af->args.op_set[x];
			std::string data = af->args.data[x];
			if (op == OP_PUSH) //any value that needs to be pushed
			{
				//std::cout << "Arg: " << data << "\n";
				af->req_args++;
				af->variable_names.push_back(data);
			}
		}

		af->is_executable = true;
		//return *af;
		AFunction_Stack.stack.push_back(*af);
		return new_idx; //to jump to in main loop
		//af->name = ar_popstack(A);
	}
	void ar_af_setlocalvariables(Auroria_Env* A, AFunction af)
	{
		printf("\nSETLOCALVAR\n\n");
		std::reverse(af.variable_names.begin(), af.variable_names.end()); //top of stack would be final value.


		DBG_PRINT_STACK(A);

		for (int i = 0; i < af.variable_names.size(); i++)
		{
			if (i == 0) { 
				if (af.req_args == 1) { ar_popstack(A); break; }
				continue; 
			}
			std::cout << "AFUNC PREPOP: " << ar_popstack(A) << "\n";
		}

		for (std::string var : af.variable_names)
		{
			std::string stop = ar_popstack(A);
			//std::cout << "\nSLV POPPED: " << top << " FOR " << var << "\n";
			//std::cout << "\nSLV POPPED: " << stop << " FOR " << var << "\n";
			
			A->variables->push_variable(var, 0, stop);
		}

	}

	void ar_af_cleanlocalvariables(Auroria_Env* A, AFunction af)
	{
		std::reverse(af.variable_names.begin(), af.variable_names.end()); //top of stack would be final value.
		for (std::string var : af.variable_names)
		{
			A->variables->push_variable(var, 0, "nil");
		}
	}

	void ar_af_inject_function(Auroria_Env* A, int idx, AFunction af)
	{
		if (!af.is_executable) { printf("fail"); return; }

	/*	if (ar_stacksize(A) < af.req_args)
		{
			Error e = ar_e_newerror(A->lineinfo, ERROR_3406);
			ar_e_pusherror(A, e);
			return;
		}*/


		ar_af_reversebytecode(af); //so that first values come first

		size_t byte_size = af.data.op_set.size();
		size_t arg_size = af.args.op_set.size();
		std::string f_name = af.name;

		/* because of the way vector.insert works, we need to do it in reverse. */

		A->bytes->writeByteLoc(OP_TAILCALL, f_name, idx);

		for (int i = 0; i < byte_size; i++)
		{
			OpCode op = af.data.op_set[i];
			std::string data = af.data.data[i];

			//std::cout << "\nWRITE DATA: " << op << " | " << data << "\n";

			A->bytes->writeByteLoc(op, data, idx);
		}

		A->bytes->writeByteLoc(OP_PRECALL, f_name, idx);

		for (int i = 0; i < arg_size; i++)
		{
			OpCode op = af.args.op_set[i];
			std::string data = af.args.data[i];
			//std::cout << "\nWRITE ARG: " << op << " | " << data << "\n";
			A->bytes->writeByteLoc(op, data, idx);
		}

		//ar_ce_pushaccess(A, CENV_AFUNCTION);
		
	}
}