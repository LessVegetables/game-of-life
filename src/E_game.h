#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h" // for the Color struct

/*
wiki: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
At each step in time, the following transitions occur:
1. Any live cell with fewer than two live neighbors dies, as if by underpopulation.
2. Any live cell with two or three live neighbors lives on to the next generation.
3. Any live cell with more than three live neighbors dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
*/


extern const int ARRAY_SIZE;
extern const int TILE_SIZE;     // Size of NxN tile (with border) in pixels

// row maijor
// 0 — dead, 1 — alive
extern int cellArray[100 * 100];
extern Color cellArrayColor[100 * 100];
extern int teleport;   // toggle teleport or not (T)

void updateArray();
void clearArray();

#endif