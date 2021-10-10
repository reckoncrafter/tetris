#include "main.hpp"
#include "constrained.h"

#define TICK 65536

using namespace std;

volatile char INPUT = '\0';

int main(){
    srand(time(NULL));
    Field gameBoard;
    auto inputEvent = [](){
        while(true){
            INPUT = getch();
        }
    };
    thread handler(inputEvent);
    vector<int> elim_lines;
    
    Piece roster[7];
    roster[0].init(tetriminos::L_l,tetriminos::FirstRotation::L_l,tetriminos::SecondRotation::L_l,tetriminos::ThirdRotation::L_l);
    roster[1].init(tetriminos::L_r,tetriminos::FirstRotation::L_r,tetriminos::SecondRotation::L_r,tetriminos::ThirdRotation::L_r);
    roster[2].init(tetriminos::O,tetriminos::O,tetriminos::O,tetriminos::O);
    roster[3].init(tetriminos::I,tetriminos::FirstRotation::I,tetriminos::I,tetriminos::FirstRotation::I);
    roster[4].init(tetriminos::S_l,tetriminos::FirstRotation::S_l,tetriminos::S_l,tetriminos::FirstRotation::S_l);
    roster[5].init(tetriminos::S_r,tetriminos::FirstRotation::S_r,tetriminos::S_r,tetriminos::FirstRotation::S_r);
    roster[6].init(tetriminos::T,tetriminos::FirstRotation::T,tetriminos::SecondRotation::T,tetriminos::ThirdRotation::T);
    
    for(int i = 0; i < 7; i++){
        roster[i].offset.x = 5;
        roster[i].offset.y = 20;
    }
    

    /*
    Piece test;
    test.init(tetriminos::S_l,tetriminos::FirstRotation::S_l,tetriminos::S_l,tetriminos::FirstRotation::S_l);
    test.offset.x = 5;
    test.offset.y = 10;
    */
    int sel = 3;
    Rotation R = Zeroth;
    int intervalCounter = 0;
    Piece curr = roster[sel];
    while(true){
        gameBoard.draw();
        usleep(TICK);
        intervalCounter++;
        
        if(INPUT != '\0'){
            switch(INPUT){
                case 'a':
                    if(gameBoard.Colliders(curr, R, 'l')){
                        break;
                    }
                    gameBoard.left(curr, 1, R);
                    break;
                case 'd':
                    if(gameBoard.Colliders(curr, R, 'r')){
                        break;
                    }
                    gameBoard.right(curr, 1, R);
                    break;
                case 'r':
                    gameBoard.despawn(curr, R);
                    operator ++(R);
                    gameBoard.spawn(curr, R);
                    break;
                case 's':
                    gameBoard.down(curr, 1, R);
                    break;
                default:
                    break;
            }
            INPUT = '\0';
        }
        if(gameBoard.Colliders(curr, R, 'd')){
            if(gameBoard.Eliminate(elim_lines)){
                gameBoard.Cascade(elim_lines);
            };
            gameBoard.undraw();
            //sel = rand()%6;
            curr = roster[sel];
            continue;
            // breaking here causes a core dump
        }        
        if(intervalCounter == 8){
            gameBoard.down(curr,1,R);
            intervalCounter = 0;
        }
        gameBoard.undraw();
    }
    return 0;
}