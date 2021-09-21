#ifndef MAIN
#include "main.hpp"
#endif

namespace tetriminos{
    point L_l[4] = {{0,0}, {0,1}, {1,1}, {1,2}};
    // [*
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
}