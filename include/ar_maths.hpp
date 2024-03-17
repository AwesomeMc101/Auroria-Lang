#pragma once
#include "ar_environment.hpp"
#include <math.h>
namespace Mathematic
{
	void enable_math(Auroria_Env* A);
	bool ar_math_istoken(char tok);
	bool ar_math_isvalidnum(std::string str);

	void ar_math_concat(Auroria_Env* A);
	void ar_math_strsub(Auroria_Env* A);

	void ar_math_add(Auroria_Env* A);
	void ar_math_sub(Auroria_Env* A);
	void ar_math_mul(Auroria_Env* A);
	void ar_math_div(Auroria_Env* A);
	void ar_math_pow(Auroria_Env* A);
	void ar_math_mod(Auroria_Env* A);

}