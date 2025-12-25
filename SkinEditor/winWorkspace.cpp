#include "imgui/imgui.h"
#include "../LR2/LR2_skinmanage.h"
#include "../LR2/LR2_skinload.h"
#include "../LR2/LR2_skindraw.h"
#include "../LR2/En_fileutil.h"
#include "../LR2/Scene07_Skinselect.h"
#include "../LR2/En_timer.h"
#include "DxLib//DxLib.h"
#include "winWorkspace.h"

#include "seWindowManager.h"
#include "ImageLoader.h"
#include "imgui/TextEditor.h"

#include "skin.h"
#include "op.h"
#include "arr.hpp"
#include "seHelper.h"

const char* SKINTYPESTR[]= {
    "7KEYS",
    "5KEYS",
    "14KEYS",
    "10KEYS",
    "9KEYS",
    "SELECT",
    "DECIDE",
    "RESULT",
    "KEYCONFIG",
    "SKINSELECT",
    "SOUNDSET",
    "THEME",
    "7KEYSBATTLE",
    "5KEYSBATTLE",
    "9KEYSBATTLE",
    "COURSERESULT",
    "OPENING",
    "MODESELECT",
    "MODEDECIDE",
    "COURSESELECT",
    "COURSEEDIT"
}; 
extern SDL_Renderer* renderer;
///////////////////
SDL_Texture* transBackground;
byte TexTransparent[] =    "\x42\x4D\x76\x02\x00\x00\x00\x00\x00\x00\x76\x00\x00\x00\x28\x00\x00\x00\x20\x00\x00\x00\x20\x00\x00\x00\x01\x00\x04\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\x00\x00\x80\x00\x00\x00\x80\x80\x00\x80\x00\x00\x00\x80\x00\x80\x00\x80\x80\x00\x00\x80\x80\x80\x00\xC0\xC0\xC0\x00\x00\x00\xFF\x00\x00\xFF\x00\x00\x00\xFF\xFF\x00\xFF\x00\x00\x00\xFF\x00\xFF\x00\xFF\xFF\x00\x00\xFF\xFF\xFF\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x88\x88\x88\x88\x88\x88\x88\x88\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
int makeTransBackground(){
    int x, y;
    LoadTextureFromMemory(TexTransparent, sizeof(TexTransparent), renderer, &transBackground, &x, &y);
    //LoadTextureFromRawMemory(TexTransparent, renderer, &transBackground, 64, 64, 1);
    return 0;
}


///////////////////
int WORKSPACE::proc() {

    return 0;
}
int WORKSPACE::init() {

    return 0;
}
int WORKSPACE::draw() {
    ImGui::Begin(title, &alive, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", NULL, &wSkinList)) {
                ScanSkins();
            }
            if (loaded) {
                ImGui::MenuItem("Save as", NULL, &wSaveMenu);
            }
            ImGui::EndMenu();
        }
        if (loaded) {
            
            if (ImGui::BeginMenu("Windows")) {
                //HOW TO ADD FEATURE - STEP 3 : add menu and flag here
                ImGui::MenuItem("Text Editor", NULL, &wTextEdit);
                ImGui::MenuItem("Preview", NULL, &wPreview);
                ImGui::MenuItem("Customize", NULL, &wCustomize);
                ImGui::MenuItem("ImgManager", NULL, &wImgManager);
                //if (ImGui::MenuItem("ImgManager", NULL, &wImgManager)) { loadSRC(); };
                ImGui::MenuItem("fileManager", NULL, &wFileManager);
                ImGui::MenuItem("treeVeiw", NULL, &wTreeView);
                ImGui::EndMenu();
            }
        }
        ImGui::EndMenuBar();
    }

    ImGui::Text("%s", loaded? "" : "no skin loaded");

    ImGui::End();

    //subwindows
    //HOW TO ADD FEATURE - STEP 4 : call function. end
    if (wSkinList) drawSkinList();

    if (wSaveMenu) drawSaveMenu();

    if (wTextEdit) drawTextEdit();
    if (wPreview) drawPreview();
    if (wCustomize) drawCustomize();
    if (wImgManager) drawImgManager();
    if (wFileManager) drawFileManager();
    if (wTreeView) drawTreeView();


    return 0;
}

int WORKSPACE::ScanSkins() {
    InitSkinData(&g.skinData);
    MakeSkinList(&g.skinData, CSTR("LR2files\\Theme\\"));
    MakeSkinList(&g.skinData, CSTR("LR2files\\Sound\\"));
    return 1;
}

int WORKSPACE::drawSkinList() {
    static int item_selected_idx = 0;
    int oldSelected = item_selected_idx;
    static SDL_Texture* preview_tex;
    static int preview_size_x, preview_size_y;
    static bool isPreview = false;

    ImGui::Begin("Open from skinlist", &wSkinList);

    if (ImGui::BeginChild(ImGuiChildFlags_FrameStyle, { 300,-1 }))
    {
        for (int n = 0; n < g.skinData.Count; n++)
        {
            const bool is_selected = (item_selected_idx == n);
            char itemname[260];
            snprintf(itemname, sizeof(itemname), "%02d:%s -%s", n, g.skinData.Data[n].title.outstr(), SKINTYPESTR[g.skinData.Data[n].type]);
            if (ImGui::Selectable(itemname, is_selected)) {
                item_selected_idx = n;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndChild();
    }
    

    int& m = item_selected_idx;
    ImGui::SameLine(0, 0);
    ImGui::BeginGroup();
    //ImTextureRef preview_texid = io.Fonts->TexRef;

    if (oldSelected != m) {
        isPreview = LoadTextureFromFile(g.skinData.Data[m].thumbnail.outstr(), renderer, &preview_tex, &preview_size_x, &preview_size_y);
        oldSelected = m;
    }
    ImGui::Image(isPreview ? preview_tex : NULL, { 320, 240 }, { 0, 0 }, { 1, 1 });
    ImGui::Text("%s", g.skinData.Data[m].skinFile.outstr());
    ImGui::Text("%s", g.skinData.Data[m].title.outstr());
    ImGui::Text("%s", g.skinData.Data[m].maker.outstr());
    ImGui::Text("%s", SKINTYPESTR[g.skinData.Data[m].type]);
    ImGui::Text("%d * %d", g.skinData.Data[m].targetX, g.skinData.Data[m].targetY);

    if (ImGui::Button("LOAD", { 0, 0 })) {
        meta = g.skinData.Data[m];
        snprintf(title, 260, "%s -%s", meta.title.outstr(), SKINTYPESTR[meta.type]);
        
        strncpy(mainpath, g.skinData.Data[m].skinFile.outstr(), MAX_PATH);
        LoadSkin(mainpath);
        wSkinList = false;
        loaded = true;
        
    }

    ImGui::SameLine(0, 3);
    ImGui::Button("CLONE", { 0, 0 });
    ImGui::SameLine(0, 3);
    ImGui::Button("BROWSE", { 0, 0 });
    ImGui::SameLine(0, 0);
    ImGui::EndGroup();
    ImGui::End();

    return 0;
}

int WORKSPACE::ReadSkin(char* path) {
    FILE* pFile;

    pFile = fopen(path, "rb");
    if (pFile) {
        int c = 0;
        
        SKINFILELINEREAD& readS = ((SKINFILELINEREAD*)skinfileLines.data)[skinfileLines.count];
        readS.line.resize(1024);
        sprintf(readS.line, "$FILE %s start", path);
        readS.isComment = true;
        readS.numTotal = skinfileLines.count;
        readS.num = 0;
        skinfileLines.count++;

        while (1) {
            int i = skinfileLines.count;
            SKINFILELINEREAD& read = ((SKINFILELINEREAD*)skinfileLines.data)[i];
            read.line.resize(1024);
            read.filename.resize(260);

            if (fgets(read.line.outstr(), 1023, pFile) == 0) break;
            DealWhiteSpace(&read.line);
            read.numTotal = i;
            read.filename.assign(path);
            read.num = c;
            read.isComment = (*read.line.atPos(0) != '#');

            if(!read.isComment) SplitCSV(read.line, &read.csv, ",");
            
            skinfileLines.count++;
            c++;
            if (skinfileLines.count == skinfileLines.bufSize) skinfileLines.Realloc(skinfileLines.bufSize + 1000);

            SKINFILELINEREAD& read2 = ((SKINFILELINEREAD*)skinfileLines.data)[i];
            if (read2.line.left(8).isSame("#INCLUDE")) {
                if (read2.csv.str[1].canOpenFile()) {
                    CSTR& tmp = read2.csv.str[1];
                    arr_subpath.push_back(&tmp);

                    ReadSkin(read2.csv.str[1]);
                }
            }
        }
        SKINFILELINEREAD& readE = ((SKINFILELINEREAD*)skinfileLines.data)[skinfileLines.count];
        readE.line.resize(1024);
        sprintf(readE.line, "$FILE %s end", path);
        readE.isComment = true;
        readE.numTotal = skinfileLines.count;
        readE.num = 0;
        skinfileLines.count++;
        fclose(pFile);
    }

    return 0;
}
int WORKSPACE::ParseSkin() {

    int IFcur = 0;
    int IFdepth = 0;    
    int cOrder = 0;

    int grCount = 0;
    int grInIf = 0;
    
    LoadCommandHelp("skinHelper.txt");

    for (int i = 0; i < skinfileLines.count; i++) {
        SKINFILELINEREAD& read = ((SKINFILELINEREAD*)skinfileLines.data)[i];
        bool isif = false;
        
        if (i == 0) {
            IFUNIT tif;
            arr_ifunit.push_back(&tif);
        }

        if (read.line.left(3).isSame("#IF")) {
            IFdepth++;

            isif = true;

            IFUNIT tif;
            for (int val = 0; val < 10; val++) {
                tif.data[val] = read.csv.val[val];
            }
            tif.depth = IFdepth;
            tif.order = cOrder = 0;
            tif.parentID = IFcur;
            tif.declare = i;

            IFcur = arr_ifunit.count;
            arr_ifunit.push_back(&tif);
            read.ifgroup = IFcur;

        }
        else if (read.line.left(7).isSame("#ELSEIF")) {
            isif = true;

            IFUNIT tif;
            for (int val = 0; val < 10; val++) {
                tif.data[val] = read.csv.val[val];
            }
            tif.depth = IFdepth;
            tif.order = ++cOrder;
            tif.parentID = ((IFUNIT*)arr_ifunit.data)[IFcur].parentID;
            tif.declare = i;

            arr_ifunit.push_back(&tif);;
            read.ifgroup = IFcur + cOrder;

            grCount -= ((IFUNIT*)arr_ifunit.data)[read.ifgroup].grCount;
            grInIf = 0;
        }
        else if (read.line.left(5).isSame("#ELSE")) {
            isif = true;

            IFUNIT tif;
            for (int val = 0; val < 10; val++) {
                tif.data[val] = read.csv.val[val];
            }
            tif.depth = IFdepth;
            tif.order = ++cOrder;
            tif.parentID = ((IFUNIT*)arr_ifunit.data)[IFcur].parentID;
            tif.declare = i;

            arr_ifunit.push_back(&tif);
            read.ifgroup = IFcur + cOrder;

            grCount -= ((IFUNIT*)arr_ifunit.data)[read.ifgroup].grCount;
            grInIf = 0;
        }
        else if (read.line.left(6).isSame("#ENDIF")) {
            isif = true;

            read.ifgroup = IFcur + cOrder;
            IFcur = ((IFUNIT*)arr_ifunit.data)[IFcur].parentID;;

            IFdepth--;
            cOrder = 0;

            grCount -= ((IFUNIT*)arr_ifunit.data)[read.ifgroup].grCount;
            grInIf = 0;
        }
        else {
            read.ifgroup = IFcur + cOrder;
        }


        if (isif) continue;


        if (read.line.left(11).isSame("#CUSTOMFILE")) {
            CSTR* tmpstr = (CSTR*)arr_CustomFile.Get_new();
            tmpstr->assign(read.csv.str[2]);
        }


        else if (read.line.left(6).isSame("#IMAGE")) {
            /*CSTR& tmp = read.csv.str[1];
            arr_imgpath.push_back(&tmp);*/

            SRCGR* tmp = (SRCGR*)(arr_SRCGR.Get_new());
            tmp->path.assign(read.csv.str[1]);
            char* cur = strrchr(read.csv.str[1].outstr(), '/');
            if (cur == NULL) cur = strrchr(read.csv.str[1].outstr(), '\\');
            if (cur)         tmp->filename.assign(cur + 1);

            tmp->gr = grCount;
            tmp->isIf = read.ifgroup;
            
            for (int wc = 0; wc < arr_CustomFile.count; wc++) {
                if (tmp->path.isSame( ((CSTR*)arr_CustomFile.data)[wc].outstr()) ){
                    WIN32_FIND_DATA FindFileData;
                    LPWIN32_FIND_DATAA lpFindFileData;
                    HANDLE hFindFile;
                    
                    CSTR str1(tmp->path.left(tmp->path.findStrPos("*")));
                    CSTR str2(tmp->path.right(tmp->path.length() - str1.length() - 1));
                    CSTR str3(str1);
                    str3.add("*");
                   
                    hFindFile = FindFirstFileA(str3, (LPWIN32_FIND_DATAA)&FindFileData);
                    if (hFindFile != (HANDLE)-1) {
                        do {
                            if (strcmp("..", (char*)FindFileData.cFileName) && strcmp(".", (char*)FindFileData.cFileName)) {
                                    
                                SRCGR* tmp2 = (SRCGR*)arr_SRCGR.Get_new();
                                tmp2->path.assign(str1);
                                tmp2->path.add(FindFileData.cFileName);
                                tmp2->filename.assign(FindFileData.cFileName);
                                    
                                //tmp2->arr_wildcard
                                tmp2->gr = grCount;
                                tmp2->isIf = read.ifgroup;
                            }
                        } while (FindNextFileA(hFindFile, (LPWIN32_FIND_DATAA)&FindFileData));
                        FindClose(hFindFile);
                    }

                }
            }
            grCount++;
            ((IFUNIT*)arr_ifunit.data)[read.ifgroup].grCount++;
        }


        ////////////////

        //if () {}
        if (read.csv.str[0].isSame("#SRC_IMAGE")) {

            if (read.csv.val[2] == GrH_Stage || read.csv.val[2] == GrH_BackBMP || 
                read.csv.val[2] == GrH_Banner || read.csv.val[2] == GrH_Preview || 
                read.csv.val[2] == 110 || read.csv.val[2] == 111) continue;

            SRC *src = (SRC*)(arr_SRC.Get_new());
                        
            src->gr = read.csv.val[2];
            src->x = read.csv.val[3];
            src->y = read.csv.val[4];

            SRCGR& img = ((SRCGR*)arr_SRCGR.data)[src->gr];

            src->sizeX = read.csv.val[5];// == -1 ? img.sizeX - src->x : read.csv.val[5];
            src->sizeY = read.csv.val[6];// == -1 ? img.sizeY - src->y : read.csv.val[6];

            src->div_x = read.csv.val[7];
            src->div_y = read.csv.val[8];
            src->cycle = read.csv.val[9];
            src->timer = read.csv.val[10];

            src->declare = read.numTotal;

            char tmp[260];
            if (src->timer) {
                sprintf(tmp, "T%d(%s) : %d*%d ##%d", src->timer, timerName(src->timer), src->sizeX, src->sizeY, read.numTotal);
            }
            else {
                sprintf(tmp, "noname : %d*%d ##%d", src->sizeX, src->sizeY, read.numTotal);
            }
            src->name.assign(tmp);
        }

        else if (read.csv.str[0].isSame("#SRC_NUMBER")) { 

            if (read.csv.val[2] == GrH_Stage || read.csv.val[2] == GrH_BackBMP ||
                read.csv.val[2] == GrH_Banner || read.csv.val[2] == GrH_Preview ||
                read.csv.val[2] == 110 || read.csv.val[2] == 111) continue;

            SRC* src = (SRC*)(arr_SRC.Get_new());

            src->gr = read.csv.val[2];
            src->x = read.csv.val[3];
            src->y = read.csv.val[4];

            SRCGR& img = ((SRCGR*)arr_SRCGR.data)[src->gr];

            src->sizeX = read.csv.val[5];// == -1 ? img.sizeX - src->x : read.csv.val[5];
            src->sizeY = read.csv.val[6];// == -1 ? img.sizeY - src->y : read.csv.val[6];

            src->div_x = read.csv.val[7]; //have to be 10, 11, 24 set divs
            src->div_y = read.csv.val[8];
            src->cycle = read.csv.val[9];
            src->timer = read.csv.val[10];

            src->num = read.csv.val[11];
            src->align = read.csv.val[12];
            src->keta = read.csv.val[13];

            src->declare = read.numTotal;

            char tmp[260];
            if (src->timer || src->type) {
                sprintf(tmp, "T%d(%s) : %s %d*%d ##%d", src->timer, timerName(src->timer), numberName(src->num), src->sizeX, src->sizeY, read.numTotal);
            }
            else {
                sprintf(tmp, "noname : %d*%d ##%d", src->sizeX, src->sizeY, read.numTotal);
            }
            src->name.assign(tmp);
        }

        else if (read.csv.str[0].isSame("#SRC_SLIDER")) {

            if (read.csv.val[2] == GrH_Stage || read.csv.val[2] == GrH_BackBMP ||
                read.csv.val[2] == GrH_Banner || read.csv.val[2] == GrH_Preview ||
                read.csv.val[2] == 110 || read.csv.val[2] == 111) continue;

            SRC* src = (SRC*)(arr_SRC.Get_new());

            src->gr = read.csv.val[2];
            src->x = read.csv.val[3];
            src->y = read.csv.val[4];

            SRCGR& img = ((SRCGR*)arr_SRCGR.data)[src->gr];

            src->sizeX = read.csv.val[5];// == -1 ? img.sizeX - src->x : read.csv.val[5];
            src->sizeY = read.csv.val[6];// == -1 ? img.sizeY - src->y : read.csv.val[6];

            src->div_x = read.csv.val[7]; //have to be 10, 11, 24 set divs
            src->div_y = read.csv.val[8];
            src->cycle = read.csv.val[9];
            src->timer = read.csv.val[10];

            src->muki = read.csv.val[11];
            src->range = read.csv.val[12];
            src->type = read.csv.val[13];
            src->disable = read.csv.val[14];

            src->declare = read.numTotal;

            char tmp[260];
            if (src->timer || src->type) {
                sprintf(tmp, "T%d(%s) : %s %d*%d ##%d", src->timer, timerName(src->timer), sliderName(src->type), src->sizeX, src->sizeY, read.numTotal);
            }
            else {
                sprintf(tmp, "noname : %d*%d ##%d", src->sizeX, src->sizeY, read.numTotal);
            }
            src->name.assign(tmp);
        }

        else if (read.csv.str[0].isSame("#SRC_BARGRAPH")) {

            if (read.csv.val[2] == GrH_Stage || read.csv.val[2] == GrH_BackBMP ||
                read.csv.val[2] == GrH_Banner || read.csv.val[2] == GrH_Preview ||
                read.csv.val[2] == 110 || read.csv.val[2] == 111) continue;

            SRC* src = (SRC*)(arr_SRC.Get_new());

            src->gr = read.csv.val[2];
            src->x = read.csv.val[3];
            src->y = read.csv.val[4];

            SRCGR& img = ((SRCGR*)arr_SRCGR.data)[src->gr];

            src->sizeX = read.csv.val[5];// == -1 ? img.sizeX - src->x : read.csv.val[5];
            src->sizeY = read.csv.val[6];// == -1 ? img.sizeY - src->y : read.csv.val[6];

            src->div_x = read.csv.val[7];
            src->div_y = read.csv.val[8];
            src->cycle = read.csv.val[9];
            src->timer = read.csv.val[10];

            src->type = read.csv.val[11];
            src->muki = read.csv.val[12];

            src->declare = read.numTotal;

            char tmp[260];
            if (src->timer || src->type) {
                sprintf(tmp, "T%d(%s) : %s %d*%d ##%d", src->timer, timerName(src->timer), bargraphName(src->type), src->sizeX, src->sizeY, read.numTotal);
            }
            else {
                sprintf(tmp, "noname : %d*%d ##%d", src->sizeX, src->sizeY, read.numTotal);
            }
            src->name.assign(tmp);
         }

        else if (read.csv.str[0].isSame("#SRC_BUTTON")) {

            if (read.csv.val[2] == GrH_Stage || read.csv.val[2] == GrH_BackBMP ||
                read.csv.val[2] == GrH_Banner || read.csv.val[2] == GrH_Preview ||
                read.csv.val[2] == 110 || read.csv.val[2] == 111) continue;

            SRC* src = (SRC*)(arr_SRC.Get_new());

            src->gr = read.csv.val[2];
            src->x = read.csv.val[3];
            src->y = read.csv.val[4];

            SRCGR& img = ((SRCGR*)arr_SRCGR.data)[src->gr];

            src->sizeX = read.csv.val[5];// == -1 ? img.sizeX - src->x : read.csv.val[5];
            src->sizeY = read.csv.val[6];// == -1 ? img.sizeY - src->y : read.csv.val[6];
                
            src->div_x = read.csv.val[7];
            src->div_y = read.csv.val[8];
            src->cycle = read.csv.val[9];
            src->timer = read.csv.val[10];

            src->type = read.csv.val[11];
            src->click = read.csv.val[12];
            src->panel = read.csv.val[13];
            src->plusonly = read.csv.val[14];

            src->declare = read.numTotal;

            char tmp[260];
            if (src->timer || src->type) {
                sprintf(tmp, "T%d(%s) : %s %d*%d ##%d", src->timer, timerName(src->timer), buttonName(src->type), src->sizeX, src->sizeY, read.numTotal);
            }
            else {
                sprintf(tmp, "noname : %d*%d ##%d", src->sizeX, src->sizeY, read.numTotal);
            }
            src->name.assign(tmp);
        }

    }
    



    //images load
    int gr = 0;
    for (int n = 0; n < arr_SRCGR.count; n++) {
        SRCGR& img = ((SRCGR*)arr_SRCGR.data)[n];
        CSTR& path = ((SRCGR*)arr_SRCGR.data)[n].path;


        if (path.isSame("CONTINUE")) {
            gr++;
            
            img.sizeX = 0;
            img.sizeY = 0;
            continue;
        }

        //TODO: deal *

        bool isLoaded = LoadTextureFromFile(path.outstr(), renderer, &(img.texture), &img.sizeX, &img.sizeY);

        if (!isLoaded) {
            int dxf = DxLib::FileRead_open(path);
            if (dxf) {
                void* Buffer;
                int FileSize;

                FileSize = FileRead_size(path.outstr());
                Buffer = malloc(FileSize);
                DxLib::FileRead_read(Buffer, FileSize, dxf);

                DxLib::FileRead_close(dxf);

                isLoaded = LoadTextureFromMemory(Buffer, FileSize,renderer, &(img.texture), &img.sizeX, &img.sizeY);

                free(Buffer);
            }
        }

        if (isLoaded) {
            img.loaded = true;
        }
    }
    return 0;
}

int WORKSPACE::LoadSkin(char* path) {
    //FILE* pFile;
    //pFile = fopen(path, "rb");
    //if (pFile) {
    //    fseek(pFile, 0, SEEK_END);
    //    filedatasize = ftell(pFile);

    //    if (filedata) free(filedata);
    //    filedata = (byte*)malloc(filedatasize + 1);//we need null to work
    //    if (filedata) {
    //        memset(filedata, 0, filedatasize + 1);
    //        fseek(pFile, 0, SEEK_SET);
    //        fread(filedata, 1, filedatasize, pFile);
    //    }

    //    fclose(pFile);
    //}
    skinSizeX = meta.targetX;
    skinSizeY = meta.targetY;

    skinfileLines.Free();
    skinfileLines.Alloc(sizeof(SKINFILELINEREAD), 1000);
    arr_subpath.Free();
    arr_subpath.Alloc(sizeof(CSTR), 1);
    arr_ifunit.Free();
    arr_ifunit.Alloc(sizeof(IFUNIT), 50);
    arr_SRCGR.Free();
    arr_SRCGR.Alloc(sizeof(SRCGR), 10);
    arr_SRC.Free();
    arr_SRC.Alloc(sizeof(SRC), 100);
    arr_CustomFile.Free();
    arr_CustomFile.Alloc(sizeof(CSTR), 10);

    ReadSkin(path);
    ParseSkin();

    LR2SEInit(&g);

    for (int i = 0; i < 200; i++) g.skstruct.caption[i].fillzero();
    for (int i = 0; i < 10; i++) g.skstruct.helpfilePath[i].fillzero();
    for (int i = 0; i < 20; i++) g.skstruct.customfileRANDOM[i].fillzero();
    for (int i = 0; i < 20; i++) g.skstruct.customfile[i].fillzero();
    g.skstruct.skinMD5.fillzero();
    g.skstruct.skFontname.fillzero();
    memset(&g.skstruct, 0, sizeof(skstruct));
    for (int i = 0; i < 200; i++) g.skstruct.caption[i].fillzero();
    for (int i = 0; i < 200; i++) g.skstruct.caption[i].assign("(null)");
    for (int i = 0; i < 200; i++) g.skstruct.GrHandle[i] = -1;
    for (int i = 0; i < 10; i++) g.skstruct.helpfilePath[i].fillzero();
    for (int i = 0; i < 10; i++) g.skstruct.helpfilePath[i].assign("(null)");
    //g.skstruct.skFontname.assign(&config.skin.fontname);
    //g.skstruct.disableimagefont = (config.skin.disableimagefont != 0);
    g.skstruct.skinMD5.fillzero();
    g.skstruct.skinMD5.resize2(34);
    AllocDrawingBuffer(&g.skstruct.drBuf);
    LoadScene(&g.skstruct, mainpath, 0, 0);


    SetWindowUserCloseEnableFlag(0);

    //previewScreen = MakeScreen(640, 480);
    //SetDrawScreen(previewScreen);
    //previewScreen = MakeSoftImage(640, 480);
    previewScreen = MakeARGB8ColorSoftImage(skinSizeX, skinSizeY);

    return 0;
}


int WORKSPACE::drawCustomize() {
    char title[260];
    snprintf(title, sizeof(title), "Customize##%d", num);
    ImGui::Begin(title, &wCustomize);

    for (int i = 0; i < meta.custom_count; i++) {
        SkinCustom& cu = meta.customs[i];

        ImGui::Text("%s", cu.title);
        ImGui::SameLine();
        
        char item[64];

        if(cu.dst_op_start)
            snprintf(item, sizeof(item), "%03d : %s", cu.dst_op_start + cu.dst_op_selected, cu.op_label[cu.dst_op_selected].outstr());
        else
            snprintf(item, sizeof(item), "FILE : %s", cu.op_label[cu.dst_op_selected].outstr());

        char label[32];
        snprintf(label, sizeof(label), "##%d", i);
        if (ImGui::BeginCombo(label, item, ImGuiComboFlags_WidthFitPreview))
        {
            for (int n = 0; n < cu.dst_op_count; n++)
            {
                if (cu.dst_op_start)
                    snprintf(item, sizeof(item), "%03d : %s", cu.dst_op_start + n, cu.op_label[n].outstr());
                else
                    snprintf(item, sizeof(item), "FILE : %s", cu.op_label[n].outstr());

                const bool is_selected = (cu.dst_op_selected == n);
                if (ImGui::Selectable(item, is_selected))
                    cu.dst_op_selected = n;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
    
    ImGui::End();
    return 0;
}

int WORKSPACE::drawPreview() {
    static SDL_Texture* preview_tex;
    
    char title[260];
    snprintf(title, sizeof(title), "Preview##%d", num);

    static bool playing = false;

    LR2SEDrawLoop(&g, previewScreen, skinSizeX, skinSizeY);
    
    ImGui::Begin(title, &wPreview, ImGuiWindowFlags_HorizontalScrollbar);
    LoadTextureFromRawMemory(GetImageAddressSoftImage(previewScreen), renderer, &preview_tex, skinSizeX, skinSizeY, 4);
    ImGui::Image(preview_tex, { (float)skinSizeX, (float)skinSizeY }, { 0, 0 }, { 1, 1 });

    if (ImGui::Button("Start")) {
        LR2SESceneInit(&g, meta.type);
        playing = true;
    }
    if(playing)
        LR2SESceneProc(&g, meta.type);

    //D_IDirect3DSurface9* d9 = (D_IDirect3DSurface9*)GetUseDirect3D9BackBufferSurface();
    //d9->GetDC()

    ImGui::End();

    if (!wPreview) SetWindowVisibleFlag(0);
    return 0;
}

int WORKSPACE::drawTextEdit() {
    char title[260];
    snprintf(title, sizeof(title), "TextEdit##%d", num);

    ImGui::Begin(title, &wTextEdit, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar);

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Hide")) {

            ImGui::MenuItem("Blank", NULL, &hideBlank);
            ImGui::MenuItem("Comment", NULL, &hideComment);

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }


    for (int n = 0; n < skinfileLines.count; n++) {
        SKINFILELINEREAD& read = ((SKINFILELINEREAD*)skinfileLines.data)[n];
        /*char itemname[260];
        snprintf(itemname, sizeof(itemname), "%04d:%04d: %s", read.numTotal, read.num, read.line.outstr());*/
        ImVec4 color;
        color = { 1.0f, 1.0f, 1.0f, 1.0f };

        if (hideBlank && *read.line.atPos(0) == '\0') continue;
        if (hideComment && read.isComment) continue;

        //ImGui::PushID(n);
        //ImGui::Button(" ");
        ///*if (ImGui::BeginItemTooltip())
        //{
        //    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        //    ImGui::Text("%d", read.ifgroup);
        //    ImGui::EndTooltip();
        //}*/

        //ImGui::PopID();

        //ImGui::SameLine();

        IFUNIT& ifs = ((IFUNIT*)arr_ifunit.data)[read.ifgroup];
        bool head = (read.numTotal == ifs.declare);
        bool isHide = ifs.hide;
        int parIFCursor = ifs.parentID;
        for (int i = 0; i < ifs.depth; i++) {
            isHide |= ((IFUNIT*)arr_ifunit.data)[parIFCursor].hide;
            parIFCursor = ((IFUNIT*)arr_ifunit.data)[parIFCursor].parentID;
        }
        if (isHide && !head) continue;

        ImGui::PushID(n);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV((head + isHide) / 3.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV((head + isHide) / 3.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV((head + isHide) / 3.0f, 0.8f, 0.8f));
        if (ImGui::Button(" ")) {

            if (read.numTotal == ifs.declare) {
                ifs.hide ^= 1;
            }
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
        ImGui::SameLine();

        ImGui::Text("%d ", read.ifgroup);
        ImGui::SameLine();

        if (read.isComment) {
            ImGui::TextDisabled("%04d:%04d: ", read.numTotal, read.num);

            //ImGui::SameLine();
            //ImGui::PushID(n);
            //ImGui::Button(" ");
            //
            ///*if (ImGui::BeginItemTooltip())
            //{
            //    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            //    ImGui::Text("%d", read.ifgroup);
            //    ImGui::EndTooltip();
            //}*/

            //ImGui::PopID();

            ImGui::SameLine();
            ImGui::TextDisabled("%s", read.line.outstr());
            //ImGui::TextColored(color, "%04d:%04d: %s", read.numTotal, read.num, read.line.outstr());


        }
        else {
            //ImGui::TextColored(color, "%04d:%04d: %s", read.numTotal, read.num, read.line.outstr());

            for (int depth = 0; depth < ((IFUNIT*)(arr_ifunit.data))[read.ifgroup].depth; depth++) {
                ImGui::TextColored(color, "_");
                ImGui::SameLine();
            }
            ImGui::TextColored(color, "%04d:%04d: ", read.numTotal, read.num);

            ImGui::SameLine();

            char tablename[260];
            snprintf(tablename, sizeof(tablename), "##w%d_text", num);
            if (ImGui::BeginTable(tablename, 22, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoPadInnerX, {4000,30},3999))
            {
                ImGui::PushItemWidth(FLT_MAX);
                ImGui::TableNextRow();
                for (int column = 0; column < 22; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    if (read.csv.str[column].atPos(0) == nullptr) {
                        ImGui::TextDisabled("%s", read.csv.str[column]);
                    }
                    else {
                        //ImGui::Text("%s", read.csv.str[column]);
                        char inputname[260];
                        sprintf(inputname, "##%d_%d_cell", read.numTotal, column);
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        ImGui::InputText(inputname, read.csv.str[column], 260, ImGuiInputTextFlags_AutoSelectAll);
                        
                    }

                    if (ImGui::BeginItemTooltip())
                    {
                        //ImGui::Text("%d",ifs.grCount);
                        ImGui::Text("%s", GetCommandHelp(read.csv.str[0], column));
                        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                        ImGui::TextUnformatted(read.csv.str[column]);
                        if (read.csv.str[0].isSame("#SRC_IMAGE")) {

                            int handle = read.csv.val[2];

                            SRCGR& img = ((SRCGR*)arr_SRCGR.data)[handle];

                            int iX = read.csv.val[3];
                            int iY = read.csv.val[4];
                            int iW = read.csv.val[5] == -1 ? img.sizeX - iX : read.csv.val[5];
                            int iH = read.csv.val[6] == -1 ? img.sizeY - iY : read.csv.val[6];

                            if (img.texture != NULL) {
                                ImVec2 display_min = ImVec2(iX / (float)img.sizeX, iY / (float)img.sizeY);
                                ImVec2 display_max = ImVec2((iX + iW) / (float)img.sizeX, (iY + iH) / (float)img.sizeY);
                                ImVec2 display_size = ImVec2(iW, iH);

                                ImGui::Image(img.texture, display_size, display_min, display_max);;
                            }
                        }
                        ImGui::PopTextWrapPos();
                        ImGui::EndTooltip();
                    }
                }
                ImGui::EndTable();
            }
        }

        /*if (ImGui::BeginChild(ImGuiChildFlags_FrameStyle))
        {
            for (int n = 0; n < skinfileLines.count; n++){
                SKINFILELINEREAD& read = ((SKINFILELINEREAD*)skinfileLines.data)[n];

                const bool is_selected = 0;
                char itemname[260];
                snprintf(itemname, sizeof(itemname), "%04d:%04d: %s", read.numTotal, read.num, read.line.outstr());
                ImGui::Selectable(itemname, is_selected);
            }
        }
        ImGui::EndChild();*/


    }
    ImGui::End();
    return 0;
}

//deprecated
int WORKSPACE::loadSRC() {

    for (int n = 0; n < arr_SRCGR.count; n++) {
        CSTR& path = ((SRCGR*)arr_SRCGR.data)[n].path;
        SRCGR& img = ((SRCGR*)arr_SRCGR.data)[n];

        bool isLoaded = LoadTextureFromFile(path.outstr(), renderer, &(img.texture), &img.sizeX, &img.sizeY);
    }
    return 0;
}

int WORKSPACE::drawImgManager() {
    char title[260];
    snprintf(title, sizeof(title), "ImageManager##%d", num);
    ImGui::Begin(title, &wImgManager, ImGuiWindowFlags_HorizontalScrollbar);
    
    bool clicked = 0;
    //tree list
    snprintf(title, sizeof(title), "ImgTree##%d", num);
    if (ImGui::BeginChild(title, { 250,-1 }, ImGuiChildFlags_ResizeX | ImGuiChildFlags_FrameStyle)) {
        for (int i = 0; i < arr_SRCGR.count; i++) {

            SRCGR& img = ((SRCGR*)arr_SRCGR.data)[i];
           
            char buf[260];
            sprintf(buf, "%02d:%s", img.gr, img.filename.outstr());

            if (ImGui::TreeNodeEx(buf, ImGuiTreeNodeFlags_DrawLinesFull | ((gr_selected == i) ? ImGuiTreeNodeFlags_Selected : NULL)))
            {
                if (ImGui::BeginItemTooltip())
                {
                    ImGui::Text("%s", img.path.outstr());
                    ImGui::EndTooltip();
                }

                for (int srcID = 0; srcID < arr_SRC.count; srcID++) {
                    SRC& src = ((SRC*)arr_SRC.data)[srcID];
                    if (src.gr == img.gr)
                    {
                        sprintf(buf, "%04d %s", src.declare, src.name.outstr());
                        /*if (strchr(buf, '*')) {
                            if (ImGui::TreeNodeEx(buf, ImGuiTreeNodeFlags_DrawLinesFull)) {


                                ImGui::TreePop();
                            }
                        }
                        else */if (ImGui::TreeNodeEx(buf, ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_Leaf)) {
                            if (ImGui::IsItemClicked()) {
                                if (gr_selected != i) gr_selected = i;
                                src_selected = srcID;
                                clicked = 1;
                            }
                            ImGui::TreePop();
                        }

                        if (ImGui::BeginItemTooltip())
                        {
                            if (img.texture != NULL) {
                                int sizeX = src.sizeX == -1 ? img.sizeX - src.x : src.sizeX;
                                int sizeY = src.sizeY == -1 ? img.sizeY - src.y : src.sizeY;
                                ImVec2 display_min = ImVec2(src.x / (float)img.sizeX, src.y / (float)img.sizeY);
                                ImVec2 display_max = ImVec2((src.x + sizeX) / (float)img.sizeX, (src.y + sizeY) / (float)img.sizeY);
                                ImVec2 display_size = ImVec2(sizeX, sizeY);

                                if (src.cycle && (src.div_x >= 1 || src.div_y >= 1)) {
                                    if (src.div_x == 0) src.div_x = 1;
                                    if (src.div_y == 0) src.div_y = 1;
                                    ImVec2 chopsize = { src.sizeX / (float)src.div_x , src.sizeY / (float)src.div_y }; 
                                    
                                    int units = src.div_x * src.div_y;
                                    int tick = src.cycle / units;
                                    int ani = ((int)GetTimeLapse(0, &(g.timer1)) % src.cycle) / tick;

                                    int ax = ani % src.div_x;
                                    int ay = ani / src.div_x;

                                    ImVec2 chopstart = { (src.x + chopsize.x * ax) / (float)img.sizeX ,
                                                        (src.y + chopsize.y * ay) / (float)img.sizeY };

                                    ImVec2 chopend = { (src.x -1 + chopsize.x * (ax+1) ) / (float)img.sizeX ,
                                                        (src.y -1 + chopsize.y * (ay+1) ) / (float)img.sizeY };

                                    ImGui::Image(img.texture, chopsize, chopstart, chopend);                                    
                                }
                                else {
                                    ImGui::Image(img.texture, display_size, display_min, display_max);;
                                }
                            }
                            ImGui::EndTooltip();
                        }

                    }
                }
                ImGui::TreePop();
            }
            else {
                SRC& src = ((SRC*)arr_SRC.data)[src_selected];
                if (ImGui::IsItemClicked()) {
                    gr_selected = img.gr;
                }
                if (ImGui::BeginItemTooltip())
                {
                    ImGui::Text("%s", img.path.outstr());
                    ImGui::EndTooltip();
                }
            }
        }
    }
    ImGui::EndChild();
    
    //image
    ImGui::SameLine(0, 0);
    snprintf(title, sizeof(title), "ImgWorking##%d", num);
    if (ImGui::BeginChild(title, { -1, -1 }, ImGuiChildFlags_Borders | ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar)) {
        SRCGR& img = ((SRCGR*)arr_SRCGR.data)[gr_selected];
        ImGui::Text("%s %d %d", img.path.outstr(), img.sizeX, img.sizeY);
        ImGui::SameLine(0, 0);
        snprintf(title, sizeof(title), "grReload##%d", num);
        ImGui::Button(title);
        ImGui::SameLine(0, 0);
        ImGui::ColorEdit4("MyColor##3", (float*)&bgColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None);

        const ImVec2 pb = ImGui::GetCursorScreenPos();
        ImGui::Image(transBackground, { (float)img.sizeX, (float)img.sizeY }, { 0,0 }, { img.sizeX / (float)32.0, img.sizeY / (float)32.0});
        ImGui::SetCursorScreenPos(pb);
        if(img.texture)
            ImGui::ImageWithBg(img.texture, { (float)img.sizeX, (float)img.sizeY }, { 0,0 }, { 1, 1 }, bgColor);
        
        
        if (img.texture != NULL) {
            const ImVec2 p = ImGui::GetCursorScreenPos();
            ImVec2 grpos = { p.x, p.y - img.sizeY - 4 };


            SRC& src = ((SRC*)arr_SRC.data)[src_selected];
            int sizeX = src.sizeX == -1 ? img.sizeX - src.x : src.sizeX;
            int sizeY = src.sizeY == -1 ? img.sizeY - src.y : src.sizeY;

            ImVec2 srcposLU = { grpos.x + src.x - 1, grpos.y + src.y - 1 };
            ImVec2 srcposRB = { grpos.x + src.x + sizeX + 1, grpos.y + src.y + sizeY + 1 };

            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            bool flicking = ((int)GetTimeLapse(1, &g.timer1) % 200 > 100);
            ImColor color = flicking ? 0xff0080ff : 0x000000;
            draw_list->AddRect(srcposLU, srcposRB, color, 0.0f, ImDrawFlags_Closed, 1.0f);

            if (clicked) {
                ImGui::SetScrollX(src.x);
                ImGui::SetScrollY(src.y);
            }


            for (int i = 0; i < arr_SRC.count; i++){
                SRC& hoversrc = ((SRC*)arr_SRC.data)[i];
                if (hoversrc.gr != src.gr) continue;
                srcposLU = { (float)grpos.x + hoversrc.x, (float)grpos.y + hoversrc.y };
                srcposRB = { (float)grpos.x + hoversrc.x + hoversrc.sizeX, (float)grpos.y + hoversrc.y + hoversrc.sizeY };
                if (ImGui::IsMouseHoveringRect(srcposLU, srcposRB)){
                    flicking = ((int)GetTimeLapse(1, &g.timer1) % 200 < 100);
                    ImColor color = flicking ? 0xffff0000 : 0x000000;
                    draw_list->AddRect(srcposLU, srcposRB, color, 0.0f, ImDrawFlags_Closed, 1.0f);

                    if (ImGui::IsItemClicked()) {
                        gr_selected = hoversrc.gr;
                    }
                    if (ImGui::BeginItemTooltip())
                    {
                        ImGui::Text("line %d : %d %d ~ %d %d",hoversrc.declare , hoversrc.x, hoversrc.y, hoversrc.x + hoversrc.sizeX, hoversrc.y + hoversrc.sizeY);
                        ImGui::EndTooltip();
                    }
                }
            }


        }
    }
    ImGui::EndChild();
    

    ImGui::End();
    return 0;
}


//TODO apply split
int WORKSPACE::SaveSkinScript(char* path, bool split, bool nocomment) {

    FILE* pFile;
    pFile = fopen(path, "wb");
    if (pFile == NULL) return -1;
    for (int i = 0; i < skinfileLines.count; i++) {
        if (nocomment && ((SKINFILELINEREAD*)skinfileLines.data)[i].isComment) continue;

        fputs(((SKINFILELINEREAD*)skinfileLines.data)[i].line, pFile);
        fputs("\n", pFile);
    }
    fclose(pFile);
    return 0;
}

int WORKSPACE::drawSaveMenu() {
    char title[260], input[32], result[32];
    snprintf(title, sizeof(title), "SaveMenu##%d", num);
    snprintf(input, sizeof(input), "##savePathInput%d", num);


    if(ImGui::Begin(title, &wSaveMenu, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_Modal)) {

        ImGui::Text("old path is %s ", mainpath);
        if (newPath[0] == '\0') { //very init
            strncpy(newPath, mainpath, sizeof(newPath));
        }

        if (ImGui::Button("BROWSE", { 0, 0 })) {
            //TODO
        }
        ImGui::SameLine(0, 0);
        ImGui::InputText(input, newPath, IM_ARRAYSIZE(newPath));
        exist = IsFileExist(newPath) || !strcmp(mainpath, newPath); //TODO reduce cpu usage

        if (exist) ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "CAUTION: OVERWRITE");
        ImGui::SeparatorText("Script files");
        ImGui::RadioButton("merge scripts", &split, 0);
        ImGui::RadioButton("split scripts", &split, 1);
        ImGui::SeparatorText("Comments");
        ImGui::RadioButton("maintain memo", &nocomment, 0);
        ImGui::RadioButton("delete memo", &nocomment, 1); //TODO : add tooltip
        
        if (ImGui::Button("SAVE", { 0, 0 })) {
            success = (SaveSkinScript(newPath, split, nocomment) == 0);
            if(success) snprintf(result, sizeof(result), "SaveResult##Save%d", num);
            ImGui::OpenPopup(result);
        }
        
        snprintf(result, sizeof(result), "SaveResult##Save%d", num);
        if (ImGui::BeginPopupModal(result, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("save success");
            if (ImGui::Button("OK")) {
                wSaveMenu = 0;
                success = 0;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        ImGui::End();
    }
    return 0;
}
//{
//    CSTR savepath(mainpath);
//    savepath.add("_master");
//    SaveMaster(savepath);
//}

int WORKSPACE::drawFileManager() {
    char title[260];
    snprintf(title, sizeof(title), "FileManager##%d", num);

    ImGui::Begin(title, &wFileManager);
    //ImGui::Text("they are related files. %d scripts, %d images", arr_subpath.count, arr_imgpath.count);
    ImGui::SeparatorText("Scripts");
    for (int i = 0; i < arr_subpath.count; i++) {
        CSTR& path = ((CSTR*)arr_subpath.data)[i];
        ImGui::Text("%s", path);
    }
    ImGui::SeparatorText("Images");
    for (int i = 0; i < arr_SRCGR.count; i++) {
        SRCGR& img = ((SRCGR*)arr_SRCGR.data)[i];
        ImGui::Text("%02d(%03d) - %s", img.gr, img.isIf, img.path);
    }

    ImGui::End();
    return 0;
}

int WORKSPACE::drawTreeView() {
    char title[260];
    snprintf(title, sizeof(title), "TreeView##%d", num);
    ImGui::Begin(title, &wTreeView);

    snprintf(title, sizeof(title), "Tree##%d", num);
    

    if (ImGui::BeginTable(title, 24, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody)) //ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody))
    {
        // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
        ImGui::TableSetupColumn("Command", ImGuiTableColumnFlags_NoHide);
        for (int p = 0; p < 24 - 1; p++) {
            ImGui::TableSetupColumn("Params", ImGuiTableColumnFlags_WidthFixed, 12.0f);
        }
        ImGui::TableHeadersRow();

        static int hideGroups[MAX_IFDEPTH];
        for (int i = 0; i < MAX_IFDEPTH; i++) {
            hideGroups[i] = -1;
        }
        
        for (int l = 0; l < skinfileLines.count; l++) {
            int hide = 0;
            SKINFILELINEREAD& line = ((SKINFILELINEREAD*)skinfileLines.data)[l];
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            if (line.isComment) continue;
            CSTR& command = line.csv.str[0];
            CSTR *params = line.csv.str;
            bool is_head = //!(strcmp(command, "#INCLUDE")) ||
                            !(strcmp(command, "#IF")) ||
                            !(strcmp(command, "#ELSEIF")) ||
                            !(strcmp(command, "#ELSE"));
            bool is_end = !(strcmp(command, "#ENDIF"));

            static ImGuiTreeNodeFlags tree_node_flags_base = ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_DrawLinesFull;
            ImGuiTreeNodeFlags node_flags = tree_node_flags_base;


            int lineID = ((SKINFILELINEREAD*)skinfileLines.data)[l].numTotal;
            sprintf(title, "%s##%d", command.outstr(), lineID);

            if (is_head)
            {   
                bool open = ImGui::TreeNodeEx(title, node_flags);
                
                for (int p = 1; p < 24 - 1; p++) {
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", params[p]);
                };
            }
            else if (is_end) {
                ImGui::TreePop();
            }
            else if (!hide){
                ImGui::TreeNodeEx(title, node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);

                for (int p = 1; p < 24 - 1; p++) {
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", params[p]);
                };
            }

        }        
        ImGui::EndTable();
    }

    ImGui::End();
    return 0;
}

int WORKSPACE::wildcardTOAll(char* path) {
    WIN32_FIND_DATA FindFileData;
    LPWIN32_FIND_DATAA lpFindFileData;
    HANDLE hFindFile = FindFirstFileA(path, (LPWIN32_FIND_DATAA)&FindFileData);
    if (hFindFile == (HANDLE)-1) {
        return -1;
    }




    return 0;
}

//HOW TO ADD FEATURE - STEP 2 : write function









ARR workspaceList;

