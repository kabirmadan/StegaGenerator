#include <iostream>                             //cout
#include <string>                               //string
#include <vector>                               //vector

#include "rgb.h"                                //getRed, getGreen, getBlue, getDimension
#include "string_util.h"                        //getTitle, getLongTextInput, getStrLength, getHexStr, evenSix

#include "third_party/bitmap_image.hpp"         //bitmap_image, set_all_channels, set_pixel, save_image


/*Print number of characters in text input and in hex conversion*/
void printTextDetails(std::string input, std::string hexString){
    std::cout <<"\n\nLength of input: " << getStrLength(input) << " characters\n";
    std::cout << "Hex length: " << getStrLength(hexString) << " characters\n";
}


/*Print hex values (strings)*/
void printHexValues (std::vector<std::string> hexValues){
    std::cout << "Hex values: [ ";
    for (std::string value : hexValues){
        std::cout << value << " ";
    }
    std::cout << "]\n";
}


/*Print RGB values (8 bit ints)*/
void printRgbValues (std::vector<std::string> hexValues){
    std::cout << "RGB values: ";
    for(std::string value : hexValues){
        std::cout << "("
                  << static_cast<unsigned>(getRed(value)) << ","
                  << static_cast<unsigned>(getGreen(value)) << ","
                  << static_cast<unsigned>(getBlue(value)) << ") ";
    }
    std::cout << "\n";
}


int main(){

    //variable declarations
    std::string filename {getTitle() + ".bmp"};
    std::string input {getLongTextInput()};
    std::string hexString{};
    std::vector<std::string> hexValues{};
    int numRows;
    int numCols;
    int idx;

    //convert each char to hex and append
    for (char c : input) hexString += getHexStr(c);

    printTextDetails(input, hexString);

    //make length of full hex string a multiple of 6 if necessary + print changes
    if (getStrLength(hexString) % 6 != 0){
        hexString = evenSix(hexString);
        std::cout << "New Hex length: " << getStrLength(hexString) << "\n";
    }

    //break apart full hex string into lengths of 6 (hex color codes) and add to hexValues array
    hexValues.resize( (getStrLength(hexString) / 6) );
    idx = 0;
    for (int i{0}; i < (getStrLength(hexString)); i+=6){
        hexValues[idx] = hexString.substr(i, 6);
        ++idx;
    }

    printHexValues(hexValues);
    printRgbValues(hexValues);

    //set and print number of rows and columns of bitmap image
    numRows = getDimension(static_cast<int>(std::size(hexValues)));
    numCols = numRows;
    std::cout << "Canvas: " << numRows << "x" << numCols;


    /*--------------------------image--------------------------*/

    //create bitmap image + set all pixels to placeholder value
    bitmap_image image (numCols,numRows);
    image.set_all_channels(255,255,255);

    //set color of pixels from left to right
    idx = 0;
    for (int row{0}; row < numRows; ++row){
        for (int col{0}; col < numCols; ++col){
            if (idx <= (static_cast<int>(std::size(hexValues)))){
                try {
                    image.set_pixel(col, row, getRed(hexValues[idx]), getGreen(hexValues[idx]), getBlue(hexValues[idx]));
                }
                catch (...) {
                    std::cout << "";
                }
                ++idx;
            }
        }
    }

    //save image
    image.save_image(filename);

    return 0;

}