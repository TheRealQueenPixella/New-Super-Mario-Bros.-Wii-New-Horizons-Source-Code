#include "staffRoll.h"
#include <profile.h>
#include <stage.h>
#include <sfx.h>

extern "C" u32 CreateBootParam();
extern int m_WarningForbid;
extern char CurrentWorld, CurrentLevel;
extern u32 Global5758;
extern u8 GlobalEnableFlag, mExecStopReq;

void startDemoW9() {
    SaveBlock *save = GetSaveFile()->GetBlock(-1);
    if (save->worlds_available[8] == 0) {
        ActivateWipe(WIPE_FADE);
        DoSceneChange(WORLD_9_DEMO, 0, 0);
    } else {
        ActivateWipe(WIPE_FADE);
        DoSceneChange(WORLD_MAP, CreateBootParam(), false);
    }
}

void startStaffCredit() {
    ActivateWipe(WIPE_CIRCLE_s);
    GameMgrP->startStaffCredit();
}


const SpriteData StaffRollSpriteData = {ProfileId::AC_STAFF_ROLL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
Profile StaffRollProfile(&daStaffRoll_c::build, SpriteId::AC_STAFF_ROLL, &StaffRollSpriteData, ProfileId::AC_ENDING_MAIN, ProfileId::AC_ENDING_MAIN, "AC_STAFF_ROLL", 0);

CREATE_STATE(daStaffRoll_c, DispWait);
CREATE_STATE(daStaffRoll_c, OpenAnime);
CREATE_STATE(daStaffRoll_c, TextAnime);
CREATE_STATE(daStaffRoll_c, Wait);
CREATE_STATE(daStaffRoll_c, ExitAnime);

dActor_c *daStaffRoll_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daStaffRoll_c));
	return new(buffer) daStaffRoll_c;
}

int daStaffRoll_c::onCreate() {
	if (!layoutLoaded) {
		if (!createLayout()) {
			return false;
		}

		// Read spritedata
		cardIdx = (settings >> 4) & 0xFF;
		dispTimer = ((settings >> 12) & 0xFF) * 10;
		destEntrance = (settings >> 24) & 0xFF;

		isDualSection = settings & 1;
		isFinalCard = (settings >> 1) & 1;
		isCenterName = (settings >> 2) & 1;

		// Move the thing up a bit
		layout.layout.rootPane->trans.y += 36.0f;

		layout.disableAllAnimations();
		layout.resetAnim(IN_WINDOW);
		layout.resetAnim(IN_TITLE);
		layout.resetAnim(IN_NAMES);

		layoutLoaded = true;
		visible = false;
	}

	// Prevent warnings from being displayed during the credits
	m_WarningForbid++;
	return true;
}

int daStaffRoll_c::onDelete() {
	return layout.free();
}

int daStaffRoll_c::beforeExecute() {
	// Fixes BG and other sprites from stopping during screen changes
	// Doesn't work for fog effects though, not sure why
	Global5758 = 0;
	GlobalEnableFlag = 0;
	mExecStopReq = 0;
	return true;
}

int daStaffRoll_c::onExecute() {
	state.execute();
	if (visible) {
		layout.execAnimations();
		layout.update();
		calcTimer();
		chkSkip();
	}
	return true;
}

int daStaffRoll_c::onDraw() {
	if (visible) {
		layout.scheduleForDrawing();
	}
	return true;
}

bool daStaffRoll_c::createLayout() {
	if (!layout.loadArc("staffRoll.arc", false)) {
		return false;
	}

	layout.build("staffRoll.brlyt");

	static const char *brlanNames[] = {
		"staffRoll_inWindow.brlan",
		"staffRoll_inTitle.brlan",
		"staffRoll_outWindow.brlan",
	};
	static const char *groupNames[] = {
		"A00_Window",
		"B00_Title", "B01_Names",
		"A00_Window",
	};
	static const int groupIDs[] = {
		0,
		1, 1,
		2,
	};

	layout.loadAnimations(brlanNames, 3);
	layout.loadGroups(groupNames, groupIDs, 4);

	static const char *nullPaneNames[] = {
		"N_titleOne_00", "N_titleTwo_00",
		"N_names_00", "N_thankYou_00",
	};
	static const char *textBoxNames[] = {
		"T_titleC_00", "T_titleC_01",
		"T_titleL_00", "T_titleL_01",
		"T_titleR_00", "T_titleR_01",
		"T_namesL_00", "T_namesR_00", "T_namesC_00",
	};

	layout.getPanes(nullPaneNames, &N_titleOne_00, 4);
	layout.getTextBoxes(textBoxNames, &T_titleC_0X[0], 9);
	return true;
}

void daStaffRoll_c::chkSkip() {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	// Allow the credits to be skipped if World 9 is unlocked
	// (meaning, we've already seen them before)
	if (nowPressed & WPAD_PLUS) {
		SaveBlock *save = GetSaveFile()->GetBlock(-1);
		if (save->worlds_available[8] != 0) {
			ActivateWipe(WIPE_CIRCLE_s);
			DoSceneChange(WORLD_MAP, CreateBootParam(), false);
			MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_OUT, 1);
		}
	}
}

void daStaffRoll_c::calcTimer() {
	dispTimer--;
	if (dispTimer == 0) {
		state.setState(&StateID_ExitAnime);
	}
}

void daStaffRoll_c::setInfo() {
	if (isFinalCard) {
		N_thankYou_00->SetVisible(true);

		N_titleOne_00->SetVisible(false);
		N_titleTwo_00->SetVisible(false);
		N_names_00->SetVisible(false);
		return;
	}
	N_thankYou_00->SetVisible(false);

	N_titleOne_00->SetVisible(!isDualSection);
	N_titleTwo_00->SetVisible(isDualSection);

	// Set text strings
	int msgCatID = 0x140;
	int msgBaseID = cardIdx * 0x10;
	dScript::Res_c *bmg = GetBMG();

	// Setup title first
	if (isDualSection) {
		// Left
		setMessage(T_titleL_0X[0], bmg, msgCatID, msgBaseID);
		setMessage(T_titleL_0X[1], bmg, msgCatID, msgBaseID);
		// Right
		setMessage(T_titleR_0X[0], bmg, msgCatID, msgBaseID+1);
		setMessage(T_titleR_0X[1], bmg, msgCatID, msgBaseID+1);
	} else {
		setMessage(T_titleC_0X[0], bmg, msgCatID, msgBaseID);
		setMessage(T_titleC_0X[1], bmg, msgCatID, msgBaseID);
	}

	// And the names
	if (isCenterName) {
		T_names_0X[0]->SetVisible(false);
		T_names_0X[1]->SetVisible(false);
		setMessage(T_names_0X[2], bmg, msgCatID, msgBaseID+2);
	} else {
		T_names_0X[2]->SetVisible(false);
		setMessage(T_names_0X[0], bmg, msgCatID, msgBaseID+2);
		setMessage(T_names_0X[1], bmg, msgCatID, msgBaseID+3);
	}
}

// Wrapper so we can make sure the message actually exists first
void daStaffRoll_c::setMessage(nw4r::lyt::TextBox *textbox, dScript::Res_c *bmg, ulong cat, ulong message) {
	if (bmg->getMsgEntry(cat, message)) {
		const wchar_t *str = dScript::Res_c::getMsg(cat, message);
		textbox->SetString(str);
	} else {
		textbox->SetString(L"String not found!");
	}
}


void daStaffRoll_c::beginState_DispWait() { }
void daStaffRoll_c::executeState_DispWait() {
	if (mFaderBase_c__getStatus(mFader) == 1) {
        state.setState(&StateID_OpenAnime);
    }
}
void daStaffRoll_c::endState_DispWait() {
	setInfo();
}


void daStaffRoll_c::beginState_OpenAnime() {
	layout.enableNonLoopAnim(IN_WINDOW, false);
	MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_IN, 1);
	visible = true;
}
void daStaffRoll_c::executeState_OpenAnime() {
	if (!layout.isAnimOn(IN_WINDOW)) {
		state.setState(&StateID_TextAnime);
	}
}
void daStaffRoll_c::endState_OpenAnime() {
	layout.enableNonLoopAnim(IN_TITLE, false);
	layout.enableNonLoopAnim(IN_NAMES, false);
}


void daStaffRoll_c::beginState_TextAnime() { }
void daStaffRoll_c::executeState_TextAnime() {
	if (!layout.isAnimOn(IN_TITLE) || !layout.isAnimOn(IN_NAMES)) {
		if (!isFinalCard) {
			MapSoundPlayer(SoundRelatedClass, SE_SYS_KO_DIALOGUE_IN, 1);
		}
		state.setState(&StateID_Wait);
	}
}
void daStaffRoll_c::endState_TextAnime() { }


void daStaffRoll_c::beginState_Wait() { }
void daStaffRoll_c::executeState_Wait() { }
void daStaffRoll_c::endState_Wait() { }


void daStaffRoll_c::beginState_ExitAnime() {
	MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_OUT_AUTO, 1);
	layout.enableNonLoopAnim(OUT_WINDOW);
}
void daStaffRoll_c::executeState_ExitAnime() {
	if (layout.isAnimOn(OUT_WINDOW)) {
		return;
	}

	if (isFinalCard) {
		startDemoW9();
		return;
	}

	for (int i = 0; i < 4; i++) {
		daPlBase_c *ply = daPlBase_c::findByID(i);
		if (ply != 0x0) {
			ply->setDemoNextGotoBlock(destEntrance, 10, 0);
		}
	}
}
void daStaffRoll_c::endState_ExitAnime() {
	visible = false;
}
