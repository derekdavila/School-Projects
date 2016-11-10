#include "sicxe_asm.h"
#include "file_parser.h"
#include "file_parse_exception.h"
#include "opcodetab.h"
#include "opcode_error_exception.h"
#include "symtab.h"
#include "symtab_exception.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

using namespace std;

sicxe_asm::sicxe_asm(string filename){
        file_parser parser(filename);
        base = -1;
        passOne(parser,filename);
        passTwo(filename);
}
sicxe_asm::~sicxe_asm(){}

void sicxe_asm::passOne(file_parser parser, string filename){
    parser.read_file();
    int line = 0;
    int temp;
    int proglength;
    locctr = 0x0;
    struct Listing_File lis;
    string label, opcode, operand, comment, stringtemp;
    bool found;
    unsigned i;

    //Skips lines that have no labels, opcodes, or operands to get to first line
    while(parser.get_token(line,0) == "" && parser.get_token(line,1) == "" && parser.get_token(line,2) == ""){
        label = parser.get_token(line,0);
        opcode = parser.get_token(line,1);
        operand = parser.get_token(line,2);
        comment = parser.get_token(line,3);
        lis.line = line + 1;
        lis.address = hex_format(locctr);
        lis.label = label;
        lis.opcode = opcode;
        lis.operand = operand;
        contents.push_back(lis);
        line++;
    }

    //Get starting address and set location counter to starting file
    if(to_upper(parser.get_token(line,1)) == "START"){
        startaddress = atoi(parser.get_token(line,2).c_str());
        stringtemp = parser.get_token(line,2);
        if(stringtemp.at(0) == '$'){ stringtemp.erase(0,1);}
        stringstream str;
        str << stringtemp;
        str >> hex >> locctr;
        label = parser.get_token(line,0);
        opcode = parser.get_token(line,1);
        operand = parser.get_token(line,2);
        comment = parser.get_token(line,3);
        lis.line = line + 1;
        lis.address = hex_format(0);
        lis.label = label;
        lis.opcode = opcode;
        lis.operand = operand;
        contents.push_back(lis);
        line++;
     }

    //If the first line contains something other than START, throw error
    else
        throw file_parse_exception("Error, START should be in the first line that is not a comment or blank.");

    while(to_upper(parser.get_token(line,1)) == "EQU" || to_upper(parser.get_token(line,1)) == "BASE")
    {
        label = parser.get_token(line,0);
        opcode = parser.get_token(line,1);
        operand = parser.get_token(line,2);
        comment = parser.get_token(line,3);
        lis.line = line + 1;
        lis.address = hex_format(0);
        lis.label = label;
        lis.opcode = opcode;
        lis.operand = operand;
        contents.push_back(lis);
        line++;
    }

    //Handles assembler directives and labels
    while(to_upper(parser.get_token(line,1)) != "END"){
        label = parser.get_token(line,0);
        opcode = parser.get_token(line,1);
        operand = parser.get_token(line,2);
        comment = parser.get_token(line,3);
        lis.line = line + 1;
        lis.address = hex_format(locctr);
        lis.label = label;
        lis.opcode = opcode;
        lis.operand = operand;
        found = false;
        i = 0;
        ostringstream line_num;
        line_num << line + 1;
	if(label != ""){ symboltable.insert(to_upper(label),lis.address);}
                if(opcode != ""){
                    //Handles labels for symbol table
                   // if(label != ""){
                         //  symboltable.insert(to_upper(label),lis.address);
                    //}
                        if(to_upper(opcode) == "WORD") {
                            found = true;
                            locctr += 3; }
                        else if(to_upper(opcode) == "RESW") {
                            //add 3 * #[OPERAND] to LOCCTR
                            if(operand[0] == '#'){
                                stringtemp = operand;
                                stringtemp.erase(0,1);
                                temp = atoi(stringtemp.c_str());
                                temp = temp * 3;
                                locctr += temp;
                            }
                            if(operand[0] == '$'){
                                int hextemp;
                                stringstream s;
                                stringtemp = operand;
                                stringtemp.erase(0,1);
                                temp = atoi(stringtemp.c_str());
                                s << temp;
                                s >> hex >> hextemp;
                                hextemp = hextemp * 3;
                                locctr += hextemp;
                            }
                            else{
                                temp = atoi(operand.c_str());
                                temp = temp * 3;
                                locctr += temp;
                                }
                            found = true;
                        }
                        else if(to_upper(opcode) == "RESB"){
                            //add #[OPERAND] to LOCCTR
                            if(operand[0] == '#'){
                                stringtemp = operand;
                                stringtemp.erase(0,1);
                                temp = atoi(stringtemp.c_str());
                                locctr += temp;
                            }
                            if(operand[0] == '$'){
                                int hextemp;
                                stringstream s;
                                stringtemp = operand;
                                stringtemp.erase(0,1);
                                temp = atoi(stringtemp.c_str());
                                s << temp;
                                s >> hex >> hextemp;
                                locctr += hextemp;
                            }
                            else{
                                temp = atoi(operand.c_str());
                                locctr += temp;
                                }
                            found = true;
                        }
                        else if(to_upper(opcode) == "BYTE"){
                            //find length of constant in bytes, add length to location counter
                            if(operand[0] == 'X' || operand[0] == 'x'){
                                if(((operand.size() - 3) % 2) != 0)
                                {
                                        throw file_parse_exception("byte must be even at line " + line_num.str());
                                }
                                locctr += (operand.size() - 3) / 2;
                            }
                            else if(operand[0] == 'C' || operand[0] == 'c')
                                locctr += operand.size() - 3;
                            else{
                                throw file_parse_exception("byte size is invalid at line " + line_num.str());}
                            found = true;
                        }

                        else if(to_upper(opcode) == "BASE"){
                            found = true;
			    base = 1;
			    cout << base << endl;
			    base_operand = operand; 
                            }
                        else if(to_upper(opcode) == "NOBASE") {
                            found = true;
			    base = -1;
                            }
                        else if(to_upper(opcode) == "EQU") {
                            found = true;
                            }
                        else{
                        for(i = 0; i < 100; i++){
                                if(to_upper(opcode) == opcodes[i])
                                {
                                        locctr += op_size[i];
                                        found = true;
                                }
                        }
                        if(found == false){ //Throw error if opcode is never and found and is therefore invalid
                                throw opcode_error_exception("opcode " + opcode + " is invalid at line " + line_num.str());}
                        }
                }
                contents.push_back(lis);
                line++; //read next input line

            } //end {while not END}
        label = parser.get_token(line,0);
        opcode = parser.get_token(line,1);
        operand = parser.get_token(line,2);
        comment = parser.get_token(line,3);
        lis.line = line + 1;
        lis.address = hex_format(locctr);
        lis.label = label;
        lis.opcode = opcode;
        lis.operand = operand;
        contents.push_back(lis); //write last line to struct
        //save (LOCCTR - starting address) as program length
        proglength = locctr - startaddress;
        write_file(filename); //write to file
}//end {Pass 1}

//Second Pass:
void sicxe_asm::passTwo(string filename)
{
    //read first input line (from intermediate file)
        int line = 0, count = 0;;
        struct Listing_File row = contents.at(line);
        string opcode = row.opcode;
        string machinecode = "";
        string operand = row.operand;
        string address = row.address, label = row.label, comment = row.comment;
        string tempaddress;
        while(label == "" && opcode == "" && operand == ""){
                line++;
                count++;
                row = contents.at(line);
                label = row.label;
                opcode = row.opcode;
                operand = row.operand;
                row.machinecode = "";
        }
        while(to_upper(opcode) == "START" || to_upper(opcode) == "EQU" || to_upper(opcode) == "BASE"){
                row.machinecode = "";
                contents.at(line) = row;
                line++;
                row = contents.at(line);
                opcode = row.opcode;
                count++;
        }
        for(unsigned int i = 0; i < (contents.size() - count); i++){
                Listing_File row = contents.at(line);
                opcode = row.opcode;
                operand = row.operand;
                address = row.address;
                machinecode = "";
                string tempoperand = operand;
                int eraselocation = 0;
                if(to_upper(opcode) == "BASE" || to_upper(opcode) == "NOBASE")
                {
                        row.machinecode = "";
                }
                if(to_upper(opcode) != "" && to_upper(opcode) != "BASE" && to_upper(opcode) != "NOBASE") {
                        if(operand[0] == '#' || operand[0] == '@') {
                                tempoperand = ignore_signs(operand);
                        }
                        if(to_upper(tempoperand).find(",X") != string::npos)
                        {
                                for(unsigned int iter = 0; iter < operand.size(); iter++)
                                {
                                        if(tempoperand[iter] == ','){
                                                eraselocation = iter;
                                        }
                                }
                                tempoperand = tempoperand.erase(eraselocation, eraselocation+1);
                        }
                        if(symboltable.contains(to_upper(tempoperand))) {
                                tempaddress = symboltable.sym_address(to_upper(tempoperand));
                                if(opcodetable.get_instruction_size(to_upper(opcode)) == 3){ //if format 3 call method
                                        machinecode = formatThree(opcode, operand, address);
                                }
                                else {//call format 4
                                        machinecode = formatFour(opcode, operand, tempaddress);
                                }
                        }// end (if symbol)
                        else if(to_upper(opcode) == "RESW" || to_upper(opcode) == "RESB" || to_upper(opcode) == "END"){
                                machinecode = "";
                        }
                        else if(to_upper(opcode) != "WORD" && to_upper(opcode) != "BYTE"){//Calculate machine code
                                if(opcodetable.get_instruction_size(to_upper(opcode)) == 1){
                                        machinecode = formatOne(opcode);
                                }
                                else if(opcodetable.get_instruction_size(to_upper(opcode)) == 2){
                                        machinecode = formatTwo(opcode, operand);
                                }
                                else if(opcodetable.get_instruction_size(to_upper(opcode)) == 3){
                                        machinecode = formatThree(opcode, operand, address);
                                }
                                else if(opcodetable.get_instruction_size(to_upper(opcode)) == 4){
                                        machinecode = formatFour(opcode, operand, address);
                                }
                                else machinecode = "";
                        }

                        else if(to_upper(opcode) == "WORD") {
                             //convert constant to object code
                                stringstream instr(operand);
                                int n;
                                instr >> n;
                                machinecode = int_to_hex(n,6);
                        }
                        else if(to_upper(opcode) == "BYTE"){
                                if(operand[0] == 'C' || operand[0] == 'c'){ // then machine code equals the ascii values of the quoted text
                                        int iter = 2;
                                        int ctemp;
                                        stringstream s;
                                        while(operand[iter] != '\''){
                                        //Convert to ascii
                                                ctemp = operand[iter];
                                                machinecode += int_to_hex(ctemp, 2);
                                                iter++;
                                        }
                                }

                                else if(operand[0] == 'X' || operand[0] == 'x'){
                                 //operand starts with x the machine code is just the quoted characters
                                        int iter = 2;
                                        string xtemp;
                                        while(operand[iter] != '\'') {
                                                xtemp += operand[iter];
                                                iter++;
                                        }
                                        stringstream s;
                                        s << xtemp << setw(6) << setfill('0');
                                        machinecode = xtemp;
                                }
                        }
                  }//end (if not comment)
      row.machinecode = machinecode;
      contents.at(line) = row; //write listing file
      line++;
      }//end (for)
      write_file(filename);//write last listing
}

void sicxe_asm::write_file(string filename){
    ofstream lisFile;
    string inputPrefix;
    stringstream input;

    //Get filename without file extention
    input << filename;
    getline(input,inputPrefix, '.');

    //Append new file extention
    string listName = inputPrefix + ".lis";

    lisFile.open(listName.c_str(), ios::out);

    //Print headers
    lisFile << left << setw(14) << "Line#"
             << left << setw(14) << "Address"
             << left << setw(14) << "Label"
             << left << setw(14) << "Opcode"
             << left << setw(14) << "Operand"
             << left << "Machine Code" << endl;
    lisFile << left << setw(14) << "====="
             << left << setw(14) << "======="
             << left << setw(14) << "====="
             << left << setw(14) << "======"
             << left << setw(14) << "======="
             << left << "============" << endl;

    //Write values in each line to file
    for(unsigned int i = 0; i < contents.size(); i++){
        lisFile << left << setw(14) << contents[i].line
             << left << setw(14) << contents[i].address
             << left << setw(14) << contents[i].label
             << left << setw(14) << contents[i].opcode
             << left << setw(14) << contents[i].operand
             << left << contents[i].machinecode << endl;
    }
    
    symboltable.print_symtab();

    lisFile.close();
}

string sicxe_asm::to_upper(string s)
{
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

string sicxe_asm::to_string(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}

string sicxe_asm::hex_format(int h) {
    stringstream temp1;
    temp1 << hex << setw(5) << setfill('0') << h;
    return to_upper(temp1.str());
}

int sicxe_asm::base_relative(string opcode) {
	if(to_upper(opcode) == "BASE")
		return 1; 
	else 
		return -1;
}

/*
Format1
Returns 8 bit opcode as machine code
*/
string sicxe_asm::formatOne(string opcode){
    return opcodetable.get_machine_code(opcode);
}

/*Format2
Returns the 8 bit opcode as machine code in addition to the register numbers
*/
string sicxe_asm::formatTwo(string opcode, string operand){
        string machine_code = opcodetable.get_machine_code(opcode);
        operand = to_upper(operand);
        string r1, r2, optemp;
        int i, temp;

        if(operand.size() == 1)
        {
                if(operand.at(0)=='A')
                        r1="0";
                else if(operand.at(0)=='X')
                        r1="1";
                else if(operand.at(0)=='L')
                        r1="2";
                else if(operand.at(0)=='B')
                        r1="3";
                else if(operand.at(0)=='S')
                        r1="4";
                else if(operand.at(0)=='T')
                        r1="5";
                else if(operand.at(0)=='F')
                        r1="6";
                else if(operand.at(0)=='P' && operand.at(1)=='C')
                        r1="8";
                else if(operand.at(0)=='S' && operand.at(1)=='W')
                        r1="9";
                return machine_code + r1 + "0";
        }
        else if(operand[1] != ',' && operand[2] != ',')
        {
                int temp = atoi(operand.c_str());
                return machine_code + int_to_hex(temp, 1) + "0";
        }
        else{

        if(operand.at(0)=='A')
                r1="0";
        else if(operand.at(0)=='X')
                r1="1";
        else if(operand.at(0)=='L')
                r1="2";
        else if(operand.at(0)=='B')
                r1="3";
        else if(operand.at(0)=='S')
                r1="4";
        else if(operand.at(0)=='T')
                r1="5";
        else if(operand.at(0)=='F')
                r1="6";
        else if(operand.at(0)=='P' && operand.at(1)=='C')
                r1="8";
        else if(operand.at(0)=='S' && operand.at(1)=='W')
                r1="9";
        else

                throw symtab_exception("invalid register.");


        if(r1 == "8" || r1 == "9")
                i = 3;
        else
                i = 2;

        if(operand.at(i)=='A')
                r2='0';
        else if(operand.at(i)=='X')
                r2='1';
        else if(operand.at(i)=='L')
                r2='2';
        else if(operand.at(i)=='B')
                r2='3';
        else if(operand.at(i)=='S')
                r2='4';
        else if(operand.at(i)=='T')
                r2='5';
        else if(operand.at(i)=='F')
                r2='6';
        else if(operand.at(i)=='P' && operand.at(i+1)=='C')
                r2='8';
        else if(operand.at(i)=='S' && operand.at(i+1)=='W')
                r2='9';
        else {
                optemp = operand.at(i);
                temp = atoi(optemp.c_str());
                temp = temp - 1;
                r2 = int_to_hex(temp,1);
        }
                //throw symtab_exception("invalid register.");

        return machine_code + r1 + r2;
        }

}
/*Format3
Returns 6 bit opcode, nixbpe, and 12 bit displacement
TODO check 0-4095 for base relative, check -2048-2047 for PC relative
*/
string sicxe_asm::formatThree(string opcode, string operand, string address){
        string object_code = machine_code(opcode, operand, address);
        string machine_code = opcodetable.get_machine_code(opcode);
        string op = machine_code;
        string sdisp = "000";
        char first = op[0];
        char second = op[1];
	
	
        if(operand != ""){
                int idisp = set_displacement(operand, address);
                sdisp = int_to_hex(idisp, 3);
        }
        string ni = set_ni(operand);
        string secondbit = opHandler(second);
        string sum = secondbit + ni;
        string secondbyte = binary_to_hex(sum);
	/*
	if(operand == "foo" && opcode == "lda") {
		string temp = symboltable.sym_address(to_upper(operand));
		cout << "lda's address " << address << endl; 
		cout << "foo's address " << temp << endl;
		int idisp = set_displacement(operand, address);
		sdisp = int_to_hex(idisp, 3);
		cout << sdisp << endl;
	}
	*/
        return first + secondbyte + object_code + sdisp;
}
/*Format4
Returns 6 bit opcode, nixbpe, 20 bit address
*/
string sicxe_asm::formatFour(string opcode, string operand, string address){
    //Look up in opcodetab to get machine_code
    string object_code = machine_code(opcode, operand, address);
    string machine_code = opcodetable.get_machine_code(opcode);
    string op = machine_code;

    char first = op[0];
    char second = op[1];
    string ni = set_ni(operand);
    string secondbit = opHandler(second);
    string sum = secondbit + ni;
    string secondbyte = binary_to_hex(sum);
    /*
    if(ni == "01" && isdigit(operand[1]) ) {
    	operand = operand.erase(0,1);
    	istringstream instr(operand); 
	cout << operand << endl;
	int n; 
	instr >> n;
    	string temp_addr = int_to_hex(n,5);
	cout << temp_addr << endl;
	return first + secondbyte + object_code + temp_addr;
    }*/
    
    return first + secondbyte + object_code + address;
}

string sicxe_asm::opHandler(char second){
        string temp;
        if(second == '0')
                temp = "00";
        else if(second == '4')
                temp = "01";
        else if(second == '8')
                temp = "10";
        else if(second == 'C')
                temp = "11";
        else
                throw opcode_error_exception("opcode does not exist.");
        return temp;
}

string sicxe_asm::set_ni(string operand){
    if(operand.size() != 0){
        if(operand[0] == '#')
                return "01";
        else if (operand[0] == '@')
                return "10";
    }
    return "11";
}

string sicxe_asm::set_x(string operand) {
        if(operand.size() != 0) {
                if(to_upper(operand).find(",X") != string::npos) {return "1";}
        }
        return "0";
}

int sicxe_asm::set_displacement(string dest, string source) {
        string value;
        dest = ignore_signs(dest);
        source = to_upper(ignore_signs(source));
	
        for(unsigned int i=0; i<dest.length(); i++){
                if((dest[0] == 'X') && (dest[1] == ',')){
                        dest = dest.substr(2);
                        break;
                }
                if(dest[i] == ','){
                        dest = dest.substr(0,i);
                        break;
                }
        }

        if(dest.size() == 0) { value = "0";}
        else if(!symboltable.contains(to_upper(dest))) {
                stringstream instr(dest);
                int n;
                instr >> n;;
                return n;
        }
        else {value = symboltable.sym_address(to_upper(dest));}

        int source_addr = hex_to_int(source);
        int destination = hex_to_int(symboltable.sym_address(to_upper(dest)));
        int offset;
        int btemp = (source_addr +3);
	

        if (destination < btemp){
                int wtemp = 0xFFF - (btemp);
                offset = destination + wtemp + 1;
		base = 1;
        }
        else {offset = (destination - (source_addr + 3));}
	
	//offset = destination - (source_addr + 3);
	
		
        int tmp_base;
        if(offset >= -2048 && offset <= 2047){ tmp_base = -1;}
        else {tmp_base = 1;}
	cout << base << endl;
        if(tmp_base == 1 && base == 1) {
		cout << "here: " << base_operand <<endl;
                int base_address = hex_to_int(symboltable.sym_address(to_upper(base_operand)));
		
		cout << destination << endl;
		cout << (destination - base_address) << endl;
                return (destination - base_address);
        }
        else {return offset;}
	
}	

string sicxe_asm::ignore_signs(string temp) {
        if(temp[0] == '#' || temp[0] == '@' || temp[0] == '$') {
                if(temp[1] == '$') { temp = temp.substr(2); }
                else {temp = temp.substr(1); }
        }
        return temp;
}

string sicxe_asm::set_xbpe(string opcode, string operand, string locctr) {
        string ni = set_ni(operand);
        string x = set_x(operand);
        string b;
        string p;
        string e;
        int temp_base = 0;
        string xbpe;
        string disp = "";
        int temp;

        if(operand[0] == '#' && isalpha(operand[1]) == false && opcodetable.get_instruction_size(to_upper(opcode)) == 3){
                if(operand[1] == '$'){disp = ignore_signs(operand);}
                else {
                        temp = atoi(ignore_signs(operand).c_str());
                        disp = int_to_hex(temp, 3);
                }
        }

        else if(operand.size() != 0) {

                if(opcodetable.get_instruction_size(opcode) == 3){

                	int displacement = set_displacement(operand, locctr);
                	
			if(displacement >= -2048 && displacement <= 2047) {temp_base = -1;} //pc 
			//else if (displacement > 0 && displacement < 4095) {temp_base = 1;} //base
			
			if(displacement < -2048 || displacement > 2047) {
				if(base == 1) {
					throw symtab_exception("Displacement out of range at address " + locctr);
				}
				int temp_baseoperand = hex_to_int(symboltable.sym_address(base_operand)); 
				int temp_operand = hex_to_int(symboltable.sym_address(operand));
				int temp_displacement = temp_operand - temp_baseoperand; 
				if (temp_displacement < 0 || temp_displacement > 4095) {
					throw symtab_exception("Displacement out of range at address " + locctr); 
				}
				temp_base = 1; 
			}
			/*
	       		else if((displacement > 2047 || displacement < -2048 ) && base == -1) {
				cout << "displacement out of range" << endl;
				cout << displacement << endl; 
                	}
			else {temp_base = 1;}
			*/
			
		}
			

                if(opcode.find('+') != string::npos) {
                        temp_base = -1;
                        b = "0";
                        p = "0";
                        e = "1";
                        xbpe = x+b+p+e;
                        return xbpe;
                }
                else {
                        string temp_operand = ignore_signs(operand);
                        bool base_relative = false;

                         if(temp_base != -1 && base != -1) { base_relative = true;}

                         if(symboltable.contains(to_upper(temp_operand))) {
                                if(x == "0" && base_relative == false) {
                                        p = "1";
                                        b = "0";
                                        e = "0";
                                        xbpe = x+b+p+e;
                                        return xbpe;
                                }
                                else if(x == "0" && base_relative != false) {
                                        p = "0";
                                        b = "1";
                                        e = "0";
                                        xbpe = x+b+p+e;
                                        return xbpe;
                                }
                                else if(x == "1" && base_relative != false) {
                                        p = "0";
                                        b = "1";
                                        e = "0";
                                        xbpe = x+b+p+e;
                                        return xbpe;
                                }
                                else if(base_relative == false && x == "1" && ni == "11") {
                                        p = "1";
                                        b = "0";
                                        e = "0";
                                        xbpe = x+b+p+e;
                                        return xbpe;
                                }
                         }
                         else {
                                b = "0";
                                p = "1";
                                e = "0";
                                xbpe = x+b+p+e;
                                return xbpe;
                         }
                }
        }

        b = "0";
        p = "0";
        e = "0";
        xbpe = x+b+p+e;
        return xbpe;

}

string sicxe_asm::machine_code(string opcode, string operand, string locctr) {
        string op = opcodetable.get_machine_code(opcode);
        string m = "";
        string machinecode;

        m = set_xbpe(opcode,operand,locctr);
        machinecode = binary_to_hex(m);
        return machinecode;
}


string sicxe_asm::int_to_hex(int num, int width){
        stringstream out;
        out << setw(width) << setfill('0') << hex << num;
        return to_upper(out.str());
}

int sicxe_asm::hex_to_int(string s) {
        if(s[0] == '$' || s[0] == '#' || s[0] == '@') {
                s.erase(0,1);
        }

        int value;
        sscanf(s.c_str(),"%x",&value);
        return value;
}

string sicxe_asm::binary_to_hex(string bin) {
        if(bin == "0000"){return "0";}
        else if(bin == "0001"){return "1";}
        else if(bin == "0010"){return "2";}
        else if(bin == "0011"){return "3";}
        else if(bin == "0100"){return "4";}
        else if(bin == "0101"){return "5";}
        else if(bin == "0110"){return "6";}
        else if(bin == "0111"){return "7";}
        else if(bin == "1000"){return "8";}
        else if(bin == "1001"){return "9";}
        else if(bin == "1010"){return "A";}
        else if(bin == "1011"){return "B";}
        else if(bin == "1100"){return "C";}
        else if(bin == "1101"){return "D";}
        else if(bin == "1110"){return "E";}
        else if(bin == "1111"){return "F";}
        else throw opcode_error_exception("Invalid binary input");
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        cout << "you must supply the name of the file " <<
        "to process at the command line." << endl;
        exit(1);
	
    }

    string filename = argv[1];
    try {
        sicxe_asm doPasses(filename);
    }
    catch(file_parse_exception excpt) {
        cout << "**Sorry, error " << excpt.getMessage() << endl;
    }
    catch(opcode_error_exception excpt){
        cout << "**Sorry, error " << excpt.getMessage() << endl;
    }
    catch(symtab_exception excpt){
        cout << "**Sorry, error " << excpt.getMessage() << endl;
    }
}
