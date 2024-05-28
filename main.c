#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>
#include "raylib.h"
#include "game.h"
#include "fileio.h"
#include "loopDetection.h"

// #include "fileio.h"

// color: RGBAlpha

/*
camera movement logic:
• camera moves left/right/up/down with wasd/arrow keys
• camera zooms in/out in where the cursor is pointing with the scroll wheel — to be implemented
*/

/*
wiki: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
At each step in time, the following transitions occur:
1. Any live cell with fewer than two live neighbors dies, as if by underpopulation.
2. Any live cell with two or three live neighbors lives on to the next generation.
3. Any live cell with more than three live neighbors dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
*/

// Daniel Gehrman

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, PAUSE} GameScreen;

typedef enum ToolSelect { BRUSH = 0, LINE, SQUARE, CIRCLE} ToolSelect;

extern const int ARRAY_SIZE;  //
extern const int TILE_SIZE;     // Size of NxN tile (with border) in pixels

#define GAME_VERSION "v1.4.0"
#define MYBLUE CLITERAL(Color){100, 158, 221, 255}

int main()
{
    int aliveCount = 0; // how many alive cells around are there
    int viewMode = 0;   // toggle grid drawing (V)
    
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
    // 0 — hidden; 1 — visible;
    int sideBar = 1;
    // 0 — brush, 1 — line, 2 — square, 3 — circle
    int currentTool = 0;
    // 0 — deleting, 1 — drawing; For mouse drawing
    int deletDraw;
    

    // const int screenWidth = ARRAY_SIZE * TILE_SIZE;
    // const int screenHeight = ARRAY_SIZE * TILE_SIZE;

    int screenWidth = 640;
    int screenHeight = 640;
    int fullScreen = 0;     // by default — it opens in a 640x640 window. [F] should toggle full screens

    InitWindow(screenWidth, screenHeight, "Game of Life");


    // audio
    InitAudioDevice();      // Initialize audio device

    Sound fxWav = LoadSound("resources/sound.wav");         // Load WAV audio file
    Sound fxOgg = LoadSound("resources/target.ogg");        // Load OGG audio file
    // audio



    // char* imagePath = get_resource_path("cell.png"); // Replace with your image filename
    // if (imagePath == NULL) // printf("Error getting resource path\n");


    // char cwd[PATH_MAX];
    // if (getcwd(cwd, sizeof(cwd)) != NULL) {
    //     // printf("\n\n\n\n\nCurrent working dir: %s\n\n\n\n\n", cwd);
    // } else {
    //     perror("getcwd() error");
    //     return 1;
    // }

    // printf("\n\n\n\n\nTexture path: %s\n\n\n\n\n\n", imagePath);
////// Texture2D button = LoadTexture("/Users/danielgehrman/Documents/Programming/personal/game-of-life/Resources/cell.png"); // Load button texture
    // Texture2D button = LoadTexture("../Resources/cell.png"); // Load button texture
    // Texture2D button = LoadTexture("./cell.png"); // Load button texture
    // Texture2D button = LoadTexture("/Applications/Life.app/Contents/Resources/cell.png"); // Load button texture
    // free(imagePath);

    // Define frame rectangle for drawing
    // float frameHeight = (float)button.height/2.0f;
    // Rectangle sourceRec = { 0, 0, (float)button.width, frameHeight };

    // Define button bounds on screen
    // Rectangle btnBounds = { screenWidth/2.0f - button.width/2.0f, screenHeight/2.0f - button.height/NUM_FRAMES/2.0f, (float)button.width, frameHeight };



    GameScreen currentScreen = LOGO;

    bool exitWindow = false;            // Flag to set window to exit
    int gameScreenUponRequest;          // Self explanatory; also doubles as a flag to request window to exit

    int framesCounter = 0;          // Useful to count frames
    int frameRate = 15;             // Every [frameRate] frames, the picture update (but game still runs at 60fps, for a smother drawing feeling)
    SetTargetFPS(60);               // Set desired framerate (frames-per-second)

    Vector2 mousePoint = { 0.0f, 0.0f };
    Vector2 mousePointWorld;

    Vector2 mousePointStart;
    Vector2 mousePointEnd;

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.target = (Vector2){ (float)(ARRAY_SIZE * TILE_SIZE / 2.0f), (float)(ARRAY_SIZE * TILE_SIZE / 2.0f) };    // essentially 1000.0f, 1000.0f
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    // float minX = 0, minY = 0, maxX = ARRAY_SIZE * TILE_SIZE, maxY = ARRAY_SIZE * TILE_SIZE;

    // for the blinking [space] on start; and blinking [y/n] in pause screen
    // 0 — LIGHTGREY; 1 — GREY
    int isGrey = 0; 

    // Main game loop
    while (!exitWindow)    // Detect window close button or ESC key
    {
        framesCounter++;

        // Detect if X-button or KEY_ESCAPE have been pressed to close window
        if ((WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) && currentScreen != PAUSE)
        {
            gameScreenUponRequest = currentScreen;
            currentScreen = PAUSE;
            gameState = 0;
        }

        // fullscreen / 640x640
        if (IsKeyPressed(KEY_F))
        {
            fullScreen = !fullScreen;
            int monitor = GetCurrentMonitor();
            if (fullScreen)
            {
                screenWidth = GetMonitorWidth(monitor);
                screenHeight = GetMonitorHeight(monitor);
                printf("SCREEN SIZE CHANGED TO\nw:\t%d\nh:\t%d\n", screenWidth, screenHeight);
                SetWindowSize(screenWidth, screenHeight);
                camera.offset = (Vector2){ (float)screenWidth / 2.0f, (float)screenHeight / 2.0f };
                camera.zoom = 1.0f;
                // DisableCursor();
                ToggleFullscreen();
            }
            else
            {
                // EnableCursor();
                ToggleFullscreen();
                screenWidth = 640;
                screenHeight = 640;
                camera.offset = (Vector2){ 320.0f, 320.0f };    // screenWidth / 2.0f, screenHeight / 2.0f
                camera.zoom = 1.0f;
                SetWindowSize(screenWidth, screenHeight);
            }

        }

        // Itirate on the next game state
        if (gameState && (framesCounter % frameRate == 0))
        {
            updateArray();
            // BEGIN: loop detection
            if (loopDetected())
            {
                printf("Loop detected!\n");
            }
            // END: loop detection
        }

        switch(currentScreen)
        {
            case LOGO:
            {
                // Wait for 2 seconds (120 frames) before jumping to TITLE screen
                if (framesCounter > 10) currentScreen = TITLE;
            } break;
            case TITLE:
            {
                // Press enter to change to GAMEPLAY screen
                // IsGestureDetected(GESTURE_TAP)
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
                {
                    currentScreen = GAMEPLAY;
                }
                if(framesCounter % 60 == 0) isGrey = !isGrey;
            } break;
            case PAUSE:
            {
                if(framesCounter % 60 == 0) isGrey = !isGrey;
                // A request for close window has been issued
                if (IsKeyPressed(KEY_Y)) exitWindow = true;
                else if (IsKeyPressed(KEY_N)) currentScreen = gameScreenUponRequest;    // note: gameState is still 0
                break;
            }
            case GAMEPLAY:
            {
                if (IsKeyPressed(KEY_SPACE))    gameState = !gameState;
                if (IsKeyPressed(KEY_T))    teleport = !teleport;
                if (IsKeyPressed(KEY_V))    viewMode = !viewMode;
                if (IsKeyPressed(KEY_H))    sideBar = !sideBar;
                if (IsKeyPressed(KEY_C))
                {
                    clearArray();
                    gameState = 0;
                    // printf("array cleared\tand\tgame mode toggled\n");
                }
                if (IsKeyPressed(KEY_R))
                {
                    camera.target.x = (float)(ARRAY_SIZE * TILE_SIZE / 2.0f);
                    camera.target.y = (float)(ARRAY_SIZE * TILE_SIZE / 2.0f);
                    camera.zoom = 1.0f;
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


                // dropping/importing files
                if (IsFileDropped())
                {
                    gameState = 0;
                    FilePathList droppedFiles = LoadDroppedFiles();
                    readCellArrayFromFile(droppedFiles.paths[0]);
                    UnloadDroppedFiles(droppedFiles);    // Unload filepaths from memory
                }

                // exporting files
                if (IsKeyPressed(KEY_E)) saveCellArrayToFile();

                // Mouse Drawing

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    mousePointStart = GetMousePosition();
                }
                switch (currentTool)
                {
                    case BRUSH:
                    {
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
                        {
                            // printf("Camera zoom level: %f\tMouce on %d %d\t%d %d\t%d %d\t%d %d\t", camera.zoom, (int)GetMousePosition().x, (int)GetMousePosition().y, (int)mousePoint.x, (int)mousePoint.y, ((int)mousePoint.x) / TILE_SIZE, ((int)mousePoint.y) / TILE_SIZE, ((int)mousePoint.x) % TILE_SIZE, ((int)mousePoint.y) % TILE_SIZE);
                            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))   deletDraw = 1;
                            else deletDraw = 0;
                            
                            mousePoint = GetMousePosition();
                            mousePoint.x = (mousePoint.x - camera.offset.x) / camera.zoom + camera.target.x;
                            mousePoint.y = (mousePoint.y - camera.offset.y) / camera.zoom + camera.target.y;
                            if (
                                mousePoint.x >= 0 &&
                                mousePoint.x < ARRAY_SIZE * TILE_SIZE &&            // in bounds of array
                                mousePoint.y >= 0 &&
                                mousePoint.y < ARRAY_SIZE * TILE_SIZE               // in bounds of array
                            )    // check that mouse doesn't go out of bounds
                            {
                                if (camera.zoom > 1.5f)      // we don't care about the border when it's super tiny
                                {
                                    if ((int)mousePoint.x % TILE_SIZE > 0 && (int)mousePoint.x % TILE_SIZE < (TILE_SIZE - 1) && (int)mousePoint.y % TILE_SIZE > 0 && (int)mousePoint.y % TILE_SIZE < (TILE_SIZE - 1))   // mouse doesn't draw on the boarder
                                    {
                                        cellArray[((int)(mousePoint.y) / TILE_SIZE) * ARRAY_SIZE + ((int)(mousePoint.x) / TILE_SIZE)] = deletDraw;
                                    }
                                }
                                else
                                {
                                    cellArray[((int)(mousePoint.y) / TILE_SIZE) * ARRAY_SIZE + ((int)(mousePoint.x) / TILE_SIZE)] = deletDraw;
                                }
                            }
                        }
                        // printf("Button on %d %d\t now: %d\n", ((int)mousePoint.x) / TILE_SIZE, ((int)mousePoint.y) / TILE_SIZE, gameState);
                    }break;
                    case LINE:
                    {
                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                        {
                            mousePointEnd = GetMousePosition();

                            mousePointStart.x -= (float)(((int)mousePointStart.x % TILE_SIZE) + 1);
                            mousePointStart.y -= (float)(((int)mousePointStart.y % TILE_SIZE) + 1);
                            mousePointEnd.x -= (float)(((int)mousePointEnd.x % TILE_SIZE) + 1);
                            mousePointEnd.y -= (float)(((int)mousePointEnd.y % TILE_SIZE) + 1);
                        }

                    }break;
                    case SQUARE:
                    {

                    }break;
                    case CIRCLE:
                    {

                    }break;
                    
                    default: break;
                }

                // Screen movement
                // printf("\t\t\t\t%d %d\t\t%d %d\n", (int)camera.target.x, (int)camera.target.y, (int)camera.offset.x, (int)camera.offset.y);
                if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) camera.target.x += ((float)TILE_SIZE / camera.zoom);
                if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) camera.target.x -= ((float)TILE_SIZE / camera.zoom);
                if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) camera.target.y -= ((float)TILE_SIZE / camera.zoom);
                if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) camera.target.y += ((float)TILE_SIZE / camera.zoom);

                // Keep screen inside grid
                if (camera.target.x - (camera.offset.x / camera.zoom) < 0) camera.target.x = (camera.offset.x / camera.zoom);
                if (camera.target.y - (camera.offset.y / camera.zoom) < 0) camera.target.y = (camera.offset.y / camera.zoom);
                if (camera.target.x - ((float)(TILE_SIZE*ARRAY_SIZE) - (camera.offset.x / camera.zoom)) > 0.0f) camera.target.x = (float)(TILE_SIZE*ARRAY_SIZE) - (camera.offset.x / camera.zoom);       // the full screen bug is here.
                if (camera.target.y - ((float)(TILE_SIZE*ARRAY_SIZE) - (camera.offset.y / camera.zoom)) > 0.0f) camera.target.y = (float)(TILE_SIZE*ARRAY_SIZE) - (camera.offset.y / camera.zoom);

                // Screen zoom
                if (GetMouseWheelMove())
                {
                    camera.zoom += ((float)GetMouseWheelMove()*0.05f);
                    if (camera.zoom < (float)(screenWidth) / (float)(ARRAY_SIZE * TILE_SIZE)) camera.zoom = (float)(screenWidth) / (float)(ARRAY_SIZE * TILE_SIZE);
                    // or if (camera.zoom < 0.32f) camera.zoom = 0.32f;
                }

            } break;
            default: break;
        }

        // Draw
        BeginDrawing();
            ClearBackground(WHITE);     // or mayb RAYWHITE
            switch(currentScreen)
            {
                case LOGO:
                {
                    DrawText("John Conway's", screenWidth / 2 - 250, 35, 40, GRAY);
                    DrawText("Game of ", screenWidth / 2 - 250, 100, 80, BLACK);
                    DrawText("Life", screenWidth / 2 - 250 + 352, 100, 80, CLITERAL(Color){100, 158, 221, 255});
                    DrawText("DANIEL GEHRMAN 2024", screenWidth / 2 - 250, screenHeight - 54, 20, LIGHTGRAY);
                    DrawText(GAME_VERSION, screenWidth - 74 - 48, screenHeight - 54, 20, LIGHTGRAY);
                } break;
                case TITLE:
                {
                    DrawText("John Conway's", screenWidth / 2 - 250, (35.0f / 640.0f) * screenHeight, 40, GRAY);
                    DrawText("Game of ", screenWidth / 2 - 250, (100.0f / 640.0f) * screenHeight, 80, BLACK);
                    DrawText("Life", screenWidth / 2 - 250 + 352, (100.0f / 640.0f) * screenHeight, 80, CLITERAL(Color){100, 158, 221, 255});
                    DrawText("Any DEAD tile with exactly 3 live neighbors\nbecomes a live tile\n\nAny LIVE tile with <2 or >3 live neighbors\nbecomes a dead tile", screenWidth / 2 - 250, (205.0f / 640.0f) * screenHeight, 20, BLACK);
                    DrawText("[LEFT_CLICK]\t-\tdraw\n[RIGHT_CLICK]\t-\terase", screenWidth / 2 - 250, (337.0f / 640.0f) * screenHeight, 20, GRAY);
                    if(isGrey) DrawText("[SPACE]\t-\ttoggle simulation", screenWidth / 2 - 250, (381.0f / 640.0f) * screenHeight, 20, GRAY);
                    else DrawText("[SPACE]\t-\ttoggle simulation", screenWidth / 2 - 250, (381.0f / 640.0f) * screenHeight, 20, LIGHTGRAY);
                    DrawText("[C]\t-\tclear all tiles\n[R]\t-\treset camera position\n[T]\t-\ttoggle TELEPORT mode\n[V]\t-\ttoggle grid view\n[+]\t-\tincrease speed\n[-]\t-\tdecrease speed\n[esc]\t-\tquit game", screenWidth / 2 - 250, (403.0f / 640.0f) * screenHeight, 20, GRAY);
                    DrawText("DANIEL GEHRMAN 2024", screenWidth / 2 - 250, screenHeight - 54, 20, LIGHTGRAY);
                    DrawText(GAME_VERSION, screenWidth - 74 - 48, screenHeight - 54, 20, LIGHTGRAY);
                } break;
                case PAUSE:
                {
                    DrawText("Are you sure you want to exit? [Y/N]", (screenWidth - 390) / 2, screenHeight / 3, 20, BLACK);
                    if(isGrey) DrawText("[Y/N]", (screenWidth - 390) / 2 + 334, screenHeight / 3, 20, BLACK);
                    else DrawText("[Y/N]", (screenWidth - 390) / 2 + 334, screenHeight / 3, 20, GRAY);
                    break;
                }
                case GAMEPLAY:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, WHITE);

                    BeginMode2D(camera);
                        for (int i = 0; i < ARRAY_SIZE; i++)
                        {
                            for (int j = 0; j < ARRAY_SIZE; j++)
                            {
                                //DrawRectangleLines(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, CLITERAL(Color){0, 0, 0, 21});

                                if (!viewMode)
                                {
                                    DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE - 1, 1, CLITERAL(Color){0, 0, 0, 21});
                                    DrawRectangle(j * TILE_SIZE + TILE_SIZE - 1, i * TILE_SIZE, 1, TILE_SIZE - 1, CLITERAL(Color){0, 0, 0, 21});
                                    DrawRectangle(j * TILE_SIZE, i * TILE_SIZE + 1, 1, TILE_SIZE - 2, CLITERAL(Color){0, 0, 0, 21});
                                    DrawRectangle(j * TILE_SIZE, i * TILE_SIZE + TILE_SIZE - 1, TILE_SIZE, 1, CLITERAL(Color){0, 0, 0, 21});
                                }

                                if (cellArray[i * ARRAY_SIZE + j])  // if cell is alive
                                {
                                    if (!viewMode) DrawRectangle(j * TILE_SIZE + 1, i * TILE_SIZE + 1, 18, 18, MYBLUE);
                                    else DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, 20, 20, MYBLUE);

                                    // DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, 20, 20, CLITERAL(Color){100, 158, 221, 255});
                                }
                                else
                                {
                                    DrawRectangle(j * TILE_SIZE + 1, i * TILE_SIZE + 1, 18, 18, CLITERAL(Color){0, 0, 0, 0});
                                }
                            }
                        }



                        if(!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                        {
                            mousePoint = GetMousePosition();
                            mousePointWorld = GetScreenToWorld2D(mousePoint, camera); // Get the world space position for a 2d camera screen space position
                            DrawRectangle((int)mousePointWorld.x - ((int)mousePointWorld.x % TILE_SIZE) + 1, (int)mousePointWorld.y - ((int)mousePointWorld.y % TILE_SIZE) + 1, 18, 18, CLITERAL(Color){0, 0, 0, 21});
                        }
                        // for debugging purposes
                        DrawRectangle(0, 0, TILE_SIZE, TILE_SIZE, RED);
                        DrawRectangle(ARRAY_SIZE * TILE_SIZE - TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, RED);
                        DrawRectangle(0, ARRAY_SIZE * TILE_SIZE - TILE_SIZE, TILE_SIZE, TILE_SIZE,RED);
                        DrawRectangle(ARRAY_SIZE * TILE_SIZE - TILE_SIZE, ARRAY_SIZE * TILE_SIZE - TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);
                    EndMode2D();
                } break;
                default: break;
            }

        EndDrawing();
    }


    UnloadSound(fxWav);     // Unload sound data
    UnloadSound(fxOgg);     // Unload sound data
    CloseAudioDevice();     // Close audio device
    CloseWindow();
    freeHashTable();

    return 0;
}