#include <game.h>
#include <utils.h>

// Makes openingTitle region-free and adds a dynamic version string

class dEventOpeningTitle_c : public dBase_c {
public:
    m2d::EmbedLayout_c layout;
    dStateWrapper_c<dEventOpeningTitle_c> state;

    nw4r::lyt::Pane *rootPane;
    nw4r::lyt::TextBox *T_E3verCheck;
    nw4r::lyt::Pane *N_proportionC_00;
    u8 pad[36]; // Completely unused

    bool layoutLoaded;
    bool visible;
    bool isUserCancel;
    u8 pad2;

    bool createLayout();
};

bool dEventOpeningTitle_c::createLayout() {
    if (!layout.loadArc("openingTitle.arc", false)) {
        return false;
    }

    layout.build("openingTitle_00.brlyt");

    static const char *brlanNames[] = {
        "openingTitle_00_inTitle.brlan",
        "openingTitle_00_inPress.brlan",
        "openingTitle_00_loopPress.brlan",
        "openingTitle_00_outPress.brlan",
    };
    static const char *groupNames[] = {
        "A00_title",
        "B00_press",
        "B00_press",
        "B00_press"
    };
    static const int groupIDs[] = {
        0, 1, 2, 3
    };

    layout.loadAnimations(brlanNames, 4);
    layout.loadGroups(groupNames, groupIDs, 4);

    rootPane = layout.layout.rootPane;
    T_E3verCheck = layout.findTextBoxByName("T_E3verCheck");
    N_proportionC_00 = layout.findPaneByName("N_proportionC_00");

    WriteBMGToTextBox(layout.findTextBoxByName("T_Press_00"),     GetBMG(), 0x32,  0x00, 0);
    WriteBMGToTextBox(layout.findTextBoxByName("T_2009nintendo"), GetBMG(), 0x3E9, 0x00, 0);

    // Set version string from global value
    T_E3verCheck->SetString(getWideVersion());
    return true;
}
