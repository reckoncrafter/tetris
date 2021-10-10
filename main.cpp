#include "main.hpp"
#include "constrained.h"

using namespace std;

volatile char INPUT = '\0';
static struct option long_options[]{
    {"vinesauce", no_argument, NULL, 'v'},
    {NULL, 0, NULL, 0}
};
bool vinesauce_enabled = false;


int main(int argc, char** argv){
    int c;
    int option_idx = 0;
    while ((c = getopt_long(argc, argv, "v", long_options, &option_idx)) != -1){
        switch(c){
            case 'v':
                vinesauce_enabled = true;
                break;
            default:
                break;
        }
    }



    srand(time(NULL));
    Field gameBoard;
    auto inputEvent = [](){
        while(true){
            INPUT = getch();
        }
    };
    /*
    auto playsound = [](){
        system("cvlc vinesauce.mp3 2&> /dev/null");
    };
    */
    thread handler(inputEvent);
    thread sound;
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
        roster[i].offset.y = 23;
    }
    

    /*
    Piece test;
    test.init(tetriminos::S_l,tetriminos::FirstRotation::S_l,tetriminos::S_l,tetriminos::FirstRotation::S_l);
    test.offset.x = 5;
    test.offset.y = 10;
    */
    int sel = rand()%7;
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
                    if(gameBoard.Colliders(curr, R, 'd')){
                        break;
                    }
                    gameBoard.down(curr, 1, R);
                    break;
                default:
                    break;
            }
            INPUT = '\0';
        } 
        if(intervalCounter >= 8){
            // collider check block moved here
            // because it allows block sliding
            // by only checking the colliders on the interval
            // and checking before the next move down
            if(gameBoard.Colliders(curr, R, 'd')){
                if(gameBoard.Eliminate(elim_lines)){
                    gameBoard.Cascade(elim_lines);
                    // if(vinesauce_enabled){sound = thread(playsound);}
                };

                // END GAME
                if(curr.offset.y > 22){
                    exit(EXIT_SUCCESS);
                }

                gameBoard.undraw();
                sel = rand()%7;
                curr = roster[sel];
                continue;
                // breaking here causes a core dump
            }       
            gameBoard.down(curr,1,R);
            intervalCounter = 0;
        }
        gameBoard.undraw();
    }
    handler.~thread();
    return 0;
}