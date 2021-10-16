#include "main.hpp"

using namespace std;

volatile char INPUT = '\0';
static struct option long_options[]{
    {"vinesauce", no_argument, NULL, 'v'},
    {NULL, 0, NULL, 0}
};
bool vinesauce_enabled = false;

vector<int> generate_bag(){
    vector<int> bag;
    for(int i = 0; i < 7; i++){
        bag.push_back(i);
    }
    random_shuffle(bag.begin(), bag.end());
    return bag;
}

int lines_cleared = 0;


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
    roster[0].init(tetriminos::I,tetriminos::FirstRotation::I,tetriminos::I,tetriminos::FirstRotation::I);
    roster[1].init(tetriminos::J,tetriminos::FirstRotation::J,tetriminos::SecondRotation::J,tetriminos::ThirdRotation::J);
    roster[2].init(tetriminos::L,tetriminos::FirstRotation::L,tetriminos::SecondRotation::L,tetriminos::ThirdRotation::L);
    roster[3].init(tetriminos::O,tetriminos::O,tetriminos::O,tetriminos::O);
    roster[4].init(tetriminos::S,tetriminos::FirstRotation::S,tetriminos::S,tetriminos::FirstRotation::S);
    roster[5].init(tetriminos::T,tetriminos::FirstRotation::T,tetriminos::SecondRotation::T,tetriminos::ThirdRotation::T);
    roster[6].init(tetriminos::Z,tetriminos::FirstRotation::Z,tetriminos::Z,tetriminos::FirstRotation::Z);

    for(int i = 0; i < 7; i++){
        roster[i].color = i+1;
    }
    
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
    vector<int> bag = generate_bag();
    int selector = 0;
    Rotation R = Zeroth;
    int intervalCounter = 0;
    Piece curr = roster[bag.at(selector)];
    while(true){
        gameBoard.draw();
        
        usleep(TICK);
        intervalCounter++;
        
        if(INPUT != '\0'){
            Rotation tmp_r = R;
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
                    ++R;
                    if(gameBoard.willClip(curr, R)){
                        --R;
                    }
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
                    lines_cleared += elim_lines.size();
                    gameBoard.Cascade(elim_lines);
                    // if(vinesauce_enabled){sound = thread(playsound);}
                };
            
            if(lines_cleared >= 8){
                TICK = TICK *.60;
                lines_cleared = 0;
            }

                // END GAME
                if(curr.offset.y > 22){
                    exit(EXIT_SUCCESS);
                }

                gameBoard.undraw();
                if(selector >= 6){
                    bag = generate_bag();
                    selector = 0;
                }
                else{
                    selector++;
                }
                curr = roster[bag.at(selector)];
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