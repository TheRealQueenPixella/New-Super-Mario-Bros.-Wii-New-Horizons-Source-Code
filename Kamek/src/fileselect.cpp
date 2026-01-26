#include <game.h>
#include <levelnumber.h>
#include <utils.h>
#include "levelinfo.h"

extern "C" void DateFile_dispCurrentWorldNum(dBase_c *self, nw4r::lyt::TextBox *T_worldNumber_01, SaveBlock *save) {
    int world = save->current_world;
    const wchar_t *worldName;

    // Setup the world ID + color
    dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(world, Stage_Home);
    if (level) {
        worldName = getWorldNumber(level->displayWorld);
    } else {
        worldName = L"?";
    }

    T_worldNumber_01->SetString(worldName);

	int color = getColorArrayIdx(world);
	T_worldNumber_01->colour1 = sc_WorldColorArray[color][0];
	T_worldNumber_01->colour2 = sc_WorldColorArray[color][1];

    // Setup player icons
    m2d::EmbedLayout_c *layout = (m2d::EmbedLayout_c*)((int)self + 0x74);

    nw4r::lyt::Picture *P_iconM_00 = layout->findPictureByName("P_iconM_00");
    nw4r::lyt::Picture *P_iconL_00 = layout->findPictureByName("P_iconL_00");
    nw4r::lyt::Picture *P_iconY_00 = layout->findPictureByName("P_iconY_00");
    nw4r::lyt::Picture *P_iconB_00 = layout->findPictureByName("P_iconB_00");
    P_iconM_00->SetVisible(false);
    P_iconL_00->SetVisible(false);
    P_iconY_00->SetVisible(false);
    P_iconB_00->SetVisible(false);

    int restNum;
    GXColor restColor;
    switch (save->lastP1Character) {
        default: // Mario
            P_iconM_00->SetVisible(true);
            restNum = save->player_lives[0];
            restColor = marioColor;
            break;
        case 1: // Luigi
            P_iconL_00->SetVisible(true);
            restNum = save->player_lives[1];
            restColor = luigiColor;
            break;
        case 2: // Blue Toad
            P_iconB_00->SetVisible(true);
            restNum = save->player_lives[2];
            restColor = kinoBColor;
            break;
        case 3: // Yellow Toad
            P_iconY_00->SetVisible(true);
            restNum = save->player_lives[3];
            restColor = kinoYColor;
            break;
    }

    // Write lives
    nw4r::lyt::TextBox *T_zanki_00 = layout->findTextBoxByName("T_zanki_00");
    const int length = 2;
    WriteNumberToTextBox(&restNum, &length, T_zanki_00, 1);

    // Set the player color
    layout->findTextBoxByName("T_x_00")->colour2 = restColor;
    T_zanki_00->colour2 = restColor;
    return;
}

extern "C" int CSMonExecute(dBase_c *self);

int CSMStorePlayer(dBase_c *self) {
    SaveBlock *saveBlock = GetSaveFile()->GetBlock(-1);

    if (saveBlock->lastP1Character != Player_ID[0]) {
		saveBlock->lastP1Character = Player_ID[0];
	}

    return CSMonExecute(self);
}