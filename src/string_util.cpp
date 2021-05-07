#include <iostream>             //cin, cout
#include <sstream>              //sstream
#include <string>               //string

#include "string_util.h"        //forward declarations


/*For readability of main -
 * get regular single line input*/
std::string getTitle(){
    std::string title{};
    std::cout << "Title: ";
    getline(std::cin, title);
    return title;
}


/*For readability of main -
 * get long input, delimited by ~ character
 * (to allow for multiple paragraph / long form input)*/
std::string getLongTextInput(){
    std::string input{};
    std::cout << "Type something (press  tilde + enter  to complete):\n";
    getline(std::cin, input, '~');
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
 * by appending values (because hex codes are 6 characters long) -
 * appends 0s because #000000 is RGB(0,0,0) aka black*/
std::string evenSix(std::string str){
    int length {static_cast<int>(str.length())};
    int zerosToAppend { 6 - (length % 6) };
    //std::cout << "Adding " << zerosToAppend << " digits\n";               //uncomment for debugging
    for (int i{0}; i < zerosToAppend; ++i){
        str += "0";
    }
    return str;
}