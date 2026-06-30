#include <game.h>

extern "C" int dWmConnect_c_GetCourseNoFromName(void *, char *);

static bool sc_decorFlag = false;

// Course nodes to indicate as non-playable courses
static const char *sc_decorList[] = {
    "W101", "W102", "W103", "W104", "W105", "W106", "W1T0", "W1C0", "W1K1", "W1K2", "W1K3",
    "W2W0",
    "W3K0",
    "W405", "W407", "W4G0", "W4C0", "W4A0", "W4K2", "W4K3", "W4W0"
};

int checkInDecorList(void *connect, char *name) {
    int courseNum = sizeof(sc_decorList) / sizeof(sc_decorList[0]);
    for (int i = 0; i < courseNum; i++) {
        if (!strcmp(name, sc_decorList[i])) {
            sc_decorFlag = true;
            break;
        }
    }

    // Original function call we replaced
    return dWmConnect_c_GetCourseNoFromName(connect, name);
}

extern "C" dActor_c *createCourse(u16 profID, ulong param, Vec *pos, S16Vec *ang) {
    return dActor_c::create((Actors)profID, param | (sc_decorFlag << 0x1C), pos, ang);
}

class daWmCourse_c : public dActor_c {
    void setDecorCourse();
    void setAnm(int type, float rate, float frame);

    u8 unk[0x5C]; // Inheritence
    u32 mPosNodeIdx;
    mHeapAllocator_c mAllocator;
    void *mResFile;
    m3d::mdl_c mModel;
    m3d::anmClr_c mAnmClrs[3];
    int mAnmType;
    u8 unk2[0x18];
};

void daWmCourse_c::setDecorCourse() {
    if (sc_decorFlag) {
        setAnm(0, 0.0f, 0.0f);
        sc_decorFlag = false;
    }
}
