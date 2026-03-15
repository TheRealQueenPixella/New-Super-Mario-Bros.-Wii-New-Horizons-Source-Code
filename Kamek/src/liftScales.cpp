#include <game.h>
#include <sfx.h>

class daLiftScales_c : public dActorState_c {
public:
    u8 pad[0x1D0];
    float mLeftRopeLen;
    float mScaleWidth;
    float mRightRopeLen;
    float mLeftRopeLenCopy;
    u8 pad2[0x10];

    // New stuff
    int restoreDelay;
    bool doRestoreMode;
    bool canRestore; // Nybble 5.3

    void createScaleStep();
    void setScalePos();

    void checkRestoreAllowed();
    void tryScaleLiftRestore();
    void checkStartRestore();
    void finalizeState_Release();

    USING_STATES(daLiftScales_c);
    REF_NINTENDO_STATE(HoldStop);
    DECLARE_STATE(Restore);
};

CREATE_STATE(daLiftScales_c, Restore);

void daLiftScales_c::checkRestoreAllowed() {
    canRestore = (settings >> 29) & 1;
}

void daLiftScales_c::tryScaleLiftRestore() {
    if (doRestoreMode) {
        if (restoreDelay != 0) {
            restoreDelay--;
            return;
        }

        doRestoreMode = false;
        acState.setState(&StateID_Restore);
    }
}

void daLiftScales_c::checkStartRestore() {
    if (canRestore) {
        acState.setState(&StateID_Restore);
    }
}

void daLiftScales_c::finalizeState_Release() {
    restoreDelay = 180;
    doRestoreMode = true;
}

void daLiftScales_c::beginState_Restore() {}
void daLiftScales_c::executeState_Restore() {
    if (restoreDelay != 0) {
        restoreDelay--;
        return;
    }

    // Reset positions
    mLeftRopeLenCopy = mLeftRopeLen;

    createScaleStep();
    setScalePos(); // Weird hotfix for the Y pos?

    // Play a sound and en effect
    PlaySound(this, SE_PLY_CSDEMO_KURI_APP);

    Vec effPos;
    Vec effScale = {1.5f, 1.5f, 1.0f};

    dStageActor_c *scaleStep = (dStageActor_c*)this->getConnectChild();
    if (scaleStep != 0x0) {
        effPos = scaleStep->pos;
        effPos.z = 5000.0f;
        SpawnEffect("Wm_en_burst_m", 0, &effPos, 0x0, &effScale);

        scaleStep = (dStageActor_c*)scaleStep->getConnectBrNext();
        if (scaleStep != 0x0) {
            effPos = scaleStep->pos;
            effPos.z = 5000.0f;
            SpawnEffect("Wm_en_burst_m", 0, &effPos, 0x0, &effScale);
        }
    }
    acState.setState(&StateID_HoldStop);
}
void daLiftScales_c::endState_Restore() { }