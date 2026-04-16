#include <game.h>
#include "sequenceFarBG.h"

bool CreateGameSetupLayouts(void *); // 0x80917C80
extern "C" void BindAnmScn(void *, void *);
extern "C" void MakeScene(void *, u16, u16, u16, bool, int);
extern void *currentHeap;

class dScGameSetup_c : public dScene_c {
public:
    dStateWrapper_c<dScGameSetup_c> stateMgr;
    FunctionChain sPhase;
    void *numPeopleChange; // dNumberOfPeopleChange_c
    void *selectCursor;    // dSelectCursor_c
    void *players[4];      // da2DPlayer_c
    void *selectPlayer;    // dSelectPlayer_c
    void *easyPairing;     // dEasyPairing_c
    void *sequenceBG;      // dSequenceBG_c
    void *dateFiles[3];    // dDateFile_c
    void *fileSelect;      // dFileSelect_c
    void *infoWindow;      // dInfoWindow_c

    int batteryCheckDelay;
    u32 _F4;
    int plyIdx;
    u32 _FC, _100;
    int currFile;
    bool isPlayerSelect;

    EGG::ProjectOrtho ortho;
    EGG::LookAtCamera camera;
    EGG::Viewport viewport;

    // New (+0xC bytes)
    EGG::ProjectOrtho *ortho0;
    EGG::LookAtCamera *camera0;
    EGG::Viewport *viewport0;

    int onDelete();

    void initScene();
    void createScene(int sceneID);
    void createLightScene(int sceneID);

    void calcCameras();
    void calcCamera(int sceneID);
};

int dScGameSetup_c::onDelete() {
    FreeScene(0);
    FreeScene(1);

    DVD_FreeFile(GetDVDClass2(), arcBufA);
    DVD_FreeFile(GetDVDClass2(), arcBufB);
    return true;
}

void dScGameSetup_c::initScene() {
    // Create scene 0 camera stuff
    ortho0 = new EGG::ProjectOrtho;
    camera0 = new EGG::LookAtCamera;
    viewport0 = new EGG::Viewport;
    ortho0->near = -100000.0f;
    ortho0->far = 100000.0f;

    MakeScene(currentHeap, 0x24, 8, 2, true, 0);
    createScene(0);

    MakeScene(currentHeap, 0x24, 8, 2, true, 1);
    createScene(1);
}

void dScGameSetup_c::createScene(int sceneID) {
    int idx = GetSaveFile()->GetBlock(-1)->current_world;
    EGG::LightManager *lightMgr = m3d::getLightMgr(sceneID);
    const char *arcName, *resName;

    if (sceneID == 0) {
        arcName = "Env_course";
        resName = "MainScene";
    } else {
        arcName = "Env_world";
        resName = "MainSelect";
    }

    nw4r::g3d::ResFile rf(DVD_GetFile(GetDVDClass2(), arcName, "scene/scene.brres"));
    nw4r::g3d::ResAnmScn resScn = rf.GetResAnmScn(resName);
    nw4r::g3d::ResAnmScn anmScn = resScn;
    BindAnmScn(&anmScn, &resScn);
    lightMgr->LoadScnLightInner(&anmScn, 0.0f, -1, 3);

    createLightScene(sceneID);
}

void dScGameSetup_c::createLightScene(int sceneID) {
    int idx = GetSaveFile()->GetBlock(-1)->current_world;
    EGG::LightManager *lightMgr = m3d::getLightMgr(sceneID);
    EGG::FogManager *fogMgr = m3d::getFogMgr(sceneID);
    char resNameBuf[32];
    void *light, *lmap, *fog;

    if (sceneID == 0) {
        sprintf(resNameBuf, "light/%s.blight", bgModelInfos[idx].envResName);
        light = DVD_GetFile(GetDVDClass2(), bgModelInfos[idx].envArcName, resNameBuf);

        sprintf(resNameBuf, "light/%s.blmap", bgModelInfos[idx].envResName);
        lmap = DVD_GetFile(GetDVDClass2(), bgModelInfos[idx].envArcName, resNameBuf);

        sprintf(resNameBuf, "fog/%s.bfog", bgModelInfos[idx].envResName);
        fog = DVD_GetFile(GetDVDClass2(), bgModelInfos[idx].envArcName, resNameBuf);
    } else {
        // Menu player lighting
        light = DVD_GetFile(GetDVDClass2(), "Env_world", "light/Layout3D.blight");
        lmap = DVD_GetFile(GetDVDClass2(),  "Env_world", "light/Layout3D.blmap");
        fog = DVD_GetFile(GetDVDClass2(),   "Env_world", "fog/Layout3D.bfog");
    }

    // Set everything
    lightMgr->setBinary(light);
    lightMgr->lightTexMgr->setBinary(lmap);
    fogMgr->setBinary(fog);
}

void dScGameSetup_c::calcCameras() {
    calcCamera(0);
    calcCamera(1);
}

void dScGameSetup_c::calcCamera(int sceneID) {
    nw4r::g3d::Camera cam(GetCameraByID(sceneID));

    float efbHeight = mVideo::m_video->renderMode->efbHeight;
    float fbWidth = mVideo::m_video->renderMode->fbWidth;
    if (IsWideScreen()) {
        fbWidth *= mVideo::l_AspectRatio;
    }

    if (sceneID == 1) {
        ortho.setVolume(efbHeight, 0.0f, 0.0f, fbWidth);

        fbWidth = mVideo::m_video->renderMode->fbWidth;
        viewport.setLUandWH(0.0f, 0.0f, fbWidth, efbHeight);

        Vec camPos = {0.0f, 0.0f, 6000.0f};
        Vec camTgt = {0.0f, 0.0f, 0.0f};
        Vec camUp  = {0.0f, 1.0f, 0.0f};
        camera.camPos = camPos;
        camera.target = camTgt;
        camera.camUp = camUp;

        viewport.setG3DCamera(cam);
        ortho.setOrthoOntoCamera(cam);
        camera.calculateMatrix();
        camera.assignToNW4RCamera(cam);
    } else {
        ortho0->setVolume(efbHeight, 0.0f, 0.0f, fbWidth);

        fbWidth = mVideo::m_video->renderMode->fbWidth;
        viewport0->setLUandWH(0.0f, 0.0f, fbWidth, efbHeight);

        Vec camPos = {0.0f, 0.0f, 6000.0f};
        Vec camTgt = {0.0f, 0.0f, 0.0f};
        Vec camUp  = {0.0f, 1.0f, 0.0f};
        camera0->camPos = camPos;
        camera0->target = camTgt;
        camera0->camUp = camUp;

        viewport0->setG3DCamera(cam);
        ortho0->setOrthoOntoCamera(cam);
        camera0->calculateMatrix();
        camera0->assignToNW4RCamera(cam);
    }
}

bool GameSetup_SetRes(void *) {
    // Uncomment this if restoring SEQUENCE_BG
    // DVD_LoadFile(GetDVDClass(), "Layout/textures", "sequenceBGTexture", 0x0);

    int idx = GetSaveFile()->GetBlock(-1)->current_world;
    sprintf(arcBufA, "bgA_%04X", bgModelInfos[idx].fileIDs[0]);
    sprintf(arcBufB, "bgB_%04X", bgModelInfos[idx].fileIDs[1]);

    DVD_LoadFile(GetDVDClass(), "Object", arcBufA, 0);
    DVD_LoadFile(GetDVDClass(), "Object", arcBufB, 0);
    return true;
}

bool GameSetup_CreateBases(void *gameSetup) {
    CreateParentedObject(ProfileId::AC_SEQUENCE_FAR_BG, gameSetup, 0x0, 0);
    CreateParentedObject(ProfileId::AC_SEQUENCE_FAR_BG, gameSetup, 0x1, 0);

    return CreateGameSetupLayouts(gameSetup);
}

void GameSetup_Painter() {
    // New stuff
    LinkScene(0);
    ChangeAlphaUpdate(true);
    SceneCalcWorld(0);
    SceneCameraStuff(0);
    DrawOpa();
    DrawXlu();
    UnlinkScene(0);
    
    // Original
    SetupLYTDrawing();
    DrawAllLayoutsBeforeX(0x81);
    GXDrawDone();
    RemoveAllFromScnRoot();
    Reset3DState();
    SetCurrentCameraID(1);
    DoSpecialDrawing1();
    LinkScene(1);
    SceneCalcWorld(1);
    SceneCameraStuff(1);
    CalcMaterial();
    DrawOpa();
    DrawXlu();
    if (GameMgrP->_AFC) {
        for (int i = 0; i < 4; i++) {
            RenderEffects(0, i + 0xB);
        }
        for (int i = 0; i < 4; i++) {
            RenderEffects(0, i + 7);
        }
    }
    RenderEffects(0, 2);
    DrawAllLayoutsAfterX(0x80);
    ClearLayoutDrawList();
    UnlinkScene(1);
    SetCurrentCameraID(0);
}
