#include <game.h>

extern "C" bool CheckCurrentLevelAndArea(uint world, uint level, uint area);
extern u8 BossCutsceneFlag;
extern char CurrentWorld, CurrentLevel;

class dBgTexMng_c : public m3d::scnLeaf_c {
public:
    u8 unk[0x4C];
    bool mKoopaStageFlag;

    void SetKoopaStageFlag();
};

void dBgTexMng_c::SetKoopaStageFlag() {
    bool isCastleW8 = CheckCurrentLevelAndArea(8, 24, 4);
    bool isCastleW9 = CheckCurrentLevelAndArea(9, 8, 4);

    mKoopaStageFlag = (isCastleW8 || isCastleW9) ? true : false;
}

// After dying in the final boss, the fake Peach cutscene will be skipped
// However this effect persists even if dying in 08-24 and then going to 09-08
void ResetKamekCutsceneFlag() {
    if ((CurrentWorld == 8 && CurrentLevel == 7) && ((BossCutsceneFlag >> 7) != 0)) {
        BossCutsceneFlag &= ~(1 << 7);
    }
}

