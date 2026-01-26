#include <game.h>
#include <levelnumber.h>
#include <utils.h>
#include "levelinfo.h"

class dWorldSelectGuide_c : public dBase_c {
public:
	m2d::EmbedLayout_c layout;                    // 0x70
	nw4r::lyt::Pane *rootPane;					  // 0x208
	nw4r::lyt::TextBox *T_worldNum_00;			  // 0x20C
	nw4r::lyt::Picture *P_pochi_00, *P_pochi_01;  // 0x210, 0x214
	nw4r::lyt::Picture *P_pochi_02, *P_pochi_03;  // 0x218, 0x21C
	nw4r::lyt::Pane *N_wCCGuide_00, *N_title;	  // 0x220, 0x224
	bool layoutLoaded;							  // 0x228
	bool visible;								  // 0x229
	bool showReds;								  // 0x22A
	bool showTopRed;							  // 0x22B
	u32 worldNum;								  // 0x22C
	u32 nextWorldNum;							  // 0x230
	f32 titleYPos;								  // 0x234

	void newUpdateWorldNum();
};

void dWorldSelectGuide_c::newUpdateWorldNum() {
	if (worldNum == nextWorldNum)
		return;

	worldNum = nextWorldNum;
	const wchar_t *number;

    // Nintendo, why is this world number not zero-indexed?
	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(worldNum-1, Stage_Home);
    if (level) {
        number = getWorldNumber(level->displayWorld);
    } else {
        number = L"?";
    }

	T_worldNum_00->SetString(number);

	// Set world color
	int color = getColorArrayIdx(worldNum-1);
	T_worldNum_00->colour1 = sc_WorldColorArray[color][0];
	T_worldNum_00->colour2 = sc_WorldColorArray[color][1];
}