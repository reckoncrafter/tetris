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
        pieces[0] = tetriminos::T;
        pieces[1] = tetriminos::S_l;
        pieces[2] = tetriminos::S_r;
        pieces[3] = tetriminos::O;
        pieces[4] = tetriminos::I;
    for(int i = 0; i < 5; i++){
        pieces[i].offset.x = 5;
        pieces[i].offset.y = 10;
    }
    Piece pieces_r0[5];
        pieces_r0[0] = tetriminos::T;
        pieces_r0[1] = tetriminos::S_l;
        pieces_r0[2] = tetriminos::S_r;
        pieces_r0[3] = tetriminos::O;
        pieces_r0[4] = tetriminos::I;
    Piece pieces_r1[5];
        pieces_r1[0] = tetriminos::FirstRotation::T;
        pieces_r1[1] = tetriminos::FirstRotation::S_l;
        pieces_r1[2] = tetriminos::FirstRotation::S_r;
        pieces_r1[3] = tetriminos::O;
        pieces_r1[4] = tetriminos::FirstRotation::I;
    Piece pieces_r2[5];
        pieces_r2[0] = tetriminos::SecondRotation::T;
        pieces_r2[1] = tetriminos::S_l;
        pieces_r2[2] = tetriminos::S_r;
        pieces_r2[3] = tetriminos::O;
        pieces_r2[4] = tetriminos::I;
    Piece pieces_r3[5];
        pieces_r3[0] = tetriminos::ThirdRotation::T;
        pieces_r3[1] = tetriminos::FirstRotation::S_l;
        pieces_r3[2] = tetriminos::FirstRotation::S_r;
        pieces_r3[3] = tetriminos::O;
        pieces_r3[4] = tetriminos::FirstRotation::I;
    
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
                                gameBoard.replace(pieces[loop], pieces_r1[loop].shape);
                                revolver++;
                                break;
                            case 1:
                                gameBoard.replace(pieces[loop], pieces_r2[loop].shape);
                                revolver++;
                                break;
                            case 2:
                                gameBoard.replace(pieces[loop], pieces_r3[loop].shape);
                                revolver++;
                                break;
                            case 3:
                                gameBoard.replace(pieces[loop], pieces_r0[loop].shape);
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