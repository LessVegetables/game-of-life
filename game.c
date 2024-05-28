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

// row maijor
// 0 — dead, 1 — alive
int cellArray[100 * 100] = {0};
int teleport = 0;   // toggle teleport or not (T)

void updateArray()
{
    int aliveCount = 0; // how many alive cells around are there
    
    int unpopCount = 0; // how many cells to kill
    int unpopulate[ARRAY_SIZE * ARRAY_SIZE];    // list of coordinates of cells to kill (arr[0] == len(arr))
    unpopulate[0] = 0;

    int popCount = 0;   // how many cells to populate
    int populate[ARRAY_SIZE * ARRAY_SIZE];        // list of coordinates of cells to populate (arr[0] == len(arr))
    populate[0] = 0;

    // for dead/alive checker
    int k, n, s, e, w, ne, nw, se, sw;
    
    
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        for (int j = 0; j < ARRAY_SIZE; j++)
        {
            aliveCount = 0;
            /*
                k = i*32 + j

                k-33 | k-32 | k-31
                k-1  |  k   | k+1
                k+31 | k+32 | K+33

                ne | n |  nw
                e  | k |  w
                se | s |  sw

            */

            k = i * ARRAY_SIZE + j;

            if (!teleport)
            {
                // n = k - ARRAY_SIZE;
                // s = k + ARRAY_SIZE;
                // e = k - 1;
                // w = k + 1;
                // ne = k - (ARRAY_SIZE + 1);
                // nw = k - (ARRAY_SIZE - 1);
                // se = k + (ARRAY_SIZE - 1);
                // sw = k + (ARRAY_SIZE + 1);
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
                // printf("teleport ON?\n");
                n = ((ARRAY_SIZE + (i - 1)) % ARRAY_SIZE) * ARRAY_SIZE + j;
                s = ((i + 1) % ARRAY_SIZE) * ARRAY_SIZE + j;
                e = i * ARRAY_SIZE + ((j + 1) % ARRAY_SIZE);
                w = i * ARRAY_SIZE + ((ARRAY_SIZE + (j - 1)) % ARRAY_SIZE);
                
                ne = ((ARRAY_SIZE + (i - 1)) % ARRAY_SIZE) * ARRAY_SIZE + ((j + 1) % ARRAY_SIZE);
                nw = ((ARRAY_SIZE + (i - 1)) % ARRAY_SIZE) * ARRAY_SIZE + ((ARRAY_SIZE + (j - 1)) % ARRAY_SIZE);
                se = ((i + 1) % ARRAY_SIZE) * ARRAY_SIZE + ((j + 1) % ARRAY_SIZE);
                sw = ((i + 1) % ARRAY_SIZE) * ARRAY_SIZE + ((ARRAY_SIZE + (j - 1)) % ARRAY_SIZE);
            }

            if (e / ARRAY_SIZE == k / ARRAY_SIZE && e >= 0) aliveCount += cellArray[e];
            if (n % ARRAY_SIZE == k % ARRAY_SIZE && n >= 0) aliveCount += cellArray[n];
            if (s % ARRAY_SIZE == k % ARRAY_SIZE && s < ARRAY_SIZE*ARRAY_SIZE) aliveCount += cellArray[s];
            if (w / ARRAY_SIZE == k / ARRAY_SIZE && w < ARRAY_SIZE*ARRAY_SIZE) aliveCount += cellArray[w];

            if (ne / ARRAY_SIZE == n / ARRAY_SIZE && ne >= 0) aliveCount += cellArray[ne];
            if (nw / ARRAY_SIZE == n / ARRAY_SIZE && nw >= 0) aliveCount += cellArray[nw];
            if (se / ARRAY_SIZE == s / ARRAY_SIZE && se < ARRAY_SIZE*ARRAY_SIZE) aliveCount += cellArray[se];
            if (sw / ARRAY_SIZE == s / ARRAY_SIZE && sw < ARRAY_SIZE*ARRAY_SIZE) aliveCount += cellArray[sw];

            if(cellArray[k])    // cell[i][j] is alive
            {
                // printf("cell %d %d is alive\n", k % ARRAY_SIZE, k / ARRAY_SIZE);
                if (aliveCount < 2 || aliveCount > 3)
                {
                    unpopulate[unpopCount + 1] = k;
                    unpopCount++;
                    unpopulate[0] = unpopCount;
                    //cellArray[k] = 0; //kill
                }
            }
            else    // cell[i][j] is dead
            {
                // printf("cell %d %d is dead\n", k % ARRAY_SIZE, k / ARRAY_SIZE);
                if (aliveCount == 3)
                {
                    populate[popCount + 1] = k;
                    popCount++;
                    populate[0] = popCount;
                    // cellArray[k] = 1;
                }
            }
        }
    }

    for (int i = 1; i <= popCount; i++)
    {
        cellArray[populate[i]] = 1;
        // printf("Populated:%d %d\n", populate[i] % ARRAY_SIZE, populate[i] / ARRAY_SIZE);
    }
    popCount = 0;
    populate[0] = popCount;
    for (int i = 1; i <= unpopCount; i++)
    {
        cellArray[unpopulate[i]] = 0;
        // printf("Unpopulated:%d %d\n", unpopulate[i] % ARRAY_SIZE, unpopulate[i] / ARRAY_SIZE);
    }
    unpopCount = 0;
    unpopulate[0] = unpopCount;

    return;
}

void clearArray()
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        for (int j = 0; j < ARRAY_SIZE; j++)
        {
            cellArray[i * ARRAY_SIZE + j] = 0;
        }
    }
}