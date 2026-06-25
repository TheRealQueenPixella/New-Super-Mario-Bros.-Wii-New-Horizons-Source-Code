#include <game.h>

extern "C" bool CheckCurrentLevelAndArea(uint world, uint level, uint area);

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
