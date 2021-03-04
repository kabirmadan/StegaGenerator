#include <cmath>         //floor, sqrt
#include <string>        //string

#include "rgb.h"         //forward declarations


/*Returns the red RGB value from a hex string*/
uint_fast8_t getRed(std::string hexValStr){
    int hexValInt {std::stoi(hexValStr, nullptr, 16)};              //gets base-16 integer from hex string
    uint_fast8_t red = hexValInt >> 16;                                       //shifts #RRGGBB to #RR
    return red;
}


/*Returns the green RGB value from a hex string*/
uint_fast8_t getGreen(std::string hexValStr){
    int hexValInt {std::stoi(hexValStr, nullptr, 16)};
    uint_fast8_t green = (hexValInt >> 8) & 0x00ff;                           //shifts #RRGGBB to #RRGG and masks for #GG
    return green;
}


/*Returns the blue RGB value from a hex string*/
uint_fast8_t getBlue(std::string hexValStr){
    int hexValInt {std::stoi(hexValStr, nullptr, 16)};
    uint_fast8_t blue = hexValInt & 0x0000ff;                                 //masks #RRGGBB for #BB
    return blue;
}


/*Returns true if n is a perfect square, false otherwise*/
bool isPerfectSquare(int n){
    if (std::floor(std::sqrt(n)) == std::sqrt(n))
        return true;
    return false;
}


/*Determines the height / width of the bitmap
 * if the number of specified hex values is a perfect square then that value's square root is returned
 * otherwise the square root of the next greatest perfect square is returned
 * ensures that all of the specified pixel values fit onto a square canvas*/
int getDimension(int numHexValues){
    int d = isPerfectSquare(numHexValues) ?
            (static_cast<int>(std::sqrt(numHexValues))) : (static_cast<int>(std::floor(std::sqrt(numHexValues)) + 1));
    return d;
}