#pragma once
#include "seWindowManager.h"
#include "../LR2/structure.h"


typedef struct ARR {
    void* data = NULL;
    int count = 0;
    int bufSize = 0;
    //
    int unitSize = 0;

    int Alloc(int structsize, int size);
    int Realloc(int size);
    int Free();

    int push_back(void* data, int size);
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
    CSTR name;
    void* data = NULL;
    int sizeX;
    int sizeY;
    int parent;
}IMG;

typedef struct WORKSPACE {

    
    //data
    game g;

    SkinHeader meta;

    bool loaded = false;
    char mainpath[MAX_PATH];
    byte* filedata = NULL;
    unsigned int filedatasize = 0;

    ARR subpath;
    ARR skinfileLines;
    ARR imgs;


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
    int LoadSkin2(char* path);
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
    

}WORKSPACE;

//every windows have procs, flags
//they have 


extern ARR workspaceList;