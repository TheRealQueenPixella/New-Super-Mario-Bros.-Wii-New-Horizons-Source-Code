#include "sequenceFarBG.h"
#include <profile.h>

extern "C" void scnLeaf_setPriorityDraw(m3d::mdl_c *model, int prioOPA, int prioXLU); // 0x8016A3E0

const SpriteData SeqFarBGSpriteData = {ProfileId::AC_SEQUENCE_FAR_BG, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
Profile SeqFarBGProfile(&daSequenceFarBG_c::build, NULL, &SeqFarBGSpriteData, ProfileId::FAR_BG, ProfileId::AC_SEQUENCE_FAR_BG, "AC_SEQUENCE_FAR_BG", NULL, 0);

dActor_c *daSequenceFarBG_c::build() {
    void *buffer = AllocFromGameHeap1(sizeof(daSequenceFarBG_c));
    return new(buffer) daSequenceFarBG_c;
}

int daSequenceFarBG_c::onCreate() {
    isBgB = (settings & 1) != 0;
    int worldID = GetSaveFile()->GetBlock(-1)->current_world;
    bgInfo = bgModelInfos[worldID];

    pos.x = 360.0f;
    pos.z = -1000.0f * (isBgB+1);
    scaleFactor = bgInfo.scaleFactor;

    posOffsetA = INITIAL_POS;
    posOffsetB = INITIAL_POS;

    // Prepare models
    allocator.link(-1, GameHeaps[0], 0, 0x20);

    for (int i = 0; i < BG_COUNT; i++) {
        createMdl(i);
    }

    allocator.unlink();
    return true;
}

int daSequenceFarBG_c::onExecute() {
    // Calc model
    for (int i = 0; i < BG_COUNT; i++) {
        float posX = calcScrollX(i);
        bgMdls[i].matrix.translation(posX, bgInfo.posOffsetsY[isBgB], pos.z);
        bgMdls[i].matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);
        Vec adjScale = {scale.x * scaleFactor, scale.y * scaleFactor, scale.z * scaleFactor};
        bgMdls[i].model.setScale(&adjScale);

        bgMdls[i].model.setDrawMatrix(bgMdls[i].matrix);
        bgMdls[i].model.calcWorld(false);

        scnLeaf_setPriorityDraw(&bgMdls[i].model, (isBgB) ? 0x13 : 0x15, (isBgB) ? 0 : 1);
    }

    // Process animations
    for (int i = 0; i < BG_COUNT; i++) {
        bgMdls[i].model._vf1C();

        if (bgMdls[i].animBitfield & 2) {
            if (bgMdls[i].anmChr.isAnimationDone()) {
                bgMdls[i].anmChr.setCurrentFrame(0.0f);
            }
        }
        if (bgMdls[i].animBitfield & 4) {
            bgMdls[i].anmClr.process();
        }
        if (bgMdls[i].animBitfield & 8) {
            bgMdls[i].anmSrt.process();
        }
    }
    return true;
}

int daSequenceFarBG_c::onDraw() {
    for (int i = 0; i < BG_COUNT; i++) {
        bgMdls[i].model.scheduleForDrawing();
    }
    return true;
}

void daSequenceFarBG_c::createMdl(int idx) {
    const char *arcName = (isBgB) ? arcBufB : arcBufA;
    char brresName[20];
    sprintf(brresName, (isBgB) ? "g3d/bgB_%04X.brres" : "g3d/bgA_%04X.brres", bgInfo.fileIDs[isBgB]);

    nw4r::g3d::ResFile rf(getResource(arcName, brresName));
    bgMdls[idx].model.setup(rf.GetResMdl(arcName), &allocator, 0x32C, 1, 0);
    SetupTextures_Map(&bgMdls[idx].model, 0);

    nw4r::g3d::ResAnmChr resChr = rf.GetResAnmChr(arcName);
    if (resChr.data != 0x0 && !bgInfo.disableAnmChr) {
        bgMdls[idx].anmChr.setup(rf.GetResMdl(arcName), resChr, &allocator, 0);
        bgMdls[idx].anmChr.bind(&bgMdls[idx].model, resChr, true);
        bgMdls[idx].model.bindAnim(&bgMdls[idx].anmChr, 0.0f);
        bgMdls[idx].anmChr.setUpdateRate(1.0f);
        bgMdls[idx].animBitfield |= 2;
    }

    nw4r::g3d::ResAnmClr resClr = rf.GetResAnmClr(arcName);
    if (resClr.data != 0x0) {
        bgMdls[idx].anmClr.setup(rf.GetResMdl(arcName), resClr, &allocator, 0, 1);
        bgMdls[idx].anmClr.bind(&bgMdls[idx].model, resClr, 0, 0);
        bgMdls[idx].model.bindAnim(&bgMdls[idx].anmClr, 0.0f);
        bgMdls[idx].anmClr.setUpdateRate(1.0f);
        bgMdls[idx].animBitfield |= 4;
    }

    nw4r::g3d::ResAnmTexSrt resSrt = rf.GetResAnmTexSrt(arcName);
    if (resSrt.data != 0x0) {
        bgMdls[idx].anmSrt.setup(rf.GetResMdl(arcName), resSrt, &allocator, 0, 1);
        bgMdls[idx].anmSrt.bindEntry(&bgMdls[idx].model, resSrt, 0, 0);
        bgMdls[idx].model.bindAnim(&bgMdls[idx].anmSrt, 0.0f);
        bgMdls[idx].animBitfield |= 8;
    }
}

float daSequenceFarBG_c::calcScrollX(int idx) {
    float offset = (isBgB) ? posOffsetB : posOffsetA;
    float out = offset + ((BG_WIDTH * scaleFactor) * idx);

    // Reset pos if we're past a certain threshold
    float resetPos = (INITIAL_POS * scaleFactor);
    if (out <= -resetPos) {
        out = resetPos;
        if (isBgB) {
            posOffsetB = resetPos;
        } else {
            posOffsetA = resetPos;
        }
    }

    // Only scroll if this is the last BG to be calced
    float scrollRate = (isBgB) ? BGB_SCROLL_RATE : BGA_SCROLL_RATE;
    if (idx + 1 == BG_COUNT) {
        out -= scrollRate;
        if (isBgB) {
            posOffsetB -= scrollRate;
        } else {
            posOffsetA -= scrollRate;
        }
    }

    return out;
}
