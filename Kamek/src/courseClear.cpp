#include <game.h>

class dCourseClear_c : public dBase_c {
public:
    bool createLayout();

    m2d::EmbedLayout_c mLayout;
    mEf::es2 mEffect;
    dStateWrapper_c<dCourseClear_c> mState;

    nw4r::lyt::Pane *mpRootPane;
    nw4r::lyt::Pane *N_marioClear_00;

    bool mHasLayoutLoaded;
    bool mIsVisible;
    bool mDoExit;
};

bool dCourseClear_c::createLayout() {
    if (!mLayout.loadArc("corseClear.arc", false)) {
        return false;
    }

    mLayout.build("corseClear_00.brlyt");

    static const char *brlanNames[] = {
        "corseClear_00_inText.brlan",
        "corseClear_00_loopText.brlan",
        "corseClear_00_outText.brlan"
    };
    static const char *groupNames[] = {
        "A00_inText",
        "A02_loopText",
        "A01_outText"
    };
    static const int groupIDs[] = {
        0, 1, 2
    };

    mLayout.loadAnimations(brlanNames, 3);
    mLayout.loadGroups(groupNames, groupIDs, 3);

    mpRootPane = mLayout.layout.rootPane;
    N_marioClear_00 = mLayout.findPaneByName("N_marioClear_00");
    return true;
}
