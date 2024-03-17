#pragma once
#include "cfunc_lib.hpp"

namespace CFunction
{
	void enable_cfunction(Auroria_Env* A)
	{
		A->AR_RESERVED_KEYS.push_back("print");
		A->AR_RESERVED_KEYS.push_back("sin");
		A->AR_RESERVED_KEYS.push_back("parsebool");
		//CFunction::cfunctions.push_back("print");

		A->enable_lib(AR_CFUNC_LIB);
	}


	bool ar_is_cfunction(std::string token)
	{
		//std::cout << "\n" << token << "vs.";
		if (token == "print")
		{
			return true;
		}
		if (token == "sin")
		{
			return true;
		}
		if (token == "return")
		{
			return true;
		}
		if (token == "parsebool")
		{
			return true;
		}
		return false;
	}

	size_t ar_cf_getargs(std::string token)
	{
		if (token == "print")
		{
			return 1;
		}
		if (token == "sin")
		{
			return 1;
		}
		if (token == "return") { return 1; }
		if (token == "parsebool") { return 1; }
		return 0;
	}

	namespace Functions {
		namespace Utility {
			bool isDouble(std::string str)
			{
				while (str[0] == ' ') { str.erase(str.begin()); }
				while (str[str.size() - 1] == ' ') { str.pop_back(); }
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
		}
		void print(std::string data)
		{
			std::cout << "\nAURORIA PRINTING: " << data << std::endl;
		}
		float ar_sin(std::string data)
		{
			if (!Utility::isDouble(data)) { return 0; }
			float value = std::stof(data);

			return (float)(sin(value));
		}
		std::string parse_boolean(std::string data)
		{
			if (data == "0") { return "false"; }
			return "true";
		}
	}

	void ar_cf_runcfunction(Auroria_Env* A, std::string token)
	{
		if (token == "print")
		{
			Functions::print(ar_popstack(A));
			ar_setcfunctionreturn(A, 0);
		}
		else if (token == "sin")
		{
			float retVal = Functions::ar_sin(ar_popstack(A));
			ar_pushstack(A, std::to_string(retVal));
			ar_setcfunctionreturn(A, 1);
		}
		else if (token == "return") //PART OF AFUNCTION LIB
		{
			//std::cout << "return;";
			if (A->primed_af_return == "nil") {
				A->primed_af_return = ar_popstack(A); //set return value of function to the latest stack value
			}
		}
		else if (token == "parsebool")
		{
			ar_pushstack(A, Functions::parse_boolean(ar_popstack(A)));
			ar_setcfunctionreturn(A, 1);
		}
	}
}