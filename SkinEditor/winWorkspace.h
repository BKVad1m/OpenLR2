#pragma once
#include "seWindowManager.h"
#include "../LR2/structure.h"
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
}ARR;

typedef struct SKINFILELINEREAD {

    int pIF[100];
    
    CSTR filename;
    int num;

    int numTotal;

    CSTR line;
    bool isComment;
    CSVbuf csv;

}SKINFILELINEREAD;

typedef struct IFUNIT {
    int data[10];
    int depth; //max 100
    int group;
    int parentID; //
    int declare;
}IFUNIT;

typedef struct IMG {
    CSTR path{};
    SDL_Texture* texture;
    CSTR name{};
    int sizeX;
    int sizeY;
    //
    int parent = -1;
    int posX, posY;
}IMG;

typedef struct WORKSPACE {

    
    //data
    game g;

    SkinHeader meta;

    bool loaded = false;
    char mainpath[MAX_PATH];

    byte* filedata = NULL;
    unsigned int filedatasize = 0;

    int skinSizeX =640 , skinSizeY = 480;

    ARR arr_subpath; //CSTR
    ARR skinfileLines; //SKINFILELINEREAD
    ARR arr_imgpath; //CSTR
    ARR arr_IMG; //IMG
    ARR arr_bigimg;
    ARR arr_srcimg;

    int previewScreen;

    //mainwindow
    int num;
    char title[260];
    int proc();
    int init();
    bool alive;
    int draw();

    //subwindows
    //HOW TO ADD FEATURE - STEP 1 : declare flag and function
    int ScanSkins();
    bool wSkinList;
    int drawSkinList();

    int LoadSkin(char* path);
    int ReadSkin(char* path);
    int ParseSkin();

    int SaveSkinScript(char* path, bool split, bool nocomment);


    bool wSaveMenu;
    int drawSaveMenu();

    bool wTextEdit;
    int drawTextEdit();
    bool wPreview;
    int drawPreview();
    bool wCustomize;
    int drawCustomize();

    bool wImgManager;
    int drawImgManager();
    int loadSRC();

    bool wFileManager;
    int drawFileManager();
    

}WORKSPACE;

//every windows have procs, flags
//they have 


extern ARR workspaceList;