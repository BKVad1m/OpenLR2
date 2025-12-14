#include "imgui/imgui.h"
#include "../LR2/LR2_skinmanage.h"
#include "../LR2/LR2_skinload.h"
#include "../LR2/LR2_skindraw.h"
#include "../LR2/En_fileutil.h"
#include "../LR2/Scene07_Skinselect.h"
#include "DxLib//DxLib.h"
#include "winWorkspace.h"

#include "seWindowManager.h"
#include "ImageLoader.h"
#include "imgui/TextEditor.h"

#include "skin.h"
#include "op.h"

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
    }
    ImGui::EndChild();

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
        }
        else if (read.line.left(6).isSame("#ENDIF")) {
            isif = true;
            
            read.ifgroup = IFcur + cOrder;
            IFcur = ((IFUNIT*)arr_ifunit.data)[IFcur].parentID;;
            
            IFdepth--;
        }
        else {
            read.ifgroup = IFcur;
        }
        

        if (isif) continue;

        else if (read.line.left(6).isSame("#IMAGE")) {
            CSTR& tmp = read.csv.str[1];
            arr_imgpath.push_back(&tmp);
        }

    }
    



    //images load
    int gr = 0;
    for (int n = 0; n < arr_imgpath.count; n++) {
        CSTR& path = ((CSTR*)arr_imgpath.data)[n];
        SRCGR& img = ((SRCGR*)arr_SRCGR.data)[n];


        if (path.isSame("CONTINUE")) {
            gr++;
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
            img.gr = gr++;
            //img.path.assign(path.outstr());
            arr_SRCGR.push_back(&img);
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
    arr_imgpath.Free();
    arr_imgpath.Alloc(sizeof(CSTR), 4);
    arr_SRCGR.Free();
    arr_SRCGR.Alloc(sizeof(SRCGR), 10);
    arr_SRC.Free();
    arr_SRC.Alloc(sizeof(SRC), 100);

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

    ImGui::Begin(title, &wTextEdit, ImGuiWindowFlags_MenuBar);

    //TEMP
    static bool hideComment = false;
    static bool hideBlank = false;

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
        color = {1.0f, 1.0f, 1.0f, 1.0f};

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
        if (isHide  && !head) continue;

        ImGui::PushID(n);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV( (head+isHide) / 3.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV( (head+isHide) / 3.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV( (head+isHide) / 3.0f, 0.8f, 0.8f));
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
            if (ImGui::BeginTable(tablename, 22, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody))
            {
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::TableNextRow();
                for (int column = 0; column < 22; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    if (read.csv.str[column].atPos(0) == nullptr) {
                        ImGui::TextDisabled("%s", read.csv.str[column]);
                    }
                    else{
                        //ImGui::Text("%s", read.csv.str[column]);
                        char inputname[260];
                        sprintf(inputname, "##%d_%d_cell", read.numTotal, column);
                        ImGui::InputText(inputname, read.csv.str[column], 260);
                    }

                    if (ImGui::BeginItemTooltip())
                    {
                        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                        ImGui::TextUnformatted(read.csv.str[column]);
                        if (read.csv.str[0].isSame("#SRC_IMAGE")) {

                            int handle = read.csv.val[2];
                            
                            SRCGR& img = ((SRCGR*)arr_SRCGR.data)[handle];

                            int iX = read.csv.val[3];
                            int iY = read.csv.val[4];
                            int iW = read.csv.val[5] == -1 ? img.sizeX - iX : read.csv.val[5];
                            int iH = read.csv.val[6] == -1 ? img.sizeY - iH : read.csv.val[6];

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




    ImGui::End();
    return 0;
}

int WORKSPACE::loadSRC() {

    for (int n = 0; n < arr_imgpath.count; n++) {
        CSTR& path = ((CSTR*) arr_imgpath.data)[n];
        SRCGR& img = ((SRCGR*)arr_SRCGR.data)[n];

        bool isLoaded = LoadTextureFromFile(path.outstr(), renderer, &(img.texture), &img.sizeX, &img.sizeY);
        
        if (isLoaded) {
            //img.path.assign(path.outstr());
            arr_SRCGR.push_back(&img);
        }
    }
    

    //for (int n = 0; n < skinfileLines.count; n++) {
    //    SKINFILELINEREAD& read = ((SKINFILELINEREAD*)skinfileLines.data)[n];
    //    if (read.line.left(10).isSame("#SRC_IMAGE")) {
    //        
    //    }
    //}

    return 0;
}

int WORKSPACE::drawImgManager() {
    char title[260];
    snprintf(title, sizeof(title), "ImageManager##%d", num);
    ImGui::Begin(title, &wImgManager, ImGuiWindowFlags_HorizontalScrollbar);
    for (int i = 0; i < arr_imgpath.count; i++) {
        CSTR& path = ((CSTR*)arr_imgpath.data)[i];
        ImGui::Text("%03d : %s", i , path);
    }
    for (int i = 0; i < arr_SRCGR.count; i++) {
        SRCGR& img = ((SRCGR*)arr_SRCGR.data)[i];
        ImGui::Text(" %d %d", img.sizeX, img.sizeY);
        ImGui::Image(img.texture, { (float)img.sizeX, (float)img.sizeY }, { 0,0 }, { 1, 1 });
    }

    
    if (ImGui::BeginChild("Assets", ImVec2(0.0f, -ImGui::GetTextLineHeightWithSpacing()), ImGuiChildFlags_Borders, ImGuiWindowFlags_NoMove)){
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        const float avail_width = ImGui::GetContentRegionAvail().x;

        // Calculate and store start position.
        ImVec2 start_pos = ImGui::GetCursorScreenPos();
        start_pos = ImVec2(start_pos.x, start_pos.y);
        ImGui::SetCursorScreenPos(start_pos);

        // Multi-select
        ImGuiMultiSelectFlags ms_flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_ClearOnClickVoid;
            ms_flags |= ImGuiMultiSelectFlags_BoxSelect2d;
            ms_flags |= ImGuiMultiSelectFlags_SelectOnClickRelease;

        // - Enable keyboard wrapping on X axis
        // (FIXME-MULTISELECT: We haven't designed/exposed a general nav wrapping api yet, so this flag is provided as a courtesy to avoid doing:
        //    ImGui::NavMoveRequestTryWrapping(ImGui::GetCurrentWindow(), ImGuiNavMoveFlags_WrapX);
        // When we finish implementing a more general API for this, we will obsolete this flag in favor of the new system)
        ms_flags |= ImGuiMultiSelectFlags_NavWrapX;

        //ImGuiMultiSelectIO* ms_io = ImGui::BeginMultiSelect(ms_flags, Selection.Size, Items.Size);

        //// Use custom selection adapter: store ID in selection (recommended)
        //Selection.UserData = this;
        //Selection.AdapterIndexToStorageId = [](ImGuiSelectionBasicStorage* self_, int idx) { ExampleAssetsBrowser* self = (ExampleAssetsBrowser*)self_->UserData; return self->Items[idx].ID; };
        //Selection.ApplyRequests(ms_io);

        //const bool want_delete = (ImGui::Shortcut(ImGuiKey_Delete, ImGuiInputFlags_Repeat) && (Selection.Size > 0)) || RequestDelete;
        //const int item_curr_idx_to_focus = want_delete ? Selection.ApplyDeletionPreLoop(ms_io, Items.Size) : -1;
        //RequestDelete = false;

        //// Push LayoutSelectableSpacing (which is LayoutItemSpacing minus hit-spacing, if we decide to have hit gaps between items)
        //// Altering style ItemSpacing may seem unnecessary as we position every items using SetCursorScreenPos()...
        //// But it is necessary for two reasons:
        //// - Selectables uses it by default to visually fill the space between two items.
        //// - The vertical spacing would be measured by Clipper to calculate line height if we didn't provide it explicitly (here we do).
        //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(LayoutSelectableSpacing, LayoutSelectableSpacing));

        //// Rendering parameters
        //const ImU32 icon_type_overlay_colors[3] = { 0, IM_COL32(200, 70, 70, 255), IM_COL32(70, 170, 70, 255) };
        //const ImU32 icon_bg_color = ImGui::GetColorU32(IM_COL32(35, 35, 35, 220));
        //const ImVec2 icon_type_overlay_size = ImVec2(4.0f, 4.0f);
        //const bool display_label = (LayoutItemSize.x >= ImGui::CalcTextSize("999").x);

        //const int column_count = LayoutColumnCount;
        //ImGuiListClipper clipper;
        //clipper.Begin(LayoutLineCount, LayoutItemStep.y);
        //if (item_curr_idx_to_focus != -1)
        //    clipper.IncludeItemByIndex(item_curr_idx_to_focus / column_count); // Ensure focused item line is not clipped.
        //if (ms_io->RangeSrcItem != -1)
        //    clipper.IncludeItemByIndex((int)ms_io->RangeSrcItem / column_count); // Ensure RangeSrc item line is not clipped.
        //while (clipper.Step())
        //{
        //    for (int line_idx = clipper.DisplayStart; line_idx < clipper.DisplayEnd; line_idx++)
        //    {
        //        const int item_min_idx_for_current_line = line_idx * column_count;
        //        const int item_max_idx_for_current_line = IM_MIN((line_idx + 1) * column_count, Items.Size);
        //        for (int item_idx = item_min_idx_for_current_line; item_idx < item_max_idx_for_current_line; ++item_idx)
        //        {
        //            ExampleAsset* item_data = &Items[item_idx];
        //            ImGui::PushID((int)item_data->ID);

        //            // Position item
        //            ImVec2 pos = ImVec2(start_pos.x + (item_idx % column_count) * LayoutItemStep.x, start_pos.y + line_idx * LayoutItemStep.y);
        //            ImGui::SetCursorScreenPos(pos);

        //            ImGui::SetNextItemSelectionUserData(item_idx);
        //            bool item_is_selected = Selection.Contains((ImGuiID)item_data->ID);
        //            bool item_is_visible = ImGui::IsRectVisible(LayoutItemSize);
        //            ImGui::Selectable("", item_is_selected, ImGuiSelectableFlags_None, LayoutItemSize);

        //            // Update our selection state immediately (without waiting for EndMultiSelect() requests)
        //            // because we use this to alter the color of our text/icon.
        //            if (ImGui::IsItemToggledSelection())
        //                item_is_selected = !item_is_selected;

        //            // Focus (for after deletion)
        //            if (item_curr_idx_to_focus == item_idx)
        //                ImGui::SetKeyboardFocusHere(-1);

        //            // Drag and drop
        //            if (ImGui::BeginDragDropSource())
        //            {
        //                // Create payload with full selection OR single unselected item.
        //                // (the later is only possible when using ImGuiMultiSelectFlags_SelectOnClickRelease)
        //                if (ImGui::GetDragDropPayload() == NULL)
        //                {
        //                    ImVector<ImGuiID> payload_items;
        //                    void* it = NULL;
        //                    ImGuiID id = 0;
        //                    if (!item_is_selected)
        //                        payload_items.push_back(item_data->ID);
        //                    else
        //                        while (Selection.GetNextSelectedItem(&it, &id))
        //                            payload_items.push_back(id);
        //                    ImGui::SetDragDropPayload("ASSETS_BROWSER_ITEMS", payload_items.Data, (size_t)payload_items.size_in_bytes());
        //                }

        //                // Display payload content in tooltip, by extracting it from the payload data
        //                // (we could read from selection, but it is more correct and reusable to read from payload)
        //                const ImGuiPayload* payload = ImGui::GetDragDropPayload();
        //                const int payload_count = (int)payload->DataSize / (int)sizeof(ImGuiID);
        //                ImGui::Text("%d assets", payload_count);

        //                ImGui::EndDragDropSource();
        //            }

        //            // Render icon (a real app would likely display an image/thumbnail here)
        //            // Because we use ImGuiMultiSelectFlags_BoxSelect2d, clipping vertical may occasionally be larger, so we coarse-clip our rendering as well.
        //            if (item_is_visible)
        //            {
        //                ImVec2 box_min(pos.x - 1, pos.y - 1);
        //                ImVec2 box_max(box_min.x + LayoutItemSize.x + 2, box_min.y + LayoutItemSize.y + 2); // Dubious
        //                draw_list->AddRectFilled(box_min, box_max, icon_bg_color); // Background color
        //                if (ShowTypeOverlay && item_data->Type != 0)
        //                {
        //                    ImU32 type_col = icon_type_overlay_colors[item_data->Type % IM_ARRAYSIZE(icon_type_overlay_colors)];
        //                    draw_list->AddRectFilled(ImVec2(box_max.x - 2 - icon_type_overlay_size.x, box_min.y + 2), ImVec2(box_max.x - 2, box_min.y + 2 + icon_type_overlay_size.y), type_col);
        //                }
        //                if (display_label)
        //                {
        //                    ImU32 label_col = ImGui::GetColorU32(item_is_selected ? ImGuiCol_Text : ImGuiCol_TextDisabled);
        //                    char label[32];
        //                    sprintf(label, "%d", item_data->ID);
        //                    draw_list->AddText(ImVec2(box_min.x, box_max.y - ImGui::GetFontSize()), label_col, label);
        //                }
        //            }

        //            ImGui::PopID();
        //        }
        //    }
        //}
        //clipper.End();
        //ImGui::PopStyleVar(); // ImGuiStyleVar_ItemSpacing

        //// Context menu
        //if (ImGui::BeginPopupContextWindow())
        //{
        //    ImGui::Text("Selection: %d items", Selection.Size);
        //    ImGui::Separator();
        //    if (ImGui::MenuItem("Delete", "Del", false, Selection.Size > 0))
        //        RequestDelete = true;
        //    ImGui::EndPopup();
        //}

        //ms_io = ImGui::EndMultiSelect();
        //Selection.ApplyRequests(ms_io);
        //if (want_delete)
        //    Selection.ApplyDeletionPostLoop(ms_io, Items, item_curr_idx_to_focus);

        //// Zooming with CTRL+Wheel
        //if (ImGui::IsWindowAppearing())
        //    ZoomWheelAccum = 0.0f;
        //if (ImGui::IsWindowHovered() && io.MouseWheel != 0.0f && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsAnyItemActive() == false)
        //{
        //    ZoomWheelAccum += io.MouseWheel;
        //    if (fabsf(ZoomWheelAccum) >= 1.0f)
        //    {
        //        // Calculate hovered item index from mouse location
        //        // FIXME: Locking aiming on 'hovered_item_idx' (with a cool-down timer) would ensure zoom keeps on it.
        //        const float hovered_item_nx = (io.MousePos.x - start_pos.x + LayoutItemSpacing * 0.5f) / LayoutItemStep.x;
        //        const float hovered_item_ny = (io.MousePos.y - start_pos.y + LayoutItemSpacing * 0.5f) / LayoutItemStep.y;
        //        const int hovered_item_idx = ((int)hovered_item_ny * LayoutColumnCount) + (int)hovered_item_nx;
        //        //ImGui::SetTooltip("%f,%f -> item %d", hovered_item_nx, hovered_item_ny, hovered_item_idx); // Move those 4 lines in block above for easy debugging

        //        // Zoom
        //        IconSize *= powf(1.1f, (float)(int)ZoomWheelAccum);
        //        IconSize = IM_CLAMP(IconSize, 16.0f, 128.0f);
        //        ZoomWheelAccum -= (int)ZoomWheelAccum;
        //        UpdateLayoutSizes(avail_width);

        //        // Manipulate scroll to that we will land at the same Y location of currently hovered item.
        //        // - Calculate next frame position of item under mouse
        //        // - Set new scroll position to be used in next ImGui::BeginChild() call.
        //        float hovered_item_rel_pos_y = ((float)(hovered_item_idx / LayoutColumnCount) + fmodf(hovered_item_ny, 1.0f)) * LayoutItemStep.y;
        //        hovered_item_rel_pos_y += ImGui::GetStyle().WindowPadding.y;
        //        float mouse_local_y = io.MousePos.y - ImGui::GetWindowPos().y;
        //        ImGui::SetScrollY(hovered_item_rel_pos_y - mouse_local_y);
        //    }
        //}
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
        static int split, nocomment, exist, success;
        static bool wSaveMenuResult;

        ImGui::Text("old path is %s ", mainpath);
        static char newPath[260] = "";
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
    ImGui::Text("they are related files. %d scripts, %d images", arr_subpath.count, arr_imgpath.count);
    ImGui::SeparatorText("Scripts");
    for (int i = 0; i < arr_subpath.count; i++) {
        CSTR& path = ((CSTR*)arr_subpath.data)[i];
        ImGui::Text("%s", path);
    }
    ImGui::SeparatorText("Images");
    for (int i = 0; i < arr_imgpath.count; i++) {
        CSTR& path = ((CSTR*)arr_imgpath.data)[i];
        ImGui::Text("%s", path);
    }

    ImGui::End();
    return 0;
}

int WORKSPACE::drawTreeView() {
    char title[260];
    snprintf(title, sizeof(title), "TreeView##%d", num);
    ImGui::Begin(title, &wTreeView);

    snprintf(title, sizeof(title), "Tree##%d", num);
    

    if (ImGui::BeginTable(title, 24, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody))
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

//HOW TO ADD FEATURE - STEP 2 : write function









ARR workspaceList;

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
    if (count >= bufSize) Realloc(bufSize*2); //make buffer size double
    memcpy((void*)((int)data + unitSize * count), newdata, unitSize);
    count++;
    return 0;
}