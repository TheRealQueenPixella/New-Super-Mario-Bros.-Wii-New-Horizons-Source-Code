#include <game.h>

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