#define MAIN
#include <unistd.h>
#include <termios.h>
#include <string>
#include <iostream>
#define T_X 3
#define T_Y 4

struct xy{
    int x;
    int y;
};

class Grid{
    public:
        int** grid;
    public:
        xy dim;
        Grid(xy dimensions){
            grid = new int*[dimensions.x];
            for(int i = 0; i < dimensions.x; i++){
                grid[i] = new int[dimensions.y];
            }
            for(int i = 0; i < dimensions.x; i++){
                for(int j = 0; j < dimensions.y; j++){
                        grid[i][j] = 0;
                }
            }
            dim = dimensions;
        }
        
        void insert(const int _tetrimino[T_X][T_Y], int x_offset, int y_offset){
            for(int i = 0; i < T_X; i++){
                for(int j = 0; j < T_Y; j++){
                    grid[x_offset+i][y_offset+j] = _tetrimino[i][j];
                }
            }
        }  
        void erase_3x4(int x_offset, int y_offset){
            for(int i = 0; i < T_X; i++){
                for(int j = 0; j < T_Y; j++){
                    grid[x_offset+i][y_offset+j] = 0;
                }
            }
        }
        void translate(xy startingPosition, xy newPosition){
            for(int i = 0; i < T_X; i++){
                for(int j = 0; j < T_Y; j++){
                    grid[newPosition.x+i][newPosition.y+j] = grid[startingPosition.x+i][startingPosition.y+j];
                    grid[startingPosition.x+i][startingPosition.y+j] = 0;
                }
            }
        }
    
        void undraw(){
            for(int i = 0; i < dim.y; i++){
                std::cout << "\033[A" << "\r";
            }
        }

        void draw(){
            for(int i = 0; i < dim.y; i++){
                for(int j = 0; j < dim.x; j++){
                    if(grid[j][i] > 0){
                            std::cout << "[]";
                    }
                    else{
                            std::cout << "* ";
                    }
                }
                std::cout << std::endl;
            }
        }
};

//https://stackoverflow.com/questions/421860/
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
