#pragma once
#include "auroria_types.h"
#include "ar_environment.hpp"
#include "ar_debug.h"

//LIBS
#include "ar_iflib.hpp"
#include "ar_maths.hpp"
#include "cfunc_lib.hpp"

	struct Unlexed_Data
	{
		//std::vector<char*> lines;
		std::vector<std::string> lines;
	};
	struct Lexed_Data {
		std::vector<std::string> lexed_values;
		size_t strings;
	};

	struct Parsed_Data
	{
		std::vector<std::pair<AR_TYPE, std::string>> parsed_data; //datatype, value
	};

	struct Tree {
		int x;
	} ;
	namespace Parser
	{
		bool isInt(std::string str);
		bool isBool(std::string str);
		bool isString(std::string& str, bool erase_string_quotes);
		bool isDouble(std::string str);
		bool isCharset(std::string& str, bool trim_edges);
		bool isSystem(std::string str);
		void mem_TrimEdges(std::string& str);
	}
	void lex_values(Auroria_Env* A, Lexed_Data* L, Unlexed_Data uD);
	void parse_values(Auroria_Env* A, Parsed_Data* Pd, Lexed_Data* L);
