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
    bool Is_full();
    void* Get_new();
}ARR;

typedef struct SKINFILELINEREAD {
    int numTotal;

    CSTR filename;
    int num;

    CSTR line;
    bool isComment;
    CSVbuf csv;

    int ifgroup;

}SKINFILELINEREAD;

typedef struct IFUNIT {
    int data[10] = {};
    int depth = 0; //
    int order = 0;
    int parentID = 0; //
    int declare;

    bool hide = 0;

    int grCount=0;
}IFUNIT;

typedef struct SRCGR {
    CSTR path{};
    CSTR filename{};

    bool loaded = false;
    SDL_Texture* texture = NULL;
    CSTR name{};
    int sizeX, sizeY;

    int gr;
    int isIf;
}SRCGR;

typedef struct SRC {
    int gr;
    CSTR name{"noname"};
    int x, y;
    int sizeX, sizeY;
    int div_x, div_y, cycle;
    int timer;

    int num, align, keta; //for numSRC
    int muki, range, type, disable;//for sliderSRC

    int declare;
}SRC;

typedef struct WORKSPACE {

    
    //data
    game g;

    SkinHeader meta;

    bool loaded = false;
    char mainpath[MAX_PATH];

    byte* filedata = NULL;
    unsigned int filedatasize = 0;

    int skinSizeX = 640 , skinSizeY = 480;

    ARR arr_subpath; //CSTR
    ARR skinfileLines; //SKINFILELINEREAD
    ARR arr_ifunit; //IFUNIT

    ARR arr_SRCGR; //SRCGR
    ARR arr_SRC; //SRC

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

    bool wTreeView;
    int drawTreeView();
    

}WORKSPACE;

//every windows have procs, flags
//they have 


extern ARR workspaceList;