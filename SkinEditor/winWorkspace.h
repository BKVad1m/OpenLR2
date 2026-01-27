#pragma once
#include "seWindowManager.h"
#include "../LR2/structure.h"
#include <SDL3/SDL.h>
#include "arr.hpp"
#include <imgui.h>

typedef struct SKINFILELINEREAD {
    int numTotal;

    CSTR filename;
    int num;

    CSTR line;
    bool isComment;
    bool isSEcomment;
    CSVbuf csv;

    int ifgroup;

}SKINFILELINEREAD;

typedef struct IFUNIT {
    int data[10] = {};
    int depth = 0; //
    int order = 0;
    int parentID = 0; //
    int declare = 0;

    bool hide = 0;

    int grCount=0;
}IFUNIT;

typedef struct IMG {
    CSTR name;
    void* data = NULL;
    int sizeX;
    int sizeY;
    int parent;
}IMG;

typedef struct SRCGR {
    CSTR path{};
    CSTR filename{};

    bool loaded = false;
    SDL_Texture* texture = NULL;
    CSTR name{};
    int sizeX, sizeY;

    int grID;
    int isIf;

    bool wildcard = false;
    bool fromWildcard = false;
    ARR arr_wildcard; //CSTR
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
    int click, panel, plusonly; //for buttonSRC

    int declare;
    int ifGroup;
    int srcID;
}SRC;

typedef struct DST {
    int leadDST = -1;
    CSTR name{"noname"};
    int src;
    int animation = 0;

    ARR arr_animation;//DST_ANIMATION

    int loop, timer;
    int op1, op2, op3;
    int op4;//for turntable

    int declare;
}DST;

typedef struct DST_ANIMATION{
    int time;
    float x, y, w, h;
    int acc;
    int a, r, g, b;
    int blend; //
    int filter; //Anti-aliasing
    float angle;
    int center; //angle axis, numpad
}DST_ANIMATION;

typedef struct SKINUNIT {
    int ID;
    int type; //0:text 1:buttoon 2:slider 3:onmouse 4:BGA 5:bargraph 6:number 7:mask //10:img
    CSTR name{ "noname" };

    int src;
    int dst;
}SKINUNIT, SEOBJ;

typedef enum HISTORYOP {
    overwriteLine,
    insertLine,
    removeLine,
    moveLine,
    group,
    ungroup,
    openFile = 255,
}HISTORYOP;

typedef struct HISTORY {
    HISTORYOP op;

    int target;
    SKINFILELINEREAD older;
    SKINFILELINEREAD newer;
}HISTORY;

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

    ARR arr_CustomFile; //CSTR
    ARR arr_SRCGR; //SRCGR
    ARR arr_SRC; //SRC
    ARR arr_DST; //DST

    ARR arr_seobj; //SKINUNIT, SEOBJ

    ARR arr_history; //HISTORY
    int previewScreen;

    //mainwindow
    int num;
    char title[260];
    int proc();
    int init();
    bool needInit = true;
    bool alive;
    int draw();

    //subwindows
    // 
    //
    int SeInit();
    int SeLoadInit();
    //HOW TO ADD FEATURE - STEP 1 : declare flag and function
    int wildcardTOAll(char* path);

    int ScanSkins();
    bool wSkinList;
    int drawSkinList();

    int LoadSkin(char* path);
    int ReadSkin(char* path);
    int ParseSkin();
    int currentLeadDST = -1;

    int SaveSkinScript(char* path, bool split, bool nocomment);


    bool wSaveMenu;
    int drawSaveMenu();

    //TextEdit
    bool wTextEdit;
    int drawTextEdit();
    bool hideComment = false;
    bool hideBlank = false;



    bool wPreview;
    int drawPreview();
    bool wCustomize;
    int drawCustomize();

    //ImgManager
    bool wImgManager;
    int drawImgManager();
    int gr_selected = 0;
    int src_selected = 0;
    int loadSRC();
    ImVec4 bgColor;
    int oldIf = -1;

    bool wFileManager;
    int drawFileManager();

    bool wTreeView;
    int drawTreeView();
    int split, nocomment, exist, success;
    bool wSaveMenuResult;
    char newPath[260] = "";

    //newfile
    bool wNewskin;
    int drawNewskin();
    
    //simpleview (mockup)
    bool wSimplePreview;
    int drawSimplePreview();

    int drawSrc(int iSRCGR, int iSRCID);
    //int drawSrc(int iSRCGR, int iSRCID, int posX, int posY);
    int drawSrc(int iSRCGR, int iSRCID, int posX, int posY, int w = -1, int h = -1, bool stretch = 0);
    //dstview
    bool wDstView;
    int drawDstView();
    int selected_dst;
    float DstViewTime;
    bool isDstViewTimeStop;

    int MakeObjects();
    bool wObjectManager;
    int drawObjectManager();
    int selected_obj;
    

}WORKSPACE;

//every windows have procs, flags
//they have 


extern ARR workspaceList;

int makeTransBackground();
