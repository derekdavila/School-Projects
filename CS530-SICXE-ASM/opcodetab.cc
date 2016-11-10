/* opcodetab.cc
   CS530, Spring 2016
   Team Minnesota
*/

#include "opcodetab.h"

using namespace std;

//Sets up the paired map of machine code and op size.
opcodetab::opcodetab(){
    for(int i = 0; i < 101; i++) {
        op[opcodes[i]] = pair<string,int>(machine_code[i], op_size[i]);
    }
}

//Returns machine code for given opcode.
string opcodetab::get_machine_code(string s){

    //Sets the input to upper case.
    for(unsigned int i=0; i < s.size(); i++) {
        s[i] = toupper(s[i]);
    }

    //Searches through the map for the opcode, returns the machine code if found.
    op_iter = op.find(s);
    if(op_iter == op.end())
        throw opcode_error_exception(s + " not found.");
    return  op_iter -> second.first;
}

//Returns the number of bytes needed to encode a given opcode.
int opcodetab::get_instruction_size(string n){

    //Sets the input to upper case.
    for(unsigned int i=0; i <n.size(); i++) {
        n[i] = toupper(n[i]);
    }

    //Searches through the map for the opcode, returns the corresponding bytes if found.
    op_iter = op.find(n);
    if(op_iter == op.end())
        throw opcode_error_exception(n + " not found.");
    return op_iter -> second.second;
}
