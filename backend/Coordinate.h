#pragma once
#include "constants.h"
class Coordinate
{
private:
    int row;
    int col;
public:
    Coordinate():row(0),col(0){}
    Coordinate(int r,int c):row(r),col(c){}
    Coordinate(int number,char letter):row(number),col(letter-Constants::kLetterToNumberSymbol){}
    int getRow() const{return row;}
    int getCol() const{return col;}
};
