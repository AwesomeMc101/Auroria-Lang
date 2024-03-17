#include "ar_maths.hpp"

void Mathematic::enable_math(Auroria_Env* A)
{
	A->AR_RESERVED_TOKENS.push_back('+'); //ADD/CONCAT
	A->AR_RESERVED_TOKENS.push_back('-'); //SUB/STRSUB
	A->AR_RESERVED_TOKENS.push_back('*'); //MUL
	A->AR_RESERVED_TOKENS.push_back('/'); //DIV
	A->AR_RESERVED_TOKENS.push_back('%'); //MOD
	A->AR_RESERVED_TOKENS.push_back('^'); //POW

//	A->libraries.push_back(AR_MATH_LIB);
	A->enable_lib(AR_MATH_LIB);
}

bool Mathematic::ar_math_isvalidnum(std::string str) //ty ar_parse.cpp
{
	short digit_counts = 0;
	for (char c : str)
	{
		if (!isdigit(c))
		{
			if (c != ' ' && c != '.')
			{
				return false;
			}
		}
		else
		{
			digit_counts++;
		}
	}

	return digit_counts; //if 0, will return FALSE
}

bool Mathematic::ar_math_istoken(char tok)
{
	if (tok == '+' || tok == '-' || tok == '/' ||
		tok == '*' || tok == '^' || tok == '%')
	{
		return true;
	}
	return false;
}

namespace Mathematic {
	void ar_math_concat(Auroria_Env* A)
	{
		std::string str_top = ar_popstack(A);
		std::string str_stop = ar_popstack(A); //this is what we want since thats the first vlaue user typed

		//printf("concat");

		str_stop.append(str_top);
		ar_pushstack(A, str_stop);
	}

	void ar_math_strsub(Auroria_Env* A)
	{
		std::string stack_top = ar_popstack(A);
		std::string stack_stop = ar_popstack(A);

		if (ar_math_isvalidnum(stack_top)) //"hello" - 1
		{
			try {
				stack_stop.resize(stack_stop.size() - std::stoi(stack_top));
			}
			catch (std::exception& e)
			{
				//idk
			}
			ar_pushstack(A, stack_stop);
		}
		else //"hello" - "ll" idk
		{
			size_t loc = stack_stop.find(stack_top);
			while (loc != std::string::npos)
			{
				stack_stop.erase(stack_stop.begin() + loc, stack_stop.end() - loc - 1);//magic number 1 to stop over erasing
				loc = stack_stop.find(stack_top);
			}
			ar_pushstack(A, stack_stop);
		}
	}

	void ar_math_add(Auroria_Env* A)
	{
		//printf("add");
		std::string stack_top = ar_popstack(A);
		std::string stack_stop = ar_popstack(A);

		//std::cout << "top: " << stack_top << " && stop: " << stack_stop << "\n";

		if (!ar_math_isvalidnum(stack_top) || !ar_math_isvalidnum(stack_stop))  /* in case var didnt push str type */
		{
			//printf("notvalid");
			ar_pushstack(A, stack_stop);
			ar_pushstack(A, stack_top);

			ar_math_concat(A);
			return;
		}

		//yes, flip
		float x = std::stof(stack_stop);
		float y = std::stof(stack_top);

		ar_pushstack(A, std::to_string(x + y));

	}

	void ar_math_sub(Auroria_Env* A)
	{
		std::string stack_top = ar_popstack(A);
		std::string stack_stop = ar_popstack(A);

		if (!ar_math_isvalidnum(stack_top) || !ar_math_isvalidnum(stack_stop)) /* in case var didnt push str type */
		{
			ar_pushstack(A, stack_stop);
			ar_pushstack(A, stack_top);

			ar_math_strsub(A);
			return;
		}

		//yes, flip
		float x = std::stof(stack_stop);
		float y = std::stof(stack_top);

		ar_pushstack(A, std::to_string(x - y));

	}

	void ar_math_mul(Auroria_Env* A)
	{
		std::string stack_top = ar_popstack(A);
		std::string stack_stop = ar_popstack(A);

		if (!ar_math_isvalidnum(stack_top) || !ar_math_isvalidnum(stack_stop))
		{
			return;
		}

		//yes, flip
		float x = std::stof(stack_stop);
		float y = std::stof(stack_top);

		ar_pushstack(A, std::to_string(x * y));

	}
	void ar_math_div(Auroria_Env* A)
	{
		std::string stack_top = ar_popstack(A);
		std::string stack_stop = ar_popstack(A);

		if (!ar_math_isvalidnum(stack_top) || !ar_math_isvalidnum(stack_stop))
		{
			return;
		}

		//yes, flip
		float x = std::stof(stack_stop);
		float y = std::stof(stack_top);

		ar_pushstack(A, std::to_string(x / y));

	}
	void ar_math_pow(Auroria_Env* A)
	{
		std::string stack_top = ar_popstack(A);
		std::string stack_stop = ar_popstack(A);

		if (!ar_math_isvalidnum(stack_top) || !ar_math_isvalidnum(stack_stop))
		{
			return;
		}

		//yes, flip
		float x = std::stof(stack_stop);
		float y = std::stof(stack_top);

		ar_pushstack(A, std::to_string(pow(x, y)));
	}
	void ar_math_mod(Auroria_Env* A)
	{
		std::string stack_top = ar_popstack(A);
		std::string stack_stop = ar_popstack(A);

		if (!ar_math_isvalidnum(stack_top) || !ar_math_isvalidnum(stack_stop))
		{
			return;
		}

		//yes, flip
		int x = roundf(std::stof(stack_stop));
		int y = roundf(std::stof(stack_top));

		ar_pushstack(A, std::to_string(x % y));

	}



}