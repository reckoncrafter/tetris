#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <getopt.h>
#include <thread>
#include <random>
#include <time.h>
#include <vector>
#include <algorithm>

static int TICK = 65536;

char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

enum Rotation{Zeroth = 0, First = 4, Second = 8, Third = 12};
Rotation& operator ++(Rotation& r){
    switch(r){
        case Zeroth:
            r = First;
            break;
        case First:
            r = Second;
            break;
        case Second:
            r = Third;
            break;
        case Third:
            r = Zeroth;
            break;
        default:
            break;
    }
    return r;
}
Rotation& operator --(Rotation& r){
    switch(r){
        case Zeroth:
            r = Third;
            break;
        case First:
            r = Zeroth;
            break;
        case Second:
            r = First;
            break;
        case Third:
            r = Second;
            break;
        default:
            break;
    }
    return r;
}

struct point{
    int x,y;
};

class Field;

namespace tetriminos{
    point J[4] = {{0,1}, {1,1}, {2,1}, {2,0}};
    // [*][*][*]
    //  +    [*]
    point L[4] = {{0,0}, {0,1}, {1,1}, {2,1}};
    // [*][*][*]    
    // [+]
    point O[4] = {{0,0}, {0,1}, {1,0}, {1,1}};
    // [*][*]
    // [+][*]
    point I[4] = {{0,1}, {1,1}, {2,1}, {3,1}};
    // [*][*][*][*]
    //  +
    point Z[4] = {{1,0}, {0,1}, {1,1}, {2,0}};
    // [*][*]
    //  + [*][*]
    point S[4] = {{0,0}, {1,0}, {1,1}, {2,1}};
    //    [*][*]
    // [+][*]
    point T[4] = {{1,0}, {0,1}, {1,1}, {2,1}};
    // [*][*][*]
    //  + [*]
    namespace FirstRotation{
        point J[4] = {{0,0}, {1,0}, {1,1}, {1,2}};
        //    [*]
        //    [*] 
        // [+][*]
        point L[4] = {{1,0}, {1,1}, {1,2}, {0,2}};
        // [*][*]
        //    [*]
        //  + [*]
        point I[4] = {{2,0}, {2,1}, {2,2}, {2,3}};
        //     [*]
        //     [*]
        //     [*]
        // +   [*]
        point Z[4] = {{1,0}, {1,1}, {2,1}, {2,2}};
        //      [*]
        //   [*][*]
        // + [*]
        point S[4] = {{2,0}, {2,1}, {1,1}, {1,2}};
        //   [*]
        //   [*][*]
        // +    [*]
        point T[4] = {{1,0}, {0,1}, {1,1}, {1,2}};
        //    [*]
        // [*][*]
        //  + [*]
    };
    namespace SecondRotation{
        point J[4] = {{0,1}, {1,1}, {2,1}, {0,2}};
        // [*]
        // [*][*][*]
        //  +
        point L[4] = {{0,1}, {1,1}, {2,1}, {2,2}};
        //       [*]
        // [*][*][*]
        //  +
        point T[4] = {{0,1}, {1,1}, {2,1}, {1,2}};
        //    [*]
        // [*][*][*]
        //  +
    };
    namespace ThirdRotation{
        point J[4] = {{1,0}, {1,1}, {1,2}, {2,2}};
        //    [*][*]
        //    [*]
        //  + [*]
        point L[4] = {{1,0}, {2,0}, {1,1}, {1,2}}; 
        //    [*]
        //    [*]
        //  + [*][*]
        point T[4] = {{1,0}, {1,1}, {1,2}, {2,1}};
        //    [*]
        //    [*][*]
        //  + [*]
    }
};

class Piece{
public:
    int revolver = 0;
    point shape[4*4];

    point offset;
    int color;

    Piece(){
        for(int i = 0; i < 16; i++){
            shape[i] = {0,0};
        }
    }
    void init(point r0[4], point r1[4], point r2[4], point r3[4]){
        for(int i = 0; i < 4; i++){
            shape[i] = r0[i];
        }
        for(int i = 0; i < 4; i++){
            shape[i+4] = r1[i];
        }
        for(int i = 0; i < 4; i++){
            shape[i+8] = r2[i];
        }
        for(int i = 0; i < 4; i++){
            shape[i+12] = r3[i];
        }
    };
    
    void down(Field F, int I);
    void up(Field F, int I);
    void left(Field F, int I);
    void right(Field F, int I);
};

class Field{
    public:
    int grid[12][27] = {0};

    Field(){
        for(int i = 0; i < 27; i++){
            grid[0][i] = 9;
            grid[11][i] = 9;
        }
        for(int i = 0; i < 12; i++){
            grid[i][0] = 9;
            grid[i][27] = 9;
        }
    }
    void spawn(Piece P, Rotation r){
        for(int i = 0; i < 4; i++){
            grid[P.shape[i+r].x + P.offset.x][P.shape[i+r].y + P.offset.y] = P.color;
        }
    }
    void despawn(Piece P, Rotation r){
        for(int i = 0; i < 4; i++){
            grid[P.shape[i+r].x + P.offset.x][P.shape[i+r].y + P.offset.y] = 0;
        }
    }
    void down(Piece &P, int I, Rotation r){
        despawn(P,r);
        P.offset.y -= I;
        spawn(P,r);
    }
    void up(Piece &P, int I, Rotation r){
        despawn(P,r);
        P.offset.y += I;
        spawn(P,r);
    }
    void right(Piece &P, int I, Rotation r){
        despawn(P,r);
        P.offset.x += I;
        spawn(P,r);
    }
    void left(Piece &P, int I, Rotation r){
        despawn(P,r);
        P.offset.x -= I;
        spawn(P,r);
    }
    void draw(){
        for(int i = 27; i >= 0; i--){
            for(int j = 0; j < 12; j++){
                
                switch(grid[j][i]){
                    case 0:
                        std::cout <<"\033[0;30m" << u8"\u2588\u2588\u2588" << "\033[0m";
                        break;
                    case 1: // CYAN
                        std::cout << "\033[38;5;51m" << u8"\u2588\u2588\u2588" << "\033[0m";
                        break;
                    case 2: // BLUE
                        std::cout << "\033[38;5;21m" << u8"\u2588\u2588\u2588" << "\033[0m";
                        break;
                    case 3: // ORANGE
                        std::cout << "\033[38;5;166m" << u8"\u2588\u2588\u2588" << "\033[0m";
                        break;
                    case 4: // YELLOW
                        std::cout << "\033[38;5;184m" << u8"\u2588\u2588\u2588" << "\033[0m";
                        break;
                    case 5: // GREEN
                        std::cout << "\033[38;5;34m" << u8"\u2588\u2588\u2588" << "\033[0m";
                        break;
                    case 6: // PURPLE
                        std::cout << "\033[38;5;56m" << u8"\u2588\u2588\u2588" << "\033[0m";
                        break;
                    case 7: // RED
                        std::cout << "\033[38;5;124m" << u8"\u2588\u2588\u2588" << "\033[0m";
                        break;
                    case 9: // 9 is wall
                        std::cout << u8"\u2588\u2588\u2588";
                        break;
                    default:
                        std::cout << u8"\u2588\u2588\u2588";
                        break;
                }
                
            }
            std::cout << std::endl;
        }
    }
    void undraw(){
        for(int i = 0; i < 27+1; i++){
            std::cout << "\033[A" << "\r";
        }
    }
   
    void pointswap(int &x, int &y){
        int temp = x;
        x = y;
        y = temp;
    }
    void Cascade(std::vector<int> &lines){
        auto c = lines.rbegin();
        do{
            for(int i = *c; i < 27; i++){
                for(int j = 1; j < 11; j++){
                    if(grid[j][i] != 0 && i != 1){
                        pointswap(grid[j][i], grid[j][i-1]);
                    }
                }
            }
            // visualizing cascade
            undraw();
            draw();
            usleep(TICK*2);
            c++;
        } while(c != lines.rend());
        lines.clear();
    }
    bool Eliminate(std::vector<int> &lines){
        bool elim = true;
        bool rtn = false;
        for(int i = 1; i < 27; i++){
            elim = true;
            for(int j = 1; j < 11; j++){
                if(grid[j][i] == 0){
                    elim = false;
                }
            }
            if(elim){
                for(int j = 1; j < 11; j++){
                    grid[j][i] = 0;
                }
                // visualizing line elimination
                undraw();
                draw();
                usleep(TICK*2);
                lines.push_back(i);
                rtn = true;
            }
        }
        return rtn;
    }
    bool willClip(const Piece P, Rotation r){
        bool rtn = false;
        for(int i = 0; i < 4; i++){
            if(grid[P.shape[i+r].x+P.offset.x][P.shape[i+r].y+P.offset.y] != 0){
                rtn = true;
            }
        }
        return rtn;
    }
    bool Colliders(const Piece P, Rotation r, char side){
        point* colliders = new point[4]();
        bool collision = false;
        switch(side){
            case 'd': // below (down)
                for(int i = 0; i < 4; i++){
                    grid[P.shape[i+r].x+P.offset.x][P.shape[i+r].y+P.offset.y] = 10;
                }
                for(int i = 0; i < 4; i++){
                    int* below = &grid[P.shape[i+r].x+P.offset.x][P.shape[i+r].y+P.offset.y-1];
                                                                                        //  ^
                    if(*below != 10 && *below != 0){
                        collision = true;
                    }
                }
                for(int i = 0; i < 4; i++){
                    grid[P.shape[i+r].x+P.offset.x][P.shape[i+r].y+P.offset.y] = P.color;
                }
                break;
            case 'l': // left side
                for(int i = 0; i < 4; i++){
                    grid[P.shape[i+r].x+P.offset.x][P.shape[i+r].y+P.offset.y] = 10;
                }
                for(int i = 0; i < 4; i++){
                    int* atleft = &grid[P.shape[i+r].x+P.offset.x-1][P.shape[i+r].y+P.offset.y];
                                                                //^
                    if(*atleft != 10 && *atleft != 0){
                        collision = true;
                    }
                }
                for(int i = 0; i < 4; i++){
                    grid[P.shape[i+r].x+P.offset.x][P.shape[i+r].y+P.offset.y] = P.color;
                }
                break;

            case 'r': // right side
                for(int i = 0; i < 4; i++){
                    grid[P.shape[i+r].x+P.offset.x][P.shape[i+r].y+P.offset.y] = 10;
                }
                for(int i = 0; i < 4; i++){
                    int* atright = &grid[P.shape[i+r].x+P.offset.x+1][P.shape[i+r].y+P.offset.y];
                                                                // ^
                    if(*atright != 10 && *atright != 0){
                        collision = true;
                    }
                }
                for(int i = 0; i < 4; i++){
                    grid[P.shape[i+r].x+P.offset.x][P.shape[i+r].y+P.offset.y] = P.color;
                }
                break;
            default:
                break;
        }
        return collision;
    }
};
