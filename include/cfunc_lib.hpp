#pragma once

#include "ar_environment.hpp"
#include <iostream>

namespace CFunction
{

	//std::vector<std::string> cfunctions;

	void enable_cfunction(Auroria_Env* A);
	bool ar_is_cfunction(std::string token);
	size_t ar_cf_getargs(std::string token);

	namespace Functions {
		namespace Utility {
			bool isDouble(std::string data);
		}
		void print(std::string data);
		float ar_sin(std::string data);
		std::string parse_boolean(std::string data);
	}
	void ar_cf_runcfunction(Auroria_Env* A, std::string token);
}