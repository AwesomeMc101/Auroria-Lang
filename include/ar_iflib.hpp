#include "ar_environment.hpp"

void enable_iflib(Auroria_Env* A);
bool ar_if_istoken(char token);
bool ar_if_iskey(std::string key);

namespace Utils {
	bool ar_if_isdigit(std::string str);
	void ar_if_swap(std::string& top, std::string& second);
}
void ar_if_comparisonclosure(Auroria_Env* A, OpCode op);
void ar_if_execute(Auroria_Env* A, int &idx);
void ar_else_execute(Auroria_Env* A, int idx);