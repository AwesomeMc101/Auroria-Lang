#include <iostream>
#include "ar_parse.hpp"
#include "ar_writebyte.hpp"

#include "ar_vm.hpp"

int main() //wrote this on github lol
{
 Unlexed_Data uD; //create new Unlexed data class to hold our script
  uD.lines.push_back("print('Hello World!')");

    Lexed_Data* L = new Lexed_Data; //create new lexed data class
    Parsed_Data* P = new Parsed_Data; //create new parsed data class

    Auroria_Env* A = ar_open(); //establish our environment

  /* Enable our libraries, most functions will not work without doing this */
   CFunction::enable_cfunction(A); //CFunctions (print, etc)
    Mathematic::enable_math(A); //Math (+-*/%^, strconcat/strsub)
    enable_iflib(A); //If else AND bitwise comparative operators
    AFunction_Lib::enable_afunc(A); //A(uroria) Functions AKA user functions 
    LoopLibrary::enable_loop(A); //Forloops

   lex_values(A, L, uD); //lex our Unlexed Data
    parse_values(A, P, L); //parse our lexed data
    ar_b_serialize(A, P); //serialize our parsed data into bytecode

  ar_m_execute(A); //execute everything. the bytecode is stored in the Environment

   printf("\n");

    for (Error e : A->error_list) //errors are in beta and don't work well but can probably help a bit if learning
    {
        if (e.is_warning) { std::cout << "WARNING | "; }
        else { std::cout << "FATAL ERROR | "; }
        std::cout << "[Line " << e.lineinfo << "] " << e.err_str << "\n";
    }
  

}
