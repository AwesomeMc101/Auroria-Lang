#include "ar_writebyte.hpp"

void Bytecode::writeByte(OpCode op, std::string datav)
{
	data.push_back(datav);
	op_set.push_back(op);
}

void Bytecode::writeByteLoc(OpCode op, std::string datav, size_t loc)
{
	data.insert(data.begin() + loc, datav);
	op_set.insert(op_set.begin() + loc, op);
}

void Bytecode::deleteByte(size_t loc)
{
	std::cout << "\nDeleting byte w/ data " << data[loc] << "\n";
	data.erase(data.begin() + loc);
	op_set.erase(op_set.begin() + loc);
}

void ar_b_serialize(Auroria_Env* A, Parsed_Data* pD)
{
	bool math_lib = A->check_lib(AR_MATH_LIB);
	bool cfunc_lib = A->check_lib(AR_CFUNC_LIB);
	bool if_lib = A->check_lib(AR_IF_LIB);
	bool afunc_lib = A->check_lib(AR_AFUNC_LIB);
	bool loop_lib = A->check_lib(AR_LOOP_LIB);

	for (int i = 0; i < pD->parsed_data.size(); i++)
	{
		//std::pair<AR_TYPE, std::string> parsed = pD->parsed_data[i];
		AR_TYPE type = pD->parsed_data[i].first;
		std::string data = pD->parsed_data[i].second;
	//	std::cout << data << type << "\n";
		switch (type)
		{
		case AR_TYPE_BOOL:
			A->bytes->writeByte(OP_PUSH, data);
			A->bytes->writeByte(OP_BOOL, data);
			break;
		case AR_TYPE_FLOAT:
			A->bytes->writeByte(OP_PUSH, data);
			A->bytes->writeByte(OP_FLOAT, data);
			break;
		case AR_TYPE_INT:
			A->bytes->writeByte(OP_PUSH, data);
			A->bytes->writeByte(OP_INTEGER, data);
			break;
		case AR_TYPE_STRING:
			A->bytes->writeByte(OP_PUSH, data);
			A->bytes->writeByte(OP_STRING, data);
			break;
		/*case AR_TYPE_CHAR:
			std::cout << "Char pushL " << data << "\n";
			A->bytes->writeByte(OP_PUSH, data);
			A->bytes->writeByte(OP_CHAR, data);
			break;*/
		case AR_TYPE_CHAR:
			std::cout << "TYPE CHAR: " << data;
			if (isalpha(data[0]))
			{
				std::cout << "ISALPHA";
				if (A->variables->isVariable(data))
				{
					std::cout << "ISVARIABLE";
					A->bytes->writeByte(OP_VAR, data);
					break;
				}
				std::cout << "NOTVAR";
				A->variables->reserve_variable(data);
				A->bytes->writeByte(OP_LOADVAL, data);
				A->bytes->writeByte(OP_PUSH, data);
				//A->bytes->writeByte(OP_VAR, data);
			}
			break;
		case AR_TYPE_TOKEN:
		{
			if (if_lib)
			{
				int next_is_eq = 0;
				if (i < (pD->parsed_data.size() - 1))
				{
					if (pD->parsed_data[i + 1].second[0] == '=')
					{
						next_is_eq = 1;
						if (data[0] == '=')
						{
							A->bytes->writeByte(OP_BDEQ, data);
							next_is_eq = 2;
							break;
						}
					 }
				}
				if (next_is_eq == 2) { break; }
				if (ar_if_istoken(data[0]))
				{
					switch (data[0])
					{

					case '<':
						if(next_is_eq){
							A->bytes->writeByte(OP_BLTEQ, data);
						}
						else {
							A->bytes->writeByte(OP_BLT, data);
						}
						break;
					case '>':
						if (next_is_eq) {
							A->bytes->writeByte(OP_BGTEQ, data);
						}
						else {
							A->bytes->writeByte(OP_BGT, data);
						}
						break;
					case '!':
						//printf("OP_BNOT");
						if (next_is_eq) {
							A->bytes->writeByte(OP_BNEQ, data);
						}
						else {
							A->bytes->writeByte(OP_BNOT, data);
						}
						break;
					}
					break;
				}
			}

			if (math_lib)
			{
				if (Mathematic::ar_math_istoken(data[0]))
				{
					switch (data[0])
					{
					case '+':
					{
						if (i > 0)
						{
							if (pD->parsed_data[i - 1].first == AR_TYPE_STRING) //ASSUME OP_CONCAT
							{
								A->bytes->writeByte(OP_CONCAT, data);
							}
							else
							{
								A->bytes->writeByte(OP_ADD, data);
							}
						}
						else
						{
							A->bytes->writeByte(OP_ADD, data);
						}
						break;
					}
					case '-':
						if (i > 0)
						{
							if (pD->parsed_data[i - 1].first == AR_TYPE_STRING) //ASSUME OP_CONCAT
							{
								A->bytes->writeByte(OP_STRSUB, data);
							}
							else
							{
								A->bytes->writeByte(OP_SUB, data);
							}
						}
						else
						{
							A->bytes->writeByte(OP_SUB, data);
						}
						break;
					case '*':
						A->bytes->writeByte(OP_MUL, data);
						break;
					case '/':
						A->bytes->writeByte(OP_DIV, data);
						break;
					case '^':
						A->bytes->writeByte(OP_POW, data);
						break;
					case '%':
						A->bytes->writeByte(OP_MOD, data);
						break;
					}
				}
			}

			switch (data[0])
			{
			case '{':
				A->bytes->writeByte(OP_ENV, data);
				break;
			case '}':
				A->bytes->writeByte(OP_CENV, data);
				break;
			case '(':
				if (i > 0)
				{
					if (pD->parsed_data[i - 1].first == AR_TYPE_CHARSET)
					{
						//A->variables->reserve_variable(pD->parsed_data[i - 1].second);
						if (!AFunction_Lib::ar_af_isafunction(pD->parsed_data[i - 1].second)) {
							AFunction_Lib::ar_af_reserve_name(pD->parsed_data[i - 1].second);
							A->bytes->writeByte(OP_NEWFUNC, pD->parsed_data[i - 1].second); //old data
						}
					}
				}
				A->bytes->writeByte(OP_ARGSET, data);
				break;
			case ')':
				A->bytes->writeByte(OP_CARGS, data);
				break;
			case '=':
				A->bytes->writeByte(OP_BEQ, data);
				if (i > 0)
				{
					if (pD->parsed_data[i - 1].first == AR_TYPE_CHARSET) //var = var + 1 scenario
					{
						if (A->variables->isVariable(pD->parsed_data[i - 1].second))
						{
							A->bytes->writeByte(OP_LOADVAL, pD->parsed_data[i - 1].second); //old data
							//A->bytes->writeByte(OP_PUSH, data);
						}
					}
				}
				break;
			}
			/*no other A->bytes needed since most other tokens are just designed to split up data*/
		}
		case AR_TYPE_KEY:

			//A->bytes->writeByte(OP)
			//A->bytes->writeByte
			//std::cout << "\nKEY " << data << "\n";
			if (if_lib)
			{
				if (ar_if_iskey(data))
				{
					if (data == "else") {
						A->bytes->writeByte(OP_ELSE, data);
						break;
					}
					A->bytes->writeByte(OP_IF, data);
					break;
				}
			}
			if (cfunc_lib)
			{
				if (CFunction::ar_is_cfunction(data))
				{
					A->bytes->writeByte(OP_CCALL, data);
					break;
				}
			}
			if (afunc_lib)
			{
				if (data == "function")
				{
					A->bytes->writeByte(OP_NEWFUNC, data);
					break;
				}
			}
			if (loop_lib)
			{
				if (LoopLibrary::is_loop_key(data))
				{
					A->bytes->writeByte(OP_FORLOOP, data);
					break;
				}
			}

			break;
		case AR_TYPE_CHARSET:

			if (A->variables->isVariable(data))
			{
				/* push after. */
				if (AFunction_Lib::ar_af_isafunction(data))
				{
					//printf("writeclal");
					A->bytes->writeByte(OP_CALL, data);
					break;
				}
				A->bytes->writeByte(OP_VAR, data);
				//A->bytes->writeByte(OP_PUSH, A->variables->ar_v_getvalue(data));
				break;
			}


			A->variables->reserve_variable(data); //moved to =
			A->bytes->writeByte(OP_LOADVAL, data);
			A->bytes->writeByte(OP_PUSH, data);

			break;
		case AR_TYPE_SYSTEM:
			if (data == "AURORIA_INC_LINEINFO")
			{
				A->bytes->writeByte(OP_CARGS, "nil");
				A->bytes->writeByte(OP_SYSLINEINFO, data);
			}
			break;
		}


	}
	//A->bytes->writeByte(OP_CARGS, "nil"); //final arg closure at end of instructions
}