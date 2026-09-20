#include <game.h>
#include <stage.h>

// "SaveFile" isn't prepared to give us anything yet, so
// we need to access the temporary save buffer
struct SaveFileRaw {
    SaveFirstBlock header;
    SaveBlock blocks[3];
    SaveBlock qsBlocks[3];
};

extern SaveFileRaw SaveFileBuffer;

void dScRestartCrsin_c__startTitle(u8 isReplay, bool forceChange) {
    RESTART_CRSIN_LevelStartStruct.unk4 = false;

    // Since replays are disabled for this mod, we can skip them here...

    // Can also ignore Quick Saves here
    u8 fileNo = SaveFileBuffer.header.current_file;
    u8 world = SaveFileBuffer.blocks[fileNo].current_world;

    RESTART_CRSIN_LevelStartStruct.world1 = world;
    RESTART_CRSIN_LevelStartStruct.level1 = STAGE_TITLE;
    RESTART_CRSIN_LevelStartStruct.purpose = 2; // Title Screen

    RESTART_CRSIN_LevelStartStruct.world2 = RESTART_CRSIN_LevelStartStruct.world1;
    RESTART_CRSIN_LevelStartStruct.level2 = RESTART_CRSIN_LevelStartStruct.level1;
    RESTART_CRSIN_LevelStartStruct.areaMaybe = 0;
    RESTART_CRSIN_LevelStartStruct.entrance = 0xFF;

    ActivateWipe(WIPE_FADE);
    DontShowPreGame = true;
    DoSceneChange(ProfileId::RESTART_CRSIN, forceChange, forceChange);
}
