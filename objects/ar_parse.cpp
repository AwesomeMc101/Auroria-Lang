#pragma once
#include "ar_parse.hpp"


namespace Parser
{
	bool isInt(std::string str)
	{
		while (str[0] == ' ') { str.erase(str.begin()); }
		while (str[str.size() - 1] == ' ') { str.pop_back(); }
		//std::vector<char> ints = {'0','1','2','3','4','5'}
		for (char c : str)
		{
			if (!isdigit(c)) { return false; };
		}
		return true;
	}
	bool isBool(std::string str)
	{
		while (str[0] == ' ') { str.erase(str.begin()); }
		while (str[str.size() - 1] == ' ') { str.pop_back(); }
		if (!isInt(str))
		{
			if (str != "false" && str != "true")
			{
				return false;
			}
		}
		return true;
	}
	bool isString(std::string& str, bool erase_string_quotes)
	{
		std::string temp_str = str;

		while (temp_str[0] == ' ') { temp_str.erase(temp_str.begin()); }
		while (temp_str[temp_str.size() - 1] == ' ') { temp_str.pop_back(); }

		char begin = temp_str.front();
		char end = temp_str.back();

		if (begin != '\'' && begin != '"' || end != '\'' && end != '"') { return false; }

		if (erase_string_quotes)
		{
			str = temp_str;
			str.erase(str.begin() + 0);
			str.pop_back();
		}

		return true;
	}
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
	bool isCharset(std::string& str, bool trim_edges)
	{
		//std::string temp_str = str;

		std::string temp_str = str;

		while (temp_str[0] == ' ') { temp_str.erase(temp_str.begin()); }
		while (temp_str[temp_str.size() - 1] == ' ') { temp_str.pop_back(); }

		for (char c : temp_str)
		{
			if (!isalnum(c))
			{
				return false;
			}
		}

		if (trim_edges)
		{
			while (str[0] == ' ') { str.erase(str.begin()); }
			while (str[str.size() - 1] == ' ') { str.pop_back(); }
		}
		return true;
	}
	bool isSystem(std::string str)
	{
		if (str == "AURORIA_INC_LINEINFO")
		{
			return true;
		}
	}
	void mem_TrimEdges(std::string& str)
	{
		while (str[0] == ' ') { str.erase(str.begin()); }
		while (str[str.size() - 1] == ' ') { str.pop_back(); }
	}
}

void lex_values(Auroria_Env* A, Lexed_Data* L, Unlexed_Data uD)
{
	//for (char* line : uD.lines)
	for(std::string line : uD.lines)
	{
		//std::cout << "LEX LINE: " << line << "\n";
		//size_t len = strlen(line);
		size_t len = line.length();
		if (!len) { continue; }
		char buffer[2000]{ "" };

		bool tab_passed = false;

		for (int i = 0; i < len; i++)
		{
			if ((line[i] == '		')) //disregard tabs
			{
				continue;
			}

			//if (!tab_passed) { tab_passed = true; }

			auto iterator = std::find(A->AR_RESERVED_TOKENS.begin(), A->AR_RESERVED_TOKENS.end(), line[i]);
			if (iterator != A->AR_RESERVED_TOKENS.end())
			{
				if (strlen(buffer))
				{
					char* l_temp = buffer;
					L->lexed_values.push_back(l_temp);

					memset(buffer, 0, 2000);
				}

				char temp[2] = { line[i] };
				L->lexed_values.push_back(temp);
				
			}
			else {
				buffer[strlen(buffer)] = line[i];
			}
			
		}

		L->lexed_values.push_back(buffer);
		memset(buffer, 0, 2000);
		L->lexed_values.push_back("AURORIA_INC_LINEINFO");
	}
}


void parse_values(Auroria_Env* A, Parsed_Data* Pd, Lexed_Data* L)
{
	

	for (unsigned int i = 0; i < L->lexed_values.size(); i++)
	{
		std::string token = L->lexed_values[i];
		if (!token.length()) { continue; }

		bool can_access_old = (i > 0);
		bool can_access_new = (i < (L->lexed_values.size() - 1));

		if (token.length() == 1)
		{
			//char like ()
			auto iterator = std::find(A->AR_RESERVED_TOKENS.begin(), A->AR_RESERVED_TOKENS.end(), token[0]);
			if (iterator != A->AR_RESERVED_TOKENS.end()) {
				if (can_access_old && can_access_new) /* imagine you wrote print('!')*/
				{
					if ((L->lexed_values[i - 1] == "'" || L->lexed_values[i - 1] == "\"") && (L->lexed_values[i + 1] == "'" || L->lexed_values[i+1] == "\""))
					{
						Pd->parsed_data.push_back(std::make_pair(AR_TYPE_STRING, token));
						continue;
					}
				}
				Pd->parsed_data.push_back(std::make_pair(AR_TYPE_TOKEN, token));
			}
			else if (isdigit(token[0])) { Pd->parsed_data.push_back(std::make_pair(AR_TYPE_INT, token)); }
			else
			{
				Pd->parsed_data.push_back(std::make_pair(AR_TYPE_CHAR, token));
			}
		}
		else
		{
			//std::cout << "iterate";
			Parser::mem_TrimEdges(token);
			auto iterator = std::find(A->AR_RESERVED_KEYS.begin(), A->AR_RESERVED_KEYS.end(), token);
			//auto iterator = 0;
			//printf("done");
			if (iterator != A->AR_RESERVED_KEYS.end())
			{
				//printf("key");
				Pd->parsed_data.push_back(std::make_pair(AR_TYPE_KEY, token));
			}

			else if (Parser::isString(token, true)) { Pd->parsed_data.push_back(std::make_pair(AR_TYPE_STRING, token)); }
			else if (Parser::isInt(token)) { Pd->parsed_data.push_back(std::make_pair(AR_TYPE_INT, token)); }
			else if (Parser::isDouble(token)) { Pd->parsed_data.push_back(std::make_pair(AR_TYPE_FLOAT, token)); }
			else if (Parser::isBool(token)) { Pd->parsed_data.push_back(std::make_pair(AR_TYPE_BOOL, token)); }
			else if (Parser::isCharset(token, true)) { Pd->parsed_data.push_back(std::make_pair(AR_TYPE_CHARSET, token)); } //likely the name of something
			else if (Parser::isSystem(token)) { Pd->parsed_data.push_back(std::make_pair(AR_TYPE_SYSTEM, token)); }
			else {
				std::cout << "nil: " << token;
				Pd->parsed_data.push_back(std::make_pair(AR_TYPE_NIL, token));
			}
			//printf("gettype");
		}
	}

}