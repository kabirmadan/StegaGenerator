#include <iostream> //cin, cout
#include <sstream>  //stringstream
#include <string>   //string
#include <vector>   //vector


/*For readability of main*/
std::string getStrInput(){
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
 * (because hex color codes are 6 characters long)*/
std::string evenSix(std::string str){
    int length {static_cast<int>(str.length())};
    int zerosToAppend { 6 - (length % 6) };
    std::cout << "Adding " << zerosToAppend << " digits";
    for (int i{0}; i < zerosToAppend; ++i){
        str += "0";                                                           //append empty characters (0s because #000000 is RGB(0,0,0) aka black)
    }
    return str;
}


int main(){
    std::string input {getStrInput()};
    int length {getStrLength(input)};
    //std::vector<char> charArr{input.begin(), input.end()};                  //copy user input to char vector
    std::string hexString{};

    for (char c : input) hexString += getHexStr(c);                           //convert each char to hex and append

    input = "";                                                               //string copy of input no longer needed

    std::cout << "\n\nLength of input: " << length << " characters\n";        //print # of characters in input
    std::cout << "Hex: " << hexString << "\n";                                //print full hex conversion
    std::cout << "Hex length: " << getStrLength(hexString) << " characters\n";//print # of characters in hex representation

    if (getStrLength(hexString) % 6 != 0){                                    //make length of hex string a multiple of 6 if necessary (see eve) + print any changes
        hexString = evenSix(hexString);
        std::cout << "\nNew Hex: " << hexString << "\n";
        std::cout << "New Hex length: " << getStrLength(hexString) << "\n";

    }

    std::vector<std::string> hexValues{};                                    //array to store 6-digit hex values
    hexValues.resize( (getStrLength(hexString) / 6) );                   //resize array
    int idx{0};
    for (int i{0}; i < (getStrLength(hexString)); i+=6){                     //add hex values to array
        hexValues[idx] = hexString.substr(i, 6);
        ++idx;
    }

    std::cout << "Hex values array: [ ";                                    //print hex values array
    for (int i{0}; i < static_cast<int>(std::size(hexValues)); ++i){
        std::cout << hexValues[i] << " ";
    }
    std::cout << "]";

    return 0;
}