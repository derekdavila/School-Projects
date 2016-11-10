/*
    Georvi Oloan, Aaron Acosta, Derek Davila, Briana Wright
    masc0849
    Team Minnesota
    prog4
    CS530, Spring 2016
*/

//sicxe_asm.h
#ifndef SICXE_ASM_H
#define SICXE_ASM_H

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
#include <vector>
#include <cctype>

using namespace std;

class sicxe_asm {

    public:
        sicxe_asm(string);
        ~sicxe_asm();

    private:
        //First Pass:
        void passOne(file_parser, string);
        void write_file(string);
        string to_string(int);
        string to_upper(string);
        string hex_format(int);
        int locctr;
        int base;
        string base_operand;
        int startaddress;
        symtab symboltable;
        opcodetab opcodetable;
        struct Listing_File{
            int line;
            string address;
            string label;
            string opcode;
            string operand;
            string comment;
            string machinecode;
        };
        vector<Listing_File> contents;

        //Second Pass:
        void passTwo(string);
        string formatOne(string);
        string formatTwo(string, string);
        string formatThree(string, string, string);
        string formatFour(string, string, string);

        string set_ni(string);
        string set_x(string);
        string set_xbpe(string, string, string);
        int set_displacement(string,string);
        string machine_code(string, string,string);
        string binary_to_hex(string);
        string ignore_signs(string);
        int hex_to_int(string);

        string opHandler(char);
        string int_to_hex(int,int);
        int string_to_int(string);


};
#endi