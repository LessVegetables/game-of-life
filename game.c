#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"


/*
wiki: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
At each step in time, the following transitions occur:
1. Any live cell with fewer than two live neighbors dies, as if by underpopulation.
2. Any live cell with two or three live neighbors lives on to the next generation.
3. Any live cell with more than three live neighbors dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
*/

// Daniel Gehrman

void clearArray();

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY} GameScreen;

int ARRAY_SIZE = 32;

// row maijor
// 0 — dead, 1 — alive
int cellArray[1024] = {0};

int main()
{

    // cellArray[35] = 1;
    // cellArray[67] = 1;
    // cellArray[99] = 1;

    // 1 * 32 + 3 = 32 + 3 = 35
    // 2 * 32 + 3 = 64 + 3 = 67
    // 3 * 32 + 3 = 96 + 3 = 99

    int aliveCount = 0; // how many alive cells around are there
    
    int unpopCount = 0; // how many cells to kill
    int unpopulate[ARRAY_SIZE * ARRAY_SIZE];    // list of coordinates of cells to kill (arr[0] == len(arr))
    unpopulate[0] = 0;

    int popCount = 0;   // how many cells to populate
    int populate[ARRAY_SIZE * ARRAY_SIZE];        // list of coordinates of cells to populate (arr[0] == len(arr))
    populate[0] = 0;


    // for dead/alive checker
    int k, n, s, e, w, ne, nw, se, sw;


    // 0 — game paused; 1 — game running;
    int gameState = 0;
    

    const int screenWidth = ARRAY_SIZE * 20;
    const int screenHeight = ARRAY_SIZE * 20;

    InitWindow(screenWidth, screenHeight, "Game of Life");

    Texture2D button = LoadTexture("/Users/danielgehrman/Documents/Programming/personal/game-of-life/resources/cell.png"); // Load button texture

    // Define frame rectangle for drawing
    float frameHeight = (float)button.height/2.0f;
    Rectangle sourceRec = { 0, 0, (float)button.width, frameHeight };

    // Define button bounds on screen
    // Rectangle btnBounds = { screenWidth/2.0f - button.width/2.0f, screenHeight/2.0f - button.height/NUM_FRAMES/2.0f, (float)button.width, frameHeight };



    GameScreen currentScreen = LOGO;


    int framesCounter = 0;          // Useful to count frames
    int frameRate = 15;             // Every [frameRate] frames, the picture update (but game still runs at 60fps, for a smother drawing feeling)
    SetTargetFPS(60);               // Set desired framerate (frames-per-second)


    Vector2 mousePoint = { 0.0f, 0.0f };

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        framesCounter++;
        // printf("gameState: %d\n", gameState);
        if (gameState && framesCounter % frameRate == 0)
        {
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
                    n = k - ARRAY_SIZE;
                    s = k + ARRAY_SIZE;
                    e = k - 1;
                    w = k + 1;
                    ne = k - (ARRAY_SIZE + 1);
                    nw = k - (ARRAY_SIZE - 1);
                    se = k + (ARRAY_SIZE - 1);
                    sw = k + (ARRAY_SIZE + 1);

                    if (e / ARRAY_SIZE == k / ARRAY_SIZE && e >= 0) aliveCount += cellArray[e];
                    if (n % ARRAY_SIZE == k % ARRAY_SIZE && n >= 0) aliveCount += cellArray[n];
                    if (s % ARRAY_SIZE == k % ARRAY_SIZE && s < ARRAY_SIZE*ARRAY_SIZE) aliveCount += cellArray[s];
                    if (w / ARRAY_SIZE == k / ARRAY_SIZE && w < ARRAY_SIZE*ARRAY_SIZE) aliveCount += cellArray[w];

                    if (ne / ARRAY_SIZE == n / ARRAY_SIZE && ne > 0) aliveCount += cellArray[ne];
                    if (nw / ARRAY_SIZE == n / ARRAY_SIZE && nw > 0) aliveCount += cellArray[nw];
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
                printf("Populated:%d %d\n", populate[i] % ARRAY_SIZE, populate[i] / ARRAY_SIZE);
            }
            popCount = 0;
            populate[0] = popCount;
            for (int i = 1; i <= unpopCount; i++)
            {
                cellArray[unpopulate[i]] = 0;
                printf("Unpopulated:%d %d\n", unpopulate[i] % ARRAY_SIZE, unpopulate[i] / ARRAY_SIZE);
            }
            unpopCount = 0;
            unpopulate[0] = unpopCount;
        }

        // Update
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            mousePoint = GetMousePosition();
            cellArray[(((int)mousePoint.y) / 20) * ARRAY_SIZE + (((int)mousePoint.x) / 20)] = 1;

            printf("Button on %d %d\t now: %d\n", ((int)mousePoint.x) / 20, ((int)mousePoint.y) / 20, gameState);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            mousePoint = GetMousePosition();
            cellArray[(((int)mousePoint.y) / 20) * ARRAY_SIZE + (((int)mousePoint.x) / 20)] = 0;

            printf("Button clicked on %d %d\t now: %d\n", ((int)mousePoint.x) / 20, ((int)mousePoint.y) / 20, gameState);
        }

        switch(currentScreen)
        {
            case LOGO:
            {
                // Wait for 2 seconds (120 frames) before jumping to TITLE screen
                if (framesCounter > 120) currentScreen = TITLE;
            } break;
            case TITLE:
            {
                // Press enter to change to GAMEPLAY screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP) || IsKeyPressed(KEY_SPACE))
                {
                    currentScreen = GAMEPLAY;
                }
            } break;
            case GAMEPLAY:
            {
                if (IsKeyPressed(KEY_SPACE))
                {
                    gameState = !gameState;
                    printf("game mode toggled\n");
                }
                if (IsKeyPressed(KEY_C))
                {
                    clearArray();
                    gameState = 0;
                    printf("array cleared\tand\tgame mode toggled\n");
                }
                if (IsKeyPressed(KEY_EQUAL) || IsKeyPressed(43))    // (int)'+' = 43
                {
                    frameRate -= 5;
                    if (frameRate <= 0) frameRate = 1;
                }
                if (IsKeyPressed(KEY_MINUS) || IsKeyPressed(95))    // (int)'_' = 95
                {
                    frameRate += 5;
                }
            } break;
            default: break;
        }

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            switch(currentScreen)
            {
                case LOGO:
                {
                    DrawText("John Conway's", screenWidth / 2 - 250, 35, 40, GRAY);
                    DrawText("Game of Life", screenWidth / 2 - 250, 100, 80, BLACK);
                    DrawText("DANIEL GEHRMAN 2024", screenWidth / 2 - 250, screenHeight - 54, 20, LIGHTGRAY);
                    DrawText("v1.0.0", screenWidth - 74 - 48, screenHeight - 54, 20, LIGHTGRAY);
                } break;
                case TITLE:
                {
                    DrawText("John Conway's", screenWidth / 2 - 250, 35, 40, GRAY);
                    DrawText("Game of Life", screenWidth / 2 - 250, 100, 80, BLACK);
                    DrawText("[LEFT_CLICK] \t-\t populate\n\n[RIGHT_CLICK] \t-\t unpopulate\n\n[SPACE] \t-\t toggle simulation\n\n[C] \t-\t clear all tiles\n\n[+] \t-\t increase speed\n\n[-] \t-\t decrease speed", screenWidth / 2 - 250, 220, 20, BLACK);
                    DrawText("Any DEAD tile with exactly 3 live neighbors\n\nbecomes a live tile\n\n\nAny LIVE tile with <2 or >3 live neighbors\n\nbecomes a dead tile", screenWidth / 2 - 250, 420, 20, BLACK);
                    DrawText("DANIEL GEHRMAN 2024", screenWidth / 2 - 250, screenHeight - 54, 20, LIGHTGRAY);
                    DrawText("v1.0.0", screenWidth - 74 - 48, screenHeight - 54, 20, LIGHTGRAY);
                } break;
                case GAMEPLAY:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, LIGHTGRAY);

                    for (int i = 0; i < ARRAY_SIZE; i++)
                    {
                        for (int j = 0; j < ARRAY_SIZE; j++)
                        {
                            sourceRec.y = cellArray[i * ARRAY_SIZE + j] * frameHeight;
                            // sourceRec.y = 0;
                            DrawTextureRec(button, sourceRec, (Vector2){j * (int)button.width, i * (int)button.width}, WHITE); // Draw button frame
                        }
                    }
                } break;
                default: break;
            }

        EndDrawing();
    }

    UnloadTexture(button);
    CloseWindow();

    return 0;
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