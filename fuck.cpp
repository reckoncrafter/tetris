#include <iostream>
#include <unistd.h>
#include <random>
#include "constrained.h"
using namespace std;

bool endFlag = false;

void draw(int _grid[10][25]){
    for(int i = 0; i < 25; i++){
        for(int j = 0; j < 10; j++){
            if(_grid[j][i] == 1){
                cout << "[*]";
            }
            else if(_grid[j][i]== -1){
                cout << "[X]";
            }
            else{
                cout << "[ ]";
            }
        }
        cout << endl;
    }
}
void undraw(){
            for(int i = 0; i < 25; i++){
                cout << "\033[A" << "\r";
            }
        }



struct point{
    int x,y;
};
struct piece{
    point blk[4];
    point offset;
};

    point L_l[4] = {{0,0}, {0,1}, {1,1}, {1,2}};
    // [*]
    // [*][*][*]
    point L_r[4] = {{0,2}, {1,0}, {1,1}, {1,2}};
    //       [*]
    // [*][*][*]
    point O[4] = {{0,0}, {0,1}, {1,0}, {1,1}};
    // [*][*]
    // [*][*]
    point I[4] = {{0,0}, {1,0}, {2,0}, {3,0}};
    // [*][*][*][*]
    point S_l[4] = {{0,0}, {1,0}, {1,1}, {2,1}};
    // [*][*]
    //    [*][*]
    point S_r[4] = {{1,0}, {2,0}, {0,1}, {1,1}};
    //    [*][*]
    // [*][*]
    point T[4] = {{0,0}, {1,0}, {2,0}, {1,1}};
    // [*][*][*]
    //    [*]

void spawn(const piece block, int grid[10][25], int posx = 0, int posy = 0){
    for(int i = 0; i < 10; i++){
        if(grid[i][0] != 0){
            endFlag = true;
        }
    }
    for(int i = 0; i < 4; i++){
        grid[block.blk[i].x+block.offset.x][block.blk[i].y+block.offset.y] = 1;
    }
}

void despawn(const piece block, int grid[10][25]){
    for(int i = 0; i < 4; i++){
        grid[block.blk[i].x+block.offset.x][block.blk[i].y+block.offset.y] = 0;
    }
}
void fall(const point block[4], int grid[10][25], int& gravity){
    for(int i = 0; i < 4; i++){
        grid[block[i].x][block[i].y + gravity] = 0;
    }
    gravity++;
    for(int i = 0; i < 4; i++){
        int* next = &grid[block[i].x][block[i].y + gravity];
        *next = 1;
    }
}

bool Colliders(const piece block, int grid[10][25]){
    point* colliders = new point[4]();
    bool collision = false;

    for(int i = 0; i < 4; i++){
        grid[block.blk[i].x+block.offset.x][block.blk[i].y+block.offset.y] = 2;
    }
    for(int i = 0; i < 4; i++){
        int* below = &grid[block.blk[i].x+block.offset.x][block.blk[i].y+block.offset.y+1];
        if(*below != 2){
            if(*below == 1){
                collision = true;
            }
            *below = -1;
        }
        grid[block.blk[i].x+block.offset.x][block.blk[i].y+block.offset.y] = 1;
    }
    return collision;
}

void step(piece block[4], int grid[10][25]){
    for(int i = 0; i < 4; i++){
        block[i].offset.y += 1;
    }
}

piece* randomSelect(){
    piece* p = new piece;
    int r = rand()%7;
    //cout << r;
    switch(r){
        case 0:
            for(int i = 0; i < 4; i++){
                p->blk[i] = L_l[i];
            }
            break;
        case 1:
            for(int i = 0; i < 4; i++){
                p->blk[i] = L_r[i];
            }
            break;
        case 2:
            for(int i = 0; i < 4; i++){
                p->blk[i] = O[i];
            }    
            break;
        case 3:
            for(int i = 0; i < 4; i++){
                p->blk[i] = I[i];
            }
            break;
        case 4:
            for(int i = 0; i < 4; i++){
                p->blk[i] = S_l[i];
            }
            break;
        case 5:
            for(int i = 0; i < 4; i++){
                p->blk[i] = S_r[i];
            }
            break;
        case 6:
            for(int i = 0; i < 4; i++){
                p->blk[i] = T[i];
            }          
    }
    return p;
}


int main(){
    srand(time(NULL));
    int grid[10][25] = {0};

    // TEST FLOOR
    for(int i = 0; i < 10; i++){
        grid[i][10] = 1;
    }
    

    while(true){
        piece* p = randomSelect();
        p->offset.x = 4;
        p->offset.y = 0;
        if(endFlag){
            break;
        }


        while(true){
            spawn(*p, grid);
            draw(grid);
            if(Colliders(*p,grid)){
                break;
            };
            sleep(1);
            despawn(*p, grid);
            step(p, grid);
            undraw();
        }
        undraw();
    }
    return 0;

}