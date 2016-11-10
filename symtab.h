/*
    Georvi Oloan, Aaron Acosta, Derek Davila, Briana Wright
    masc0849
    Team Minnesota
    prog3
    CS530, Spring 2016
*/
//symtab.h
#ifndef SYMTAB_H
#define SYMTAB_H

#include <string>
#include <map>
#include <utility>
#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std;

class symtab {

    public:
        void insert(string, string);
        bool contains(string);
        string sym_address(string);

    private:
        map <string,string> sym;

};

#endif