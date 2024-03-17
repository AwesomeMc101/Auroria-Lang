#include "ar_environment.hpp"

#include "cfunc_lib.hpp"
#include "ar_maths.hpp"
#include "ar_afunction.hpp"

struct Call {
	size_t args;
	OpCode OpType; //CCALL | CALL
	std::string data;
};

class Callstack
{
	std::vector<Call> callStack; //TOP
public:
	//size_t env_arg_count = 0;
	std::vector<size_t> env_arg_counts{};
	unsigned int eac_top();


	bool check_callstack(Auroria_Env* A, size_t stackSize); //on OP_PUSH
	bool push_callclosure(Call closure);
	bool erase_callclosure(std::string name);
	Call pop_callstack();
	size_t get_stack_size();
	size_t get_eac_top_value(); //LONG NAME :/
	size_t get_eac_value(int sub_idx);
	void new_environment();
	void dbg_printcallstack();
};

bool ar_docaller(Auroria_Env* A, Callstack* cs, int& i);
bool ar_domathcall(Auroria_Env* A, Callstack* cs);

void ar_m_execute(Auroria_Env* A);