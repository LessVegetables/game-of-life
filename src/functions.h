#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------
// current button presses and in game flags 

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// SideBar Functions Declaration
//----------------------------------------------------------------------------------
void InitSideBar(void);
void UpdateSideBar(void);
void DrawSideBar(void);
void UnloadSideBar(void);
int FinishSideBar(void);

#ifdef __cplusplus
}
#endif

#endif // SCREENS_H