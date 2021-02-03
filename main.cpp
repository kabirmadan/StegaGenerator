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


/*For readability of main*/
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


int main(){
    std::string input {getStrInput()};
    int length {getStrLength(input)};
    std::vector<char> charArr{input.begin(), input.end()};                      //copy user input to char vector
    std::string hexString{};

    input = "";                                                                 //string copy of input no longer needed

    for (char c : charArr) hexString += getHexStr(c);                           //convert each char to hex and append

    std::cout << "\n\nLength of input: " << length << " characters\n";          //print # of characters in input
    std::cout << "Hex: " << hexString;                                          //print full hex conversion
    return 0;
}