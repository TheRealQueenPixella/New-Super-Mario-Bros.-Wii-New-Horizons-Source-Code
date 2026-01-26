#include <utils.h>
#include <sfx.h>
#include "levelinfo.h"
#include <stage.h>

void SCM_WorldColorChange(int world, nw4r::lyt::TextBox *tb) {
	int color = getColorArrayIdx(world);

	tb->colour1 = sc_WorldColorArray[color][0];
	tb->colour2 = sc_WorldColorArray[color][1];
}

void SCMWorldName(int param_1) {
    nw4r::lyt::TextBox *TextBox1 = *(nw4r::lyt::TextBox **)(param_1 + 0x474);
    nw4r::lyt::TextBox *TextBox2 = *(nw4r::lyt::TextBox **)(param_1 + 0x478);

    const char *worldname;
    int CurrentWorld = *(int *)(param_1 + 0xb44);

    dLevelInfo_c::entry_s *world = dLevelInfo_c::s_info.searchByDisplayNum(CurrentWorld+1, 100);
    if (world) {
		worldname = dLevelInfo_c::s_info.getNameForLevel(world);
	} else {
		worldname = "World ?";
	}

    wchar_t wbuffer[0x40];
	for (int i = 0; i < 0x40; i++) {
		wbuffer[i] = (unsigned short)worldname[i];
	}

	TextBox1->SetString(wbuffer);
    TextBox2->SetString(wbuffer);

	SCM_WorldColorChange(CurrentWorld, TextBox1);
	SCM_WorldColorChange(CurrentWorld, TextBox2);
    return;
}

void SCMWorldName2(int param_1, int param_2) {
    nw4r::lyt::TextBox *TextBox = *(nw4r::lyt::TextBox **)(param_1 + param_2 * 4 + 0x474);

    const char *worldname;
    int CurrentWorld = *(int *)(param_1 + 0xb44);

    dLevelInfo_c::entry_s *world = dLevelInfo_c::s_info.searchByDisplayNum(CurrentWorld+1, 100);
    if (world) {
		worldname = dLevelInfo_c::s_info.getNameForLevel(world);
	} else {
		worldname = "World ?";
	}

    wchar_t wbuffer[0x40];
	for (int i = 0; i < 0x40; i++) {
		wbuffer[i] = (unsigned short)worldname[i];
	}

	TextBox->SetString(wbuffer);
	SCM_WorldColorChange(CurrentWorld, TextBox);
    return;
}

extern "C" bool IsCourseClear(int World, int Level);

void handleFileters(int worldNumber, int levelNumber, m2d::EmbedLayout_c *layout) {
	nw4r::lyt::Picture *P_coinFileter_00 = layout->findPictureByName("P_coinFileter_00");
	nw4r::lyt::Picture *P_coinFileter_01 = layout->findPictureByName("P_coinFileter_01");
	nw4r::lyt::Picture *P_coinFileter_02 = layout->findPictureByName("P_coinFileter_02");

	if (IsCourseClear(worldNumber, levelNumber)) {
		P_coinFileter_00->SetVisible(false);
		P_coinFileter_01->SetVisible(false);
		P_coinFileter_02->SetVisible(false);
	} else {
		P_coinFileter_00->SetVisible(true);
		P_coinFileter_01->SetVisible(true);
		P_coinFileter_02->SetVisible(true);
	}
}

void SCM_LevelColorChange(int world, nw4r::lyt::TextBox *T_worldNum_00, nw4r::lyt::TextBox *T_dash_00, nw4r::lyt::TextBox *T_corseNum_00) {
	int color = getColorArrayIdx(world);

	T_worldNum_00->colour1 = sc_WorldColorArray[color][0];
	T_worldNum_00->colour2 = sc_WorldColorArray[color][1];

	T_dash_00->colour1 = sc_WorldColorArray[color][0];
	T_dash_00->colour2 = sc_WorldColorArray[color][1];

	T_corseNum_00->colour1 = sc_WorldColorArray[color][0];
	T_corseNum_00->colour2 = sc_WorldColorArray[color][1];
}

void SCMWorldAndLevelNumbers(m2d::EmbedLayout_c *layout, int worldNumber, int levelNumber) {
	nw4r::lyt::Pane *rootPane = layout->layout.rootPane;
	nw4r::lyt::TextBox *T_worldNum_00 = layout->findTextBoxByName("T_worldNum_00");
	nw4r::lyt::TextBox *T_corseNum_00 = layout->findTextBoxByName("T_corseNum_00");
	nw4r::lyt::TextBox *T_pictureFont_00 = layout->findTextBoxByName("T_pictureFont_00");

	if (levelNumber == Stage_Invalid)
		rootPane->SetVisible(false);
	else
		rootPane->SetVisible(true);

	nw4r::lyt::TextBox *T_dash_00 = layout->findTextBoxByName("T_-_00");
	SCM_LevelColorChange(worldNumber, T_worldNum_00, T_dash_00, T_corseNum_00);

    dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(worldNumber, levelNumber);
    if (level) {
        const wchar_t *convWorldName = getWorldNumber(level->displayWorld);
        const wchar_t *convLevelName = getLevelNumber(level->displayLevel);

        T_worldNum_00->SetString(convWorldName);

		if (level->displayLevel > 19) {
			T_pictureFont_00->SetVisible(true);
			T_corseNum_00->SetVisible(false);
			T_pictureFont_00->SetString(convLevelName);
		} else {
			T_pictureFont_00->SetVisible(false);
			T_corseNum_00->SetVisible(true);
			T_corseNum_00->SetString(convLevelName);
		}
    } else {
        T_pictureFont_00->SetVisible(false);
		T_corseNum_00->SetVisible(true);
		T_worldNum_00->SetString(L"?");
		T_corseNum_00->SetString(L"?");
    }
	return;
}