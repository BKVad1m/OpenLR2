#include "arr.hpp"
#include <SDL3/SDL.h>

int ARR::Alloc(int structsize, int size) {
    if (data) return -1; //already exist

    unitSize = structsize;

    data = malloc(unitSize * size);
    if (!data) return -1; //malloc fail
    bufSize = size;

    memset(data, 0, unitSize * size);
    return 0;
}

int ARR::Realloc(int size) { //set new max size
    void* temp = realloc(data, unitSize * size);
    if (!temp) return -1;

    data = temp;
    memset((void*)((int)data + unitSize * bufSize), 0, unitSize * (size - bufSize));
    bufSize = size;
    return 0;
}

int ARR::Free() {
    if (data) free(data);
    data = NULL;
    bufSize = 0;
    count = 0;
    return 0;
}

int ARR::push_back(void* newdata) {
    if (count >= bufSize) Realloc(bufSize * 2); //make buffer size double
    memcpy((void*)((int)data + unitSize * count), newdata, unitSize);
    count++;
    return 0;
}

bool ARR::Is_full() {
    return (count >= bufSize);
}

void* ARR::Get_new() {
    int oldcount = count;
    if (oldcount >= bufSize) Realloc(bufSize * 2); //make buffer size double
    count++;
    return (void*)((int)data + unitSize * oldcount);
}