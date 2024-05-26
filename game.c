#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>
// #include <limits.h>
#include <string.h>
#include "raylib.h"


#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define PATH_SEPARATOR '\\'
#else
#include <unistd.h>
#define PATH_SEPARATOR '/'
#endif
// Function to get the Downloads folder path
void getDownloadsPath(char* path, size_t size);


//#include "loopDetection.h"
#define TABLE_SIZE 10007  // A prime number for the hash table size
typedef struct HashNode {
    uint32_t hash;
    struct HashNode* next;
} HashNode;
HashNode* hashTable[TABLE_SIZE] = {0};
int containsHash(uint32_t hash);
void addHash(uint32_t hash);
void freeHashTable();


// superfasthash.c
#include <stdint.h> /* Replace with "pstdint.h" if appropriate */

// Paul Hseieh • Super Fast Hash
// http://www.azillionmonkeys.com/qed/hash.html
#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

uint32_t SuperFastHash (const char * data, int len);

// #include "fileio.h"

// color: RGBAlpha

/*
camera movement logic:
• camera moves left/right/up/down with wasd/arrow keys
• camera zooms in/out in where the cursor is pointing with the scroll wheel
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

void clearArray();

char* get_resource_path(const char* filename);

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, PAUSE} GameScreen;

const int ARRAY_SIZE = 100;   //
const int TILE_SIZE = 20;     // Size of NxN tile (with border) in pixels

#define GAME_VERSION "v1.3.0"

// row maijor
// 0 — dead, 1 — alive
int cellArray[100 * 100] = {0};
int loopHash;

int main()
{
    // cellArray[35] = 1;
    // cellArray[67] = 1;
    // cellArray[99] = 1;

    // 1 * 32 + 3 = 32 + 3 = 35
    // 2 * 32 + 3 = 64 + 3 = 67
    // 3 * 32 + 3 = 96 + 3 = 99

    int aliveCount = 0; // how many alive cells around are there
    int teleport = 0;   // toggle teleport or not (T)
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

        // printf("gameState: %d\n", gameState);
        if (gameState && (framesCounter % frameRate == 0))
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


            // BEGIN: loop detection
            uint32_t hash = SuperFastHash((const char*)cellArray, sizeof(cellArray));
            // Check if this state has been seen before
            if (containsHash(hash)) {
                printf("Loop detected!\n");

                char downloadPath[512];
                getDownloadsPath(downloadPath, sizeof(downloadPath));

                char filePath[512];
                snprintf(filePath, sizeof(filePath), "%s%c%s", downloadPath, PATH_SEPARATOR, "loop.gif");

                // Now filePath contains the full path to the GIF file in the Downloads folder.
                printf("Exporting GIF to: %s\n", filePath);
                exportGif(filePath);
            }
            // Add the hash of the current state to the hash table

            // Image LoadImageFromTexture(Texture2D texture)

            addHash(hash);
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
                if (IsKeyPressed(KEY_SPACE))
                {
                    gameState = !gameState;
                    // printf("game mode toggled\n");
                }
                if (IsKeyPressed(KEY_T))
                {
                    teleport = !teleport;
                }
                if (IsKeyPressed(KEY_V))
                {
                    viewMode = !viewMode;
                }
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

                // Mouse Drawing
                mousePoint = GetMousePosition();
                mousePoint.x = mousePoint.x / camera.zoom;
                mousePoint.y = mousePoint.y / camera.zoom;
                // printf("Camera zoom level: %f\tMouce on %d %d\t%d %d\t%d %d\t%d %d\t", camera.zoom, (int)GetMousePosition().x, (int)GetMousePosition().y, (int)mousePoint.x, (int)mousePoint.y, ((int)mousePoint.x) / TILE_SIZE, ((int)mousePoint.y) / TILE_SIZE, ((int)mousePoint.x) % TILE_SIZE, ((int)mousePoint.y) % TILE_SIZE);
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    mousePoint = GetMousePosition();
                    // mousePoint.x = (mousePoint.x / camera.zoom) - camera.offset.x + camera.target.x;
                    // mousePoint.y = (mousePoint.y / camera.zoom) - camera.offset.y + camera.target.y;
                    mousePoint.x = (mousePoint.x - camera.offset.x) / camera.zoom + camera.target.x;
                    mousePoint.y = (mousePoint.y - camera.offset.y) / camera.zoom + camera.target.y;
                    if (
                        mousePoint.x >= 0 &&
                        mousePoint.x < ARRAY_SIZE * TILE_SIZE &&            // in bounds of array
                        // mousePoint.x <= screenWidth / camera.zoom &&     // in bounds of screen
                        mousePoint.y >= 0 &&
                        mousePoint.y < ARRAY_SIZE * TILE_SIZE               // in bounds of array
                        // mousePoint.y <= screenHeight / camera.zoom       // in bounds of screen
                    )    // check that mouse doesn't go out of bounds
                    {
                        if (camera.zoom > 1.5f)      // we don't care about the border when it's super tiny
                        {
                            if ((int)mousePoint.x % TILE_SIZE > 0 && (int)mousePoint.x % TILE_SIZE < (TILE_SIZE - 1) && (int)mousePoint.y % TILE_SIZE > 0 && (int)mousePoint.y % TILE_SIZE < (TILE_SIZE - 1))   // mouse doesn't draw on the boarder
                            {
                                cellArray[((int)(mousePoint.y) / TILE_SIZE) * ARRAY_SIZE + ((int)(mousePoint.x) / TILE_SIZE)] = 1;
                            }
                        }
                        else
                        {
                            cellArray[((int)(mousePoint.y) / TILE_SIZE) * ARRAY_SIZE + ((int)(mousePoint.x) / TILE_SIZE)] = 1;
                        }
                    }
                    // printf("Button on %d %d\t now: %d\n", ((int)mousePoint.x) / TILE_SIZE, ((int)mousePoint.y) / TILE_SIZE, gameState);
                }
                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
                {
                    mousePoint = GetMousePosition();
                    mousePoint.x = (mousePoint.x - camera.offset.x) / camera.zoom + camera.target.x;
                    mousePoint.y = (mousePoint.y - camera.offset.y) / camera.zoom + camera.target.y;
                    if (
                        mousePoint.x >= 0 &&
                        mousePoint.x < ARRAY_SIZE * TILE_SIZE &&                // in bounds of array
                        // mousePoint.x <= screenWidth / camera.zoom &&    // in bounds of screen
                        mousePoint.y >= 0 &&
                        mousePoint.y < ARRAY_SIZE * TILE_SIZE                // in bounds of array
                        // && mousePoint.y <= screenHeight / camera.zoom       // in bounds of screen
                    )    // check that mouse doesn't go out of bounds
                    {
                        if (camera.zoom > 1.5)      // we don't care about the border when it's super tiny
                        {
                            if ((int)mousePoint.x % TILE_SIZE > 0 && (int)mousePoint.x % TILE_SIZE < (TILE_SIZE - 1) && (int)mousePoint.y % TILE_SIZE > 0 && (int)mousePoint.y % TILE_SIZE < (TILE_SIZE - 1))   // mouse doesn't draw on the boarder
                            {
                                cellArray[((int)(mousePoint.y) / TILE_SIZE) * ARRAY_SIZE + ((int)(mousePoint.x) / TILE_SIZE)] = 0;
                            }
                        }
                        else
                        {
                            cellArray[((int)(mousePoint.y) / TILE_SIZE) * ARRAY_SIZE + ((int)(mousePoint.x) / TILE_SIZE)] = 0;
                        }
                    }
                    // printf("Button on %d %d\t now: %d\n", ((int)mousePoint.x) / TILE_SIZE, ((int)mousePoint.y) / TILE_SIZE, gameState);
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
                                    if (!viewMode) DrawRectangle(j * TILE_SIZE + 1, i * TILE_SIZE + 1, 18, 18, CLITERAL(Color){100, 158, 221, 255});
                                    else DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, 20, 20, CLITERAL(Color){100, 158, 221, 255});

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

// BEGIN: loopdetction
int containsHash(uint32_t hash)
{
    int index = hash % TABLE_SIZE;
    HashNode* node = hashTable[index];
    while (node != NULL)
    {
        if (node->hash == hash)
        {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

void addHash(uint32_t hash)
{
    int index = hash % TABLE_SIZE;
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->hash = hash;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

void freeHashTable()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        HashNode* node = hashTable[i];
        while (node != NULL)
        {
            HashNode* temp = node;
            node = node->next;
            free(temp);
        }
    }
}
// END: loopdetction

// BEGIN: superfasthash
uint32_t SuperFastHash (const char * data, int len)
{
    uint32_t hash = len, tmp;
    int rem;

    if (len <= 0 || data == NULL) return 0;

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for (;len > 0; len--)
    {
        hash  += get16bits (data);
        tmp    = (get16bits (data+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        data  += 2*sizeof (uint16_t);
        hash  += hash >> 11;
    }

    /* Handle end cases */
    switch (rem)
    {
        case 3: hash += get16bits (data);
                hash ^= hash << 16;
                hash ^= ((signed char)data[sizeof (uint16_t)]) << 18;
                hash += hash >> 11;
                break;
        case 2: hash += get16bits (data);
                hash ^= hash << 11;
                hash += hash >> 17;
                break;
        case 1: hash += (signed char)*data;
                hash ^= hash << 10;
                hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}
// END: superfasthash

// BEGIN: dowload path
// Function to get the Downloads folder path
void getDownloadsPath(char* path, size_t size) {
    #if defined(_WIN32) || defined(_WIN64)
        char userProfile[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, userProfile))) {
            snprintf(path, size, "%s\\Downloads", userProfile);
        }
    #else
        const char* home = getenv("HOME");
        if (home != NULL) {
            snprintf(path, size, "%s/Downloads", home);
        }
    #endif
}
// END: download path