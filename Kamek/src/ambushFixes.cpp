#include <game.h>
#include <sfx.h>

// ret is the return from actor's create()
// Check if the ambush is completed, and prevent spawning based on that
int checkEnemyDelete(int ret) {
    if (StageE4::instance->mIsAmbushComplete) {
        return 2; // Delete
    }

    return ret;
}

class daEnKild_c : public dEn_c {
public:
    void checkDisable();
    void FUN_80A50370();

    USING_STATES(daEnKild_c);
    DECLARE_STATE(AmbushClear);
};

CREATE_STATE(daEnKild_c, AmbushClear);

void daEnKild_c::beginState_AmbushClear() { }
void daEnKild_c::executeState_AmbushClear() { }
void daEnKild_c::endState_AmbushClear() { }

void daEnKild_c::checkDisable() {
    if (StageE4::instance->mIsAmbushComplete) {
        acState.setState(&StateID_AmbushClear);
        return;
    }

    FUN_80A50370();
}

int daBattleGame_c::execute() {
    acState.execute();

    // Check if the ambush itself is running, to avoid affecting the initial spawn
    if ((balloonActivateDelay == 0) && (gameBeginDelay == 0) && acState.getCurrentState()->isEqual(&StateID_DemoStart_SoundStartWait)) {
        daEnGreenCoin_c *from = 0x0;
        while (from = (daEnGreenCoin_c*)fBase_c::searchByProfileId(EN_GREENCOIN, from), from != 0x0) {
            // We do this manually to prevent it from playing the appear sound
            from->visible = true;
            from->acState.setState(&daEnGreenCoin_c::StateID_Move);
        }
    }
    return true;
}

// Play the chest sound just once, to prevent it from playing every time it comes on-screen
bool NewChestSound(bool playerCollected) {
    if (playerCollected && (dStage32C_c::instance->greenCoinsCollected == 8)) {
        MapSoundPlayer(SoundRelatedClass, SE_OBJ_TREASURE_BOX_APP, 0);
    }

    return playerCollected;
}
