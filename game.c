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
typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY} GameScreen;

int ARRAY_SIZE = 32;

int main()
{

    // row maijor
    // 0 — dead, 1 — alive
    int cellArray[1024] = {0};

    // cellArray[35] = 1;
    // cellArray[67] = 1;
    // cellArray[99] = 1;

    // 1 * 32 + 3 = 32 + 3 = 35
    // 2 * 32 + 3 = 64 + 3 = 67
    // 3 * 32 + 3 = 96 + 3 = 99

    int aliveCount = 0; // how many alive cells around are there
    int popCount = 0;   // how many cells to populate
    int unpopCount = 0; // how many cells to kill
    int* unpopulate = (int*) malloc((unpopCount + 1) * sizeof(int));    // list of coordinates of cells to kill (arr[0] == len(arr))
    unpopulate[0] = 0;
    int* populate = (int*) malloc((popCount + 1) * sizeof(int));        // list of coordinates of cells to populate (arr[0] == len(arr))
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
    SetTargetFPS(60);               // Set desired framerate (frames-per-second)


    Vector2 mousePoint = { 0.0f, 0.0f };

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        if (gameState)
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

                    if (e % ARRAY_SIZE == k % ARRAY_SIZE && e >= 0) aliveCount += cellArray[e];
                    if (n / ARRAY_SIZE == k / ARRAY_SIZE && n >= 0) aliveCount += cellArray[n];
                    if (s / ARRAY_SIZE == k / ARRAY_SIZE && s <= ARRAY_SIZE*ARRAY_SIZE) aliveCount += cellArray[s];
                    if (w % ARRAY_SIZE == k % ARRAY_SIZE && w <= ARRAY_SIZE*ARRAY_SIZE) aliveCount += cellArray[w];

                    if (ne % ARRAY_SIZE == n % ARRAY_SIZE && ne > 0) aliveCount += cellArray[ne];
                    if (nw % ARRAY_SIZE == n % ARRAY_SIZE && nw > 0) aliveCount += cellArray[nw];
                    if (se % ARRAY_SIZE == s % ARRAY_SIZE && se < ARRAY_SIZE*ARRAY_SIZE) aliveCount += cellArray[se];
                    if (sw % ARRAY_SIZE == s % ARRAY_SIZE && sw < ARRAY_SIZE*ARRAY_SIZE) aliveCount += cellArray[sw];

                    if(cellArray[k])    // cell[i][j] is alive
                    {
                        if (aliveCount < 2 || aliveCount > 3) cellArray[k] = 0;
                    }
                    else    // cell[i][j] is dead
                    {
                        if (aliveCount == 3) cellArray[k] = 1;
                    }
                }
            }
        }

        // Update
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            mousePoint = GetMousePosition();
            cellArray[(((int)mousePoint.y) / 20) * ARRAY_SIZE + (((int)mousePoint.x) / 20)] = 1;

            printf("Button clicked on %d %d\t now: %d\n", ((int)mousePoint.x) / 20, ((int)mousePoint.y) / 20, gameState);
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
                // Count frames
                framesCounter++;
                // Wait for 2 seconds (120 frames) before jumping to TITLE screen
                if (framesCounter > 120) currentScreen = TITLE;
            } break;
            case TITLE:
            {
                // Press enter to change to GAMEPLAY screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = GAMEPLAY;
                }
            } break;
            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!

                // Press enter to change to TITLE screen
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentScreen = TITLE;
                }

                if (IsKeyPressed(KEY_SPACE))
                {
                    gameState = !gameState;
                    printf("game mode toggled\n");
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
                    DrawText("John Conway's Game of Life", 20, 20, 40, LIGHTGRAY);
                    DrawText("DANIEL GEHRMAN 2024", 290, 220, 20, GRAY);
                } break;
                case TITLE:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, LIGHTGRAY);
                    DrawText("Game of Life", 20, 20, 40, BLACK);
                    DrawText("Click on tile to set it's state\nPress space to start simularion", 120, 220, 20, BLACK);
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


    free(unpopulate);
    free(populate);

    return 0;
}