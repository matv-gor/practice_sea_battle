#include"ship.hpp"
/*
0 - пустая клетка(ставить можно)
1 - вокруг корабля(ставить нельзя) 
2 - "живая" клетка корабля
3 - "мертвая клетка корабля "
*/
int Board::createShip(int inputSize,int inputNumberStart,char inputLetterStart,int inputNumberEnd,char inputLetterEnd){
    int numberInstedLetterStart = inputLetterStart-Constants::kLetterToNumberSymbol;//Начинается с 0(перевод буквы к цифре)
    int numberInstedLetterEnd = inputLetterEnd - Constants::kLetterToNumberSymbol;
    if (numberInstedLetterEnd==numberInstedLetterStart){
    for(int i{0};i<inputSize;++i){//можно обернуть в отдельную функцию
        editBoard(inputNumberStart+1,numberInstedLetterStart,2);
        } 
        
    for(int i{0};i<inputSize+2;++i){
        for(int j{-1};j<1;++j){
            editBoard(inputNumberStart+i-1,numberInstedLetterStart+j,1);
        }
    }
    } else if(inputNumberStart==inputLetterEnd){
        for(int i{0};i<inputSize;++i){//аналогично можно обернуть
        editBoard(inputNumberStart,numberInstedLetterStart+i,2);
        }
        
    for(int i{0};i<inputSize;++i){
        for(int j{-1};j<1;++j){
            editBoard(inputLetterStart+j,numberInstedLetterEnd+i-1,1);
        }
    }
    }

}

void Board::editBoard(int inputNumber,int numberInstedLetter,int outputValueCell){
    if(board[inputNumber][numberInstedLetter]==2||board[inputNumber][numberInstedLetter]==3){
        return;
    }
    board[inputNumber][numberInstedLetter]=outputValueCell;
}