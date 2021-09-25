#include "main.hpp"
#include "constrained.h"

using namespace std;

volatile char INPUT = '\0';

int main(){
    Field gameBoard;
    auto inputEvent = [](){
        while(true){
            INPUT = getch();
        }
    };
    thread handler(inputEvent);

    Piece Test;
    Test = tetriminos::T;
    Test.offset.x = 5;
    Test.offset.y = 20;

    Piece pieces[5];
    for(int i = 0; i < 5; i++){
        pieces[i] = tetriminos::T;
        pieces[i].offset.x = 5;
        pieces[i].offset.y = 10;
    }

    
    int revolver = 0;
    int loop = 0;
    while(true){
        gameBoard.spawn(pieces[loop]);
        while(true){
            gameBoard.draw();
            sleep(1);
            if(gameBoard.Colliders(pieces[loop])){
                gameBoard.undraw();
                break;
            }
            if(INPUT != '\0'){
                switch(INPUT){
                    case 'a':
                        gameBoard.left(pieces[loop],1);
                        break;
                    case 'd':
                        gameBoard.right(pieces[loop],1);
                        break;
                    case 'r':
                        switch(revolver){
                            case 0:
                                gameBoard.replace(pieces[loop], tetriminos::FirstRotation::T);
                                revolver++;
                                break;
                            case 1:
                                gameBoard.replace(pieces[loop], tetriminos::SecondRotation::T);
                                revolver++;
                                break;
                            case 2:
                                gameBoard.replace(pieces[loop], tetriminos::ThirdRotation::T);
                                revolver++;
                                break;
                            case 3:
                                gameBoard.replace(pieces[loop], tetriminos::T);
                                revolver = 0;
                                break;
                            default:
                                break;
                        }
                }
                INPUT = '\0';
            }
            gameBoard.down(pieces[loop],1);
            gameBoard.undraw();
        }
        loop++;
    }
    

    return 0;
}