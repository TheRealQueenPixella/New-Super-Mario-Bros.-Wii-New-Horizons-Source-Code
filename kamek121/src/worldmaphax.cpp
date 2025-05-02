#include <game.h>
#include <common.h>
#include <profile.h>
#include <sfx.h>
#include "levelinfo.h"


int spawnMapManager() {
    dActor_c* worldmapManager = (dActor_c*)fBase_c::searchByProfileId(ProfileId::WM_MANAGER);
	if(!worldmapManager) {
		dActor_c::create(WM_MANAGER, 0, 0, 0);
	}
	return 0;
}

int getLevelInfoWorldNumber(int world, int subWorld) {
	/*  for WXa this returns X      (00-07)
	    for WXb this returns X + 8  (08-15)
		for WXc this returns X + 16 (16-23)
	    don't use W9!  */
	return 8 * subWorld + world;
}

class dWMManager_c : public dActor_c {
public:
	static dActor_c* build();
	static dWMManager_c *instance;

	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();
    void CSGLoadLevelNumberFromLevelInfo(int *param_1, int param_2);
    void setWorldNumber(nw4r::lyt::TextBox *textbox, u8 world);

    m2d::EmbedLayout_c *layout;
    Remocon* rem;

    nw4r::lyt::TextBox
        *T_worldNum_00, *T_cSelect_00, *T_cSelect_pic,
        *T_levelName_00, *T_guideViewLS_00, *T_guideViewL_01;
    
    u8 World, Level; //, isMapBeingViewed; 
};

dWMManager_c *dWMManager_c::instance = 0;

const char* WMManagerNameList[] = {0};
Profile WMManagerProfile(&dWMManager_c::build, ProfileId::WM_MANAGER, NULL, ProfileId::WM_SINKSHIP, ProfileId::WM_MANAGER, "WM_MANAGER", WMManagerNameList);

dActor_c* dWMManager_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dWMManager_c));
	dWMManager_c *c = new(buffer) dWMManager_c;

	instance = c;
	return c;
}

int dWMManager_c::onCreate() {
    World = *(u8*)((int)(dCourseSelectManager_c::instance) + 0x4D3);
    Level = *(u8*)((int)(dCourseSelectManager_c::instance) + 0x4D7);
	return true;
}


int dWMManager_c::onDelete() {
	return true;
}

void dWMManager_c::setWorldNumber(nw4r::lyt::TextBox *textbox, u8 world) {
    dLevelInfo_c::entry_s *liWorld = dLevelInfo_c::s_info.searchBySlot(world, 38);
    if (liWorld) {
        const wchar_t *worldNum;
        worldNum = getWorldNumber(liWorld->displayWorld);
        textbox->SetString(worldNum);
    } else {
        textbox->SetString(L"?");
    }
} 

void dWMManager_c::CSGLoadLevelNumberFromLevelInfo(int *param_1, int param_2) {
	if (param_2 == 1) {
		*(u8*)((int)(dCourseSelectManager_c::instance) + 0x4D7) = 0xfffffffe;
	} else {
		layout = (m2d::EmbedLayout_c*)((int)(dCourseSelectManager_c::instance) + 208);

    	T_worldNum_00 = layout->findTextBoxByName("T_worldNum_00");
	    T_cSelect_00 = layout->findTextBoxByName("T_cSelect_00");
	    T_cSelect_pic = layout->findTextBoxByName("T_cSelect_pic");

		T_cSelect_pic->SetVisible(false);
		T_cSelect_00->SetVisible(true);

		World = *(u8*)((int)(dCourseSelectManager_c::instance) + 0x4D3);
		Level = *(u8*)((int)(dCourseSelectManager_c::instance) + 0x4D7);

		dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(World, Level);
		if (level) {
			const wchar_t *convWorldName;
			const wchar_t *convLevelName;

			convWorldName = getWorldNumber(level->displayWorld);
			convLevelName = getLevelNumber(World, level->displayLevel);

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
		} else {
			if(Level > 254) { //get a dot
				T_cSelect_pic->SetVisible(true);
				T_cSelect_00->SetVisible(false);
				T_cSelect_pic->SetString(L"6");
				this->setWorldNumber(T_worldNum_00, World);
			} else {
				T_cSelect_pic->SetVisible(false);
				T_cSelect_00->SetVisible(true);
				T_cSelect_00->SetString(L"?");
				T_worldNum_00->SetString(L"?");
			}
		}
	}
	return;
}

int dWMManager_c::onExecute() {
    World = *(u8*)((int)(dCourseSelectManager_c::instance) + 0x4D3);
    Level = *(u8*)((int)(dCourseSelectManager_c::instance) + 0x4D7);
	return true;
}

int dWMManager_c::onDraw() {
	return true;
} 

class dCollectionCoin_c : public dBase_c {
	public:
	u8 unk[0x420-sizeof(dBase_c)];
	dStateWrapper_c<dCollectionCoin_c> state;
	USING_STATES(dCollectionCoin_c);
	REF_NINTENDO_STATE(ExitAnimeEndWait);
};

extern "C" void CollectionCoinNextWorld(int dCoinCollection);
extern "C" void CollectionCoinPreviousWorld(int dCoinCollection);

static const int secretCode[] = {
	WPAD_UP,WPAD_UP,WPAD_DOWN,WPAD_DOWN,
	WPAD_LEFT,WPAD_RIGHT,WPAD_LEFT,WPAD_RIGHT,
	WPAD_ONE,WPAD_TWO,0
};
static const int secretCodeButtons = WPAD_UP|WPAD_DOWN|WPAD_LEFT|WPAD_RIGHT|WPAD_ONE|WPAD_TWO;
static int secretCodeIndex = 0;
static int minusCount = 0;
extern bool enableHardMode;
extern bool enableDebugMode;
extern u8 isReplayEnabled;

void addReplayAndHardMode(dCollectionCoin_c* self) {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	if ((GetActiveRemocon()->heldButtons == 0x810) && (nowPressed & 0x810)) {
		if (!enableHardMode) {
			enableHardMode = true;
			OSReport("Hard Mode enabled!\n");
			MapSoundPlayer(SoundRelatedClass, SE_VOC_MA_CS_COURSE_IN_HARD, 1);
		} else {
			enableHardMode = false;
			OSReport("Hard Mode disabled!\n");
		}
		return;
	}
	if (nowPressed & secretCodeButtons) {
		int nextKey = secretCode[secretCodeIndex];
		if (nowPressed & nextKey) {
			secretCodeIndex++;
			if (secretCode[secretCodeIndex] == 0) {
				secretCodeIndex = 0;
				if (isReplayEnabled != 100) {
					isReplayEnabled = 100;
					MapSoundPlayer(SoundRelatedClass, SE_VOC_MA_CS_COURSE_IN, 1);
					OSReport("Replay Recording enabled!\n");
				} else {
					isReplayEnabled = 0;
					OSReport("Replay Recording disabled!\n");
				}
			}
			return;
		} else {
			secretCodeIndex = 0;
		}
	}
	if (nowPressed & WPAD_MINUS) {
		minusCount++;
		if (minusCount >= 16) {
			minusCount = 0;

			enableDebugMode = !enableDebugMode;

			if (enableDebugMode) {
				MapSoundPlayer(SoundRelatedClass, SE_VOC_MA_GET_PRIZE, 1);
			}
		}
	} else if (nowPressed & WPAD_RIGHT) {
		CollectionCoinNextWorld((int)self);
	} else if (nowPressed & WPAD_LEFT) {
		CollectionCoinPreviousWorld((int)self);
	} else if (nowPressed & WPAD_ONE) {
		self->state.setState(&dCollectionCoin_c::StateID_ExitAnimeEndWait);
	}
}