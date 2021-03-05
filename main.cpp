#include <iostream>                             //cout
#include <string>                               //string
#include <vector>                               //vector

#include "rgb.h"                                //getRed, getGreen, getBlue, getDimension
#include "string_util.h"                        //getTitle, getLongTextInput, getStrLength, getHexStr, evenSix

#include "third_party/bitmap_image.hpp"         //bitmap_image, set_all_channels, set_pixel, save_image


int main(){
    std::string filename {getTitle() + ".bmp"};
    std::string input {getLongTextInput()};

    std::string hexString{};
    for (char c : input) hexString += getHexStr(c);                           //convert each char to hex and append

    std::cout << "\n\nLength of input: " << getStrLength(input) << " characters\n";
    std::cout << "Hex: " << hexString << "\n";                                //print full hex conversion
    std::cout << "Hex length: " << getStrLength(hexString) << " characters\n";//print # of characters in hex representation

    input = "";                                                               //string copy of input no longer needed

    if (getStrLength(hexString) % 6 != 0){                                    //make length of hex string a multiple of 6 if necessary (see evenSix()) + print any changes
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

    std::cout << "Hex values array: [ ";                                    //print hex values array (strings)
    for (int i{0}; i < static_cast<int>(std::size(hexValues)); ++i){
        std::cout << hexValues[i] << " ";
    }
    std::cout << "]\n";

    std::cout << "RGB values: ";                                            //print RGB values (8 bit ints)
    for(std::string value : hexValues){
        std::cout << "("
        << static_cast<unsigned>(getRed(value)) << ","
        << static_cast<unsigned>(getGreen(value)) << ","
        << static_cast<unsigned>(getBlue(value)) << ") ";
    }

    int numRows { getDimension(static_cast<int>(std::size(hexValues))) };   //set number of rows of bitmap image
    int numCols = numRows;                                                  //and number of columns

    std::cout << "\nCanvas: " << numRows << "x" << numCols;

    /*--------------------------------------------------*/

    bitmap_image image (numCols,numRows);                                  //create bitmap image
    image.set_all_channels(255,255,255);           //set all pixels to white - placeholder pixels

    idx = 0;
    for (int row = 0; row < numRows; ++row){
        for (int col = 0; col < numCols; ++col){
            if (idx <= (static_cast<int>(std::size(hexValues)))){
                try {                                                     //set each pixel color based on hex values
                    image.set_pixel(col, row, getRed(hexValues[idx]), getGreen(hexValues[idx]), getBlue(hexValues[idx]));
                }
                catch (...) {                                             //fixes stoi newline conversion bug
                    std::cout << "";
                }
                ++idx;
            }
        }
    }

    image.save_image(filename);                                          //save image

    return 0;
}