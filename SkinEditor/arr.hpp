#pragma once
#include <SDL3/SDL.h>

typedef struct ARR {
    void* data = NULL;
    int count = 0;
    int bufSize = 0;
    //
    int unitSize = 0;

    int Alloc(int structsize, int size);
    int Realloc(int size);
    int Free();

    int push_back(void* newdata);
    bool Is_full();
    void* Get_new();
    int InsertAt(int at, void* newdata);
    int DeleteAt(int at, void* newdata);
}ARR;