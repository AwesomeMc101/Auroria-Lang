#pragma once
#include <string>

#define ERROR_404 404 //
#define ERROR_9403 9403 //stack too small
#define WERROR_9405 9405 //too many arguments (WARNING)
#define ERROR_3404 3404 // no function name
#define ERROR_3406 3406 // not enough function arguments (9403 but for CALL not CCALL)

#define ERROR_1230 1230 //not a valid number for forloop


struct Error {
	int lineinfo;
	std::string err_str;
	int number;
	bool is_warning;
};

Error ar_e_newerror(int lineinfo, int error_type);
