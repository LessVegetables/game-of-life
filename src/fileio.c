#include "fileio.h"

void getDownloadsPath(char* path, size_t size)
{
    #if defined(_WIN32) || defined(_WIN64)
        char userProfile[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, userProfile)))
        {
            snprintf(path, size, "%s\\Downloads", userProfile);
        }
    #else
        const char* home = getenv("HOME");
        if (home != NULL)
        {
            snprintf(path, size, "%s/Downloads", home);
        }
    #endif
}

void saveCellArrayToFile()
{
    FILE *out;
    char downloadsPath[MAX_FILEPATH_SIZE];
    getDownloadsPath(downloadsPath, sizeof(downloadsPath));

    char filePath[MAX_FILEPATH_SIZE];
    snprintf(filePath, sizeof(filePath), "%s%c%s", downloadsPath, PATH_SEPARATOR, "gameState.life");

    printf("Saving game state to: %s\n", filePath);

    out = fopen(filePath, "w");

    for (int i = 0; i < ARRAY_SIZE * ARRAY_SIZE; i++)
    {
        if (cellArray[i] == 0) continue;
        fprintf(out, "%d %d %d %d %d %d\n", i % ARRAY_SIZE, i / ARRAY_SIZE, cellArrayColor[i].r, cellArrayColor[i].g, cellArrayColor[i].b, cellArrayColor[i].a);
    }
    fclose(out);
}

void readCellArrayFromFile(const char *filePath)
{
    FILE *inp;

    printf("Reading game state from: %s\n", filePath);

    if ((inp = fopen(filePath, "r")) == NULL)
    {
        printf("Failed to open file\n");
        return;
    }

    clearArray();
    int x, y, r, g, b, a;
    for (int i = 0; i < ARRAY_SIZE * ARRAY_SIZE; i++)
    {
        fscanf(inp, "%d %d %d %d %d %d", &x, &y, &r, &g, &b, &a);
        if (y * ARRAY_SIZE + x < ARRAY_SIZE * ARRAY_SIZE)
        {
            cellArray[y * ARRAY_SIZE + x] = 1;
            cellArrayColor[y * ARRAY_SIZE + x] = (Color){(unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a};
        }
    }
    fclose(inp);
}