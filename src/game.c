/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"
#include "functions.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

// row major
// 0 — dead, 1 — alive
// int cellArray[ARRAY_SIZE * ARRAY_SIZE] = {0};
// Color cellArrayColor[ARRAY_SIZE * ARRAY_SIZE];
int teleport = 1;   // toggle teleport or not (T)

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    framesCounter = 0;
    finishScreen = 0;
    for (int i = 0; i < ARRAY_SIZE * ARRAY_SIZE; i++)
    {
        cellArray[i].state = 0;
        cellArray[i].color.r = (unsigned char) 0;
        cellArray[i].color.g = (unsigned char) 0;
        cellArray[i].color.b = (unsigned char) 0;
        cellArray[i].color.a = (unsigned char) 0;
    }
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // TODO: Draw GAMEPLAY screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
    DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}

// In bounds checker (had to do with smth with the ruler)
int isValidCell(int cellX, int cellY) {
    return cellX >= 0 && cellX < ARRAY_SIZE && cellY >= 0 && cellY < ARRAY_SIZE;
}


//
//  SIDE BAR RELATED CRAP
//
void InitSideBar(void);
void UpdateSideBar(void);
void DrawSideBar(void);
void UnloadSideBar(void);
int FinishSideBar(void);

int Button000Pressed;   // play/pause   connected
int Button001Pressed;   // zoom in      connected
int Button002Pressed;   // zoom out     connected
int Button003Pressed;   // info
int Button004Pressed;   // brush size
int Button005Pressed;   // color
int Button006Pressed;   // tool
int Button007Pressed;   // clear        connected