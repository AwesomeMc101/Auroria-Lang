#include "ar_looplib.hpp"

namespace LoopLibrary {
	void enable_loop(Auroria_Env* A)
	{
		A->AR_RESERVED_KEYS.push_back("for");
		A->enable_lib(AR_LOOP_LIB);
	}

	bool is_loop_key(std::string key)
	{
		return (key == "for");
	}

	void new_loop(Auroria_Env* A, int idx)
	{
		LoopData LD;

		std::string var_name = ar_popstack(A);
		std::string loop_count = ar_popstack(A);

		if (!ar_p_isnumber(A, loop_count))
		{
			if (loop_count == "ever" || loop_count == "forever")
			{
				LD.max_loops = INT_MAX;
			}
			else if(loop_count.length())
			{

				LD.iteration_type = 1; 
				LD.type_1_iterator = loop_count;
				LD.max_loops = loop_count.length();
			}
			else
			{
				Error e = ar_e_newerror(A->lineinfo, ERROR_1230);
				ar_e_pusherror(A, e);

				LD.max_loops = 0;
			}
		}
		else {
			LD.max_loops = std::stoi(loop_count);
		}

		LD.variable_index_title = var_name;

		size_t nests_passed = 0;
		size_t required_nests = 0;
		--idx;
		for (int i = idx; i < A->bytes->op_set.size(); i++)
		{
			OpCode op = A->bytes->op_set[i];
			
			//std::cout << "\n" << op << " vs required " << OP_CENV << " or " << OP_ENV;

			//printf("scanning for loop");

			if (op == OP_ENV)
			{
				if (required_nests == 0) //set VM index to the first OP_ENV... this is the opcode for opening the loop :)
				{
					LD.vm_index = i;
				}
				//printf("Inc RN LOOP");
				required_nests++;
			}
			if (op == OP_CENV)
			{
				nests_passed++;
				//printf("INC NP LOOP");
				if (nests_passed >= required_nests)
				{
					//printf("cenv located");
					A->bytes->writeByteLoc(OP_TAILLOOP, std::to_string(LoopStack.loop_data_stack.size()), i); //dont need to decrement since size at time of write < size at time this function finishes
					LD.max_vm_index = i;
					break;
				}
			}

		}

		LoopStack.loop_data_stack.push_back(LD);
		A->bytes->writeByteLoc(OP_PRELOOP, std::to_string(LoopStack.loop_data_stack.size()-1), LD.vm_index);

	}

	LoopData pop_loop(Auroria_Env* A)
	{
		LoopData LD = LoopStack.loop_data_stack.back();
		LoopStack.loop_data_stack.pop_back();
		return LD;
	}

	void erase_loop_by_idx(Auroria_Env* A, int idx)
	{
		if (idx >= LoopStack.loop_data_stack.size()) { return; }
		LoopStack.loop_data_stack.erase(LoopStack.loop_data_stack.begin()+idx);
	}

	LoopData* get_loop_by_idx(Auroria_Env* A, int idx)
	{
		if (idx >= LoopStack.loop_data_stack.size())
		{
			return (new LoopData);
		}

		return &LoopStack.loop_data_stack[idx];

	}
}