#include <game.h>
#include <common.h>
#include <profile.h>

//map manager actor, do whatever you want with it

int spawnMapManager() {
    dActor_c* worldmapManager = (dActor_c*)fBase_c::searchByProfileId(ProfileId::WM_MANAGER);
	if(!worldmapManager) {
		dActor_c::create(WM_MANAGER, 0, 0, 0);
	}
	return 0;
}

class dWMManager_c : public dActor_c {
public:
	int onCreate();
	int onExecute();

	bool doInitialSave;
	int saveState;

	static dActor_c* build();
	static dWMManager_c *instance;
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
	// Save the game on our first play, since we don't play the opening movie anymore
	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	if (save->bitfield & 1) {
		doInitialSave = true;
	}

	return true;
}

int dWMManager_c::onExecute() {
	if (doInitialSave) {
		SaveBlock *save = GetSaveFile()->GetBlock(-1);
		if (saveState == 0) { // Do save
			save->onWorldDataFlag(0, 1); // Open W1
			SaveGame(0x0, false);
			saveState++;
		} else if (saveState == 1) { // Wait for save end
			if (!GetSaveFile()->CheckIfWriting()) {
				if (GetSaveHandler()->CurrentError == 0) {
					// Save complete
					save->offWorldDataFlag(0, 1); // Re-lock W1, so the airship cutscene plays
					doInitialSave = false;
				} else {
					OSReport("ERROR %d OCCURED DURING SAVE\n", GetSaveHandler()->CurrentError);
				}
			}
		}
	}

	return true;
}

class daWmIsland_c : public dActor_c {
public:
	int execute();

	void setFogIdx();
	void setAnmChr();
	void calcAngle();

	mHeapAllocator_c mAllocator;
	m3d::mdl_c mModel;
	m3d::anmChr_c mAnmChr;
	m3d::anmTexSrt_c mAnmTexSrt;
	m3d::anmClr_c mAnmClr;
	u8 pad[0x54];
	int mCurrChrAnim;
	u8 pad2[0x8];
	bool mIsModelCreated;
	bool mIsVisible;
	u8 pad3[0x6];
	int mWorldNo;
};

int daWmIsland_c::execute() {
	if (mIsModelCreated && mIsVisible) {
		setFogIdx();
		setAnmChr();
		calcAngle();
		if (mCurrChrAnim != 0) {
			mModel._vf1C();
			int world = mWorldNo;

			if (world == 0) {
				mAnmClr.process();
			} else if ((world == 1) || (world >= 5 && world <= 8)) {
				mAnmTexSrt.process();
			}
		}
	}
	return true;
}