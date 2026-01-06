#include <game.h>
#include <levelnumber.h>
#include <utils.h>
#include "levelinfo.h"

extern "C" void DateFile_dispCurrentWorldNum(SaveBlock *save, nw4r::lyt::TextBox *T_worldNumber_01) {
    int world = save->current_world;
    const wchar_t *worldName;

    dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(world, Stage_Home);
    if (level) {
        worldName = getWorldNumber(level->displayWorld);
    } else {
        worldName = L"?";
    }

    T_worldNumber_01->SetString(worldName);
}