#include <iostream>      //cin, cout
#include <sstream>       //sstream
#include <string>        //string

#include "string_util.h" //forward declarations


/*For readability of main*/
std::string getTitle(){
    std::string title{};
    std::cout << "Title: ";
    getline(std::cin, title);
    return title;
}


/*For readability of main*/
std::string getLongTextInput(){
    std::string input{};
    std::cout << "Type something (press  tilde + enter  to complete):\n";
    getline(std::cin, input, '~');                                  //user input delimited by ~ character
    return input;
}


/*For readability of main
 * (clean out casting stuff)*/
int getStrLength(std::string input){
    int length {static_cast<int>(input.length())};
    return length;
}


/*Convert single char to hex, return as string
 * Process as stringstream because of hex manipulator*/
std::string getHexStr(char c){
    std::stringstream sstr;
    sstr << std::hex << static_cast<int>(c);
    return sstr.str();
}


/*Ensures that a string's length is a multiple of 6
 * by appending values (because hex codes are 6 characters long)*/
std::string evenSix(std::string str){
    int length {static_cast<int>(str.length())};
    int zerosToAppend { 6 - (length % 6) };
    std::cout << "Adding " << zerosToAppend << " digits";
    for (int i{0}; i < zerosToAppend; ++i){
        str += "0";                                                           //append empty characters (0s because #000000 is RGB(0,0,0) aka black)
    }
    return str;
}