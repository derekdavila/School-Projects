/*
    Briana Wright, Georvi Oloan, Aaron Acosta, Derek Davila
    masc0849
    Team Minnesota
    file_parser.cc
    prog1
    CS530, Spring 2016
*/
#include "file_parser.h"
#include "file_parse_exception.h"

using namespace std;
//public methods
file_parser::file_parser(string file){
    filename = file;
}

file_parser::~file_parser(){
}

//Open file and parse tokens into columns.
void file_parser::read_file(){
    openfile();
    file_size = contents.size();

    vector<string>::iterator v_iter;
    file_parser::Tokens token;
    vector<char>::iterator char_iter;
    vector<char> *char_contents;
    int count;
    line_count = 1; // Keeps count
    //Iterate through each line.
    for(v_iter = contents.begin(); v_iter != contents.end(); v_iter++){
        //Create vector of current line.
        char_contents = new vector<char>((*v_iter).begin(),(*v_iter).end());
        char_iter = char_contents->begin();
        count = 0;

        token.label = "";
        token.opcode = "";
        token.operand = "";
        token.comment = "";

        ostringstream line_num;
        line_num << line_count;

        //Check for alphanumeric characters in label token.
        if(isalnum(*char_iter)){
            int i;
            for(i = 0; i < 8; i++){
                if(isspace(*char_iter)) break;
                if(!isalnum(*char_iter)) throw file_parse_exception("Error invalid character in label on line " + line_num.str());
                //Store in first column.
                token.label += *char_iter;
                char_iter++;
            }
            if(isalnum(*char_iter)) {
                while(!isspace(*char_iter) && char_iter != char_contents->end() && *char_iter != '.'){char_iter++;}
            }
        }
        else if(isspace(*char_iter)){
           //Checking for blank line, iterate past spaces
                while(isspace(*char_iter) && *char_iter != '.' && char_iter != char_contents->end()){char_iter++;}
           //Check for end of line
                if(char_iter == char_contents->end()) {
                        line_count ++;
                        continue;
                }
        }

        while(char_iter != char_contents->end()){
            //Iterate past spaces
            while(isspace(*char_iter) && *char_iter != '.'){
                char_iter++;
            }
            //Check for end of line
            if(char_iter == char_contents->end()) {break;}
            //Check for comment character, if found, add rest of line to column 4
            if(*char_iter == '.') {
                for(vector<char>::iterator iter = char_iter; iter != char_contents->end(); iter++)
                    token.comment += *iter;
                break;
            }
            while(!isspace(*char_iter) && char_iter != char_contents->end() && isprint(*char_iter)){
                //Check for quoted text
                if(!isspace(*char_iter) && isprint(*char_iter) && count == 0){
                    if(*char_iter == '\'' && count == 0){
                        do{
                           token.opcode += *char_iter;
                           char_iter++;
                        }while(*char_iter != '\'');
                    }
                    if(isspace(*char_iter)) {break;}
                    else{
                        token.opcode += *char_iter;
                        char_iter++;
                    }
                }
                if(!isspace(*char_iter) && isprint(*char_iter) && count == 1){
                    if (*char_iter == '\'' && count == 1){
                       do{
                           token.operand += *char_iter;
                           char_iter++;
                       }while(*char_iter != '\'');
                    }
                    if(isspace(*char_iter)){break;}
                    else{
                        token.operand += *char_iter;
                        char_iter++;
                    }
                }
                if(!isspace(*char_iter) && isprint(*char_iter) && count == 2){
                    throw file_parse_exception("Error invalid character in comment or label on line " + line_num.str());
                }
            }
            count++;
        }
        tokens.push_back(token);
        line_count++;
    }
}
//Gets token at specified row and column
string file_parser::get_token(unsigned int row, unsigned int column){
    unsigned int c = column;
    unsigned int r = row;
    Tokens gtokens = tokens.at(r);
    if (r < (unsigned)file_size && r >= 0){
    switch (c)
        {
        case 0:
            return gtokens.label;
            break;
        case 1:
            return gtokens.opcode;
            break;
        case 2:
            return gtokens.operand;
            break;
        case 3:
            return gtokens.comment;
            break;
        default:
            throw file_parse_exception("Invalid column");
            break;
        }
    }
    else
        throw file_parse_exception("Invalid row");
    return 0;
}

//Formats and prints file.
void file_parser::print_file() {
    vector<Tokens>::iterator v_iter;
    for (v_iter = tokens.begin(); v_iter != tokens.end(); v_iter++)
    {
        cout << left << setw(14) << v_iter->label
             << left << setw(14) << v_iter->opcode
             << left << setw(14) << v_iter->operand
             << left << v_iter->comment << endl;
    }
}
//Returns file size
int file_parser::size(){
    return file_size;
}

//private methods

//Reads given file into contents vector
void file_parser::openfile() {
    ifstream infile;
    infile.open(filename.c_str(), ios::in);
    if(!infile) {
        throw file_parse_exception("Error: " + filename + " does not exist. Please choose another file.");
    }
    while (getline(infile,line)) {
        contents.push_back(line);
    }
    infile.close();
}