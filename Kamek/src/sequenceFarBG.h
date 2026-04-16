#pragma once
#include <game.h>

#define BG_COUNT 3

#define INITIAL_POS 360.0f
#define BG_WIDTH 720.0f

// Make these negative to scroll right
#define BGA_SCROLL_RATE 0.55f
#define BGB_SCROLL_RATE 0.35f

static char arcBufA[10], arcBufB[10];

struct BgModelInfo_s {
    u16 fileIDs[2];
    const char *envArcName;
    const char *envResName;
    float posOffsetsY[2];
    float scaleFactor;
    // Hotfix for bgX_0C02, uses CHR anim for shooting the volacano rocks
    bool disableAnmChr;
};

// Note: 250.0f, 2.0f for kinda retail scale
static BgModelInfo_s bgModelInfos[10] = {
    /* W0 */ {0x0C02, 0x0C02, "Env_course", "Fire2Scene",         125.0f, 200.0f, 1.25f, true},
    /* WB */ {0x0A02, 0x0A02, "Env_course", "Fire2Scene",         125.0f, 125.0f, 1.25f, false},
    /* WA */ {0x0703, 0x06B3, "Env_course", "Fire2Scene",         125.0f, 125.0f, 1.25f, false},
    /* WC */ {0x10B2, 0x10B2, "Env_course", "KurayamiChikaScene", 125.0f, 125.0f, 1.25f, false},
    /* W1 */ {0x0703, 0x0603, "Env_course", "MainScene",          125.0f, 125.0f, 1.25f, false},
    /* W2 */ {0x0A02, 0x0A02, "Env_course", "Fire2Scene",         125.0f, 125.0f, 1.25f, false},
    /* W3 */ {0x0E02, 0x57B2, "Env_course", "KurayamiChikaScene", 200.0f, 125.0f, 1.25f, false},
    /* W4 */ {0x3F02, 0x3F02, "Env_course", "ObakeOutScene",      250.0f, 300.0f, 2.0f,  false},
    /* W5 */ {0x0B02, 0xFFFF, "Env_course", "MainScene",          125.0f, 125.0f, 1.25f, false},
    /*    */ {0x000A, 0x000A, "Env_world",  "Layout3D",           125.0f, 125.0f, 1.25f, false}, // Dummy
};

class daSequenceFarBG_c : public dActor_c {
public:
    struct BgModel_s {
        mMtx matrix;
        m3d::mdl_c model;
        m3d::anmChr_c anmChr;
        m3d::anmClr_c anmClr;
        m3d::anmTexSrt_c anmSrt;
        u8 animBitfield; // 2 = anmChr, 4 = anmClr, 8 = anmTexSrt
    };

    mHeapAllocator_c allocator;
    BgModel_s bgMdls[3];
    BgModelInfo_s bgInfo;

    float scaleFactor;
    bool isBgB;

    int onCreate();
    int onExecute();
    int onDraw();

    void createMdl(int idx);
    void scrollBG();
    float calcScrollX(int idx);

    static dActor_c *build();
    static float posOffsetA;
    static float posOffsetB;
};

float daSequenceFarBG_c::posOffsetA;
float daSequenceFarBG_c::posOffsetB;

