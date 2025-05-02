#include <game.h>
#include <levelinfo.h>

extern char CurrentLevel;
extern char CurrentWorld;

void LoadPauseMenuNameAndNumber(m2d::EmbedLayout_c *layout) {
	const wchar_t *convWorldName;
	const wchar_t *convLevelName;
	const char *levelname;

	GameMgr *dInfo_c;
	u32 unk;

	dInfo_c = GameMgrP;
	unk = GameMgrP->_3E8;

	if ((int)unk > 9) {
    	unk = unk - 10;
	}

	int wnum = (int)CurrentWorld;
	int lnum = (int)CurrentLevel;

	nw4r::lyt::TextBox
		*T_worldNum_00, *T_corseNum_00,
		*T_corsePic_00, *T_levelName_00;

	nw4r::lyt::Picture *P_coin_00;

	T_worldNum_00 = layout->findTextBoxByName("T_worldNum_00");
	T_corseNum_00 = layout->findTextBoxByName("T_corseNum_00");
	T_corsePic_00 = layout->findTextBoxByName("T_corsePic_00");
	T_levelName_00 = layout->findTextBoxByName("T_levelName_00");
	P_coin_00 = layout->findPictureByName("P_coin_00");

	if ((((GameFlag & 0x40) == 0) || (dInfo_c->_3E4 != 0)) || ((int)unk > 4)) {
		P_coin_00->SetVisible(false);
		T_worldNum_00->SetVisible(true);
	} else {
		P_coin_00->SetVisible(true);
		T_worldNum_00->SetVisible(false);
	}
	
	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(CurrentWorld, CurrentLevel);
	if (level) {
		convWorldName = getWorldNumber(level->displayWorld);
		convLevelName = getLevelNumber((int)CurrentWorld, level->displayLevel);

		T_worldNum_00->SetString(convWorldName);

		if (level->displayLevel > 19) {
			T_corsePic_00->SetVisible(true);
			T_corseNum_00->SetVisible(false);
			T_corsePic_00->SetString(convLevelName);
		} else {
			T_corsePic_00->SetVisible(false);
			T_corseNum_00->SetVisible(true);
			T_corseNum_00->SetString(convLevelName);
		}

		if (CurrentLevel == 38) {
			SaveFile *file = GetSaveFile();
			SaveBlock *block = file->GetBlock(file->header.current_file);
			switch (block->toad_level_idx[CurrentWorld]) {
				case 4: //yellow
					levelname = dLevelInfo_c::s_info.getNameForLevel(dLevelInfo_c::s_info.searchBySlot(CurrentWorld, 27));
					break;
				case 5: //red
					levelname = dLevelInfo_c::s_info.getNameForLevel(dLevelInfo_c::s_info.searchBySlot(CurrentWorld, 26));
					break;
				default: //green
					levelname = dLevelInfo_c::s_info.getNameForLevel(dLevelInfo_c::s_info.searchBySlot(CurrentWorld, 25));
					break;
			}
		} else {
			levelname = dLevelInfo_c::s_info.getNameForLevel(level);
		}
	} else {
		T_corsePic_00->SetVisible(false);
		T_corseNum_00->SetVisible(true);
		T_worldNum_00->SetString(L"?");
		T_corseNum_00->SetString(L"?");
		char levelNumber[15];
		sprintf(levelNumber, "%d-%d (UNNAMED)", wnum+1, lnum+1);
		levelname = levelNumber;
	}

	wchar_t lbuffer[0x40];
	for (int i = 0; i < 0x40; i++) {
		lbuffer[i] = (unsigned short)levelname[i];
	}

	T_levelName_00->SetString(lbuffer);
}

extern "C" void InsertPauseWindowText(void *thing) {
	m2d::EmbedLayout_c *el = (m2d::EmbedLayout_c*)(((u8*)thing)+0x70);
	LoadPauseMenuNameAndNumber(el);
}

