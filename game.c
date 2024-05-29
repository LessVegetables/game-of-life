#include "game.h"

/*
wiki: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
At each step in time, the following transitions occur:
1. Any live cell with fewer than two live neighbors dies, as if by underpopulation.
2. Any live cell with two or three live neighbors lives on to the next generation.
3. Any live cell with more than three live neighbors dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
*/

// Daniel Gehrman

const int ARRAY_SIZE = 100;
const int TILE_SIZE = 20;     // Size of NxN tile (with border) in pixels

// row major
// 0 — dead, 1 — alive
int cellArray[100 * 100] = {0};
Color cellArrayColor[100 * 100];
int teleport = 0;   // toggle teleport or not (T)

void updateArray()
{
    int aliveCount = 0; // how many alive cells around are there
    
    int unpopCount = 0; // how many cells to kill
    int unpopulate[ARRAY_SIZE * ARRAY_SIZE];    // list of coordinates of cells to kill

    int popCount = 0;   // how many cells to populate
    int populate[ARRAY_SIZE * ARRAY_SIZE];      // list of coordinates of cells to populate
    Color populateColor[ARRAY_SIZE * ARRAY_SIZE]; // list of colors for cells to populate

    int k, n, s, e, w, ne, nw, se, sw;
    
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        for (int j = 0; j < ARRAY_SIZE; j++)
        {
            aliveCount = 0;
            int totalR = 0, totalG = 0, totalB = 0;
            k = i * ARRAY_SIZE + j;

            if (!teleport)
            {
                n = (i - 1) * ARRAY_SIZE + j;
                s = (i + 1) * ARRAY_SIZE + j;
                e = i * ARRAY_SIZE + j + 1;
                w = i * ARRAY_SIZE + j - 1;
                ne = (i - 1) * ARRAY_SIZE + j + 1;
                nw = (i - 1) * ARRAY_SIZE + j - 1;
                se = (i + 1) * ARRAY_SIZE + j + 1;
                sw = (i + 1) * ARRAY_SIZE + j - 1;
            }
            else
            {
                n = ((ARRAY_SIZE + (i - 1)) % ARRAY_SIZE) * ARRAY_SIZE + j;
                s = ((i + 1) % ARRAY_SIZE) * ARRAY_SIZE + j;
                e = i * ARRAY_SIZE + ((j + 1) % ARRAY_SIZE);
                w = i * ARRAY_SIZE + ((ARRAY_SIZE + (j - 1)) % ARRAY_SIZE);
                ne = ((ARRAY_SIZE + (i - 1)) % ARRAY_SIZE) * ARRAY_SIZE + ((j + 1) % ARRAY_SIZE);
                nw = ((ARRAY_SIZE + (i - 1)) % ARRAY_SIZE) * ARRAY_SIZE + ((ARRAY_SIZE + (j - 1)) % ARRAY_SIZE);
                se = ((i + 1) % ARRAY_SIZE) * ARRAY_SIZE + ((j + 1) % ARRAY_SIZE);
                sw = ((i + 1) % ARRAY_SIZE) * ARRAY_SIZE + ((ARRAY_SIZE + (j - 1)) % ARRAY_SIZE);
            }

            int neighbors[8] = {n, s, e, w, ne, nw, se, sw};
            for (int idx = 0; idx < 8; idx++)
            {
                int neighbor = neighbors[idx];
                if (neighbor >= 0 && neighbor < ARRAY_SIZE * ARRAY_SIZE)
                {
                    if (cellArray[neighbor])
                    {
                        aliveCount++;
                        totalR += (int)cellArrayColor[neighbor].r;
                        totalG += (int)cellArrayColor[neighbor].g;
                        totalB += (int)cellArrayColor[neighbor].b;
                    }
                }
            }

            if (cellArray[k])
            {
                if (aliveCount < 2 || aliveCount > 3)
                {
                    unpopulate[unpopCount] = k;
                    unpopCount++;
                }
            }
            else
            {
                if (aliveCount == 3)
                {
                    populate[popCount] = k;
                    totalR /= 3;
                    totalG /= 3;
                    totalB /= 3;
                    populateColor[popCount] = (Color){(unsigned char)totalR, (unsigned char)totalG, (unsigned char)totalB, (unsigned char) 255};
                    popCount++;
                }
            }
        }
    }

    for (int i = 0; i < popCount; i++)
    {
        cellArray[populate[i]] = 1;
        cellArrayColor[populate[i]] = populateColor[i];
    }
    for (int i = 0; i < unpopCount; i++)
    {
        cellArray[unpopulate[i]] = 0;
        cellArrayColor[unpopulate[i]] = (Color){0, 0, 0};
    }
}

void clearArray()
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        for (int j = 0; j < ARRAY_SIZE; j++)
        {
            cellArray[i * ARRAY_SIZE + j] = 0;
            cellArrayColor[i * ARRAY_SIZE + j] = (Color){0, 0, 0};
        }
    }
}
