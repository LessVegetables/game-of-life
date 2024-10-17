#ifndef FILEIO_H
#define FILEIO_H

#include <string.h>
#include <stdio.h>
#include "game.h"   // for cellArray
#include "raylib.h" // for Colort struct

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define PATH_SEPARATOR '\\'
#else
#include <unistd.h>
#define PATH_SEPARATOR '/'
#endif

#define MAX_FILEPATH_SIZE 1024

void getDownloadsPath(char* path, size_t size);
void saveCellArrayToFile();
void readCellArrayFromFile(const char* filePath);
#endif