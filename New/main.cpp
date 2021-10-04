#include "main.hpp"
#include "constrained.h"

#define TICK 65536

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

    Piece Test(tetriminos::T,tetriminos::FirstRotation::T,tetriminos::SecondRotation::T,tetriminos::ThirdRotation::T);
    Test.offset.x = 5;
    Test.offset.y = 20;

    /*
    Piece pieces[5];
        pieces[0] = tetriminos::T;
        pieces[1] = tetriminos::S_l;
        pieces[2] = tetriminos::S_r;
        pieces[3] = tetriminos::O;
        pieces[4] = tetriminos::I;
    for(int i = 0; i < 5; i++){
        pieces[i].offset.x = 5;
        pieces[i].offset.y = 10;
    }
    */

    
    Rotation R = Zeroth;
    /*
    while(true){
        gameBoard.spawn(Test, R);
        while(true){
            gameBoard.draw();
            sleep(1);
            if(gameBoard.Colliders(Test, R)){
                gameBoard.undraw();
                break;
            }
            if(INPUT != '\0'){
                switch(INPUT){
                    case 'a':
                        gameBoard.left(Test, 1, R);
                        break;
                    case 'd':
                        gameBoard.right(Test, 1, R);
                        break;
                    case 'r':
                        gameBoard.despawn(Test, R);
                        operator ++(R);
                        gameBoard.spawn(Test, R);
                        
                    default:
                        break;
                }
                INPUT = '\0';
            }
            gameBoard.down(Test, 1, R);
            gameBoard.undraw();
        }
    }
    */
   int intervalCounter = 0;
    while(true){
        gameBoard.draw();
        usleep(TICK);
        intervalCounter++;
        
        if(INPUT != '\0'){
            switch(INPUT){
                case 'a':
                    if(gameBoard.Colliders(Test, R, 'l')){
                        break;
                    }
                    gameBoard.left(Test, 1, R);
                    break;
                case 'd':
                    if(gameBoard.Colliders(Test, R, 'r')){
                        break;
                    }
                    gameBoard.right(Test, 1, R);
                    break;
                case 'r':
                    gameBoard.despawn(Test, R);
                    operator ++(R);
                    gameBoard.spawn(Test, R);
                    break;
                case 's':
                    gameBoard.down(Test, 1, R);
                    break;
                        
                default:
                    break;
            }
            INPUT = '\0';
        }
        if(gameBoard.Colliders(Test, R, 'd')){
            gameBoard.undraw();
            continue;
        }        
        if(intervalCounter == 8){
            gameBoard.down(Test,1,R);
            intervalCounter = 0;
        }
        gameBoard.undraw();
    }
    return 0;
}