#include <game.h>
#include <utils.h>
#include "levelinfo.h"

class dCourseSelectGuide_c {
public:
	void *vtable;
	u32 unk;
	m2d::EmbedLayout_c layout;

	// state machines
	dStateWrapper_c<dCourseSelectGuide_c> state_courseInfo, state_guide, state_mapViewScrollUp,
	state_mapViewScrollDown, state_mapViewScrollLeft, state_MapViewScrollRight, state_shadow,
	state_mapView;

	nw4r::lyt::Pane
		*rootPane, *N_IconPos1P_00, *N_IconPos2P_00, *N_IconPos3P_00,
		*N_IconPos4P_00, *N_mapArrow_00, *N_proportionL_00, *N_proportionR_00,
		*N_proportionC_00, *N_guideViewC_00, *N_guideViewR_01, *N_left_00;

	nw4r::lyt::TextBox
		*T_worldNum_00, *T_cSelect_00, *T_cSelect_pic,
		*T_lifeNumber_00, *T_lifeNumber_01, *T_lifeNumber_02,
		*T_lifeNumber_03, *T_guideViewLS_00, *T_guideViewL_01;

	nw4r::lyt::Picture
		*P_cC_1_00, *P_cC_2_00, *P_cC_3_00, *P_cC_1s_00,
		*P_cC_2s_00, *P_cC_3s_00, *P_flagSkull_00, *P_marioFace_00,
		*P_luigiFace_00, *P_BkinoFace_00, *P_YkinoFace_00, *P_bgShadow_00;

	u32 lastControllerTypeUsed, currentWorldNum, currentLevelNum, currentLevelGroup,
	currentLivesValue[4], timer_WorldCourseOnStageWait, timer_GuideOnStageWait,
	currentLivesAlpha, isLivesFadedIn;

	u8 keyPressValue;
	bool layoutLoaded, shouldHideLives, shouldHideCourseInfo, hidingCourseInfo, setCourseInfoToLastFrame,
	hidingGuide, shouldHideGuide, _43C, setLivesAndGuideToLastFrame, keyWasPressed, exitingMapView, _440,
	isVisible, inhibitHUDShowAndHide, showShadow, hideShadow, isMapViewExitVisible, enterMapView,
	exitMapView, enteringMapView, animationActive, upArrowAnimActive, downArrowAnimActive,
	leftArrowAnimActive, rightArrowAnimActive, livesUpdated;

	void newUpdateLevelDisplay(u32 param);
	void levelColorChange(int world);
};

void dCourseSelectGuide_c::levelColorChange(int world) {
	int color = getColorArrayIdx(world);

	T_worldNum_00->colour1 = sc_WorldColorArray[color][0];
	T_worldNum_00->colour2 = sc_WorldColorArray[color][1];

	nw4r::lyt::TextBox *T_worldNum_01 = layout.findTextBoxByName("T_worldNum_01");
	T_worldNum_01->colour1 = sc_WorldColorArray[color][0];
	T_worldNum_01->colour2 = sc_WorldColorArray[color][1];

	T_cSelect_00->colour1 = sc_WorldColorArray[color][0];
	T_cSelect_00->colour2 = sc_WorldColorArray[color][1];
}

void dCourseSelectGuide_c::newUpdateLevelDisplay(u32 pointType) {
	nw4r::lyt::TextBox *T_levelName_00 = layout.findTextBoxByName("T_levelName_00");
	nw4r::lyt::Pane *N_zanki_00 = layout.findPaneByName("N_zanki_00");

    if (pointType == 1) {
		// Key point, don't update the HUD
		currentLevelNum = 0xfffffffe;
	} else {
		N_zanki_00->trans.y = 18.0f; // shifted pos
		T_levelName_00->SetVisible(true);

		T_cSelect_pic->SetVisible(false);
		T_cSelect_00->SetVisible(true);

		levelColorChange(currentWorldNum);

		dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(currentWorldNum, currentLevelNum);
		if (level) {
			const wchar_t *convWorldName;
			const wchar_t *convLevelName;
			const char *levelName;

			convWorldName = getWorldNumber(level->displayWorld);
			convLevelName = getLevelNumber(level->displayLevel);

			T_worldNum_00->SetString(convWorldName);

			if (level->displayLevel > 19) {
				T_cSelect_pic->SetVisible(true);
				T_cSelect_00->SetVisible(false);
				T_cSelect_pic->SetString(convLevelName);
			} else {
				T_cSelect_pic->SetVisible(false);
				T_cSelect_00->SetVisible(true);
				T_cSelect_00->SetString(convLevelName);
			}

			// Start Node, don't display level name
			if (pointType == 2) {
				T_levelName_00->SetVisible(false);
				N_zanki_00->trans.y = 42.0f; // retail position
				return;
			}

			levelName = dLevelInfo_c::s_info.getNameForLevel(level);
			wchar_t lbuffer[0x40];
			mbstowcs(lbuffer, levelName, 0x40);

			T_levelName_00->SetString(lbuffer);
		} else {
			if (currentLevelNum > 254) { //get a dot
				T_cSelect_pic->SetVisible(true);
				T_cSelect_00->SetVisible(false);
				T_cSelect_pic->SetString(L"6");
				T_levelName_00->SetVisible(false);
				N_zanki_00->trans.y = 42.0f; // retail position

				dLevelInfo_c::entry_s *liWorld = dLevelInfo_c::s_info.searchBySlot(currentWorldNum, 38);
                if (liWorld) {
                    const wchar_t *worldNum;
                    worldNum = getWorldNumber(liWorld->displayWorld);
                    T_worldNum_00->SetString(worldNum);
                } else {
                    T_worldNum_00->SetString(L"?");
                }
			} else {
				T_cSelect_pic->SetVisible(false);
				T_cSelect_00->SetVisible(true);
				T_cSelect_00->SetString(L"?");
				T_worldNum_00->SetString(L"?");
				T_levelName_00->SetString(L"Unknown Level Name!");
			}
		}
	}
    return;
}
