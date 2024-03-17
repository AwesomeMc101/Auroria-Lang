#include "ar_environment.hpp"


Auroria_Env* ar_open()
{
	Auroria_Env* aE = new Auroria_Env;
	aE->bytes = new Bytecode;
	aE->variables = new Variable_Env;

	return aE;
}

void Variable_Env::push_variable(std::string idx, AR_TYPE type, std::string value)
{
	//std::cout << "\n----PUSH:" << idx << " @ VALUE: " << value<<".\n";
	variables[idx] = std::make_pair(type, value);
}

AR_TYPE Variable_Env::ar_v_gettype(std::string idx)
{
	return (variables[idx].first);
}
std::string Variable_Env::ar_v_getvalue(std::string idx)
{
	//std::cout << "\nIDX:" << idx << ".\n";
	//std::cout << "\nVALUE: " << variables[idx].second << "\n";
	return (variables[idx].second);
}


bool Variable_Env::isVariable(std::string idx)
{
	/*for (const auto& set : variables)
	{
		std::string comp = (std::string)set.first;
		if (comp == idx)
		{
			return true;
		}
	}*/
	//std::cout << "ISVARIABLE";
	if (!variables.size()) { return false; }
	for (std::map<std::string, std::pair<AR_TYPE, std::string>>::iterator iter = variables.begin(); iter != variables.end(); ++iter)
	{
		std::string k = iter->first;
	//	std::cout << k << "vs" << idx << "\n";
		if (k == idx) { return true; }
	}
	return false;
}

void Variable_Env::reserve_variable(std::string idx)
{
	if (!Variable_Env::isVariable(idx)) { //dont overwrite
		variables[idx] = std::make_pair(0, "nil");
	}
	//printf("reserved");
}

void Auroria_Env::enable_lib(int lib)
{
	this->libraries.push_back(lib);
}

bool Auroria_Env::check_lib(int lib)
{
	auto iterator = std::find(libraries.begin(), libraries.end(), lib);

	
	return (iterator == libraries.end() ?  false : true);

}

void ar_pushstack(Auroria_Env* A, std::string value)
{
	//std::cout << "PUSHSTACK: " << value << "\n";
	A->stack.push_back(value);

}
std::string ar_popstack(Auroria_Env* A)
{
	if (!A->stack.size()) {
		return std::to_string(AR_STACK_NIL);
	}
	std::string value = A->stack.back();
	A->stack.pop_back();
	return value;
}

bool ar_p_isnumber(Auroria_Env* A, std::string data)
{
	for (int i = 0; i < data.size(); i++)
	{
		char c = data[i];
		if (!isdigit(c))
		{
			return false;
		}
	}
	return true;
}

void DBG_PRINT_STACK(Auroria_Env* A)
{
	for (int i = 0; i < A->stack.size(); i++)
	{
		//std::cout << "DBG L" << i << " | " << A->stack[i] << "\n";
	}
}

void ar_sys_inclineinfo(Auroria_Env* A)
{
	A->lineinfo += 1;
}

int ar_sys_getlineinfo(Auroria_Env* A)
{
	return A->lineinfo;
}

size_t ar_stacksize(Auroria_Env* A)
{
	return A->stack.size();
}

void ar_e_pusherror(Auroria_Env* A, Error e)
{
	A->error_list.push_back(e);
}

void ar_setcfunctionreturn(Auroria_Env* A, int value)
{
	A->cfunction_return = value;
}

void ar_resetcfunctionreturn(Auroria_Env* A)
{
	A->primed_af_return = "nil";
}

int ar_getcfunctionreturn(Auroria_Env* A)
{
	return A->cfunction_return;
}


void ar_ce_pushaccess(Auroria_Env* A, std::string type)
{
	//A->cenv_access_stack++;
	A->cenv_access_vector.push_back(type);
}

bool ar_ce_checkclose(Auroria_Env* A, std::string identifier)
{
	return (A->cenv_access_vector.back() == identifier);
}


std::string ar_ce_popaccess(Auroria_Env* A)
{
	std::string type = A->cenv_access_vector.back();
	/*A->cenv_access_stack--;
	A->cenv_access_vector.pop_back();*/
	return type;
}