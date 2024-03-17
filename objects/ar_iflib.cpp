#include "ar_iflib.hpp"

void enable_iflib(Auroria_Env* A)
{
	A->AR_RESERVED_KEYS.push_back("if");
	A->AR_RESERVED_KEYS.push_back("elseif");
	A->AR_RESERVED_KEYS.push_back("else");

	A->AR_RESERVED_TOKENS.push_back('>');
	A->AR_RESERVED_TOKENS.push_back('<');
	A->AR_RESERVED_TOKENS.push_back('!');

	A->enable_lib(AR_IF_LIB);
}

bool ar_if_istoken(char token)
{
	if (token == '>' || token == '<' || token == '!')
	{
		return true;
	}
	return false;
}

bool ar_if_iskey(std::string key)
{
	if (key == "if" || key == "elseif" || key == "else")
	{
		return true;
	}
	return false;
}

namespace Utils
{
	bool ar_if_isdigit(std::string str)
	{
		int non_digit_count = 0;
		for (char c : str)
		{
			if (!isdigit(c))
			{
				if (c != ' ' && c != '.')
				{
					return false;
				}
				non_digit_count++;
			}
		}

		return(!(non_digit_count == str.length())); //it shouldn't.
	}
	void ar_if_swap(std::string& top, std::string& second)
	{
		std::string temp = second;
		second = top;
		top = temp;
	}
}

void ar_if_comparisonclosure(Auroria_Env* A, OpCode op)
{
	//std::vector<std::string> comparison_data = { ar_popstack(A), ar_popstack(A), ar_popstack(A) };
	//printf("ifcomp");
	//;; std::cout << " op: " << op << "";
	std::string top = ar_popstack(A);
	switch (op)
	{
	case OP_BNOT:
	{
		//!x
		if (top == "false" || top == "0")
		{
			ar_pushstack(A, "1");
		}
		else
		{
			ar_pushstack(A, "0");
		}
		return;
	}
	}

	std::string second = ar_popstack(A);
	bool number_eval = (Utils::ar_if_isdigit(top) && Utils::ar_if_isdigit(second));
	Utils::ar_if_swap(top, second);
	//std::cout << " || nE: " << number_eval << "\n";

	std::cout << "\nCompare " << top << " to " << second << "\n";

	switch (op)
	{
	case OP_BNEQ:
	{
		if (number_eval)
		{
			if(std::stof(top) != std::stof(second)) { ar_pushstack(A, "1"); }
			else { ar_pushstack(A, "0"); }
			return;
		}
		if (top != second) { ar_pushstack(A, "1"); }
		else { ar_pushstack(A, "0"); }
		return;
	}
	case OP_BDEQ:
	{
		if (number_eval)
		{
			if (std::stof(top) == std::stof(second)) { ar_pushstack(A, "1"); }
			else { ar_pushstack(A, "0"); }
			return;
		}
		if (top == second) { ar_pushstack(A, "1"); }
		else { ar_pushstack(A, "0"); }
		return;
	}
	case OP_BGT:
	{
		if (number_eval) {
			if (std::stof(top) > std::stof(second))
			{
				ar_pushstack(A, "1");
			}
			else
			{
				ar_pushstack(A, "0");
			}
			return;
		}
		
		/* not numbers, compare string lengths */
		int size_top = top.length(); 
		int size_second = second.length();
		if (size_top > size_second)
		{
			ar_pushstack(A, "1");
		}
		else
		{
			ar_pushstack(A, "0");
		}

		return;
	}
	case OP_BGTEQ:
	{
		if (number_eval) {
			if (std::stof(top) >= std::stof(second))
			{
				ar_pushstack(A, "1");
			}
			else
			{
				ar_pushstack(A, "0");
			}
			return;
		}

		/* not numbers, compare string lengths */
		int size_top = top.length();
		int size_second = second.length();
		if (size_top >= size_second)
		{
			ar_pushstack(A, "1");
		}
		else
		{
			ar_pushstack(A, "0");
		}

		return;
	}
	case OP_BLT:
	{
		if (number_eval) {
			if (std::stof(top) < std::stof(second))
			{
				ar_pushstack(A, "1");
			}
			else
			{
				ar_pushstack(A, "0");
			}
			return;
		}

		/* not numbers, compare string lengths */
		int size_top = top.length();
		int size_second = second.length();
		if (size_top < size_second)
		{
			ar_pushstack(A, "1");
		}
		else
		{
			ar_pushstack(A, "0");
		}

		return;
	}
	case OP_BLTEQ:
	{
		if (number_eval) {
			if (std::stof(top) <= std::stof(second))
			{
				ar_pushstack(A, "1");
			}
			else
			{
				ar_pushstack(A, "0");
			}
			return;
		}

		/* not numbers, compare string lengths */
		int size_top = top.length();
		int size_second = second.length();
		if (size_top <= size_second)
		{
			ar_pushstack(A, "1");
		}
		else
		{
			ar_pushstack(A, "0");
		}

		return;
	}
	}

}

void ar_if_execute(Auroria_Env* A, int& idx)
{
	//A != B
	//std::vector<std::string> comparison_data = {ar_popstack(A), ar_popstack(A), ar_popstack(A)};

	bool run = (ar_popstack(A) != "0");

	A->if_results.push_back(run);

	if (!run)
	{
		//skip code
		size_t nests_passed = 0;
		size_t required_nests = 0;

		for (int i = idx; i < A->bytes->op_set.size(); i++)
		{
			OpCode op = A->bytes->op_set[i];
			
			if (op == OP_ENV)
			{
				required_nests++;
			}
			else if (op == OP_CENV)
			{
				nests_passed++;
				if (nests_passed >= required_nests)
				{
					idx = i;
					break;
				}
			}

			/*A->bytes->deleteByte(i);
			--i;*/
		}
	}
}

void ar_else_execute(Auroria_Env* A, int idx)
{
	bool run = A->if_results.back(); //if true, dont run anything
	A->if_results.pop_back();

	if (run) //this is because if the original loop passed, we dont want the else to run
	{
		//skip code
		size_t nests_passed = 0;
		size_t required_nests = 0;

		for (int i = idx; i < A->bytes->op_set.size(); i++)
		{
			OpCode op = A->bytes->op_set[i];

			if (op == OP_ENV)
			{
				required_nests++;
			}
			else if (op == OP_CENV)
			{
				nests_passed++;
				if (nests_passed >= required_nests)
				{
					break;
				}
			}

			A->bytes->deleteByte(i);
			--i;
		}
	}

}