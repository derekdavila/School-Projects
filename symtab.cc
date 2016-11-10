/*
    Georvi Oloan, Aaron Acosta, Derek Davila, Briana Wright
    masc0849
    Team Minnesota
    prog3
    CS530, Spring 2016
*/
//symtab.cc
#include "symtab.h"
#include "symtab_exception.h"

using namespace std;

void symtab::insert(string label, string value) {
    if(sym.find(label) == sym.end()) {
        sym[label] = value;
    }
    else {throw symtab_exception("Symbol is already defined."); }
    }

bool symtab::contains(string symbol) {
    if(sym.find(symbol) != sym.end()) {return true;}
        else {return false;}
    }

string symtab::sym_address(string symbol) {
    if(sym.find(symbol) != sym.end()) {
        return sym[symbol];
    }
        else {throw symtab_exception("Symbol not found.");}
}