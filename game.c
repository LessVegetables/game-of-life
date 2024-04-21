#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY} GameScreen;

int main()
{

    // row maijor
    // 0 — dead, 1 — alive
    int cellArray[1024] = {0};

    const int screenWidth = 640;
    const int screenHeight = 640;

    InitWindow(screenWidth, screenHeight, "Game of Life");

    GameScreen currentScreen = LOGO;


    int framesCounter = 0;          // Useful to count frames
    SetTargetFPS(60);               // Set desired framerate (frames-per-second)


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
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
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = TITLE;
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
                } break;
                default: break;
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}