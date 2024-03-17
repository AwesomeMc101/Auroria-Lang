#include "ar_error.hpp"

Error ar_e_newerror(int lineinfo, int error_type)
{
	Error er;
	er.lineinfo = lineinfo;
	er.number = error_type;

	switch (error_type)
	{
	case ERROR_9403:
		//stack too small
		er.err_str = "STACK TOO SMALL";
		er.is_warning = false;
		break;
	case WERROR_9405:
		er.err_str = "MORE ARGUMENTS THAN REQUIRED";
		er.is_warning = true;
		break;
	case ERROR_3404:
		er.err_str = "NO FUNCTION NAME FOUND";
		er.is_warning = false;
		break;
	case ERROR_3406:
		er.err_str = "NOT ENOUGH FUNCTION ARGUMENTS";
		er.is_warning = false;
		break;
	case ERROR_1230:
		er.err_str = "NOT A VALID VALUE FOR FORLOOP COUNT";
		er.is_warning = false;
		break;
	}

	return er;
}