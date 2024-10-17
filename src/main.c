/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "screens.h"    // NOTE: Declares global (extern) variables and screens functions

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------
GameScreen currentScreen = LOGO;
Music music = { 0 };
Sound fxCoin = { 0 };

int screenWidth = 640;
int screenHeight = 640;

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void ChangeToScreen(int screen);     // Change to screen, no transition effect

static void TransitionToScreen(int screen); // Request transition to next screen
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)

static void UpdateDrawFrame(void);          // Update and draw one frame

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Life");

    InitAudioDevice();      // Initialize audio device

    // // Load global data (assets that must be available in all screens, i.e. font)
    // music = LoadMusicStream("resources/ambient.ogg");
    // fxCoin = LoadSound("resources/coin.wav");

    // SetMusicVolume(music, 1.0f);
    // PlayMusicStream(music);


    int framesCounter = 0;          // Useful to count frames
    int frameRate = 15;             // Every [frameRate] frames, the picture update (but game still runs at 60fps, for a smother drawing feeling)

    Vector2 mousePoint = { 0.0f, 0.0f };
    Vector2 mousePointWorld;

    //---------------------i think this should be a global variable...-------------------------
    // camera = { 0 };
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.target = (Vector2){ (float)(ARRAY_SIZE * TILE_SIZE / 2.0f), (float)(ARRAY_SIZE * TILE_SIZE / 2.0f) };    // essentially 1000.0f, 1000.0f
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    
    bool exitWindow = false;            // Flag to set window to exit
    int gameScreenUponExitRequest;          // Self explanatory*; also doubles as a flag to request window to exit;


    // Setup and init first screen
    currentScreen = LOGO;
    InitLogoScreen();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    // Main game loop
    while (exitWindow)    // Detect window close button or ESC key
    {
        // !WindowShouldClose();
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unload current screen data before closing
    switch (currentScreen)
    {
        case LOGO: UnloadLogoScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        default: break;
    }

    // Unload global data loaded
    UnloadMusicStream(music);
    UnloadSound(fxCoin);

    CloseAudioDevice();     // Close audio context

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
// Change to next screen, no transition
static void ChangeToScreen(GameScreen screen)
{
    // Unload current screen
    switch (currentScreen)
    {
        case LOGO: UnloadLogoScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        default: break;
    }

    // Init next screen
    switch (screen)
    {
        case LOGO: InitLogoScreen(); break;
        case TITLE: InitTitleScreen(); break;
        case GAMEPLAY: InitGameplayScreen(); break;
        case ENDING: InitEndingScreen(); break;
        default: break;
    }

    currentScreen = screen;
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    // UpdateMusicStream(music);       // NOTE: Music keeps playing between screens

    framesCounter++;

    switch(currentScreen)
    {
        case LOGO:
        {
            UpdateLogoScreen();

            if (FinishLogoScreen()) TransitionToScreen(TITLE);

        } break;
        case TITLE:
        {
            UpdateTitleScreen();

            if (FinishTitleScreen() == 1) TransitionToScreen(OPTIONS);
            else if (FinishTitleScreen() == 2) TransitionToScreen(GAMEPLAY);

        } break;
        case OPTIONS:
        {
            UpdateOptionsScreen();

            if (FinishOptionsScreen()) TransitionToScreen(TITLE);

        } break;
        case GAMEPLAY:
        {
            UpdateGameplayScreen();

            if (FinishGameplayScreen() == 1) TransitionToScreen(ENDING);
            //else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);

        } break;
        case ENDING:
        {
            UpdateEndingScreen();

            if (FinishEndingScreen() == 1) TransitionToScreen(TITLE);

        } break;
        default: break;
    }
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);

        switch(currentScreen)
        {
            case LOGO: DrawLogoScreen(); break;
            case TITLE: DrawTitleScreen(); break;
            case OPTIONS: DrawOptionsScreen(); break;
            case GAMEPLAY: DrawGameplayScreen(); break;
            case ENDING: DrawEndingScreen(); break;
            default: break;
        }

        //DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------
}
